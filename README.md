# 🌡️ Automatic Temperature-Based Fan Control System (STM32)

![C](https://img.shields.io/badge/Language-C-blue)
![Embedded](https://img.shields.io/badge/Domain-Embedded%20Systems-orange)
![MCU](https://img.shields.io/badge/Microcontroller-STM32F4-green)
![ADC](https://img.shields.io/badge/Peripheral-ADC-yellow)
![Status](https://img.shields.io/badge/Project-Completed-success)

---

## 📌 Project Title
Automatic Temperature-Based Fan Control System using STM32

---

## 📌 Project Description

This project implements an **automatic temperature-controlled fan system** using the **STM32F4 series microcontroller**.  
The system continuously monitors temperature using an **LM35 temperature sensor** via the **ADC peripheral** and automatically controls a **fan** based on predefined temperature thresholds.

An **I2C-based LCD display** shows real-time temperature values and fan status.  
User interaction is enabled using an **external interrupt (push button)**.

The project is developed using **register-level programming**, focusing on core embedded system concepts.

---

## 🖼️ Hardware Setup
![Hardware Setup](hardware.jpg)

---

## ❓ Why This Project?

Temperature-based automation is widely used in:
- Smart home systems
- Industrial cooling systems
- Embedded control applications

This project was developed to gain hands-on experience in:
- STM32 register-level programming
- ADC-based sensor interfacing
- Interrupt handling
- I2C communication
- Real-time embedded system design

---

## 🎯 Objectives
- Measure temperature using LM35 sensor
- Automatically control fan operation
- Display temperature and fan status on LCD
- Implement interrupt-based user control
- Build a reliable embedded system

---

## 🧠 Technical Approach

### Embedded System Design
- Microcontroller: STM32F4
- Programming style: Register-level (no HAL abstraction)

### Peripherals Used
- ADC – Temperature sensing
- GPIO – Fan and LED control
- I2C – LCD interfacing
- EXTI – Push-button interrupt
- SysTick – Timing and delay

---

## 🧩 System Working

1. LM35 sensor outputs voltage proportional to temperature  
2. ADC converts analog voltage to digital value  
3. Temperature is calculated from ADC data  
4. Fan is switched ON/OFF based on threshold  
5. LCD displays temperature and fan status  
6. Push button interrupt enables user interaction

---

## ⚙️ Hardware Components

| Component | Purpose |
|---------|--------|
| STM32F4 Board | Main controller |
| LM35 Sensor | Temperature sensing |
| DC Fan | Cooling device |
| I2C LCD (PCF8574) | Display output |
| Push Button | External interrupt |
| LED | Status indication |

---

## 🔌 Pin Configuration

### LM35 (ADC)
| Signal | STM32 Pin |
|------|----------|
| ADC Input | PA1 (ADC1_IN1) |

### LCD (I2C)
| Signal | STM32 Pin |
|------|----------|
| SCL | PB6 |
| SDA | PB7 |

### Fan and LED
| Function | Pin |
|--------|-----|
| Fan Control | PC14 |
| Status LED | PC13 |

### Button (EXTI)
| Function | Pin |
|--------|-----|
| Push Button | PA6 |

---

## 📊 Output and Results
- Real-time temperature monitoring
- Automatic fan control
- LCD-based status display
- Stable and reliable operation

---

## ⚠️ Limitations
- Threshold-based control only
- Accuracy depends on sensor calibration
- Suitable for limited temperature range

---

## 🛠️ Tools and Technologies Used

| Category | Tools |
|--------|------|
| Programming Language | C |
| IDE | STM32CubeIDE |
| Microcontroller | STM32F4 |
| Embedded Concepts | ADC, GPIO, I2C, EXTI |
| Version Control | Git & GitHub |

---

## ▶️ How to Clone and Run

```bash
git clone https://github.com/your-username/Automatic-Temperature-Based-Fan-Control.git
cd Automatic-Temperature-Based-Fan-Control
