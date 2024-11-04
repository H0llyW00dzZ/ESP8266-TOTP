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

// Wi-Fi credentials
const char* ssid = "your_SSID"; // Use secure methods to store credentials
const char* password = "your_PASSWORD"; // Use secure methods to store credentials

// NTP client setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);

// Base32 encoded secret key for TOTP
const char* base32Secret = "BASE32EncodedSecret"; // Consider secure storage
uint8_t decodedSecret[20]; // Adjust size based on your secret length

TOTP totp(decodedSecret, sizeof(decodedSecret)); // Instantiate TOTP object

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize NTP client
  timeClient.begin();

  // Decode Base32 secret
  int secretLength = base32decode((char*)base32Secret, decodedSecret, sizeof(decodedSecret));
  if (secretLength <= 0) {
    Serial.println("Error decoding Base32 secret");
    while (true); // Halt execution
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
    lastOTPTimestamp = millis();
  }

  // Other non-blocking code can be added here
}
