// Copyright (c) 2024 H0llyW00dz All rights reserved.
//
// By accessing or using this software, you agree to be bound by the terms
// of the License Agreement, which you can find at LICENSE files.
//
// Note: This is experimental IoT and might not work as expected.

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Hash.h>
#include <TOTP.h>
#include <Base32-Decode.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Wi-Fi credentials
const char* ssid = "your_SSID"; // Use secure methods to store credentials
const char* password = "your_PASSWORD"; // Use secure methods to store credentials

// NTP client setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);

// Base32 encoded secret key for TOTP
const char* base32Secret = "BASE32EncodedSecret"; // Consider secure storage
uint8_t decodedSecret[20]; // Adjust size based on your secret length

// OLED setup
//
// Note: Don't forget Adjust screen w & h based on needed.
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

TOTP totp(decodedSecret, sizeof(decodedSecret)); // Instantiate TOTP object

void setup() {
  Serial.begin(115200);

  // Initialize OLED
  // Note: Ensure the I2C address (0x3C) is correct for your OLED. Some displays use different addresses like 0x3D
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    while (true); // Don't proceed, loop forever
  }

  display.display();
  delay(2000);
  display.clearDisplay();

  // Connect to Wi-Fi with timeout
  WiFi.begin(ssid, password);
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) { // 15 seconds timeout
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Exit setup if unable to connect
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi. Check your credentials.");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("WiFi Error");
    display.display();
    return;
  }

  Serial.println("Connected to WiFi");

  // Initialize NTP client
  timeClient.begin();

  // Decode Base32 secret
  int secretLength = base32decode((char*)base32Secret, decodedSecret, sizeof(decodedSecret));
  if (secretLength <= 0) {
    Serial.println("Error decoding Base32 secret");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Decode Error");
    display.display();
    return;
  }
}

unsigned long lastOTPTimestamp = 0;

void loop() {
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();

  // Generate TOTP every 30 seconds
  if (millis() - lastOTPTimestamp >= 30000) {
    String otp = totp.getCode(epochTime);
    Serial.println("Current OTP: " + otp);

    // Display OTP on OLED
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("OTP:");
    display.setTextSize(3);
    display.setCursor(0, 20);
    display.println(otp);
    display.display();

    lastOTPTimestamp = millis();
  }

  // Other non-blocking code can be added here.
  // For example, you might want to use this TOTP for opening your front door house.

}
