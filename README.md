# bajathopter
A multipart software suite that provides live information and data aquisition for nearly any sensor, initially designed for SAE Baja.

## Table of Contents
1. [Project Overview](#project-overview)
2. [Components](#components)
   - CORE
   - BANSHEE
   - BEACON
3. [Getting Started](#getting-started)

## Project Overview
Bajathopter is a three-part software suite that is design to record, transmit, and display live data from a wide range of sensors, specifically made with the SAE Baja competition. 
- **CORE**: This module runs on custom-made esp32c3 based boards designed by Quinn Williams. These boards are on the vehicle, connected to the sensors. They have a focus on high throughput at the lowest power consumption, as they are battery powered. Using wifi, they transmit live data at specified intervals to the **BANSHEE** module, while on another interval **CORE** will record and store data for later parsing (when not battery powered).
- **BANSHEE**: This module utilizes another esp32c3. The board functions similarly to a router, receiving instructions from the **BEACON** when necessary, then relaying those instructions to the specified **CORE** board. The **CORE** boards also send the **BANSHEE** data at intervals to be transmitted via long range radio (LoRaWAN) to the **BEACON**, for live data. **BANSHEE** also functions as an Mqtt broker, which is how the communcation protocol is handled.
- **BEACON**: This module is not desgined for a specific platform, as it does not need the fine grained control both **CORE** and **BANSHEE** need for power management. Initially a Raspberry Pi 3B is used. **BEACON** hosts a web server that can be access both by the device hosting it, and other nearby devices. The web server displays live diagnositic data from the **CORE** boards. It will receive data from **BANSHEE** through its own radio transiever. **BEACON** also has capabilities to send instructions to the vehicle, allowing the operator some control of the vehicle, if equipped.

## Getting Started

### Hardware requirements
The minimum requirement for this project to work is two esp32-based boards, as well as a third device to host the web server. The program is split into three parts, **CORE** and **BANSHEE** are flashed to the esp32 boards and **BEACON** is the web server.

### CORE and BANSHEE
First, the Espressif IDF build tool is required, the instructions for installing can be found here: [Espressif IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32c3/get-started/index.html). If using VS-Code, the [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/v4.2.3/esp32/get-started/vscode-setup.html) extension can be installed.

With the VS-Code extension, the configuration options can be set on the bottom toolbar (shown below). The code can be flashed using the *ESP-IDF: Build, Flash, and Monitor* option, which compiles, flashes, and opens logs for the connected board.

![image](https://github.com/user-attachments/assets/5c2f01ac-1af8-4d84-99dd-23ca432b60f7)


The **CORE** board was developed to run on a custom board using an ESP32-C3 chip, while **BANSHEE** was built for an ESP32-S3-DevKitC-1 board. The IDF tool must be configured for these architectures with `idf.py set-target <board>`.

With the build tool installed and active, the code for both modules can be flash by running `idf.py flash` in their main directory with an esp32 board plugged in via USB. No configuration is required.


### BEACON
The **BEACON** web server can be run on any PC that has Rust installed. The installation script can be found here: [Rust Install](https://www.rust-lang.org/tools/install).

After Rust is installed, the `cargo` tool must be used to install development dependencies. Within the **BEACON** directory, the command `cargo sqlx install sqlx --features sqlite` should be run, installing the CLI tool for managing database connections. Once this is completed, the commands `cargo sqlx db create` and `cargo sqlx migrate run` should be run **within** the *src* directory, creating the database. Then, the web server can be ran using `cargo run`. This may require admin permissions, as it runs on `0.0.0.0:80` (all interfaces). The web server can then be accessed at `http://localhost`.

The query parameter *page* can be set to page through the data, offsetting the results shown. For example, *http://localhost?page=25* would show readings 25-50.

#### Connecting to the AP
The **BEACON** board must act as a web server and MQTT broker for the project. It must be connected to the Wi-Fi network hosted on **BANSHEE**. This network uses `test_wifi_network` as the *SSID* and `test_password` as the password. **BEACON** should be configured to use a static IP address of `192.168.4.254`, and a default gateway of `192.168.4.1`.

#### MQTT Broker
As this project uses MQTT, an MQTT broker must be available. Due to constraints, this broker must be hosted on **BEACON** itself. If using Linux, the *mosquitto* broker can be used.

### bajathopter?
[This is why](https://www.cia.gov/legacy/museum/artifact/insectothopter/)
