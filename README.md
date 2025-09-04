# Esp32-RTC-based-motion-detection-using-PIR-sensor
To develop a system to develop a cost-effective, compact, and efficient system capable of detecting motion in real time. Utilizing the ESP32-CAM module equipped with an OV2640 camera sensor, this project integrates motion detection, real-time clock (RTC) for timestamping, and a web server for remote monitoring and control.

This mini-project is a low-power motion detection system that uses an ESP32 microcontroller, a PIR motion sensor, and a Real-Time Clock (RTC) module. It is designed for security and automation applications, such as detecting human presence, logging activity time, or triggering alerts.

## ⚙️ How the Project Works

<img width="1010" height="397" alt="image" src="https://github.com/user-attachments/assets/a21f8966-8ef6-4b7d-8a9c-c6dfd43f7283" />

## 1. Hardware Components

ESP32 → Main controller for processing signals and managing data.

PIR Sensor (Passive Infrared Sensor) → Detects human motion by sensing infrared radiation changes.

RTC Module (DS3231/DS1307) → Keeps track of the exact time and date of motion events.

Buzzer/LED (optional) → Alerts the user when motion is detected.

Power Supply (USB/Adapter/Battery)

## Hardware Connections
Components:

ESP32 (or ESP32-CAM if you also want photo capture)

PIR Motion Sensor (HC-SR501)

RTC Module (DS3231 or DS1307)

Jumper wires

Breadboard

Optional: MicroSD card module for storing logs/pictures

1. PIR Sensor → ESP32

VCC → 5V (or 3.3V, depending on PIR model)

GND → GND

OUT → GPIO 14 (you can use another GPIO if you prefer)

2. RTC DS3231 → ESP32 (I²C)

VCC → 3.3V

GND → GND

SDA → GPIO 21 (default SDA on ESP32)

SCL → GPIO 22 (default SCL on ESP32)

## Arduino IDE Setup

Install ESP32 Board Package

Open Arduino IDE → File → Preferences.

In “Additional Board Manager URLs,” paste:

https://dl.espressif.com/dl/package_esp32_index.json


Go to Tools → Board → Board Manager.

Search ESP32 → Install.

Select Board

Tools → Board → ESP32 Arduino → Select ESP32 Dev Module (or ESP32-CAM if you use camera).

Install Required Libraries

RTClib (by Adafruit) → for RTC DS3231

Wire.h (already built-in) → for I²C

SD / SPIFFS (if storing logs/pictures)

WiFi.h (if you want cloud upload or server access)

Upload Settings

Tools → Upload Speed: 115200

Partition Scheme: Default

Port: Select correct COM port

## 2. Working Principle

Motion Detection

The PIR sensor continuously monitors for changes in infrared radiation.

When a human body passes in front of the sensor, it generates a HIGH signal.

ESP32 Processing

The ESP32 reads the PIR sensor’s output.

If motion is detected, the ESP32 immediately requests the current timestamp from the RTC module.

RTC Timestamp Logging

The exact date and time of the motion event is recorded.

This can be stored in memory (EEPROM/SD card) or displayed on a serial monitor/LCD.

Alert/Action

An LED or buzzer can be triggered for real-time feedback.

Optionally, the ESP32 can send notifications via Wi-Fi (MQTT/HTTP/Telegram/Email) for smart IoT integration.

## 3. Flow of the System

PIR detects motion → sends HIGH signal.

ESP32 reads signal → requests timestamp from RTC.

Timestamp + motion event is logged/displayed.

Alert (LED/Buzzer/Message) is activated.

<img width="1500" height="561" alt="image" src="https://github.com/user-attachments/assets/66ef4550-81a9-47e5-b610-97347ed9a18b" />


## 4. Applications

Home & Office security systems

Attendance/Activity logging with timestamps

IoT-based smart surveillance

Automatic light/fan control when someone enters a room

## 📊 Features

✔️ Low-cost, low-power security solution
✔️ Accurate time-stamped motion detection using RTC
✔️ Expandable with IoT (Wi-Fi alerts, cloud logging)
✔️ Works standalone with simple hardware

## 🔮 Future Enhancements

Store logs in an SD card / cloud database

Integration with ESP32-CAM for capturing images when motion is detected

Mobile app notification system

Battery backup for RTC to maintain accuracy during power failure

## ⚡ With this project, the ESP32 and PIR sensor create a smart and reliable motion detection system, while the RTC ensures precise time logging for each event.
