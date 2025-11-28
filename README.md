# Arduino RFID Scanner with SMS Notifications

[![Arduino](https://img.shields.io/badge/Platform-Arduino-blue)](https://www.arduino.cc/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

This project is an **Arduino-based RFID scanner** designed for attendance, access control, or student tracking systems. It integrates **Wi-Fi connectivity**, an **RC522 RFID reader**, and a **SIM900A GSM module** to send SMS notifications based on scanned RFID cards.

---

## Features

- **RFID scanning** using the MFRC522 module  
- **Wi-Fi communication** to a Laravel API backend  
- **Toggle button** to control a yellow LED indicator (switch ON/OFF for Attendance request or register rfid to student)
- **Red/Green LEDs** for Wi-Fi connection status  
- **SMS notifications** via SIM900A GSM module (For notifying parents)
- Easy integration with student or employee databases  

---

## Hardware Requirements

- Arduino R4 Wi-Fi (or compatible)  
- MFRC522 RFID reader  
- SIM900A GSM module (requires separate 5V 2A+ power supply)  
- LEDs: Red, Green, Yellow (with 220Ω resistors)  
- Push button for toggle control  
- Breadboard and jumper wires  

---

## Wiring Overview

| Component | Arduino Pin |
|-----------|-------------|
| RC522 SS  | D10         |
| RC522 RST | D7          |
| RED LED   | D6          |
| GREEN LED | D5          |
| YELLOW LED| D3          |
| BUTTON    | D4 (to GND, INPUT_PULLUP) |
| SIM900A RX| Arduino TX (via level shifter if needed) |
| SIM900A TX| Arduino RX |

**Note:** SIM900A must use a **separate 5V 2A+ power supply**. Arduino cannot provide enough current.

---

## Software Requirements

- Arduino IDE (latest version)  
- Libraries:
  - [MFRC522](https://github.com/miguelbalboa/rfid)  
  - Wi-Fi library for Arduino R4 (WiFiS3)  

---

## Installation

1. Clone or download this repository:  
```bash
git clone https://github.com/yourusername/arduino-rfid-sms.git
