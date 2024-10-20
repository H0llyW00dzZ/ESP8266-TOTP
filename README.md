# ESP8266 Arduino TOTP (Expermintal)

> [!WARNING]  
> This is experimental and might not work as expected.

## Overview

This project demonstrates how to implement a Time-based One-Time Password (TOTP) generator using an ESP8266 board with Arduino. The system connects to Wi-Fi, synchronizes time using an NTP client, and generates OTPs based on a shared secret.

## Features

- Connects to Wi-Fi and synchronizes with an NTP server.
- Decodes a Base32 encoded secret for TOTP generation.
- Generates and displays a TOTP every 30 seconds.

## Usage

1. **Configure Wi-Fi Credentials**:
   - Open the sketch and replace `your_SSID` and `your_PASSWORD` with your Wi-Fi credentials.

2. **Set Base32 Secret**:
   - Replace `BASE32EncodedSecret` with your actual Base32 encoded secret.

3. **Upload Code**:
   - Connect your ESP8266 board to your computer.
   - Select the correct board and port in the Arduino IDE.
   - Upload the code to the board.

4. **Monitor Output**:
   - Open the Serial Monitor in Arduino IDE to view the generated OTPs.

## License

This project is licensed under the [BSD 3-Clause License](LICENSE).

## Acknowledgments

- Based on libraries and examples from the Arduino community.
