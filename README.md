# Esp32-RTC-based-motion-detection-using-PIR-sensor
To develop a system to develop a cost-effective, compact, and efficient system capable of detecting motion in real time. Utilizing the ESP32-CAM module equipped with an OV2640 camera sensor, this project integrates motion detection, real-time clock (RTC) for timestamping, and a web server for remote monitoring and control.

This mini-project is a low-power motion detection system that uses an ESP32 microcontroller, a PIR motion sensor, and a Real-Time Clock (RTC) module. It is designed for security and automation applications, such as detecting human presence, logging activity time, or triggering alerts.

⚙️ How the Project Works
1. Hardware Components

ESP32 → Main controller for processing signals and managing data.

PIR Sensor (Passive Infrared Sensor) → Detects human motion by sensing infrared radiation changes.

RTC Module (DS3231/DS1307) → Keeps track of the exact time and date of motion events.

Buzzer/LED (optional) → Alerts the user when motion is detected.

Power Supply (USB/Adapter/Battery)

2. Working Principle

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

3. Flow of the System

PIR detects motion → sends HIGH signal.

ESP32 reads signal → requests timestamp from RTC.

Timestamp + motion event is logged/displayed.

Alert (LED/Buzzer/Message) is activated.

4. Applications

Home & Office security systems

Attendance/Activity logging with timestamps

IoT-based smart surveillance

Automatic light/fan control when someone enters a room

📊 Features

✔️ Low-cost, low-power security solution
✔️ Accurate time-stamped motion detection using RTC
✔️ Expandable with IoT (Wi-Fi alerts, cloud logging)
✔️ Works standalone with simple hardware

🔮 Future Enhancements

Store logs in an SD card / cloud database

Integration with ESP32-CAM for capturing images when motion is detected

Mobile app notification system

Battery backup for RTC to maintain accuracy during power failure

⚡ With this project, the ESP32 and PIR sensor create a smart and reliable motion detection system, while the RTC ensures precise time logging for each event.
