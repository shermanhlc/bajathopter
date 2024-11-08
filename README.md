# bajathopter
A multipart software suite that provides live information and data aquisition for nearly any sensor, initially designed for SAE Baja.

## Table of Contents
1. [Project Overview](#project-overview)
2. [Components](#components)
   - CORE
   - BANSHEE
   - BEACON
3. [Getting Started](#getting-started)
4. [Installation](#installation)
5. [Usage](#usage)
6. [Contributing](#contributing)
7. [License](#license)
8. [Changelog](#changelog)
9. [Contact](#contact)

## Project Overview
Bajathopter is a three-part software suite that is design to record, transmit, and display live data from a wide range of sensors, specifically made with the SAE Baja competition. 
- **CORE**: This module runs on custom-made esp32c3 based boards designed by Quinn Williams. These boards are on the vehicle, connected to the sensors. They have a focus on high throughput at the lowest power consumption, as they are battery powered. Using BluetoothLE, they transmit live data at specified intervals to the **BANSHEE** module, while on another interval **CORE** will record and store data for later parsing (when not battery powered).
- **BANSHEE**: This module utilizes another _insert board type here_. The board functions similarly to a router, receiving instructions from the **BEACON** when necessary, then relaying those instructions to the specified **CORE** board. The **CORE** boards also send the **BANSHEE** data at intervals to be transmitted via long range radio to the **BEACON**, for live data. **BANSHEE** also functions as an Mqtt broker, which is how the communcation protocol is handled.
- **BEACON**: This module is not desgined for a specific platform, as it does not need the fine grained control both **CORE** and **BANSHEE** need for power management. Initially a Raspberry Pi 3B is used. **BEACON** hosts a web server that can be access both by the device hosting it, and other nearby devices. The web server displays live diagnositic data from the **CORE** boards. It will receive data from **BANSHEE** through its own radio transiever. **BEACON** also has capabilities to send instructions to the vehicle, allowing the operator some control of the vehicle, if equipped.

## Getting Started



### bajathopter?
[This is why](https://www.cia.gov/legacy/museum/artifact/insectothopter/)
