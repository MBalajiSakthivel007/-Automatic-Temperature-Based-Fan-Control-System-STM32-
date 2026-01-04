#include "stm32f4xx.h"
#include "string.h"
#include <stdint.h>
#include <stdio.h>

uint16_t adcData;
int ext_flag = 0;
int fan_state = 0;

#define PCF8574_ADDR  0x4E


volatile uint32_t msTicks = 0; // SysTick tick

void SysTick_Handler(void) { msTicks++; }

// Delay and peripherals
void delay_ms(uint32_t ms) {
    uint32_t start = msTicks;
    while ((msTicks - start) < ms);
}

void LED_Fan_Init(void) {
    RCC->AHB1ENR |= (1U << 2);
    GPIOC->MODER &= ~(3U<<26);
    GPIOC->MODER |= (1<<26);
    GPIOC->MODER &= ~(1<<27);
    GPIOC->MODER &= ~(3U<<28);
    GPIOC->MODER |= (1<<28);
    GPIOC->MODER &= ~(1<<29);
	
		GPIOC->ODR |= (1<<13);
}

// ADC (LM35)
void ADC_Init(void) {
		RCC->AHB1ENR |= (1<<0);
    RCC->APB2ENR |= (1 << 8);
    GPIOA->MODER |= (3U << 2);
    ADC1->SMPR2 = 0;
    ADC->CCR = 0;
    ADC1->CR1 = 0;
    ADC1->CR2 = 0;
    ADC1->CR2 |= (1U << 1);
    ADC1->SQR3 = 1;
    ADC1->SQR1 = 0;
    ADC1->CR2 |= 1;
    while((ADC1->CR2 & 1) == 0) {}
    ADC1->CR2 |= (1 << 30);
}

// I2C1 & LCD
void I2C1_Init(void) {
    RCC->AHB1ENR |= (1<<1);
    RCC->APB1ENR |= (1<<21);
    GPIOB->MODER &= ~((3<<12)|(3<<14));
    GPIOB->MODER |=  ((2<<12)|(2<<14));
    GPIOB->OTYPER |= (1<<6)|(1<<7);
    GPIOB->OSPEEDR |= (3<<12)|(3<<14);
    GPIOB->PUPDR   |= (1<<12)|(1<<14);
    GPIOB->AFR[0]  |= (4<<24) | (4<<28);
    I2C1->CR1 |= (1<<15);
    I2C1->CR1 &= ~(1<<15);
    I2C1->CR2   = 16;
    I2C1->CCR   = 80;
    I2C1->TRISE = 17;
    I2C1->CR1 |= (1<<0);
}

void I2C1_Write(uint8_t addr, uint8_t data) {
    while(I2C1->SR2 & (1<<1));
    I2C1->CR1 |= (1<<8);
    while(!(I2C1->SR1 & (1<<0)));
    I2C1->DR = addr;
    while(!(I2C1->SR1 & (1<<1)));
    (void)I2C1->SR2;
    while(!(I2C1->SR1 & (1<<7)));
    I2C1->DR = data;
    while(!(I2C1->SR1 & (1<<2)));
    I2C1->CR1 |= (1<<9);
}

void LCD_SendCmd(uint8_t cmd) {
    uint8_t high = (cmd & 0xF0);
    uint8_t low  = ((cmd << 4) & 0xF0);
    I2C1_Write(PCF8574_ADDR, high | 0x0C); delay_ms(1);
    I2C1_Write(PCF8574_ADDR, high | 0x08);
    I2C1_Write(PCF8574_ADDR, low | 0x0C); delay_ms(1);
    I2C1_Write(PCF8574_ADDR, low | 0x08);
}

void LCD_SendData(uint8_t data) {
    uint8_t high = (data & 0xF0);
    uint8_t low  = ((data << 4) & 0xF0);
    I2C1_Write(PCF8574_ADDR, high | 0x0D); delay_ms(1);
    I2C1_Write(PCF8574_ADDR, high | 0x09);
    I2C1_Write(PCF8574_ADDR, low | 0x0D); delay_ms(1);
    I2C1_Write(PCF8574_ADDR, low | 0x09);
}

void LCD_Init(void) {
    delay_ms(50);
    LCD_SendCmd(0x33);
    LCD_SendCmd(0x32);
    LCD_SendCmd(0x28);
    LCD_SendCmd(0x0C);
    LCD_SendCmd(0x06);
    LCD_SendCmd(0x01);
    delay_ms(5);
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0 ? 0x80 : 0xC0) + col;
    LCD_SendCmd(addr);
}

void LCD_Print(char *str) {
    while(*str) LCD_SendData(*str++);
}

void LCD_Print_Value_Int1(uint32_t temp, int fan_state) {
    char buf[32];
    LCD_SetCursor(0, 0);
  	LCD_Print("Temp:");
    sprintf(buf, "%u", temp);
    LCD_SetCursor(0, 5);
    LCD_Print(buf);
    LCD_Print("C         ");
	
		LCD_SetCursor(1, 0);
		LCD_Print("FAN MODE: ");
		if(fan_state) {
			LCD_SetCursor(1, 10);
			LCD_Print("ON  ");
		} else {
				LCD_SetCursor(1, 10);
				LCD_Print("OFF  ");
		}
}

void Interrupt_Init() {
		RCC->AHB1ENR |= (1<<0);
		RCC->APB2ENR |= (1 << 14);
    
    GPIOA->MODER &= ~(3U << 12);
		GPIOA->PUPDR |= (1U << 12);
		
		SYSCFG->EXTICR[1] &= ~(0xF << 8);
		SYSCFG->EXTICR[1] |= (0 << 8);
	
	
		EXTI->IMR |= (1<<6);
		EXTI->FTSR |= (1<<6);

}

void EXTI9_5_IRQHandler(void) {
    if(EXTI->PR & (1<<6)) {
        EXTI->PR |= (1<<6);   // Clear interrupt flag
        ext_flag = !(ext_flag);       // Signal main loop to read ADC and update LCD
    }
}

void LCD_Clear() {
	LCD_SetCursor(0,0);
	LCD_Print("                 ");
	LCD_SetCursor(1,0);
	LCD_Print("                 ");
}

// MAIN
int main(void) {
    SystemInit();
    SysTick_Config(SystemCoreClock / 1000);
		
		
		I2C1_Init();
    LCD_Init();
    ADC_Init();
    Interrupt_Init();
		LED_Fan_Init();
		int i=0;
		NVIC_EnableIRQ(EXTI9_5_IRQn);
		
    LCD_SetCursor(0, 0);
    LCD_Print("Fan Control");
    LCD_SetCursor(1, 0);
    LCD_Print("    System");
    
		while(1) {
			
				if(ext_flag) {
					while(!(ADC1->SR & (1 << 1))) {}
          adcData = ADC1->DR;
          uint16_t temp = 4095.0/(adcData*1.0);
					fan_state = ((temp >25)?0:1); 
					LCD_Print_Value_Int1(temp, fan_state);
						
					if(fan_state) {
						GPIOC->ODR &= ~(1<<13);
						GPIOC->ODR |= (1<<14);
					} else {
						GPIOC->ODR |= (1<<13);
						GPIOC->ODR &= ~(1<<14);
					}
					i=1;
				}
				else {
					if(i)
						LCD_Clear();
				}
    }
}