#include <Wire.h>
#include <RTClib.h>
#include <SPIFFS.h>
#include "esp_camera.h"
#include "Arduino.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "driver/rtc_io.h"
#include <EEPROM.h>
#include <WiFi.h>
#include <WebServer.h>

// RTC instance
RTC_DS3231 rtc;

#define EEPROM_SIZE 1

RTC_DATA_ATTR int bootCount = 0;

// Replace with your network credentials
const char* ssid = "  ";
const char* password = "  ";

WebServer server(80);

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define PIR_PIN           13    // PIR sensor pin
#define LED_PIN            4    // On-board LED

camera_fb_t *currentImage = NULL;
camera_fb_t *previousImage = NULL;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Disable brownout detector
  Serial.begin(115200);

  // Initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);

  // Initialize the PIR sensor pin
  pinMode(PIR_PIN, INPUT);

  // Initialize the LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize I2C for RTC
  Wire.begin(14, 15); // SDA = GPIO 14, SCL = GPIO 15

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set to compile time
  }

  // Configure camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Init Camera
  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }

  // Configure web server routes
  server.on("/", handleRoot);
  server.on("/current", handleCurrentImage);
  server.on("/previous", handlePreviousImage);
  server.begin();
  Serial.println("HTTP server started");

  Serial.println("Setup complete. Waiting for PIR trigger to capture image.");
}

void loop() {
  server.handleClient();

  // If motion detected, take picture
  if (digitalRead(PIR_PIN) == HIGH) {
    Serial.println("Motion detected");
    captureImageWithTimestamp();
    delay(1000); // Debounce delay
  }
}

// Function to capture image and store it in memory with timestamp
void captureImageWithTimestamp() {
  // Move current image to previous
  if (previousImage) {
    esp_camera_fb_return(previousImage); // Release old previous image
  }
  previousImage = currentImage;

  // Capture a new image
  digitalWrite(LED_PIN, HIGH); // Turn on LED to indicate capture
  currentImage = esp_camera_fb_get();
  if (!currentImage) {
    Serial.println("Camera capture failed");
    digitalWrite(LED_PIN, LOW);
    return;
  }

  DateTime now = rtc.now();
  Serial.printf("Image captured at: %04d-%02d-%02d %02d:%02d:%02d\n",
                now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());

  digitalWrite(LED_PIN, LOW); // Turn off LED after capture
}

// Function to serve the current image
void handleCurrentImage() {
  if (currentImage == NULL) {
    server.send(404, "text/plain", "No current image captured yet");
    return;
  }
  Serial.println("Sending current image over HTTP");
  server.sendHeader("Content-Disposition", "inline; filename=current.jpg");
  server.send_P(200, "image/jpeg", (const char*)currentImage->buf, currentImage->len);
}

// Function to serve the previous image
void handlePreviousImage() {
  if (previousImage == NULL) {
    server.send(404, "text/plain", "No previous image available");
    return;
  }
  Serial.println("Sending previous image over HTTP");
  server.sendHeader("Content-Disposition", "inline; filename=previous.jpg");
  server.send_P(200, "image/jpeg", (const char*)previousImage->buf, previousImage->len);
}

// Function to handle the root request
/*void handleRoot() {
  String html = "<html><body><h1>ESP32-CAM captured images</h1>";
  html += "<p><a href='/current'>View Current Image</a></p>";
  html += "<p><a href='/previous'>View Previous Image</a></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}*/

void handleRoot() {
  String html = "<html><head><style>"
                "body { font-family: Arial, sans-serif; background-color: #f0f0f0; margin: 0; padding: 0; display: flex; justify-content: center; align-items: center; height: 100vh; }"
                "div.container { background-color: white; padding: 20px; border-radius: 8px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1); text-align: center; }"
                "h1 { color: #333; font-size: 24px; }"
                "p { font-size: 18px; margin-bottom: 20px; }"
                "a { text-decoration: none; padding: 10px 20px; font-size: 18px; color: white; background-color: #4CAF50; border-radius: 5px; transition: background-color 0.3s ease; }"
                "a:hover { background-color: #45a049; }"
                "footer { margin-top: 20px; font-size: 14px; color: #777; }"
                "</style></head><body>"
                "<div class='container'>"
                "<h1>ESP32-CAM Motion Detection</h1>"
                "<p><a href='/current'>View Current Image</a></p>"
                "<p><a href='/previous'>View Previous Image</a></p>"
                "<footer>ESP32-CAM Project by 420,21,47</footer>"
                "</div>"
                "</body></html>";
  server.send(200, "text/html", html);
}

