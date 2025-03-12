# Ecoquality Measurement System

## Overview
The **Ecoquality Measurement System** is an IoT-based solution designed to monitor environmental quality by measuring key air and water quality parameters. The system collects real-time data using sensors, transmits it to the cloud, and provides insights through a mobile app.

## Features
- **Air Quality Monitoring:** Measures **CO (Carbon Monoxide)** and **PM2.5 (Particulate Matter)** levels.
- **Water Quality Monitoring:** Measures **Turbidity** and **Total Dissolved Solids (TDS)**.
- **Master-Slave Communication:** Utilizes an **Arduino Uno (Master)** and **NodeMCU (Slave)** setup.
- **Cloud Storage:** Sends sensor data to **Firebase** for real-time storage and analysis.
- **Mobile App Integration:** Displays real-time environmental data with graphical visualization.
- **Hardware & Software Combination:** A seamless blend of hardware components and software tools ensures accurate monitoring.

## System Architecture
The system consists of:
1. **Sensors**: CO, PM2.5, Turbidity, TDS
2. **Microcontrollers**:
   - **Arduino Uno** (Master): Collects data and sends it to NodeMCU.
   - **NodeMCU ESP8266** (Slave): Handles cloud communication.
3. **Cloud Storage**: Firebase for data storage.
4. **Mobile App**: Developed using **MIT App Inventor** for data visualization.

## Technologies Used
- **Hardware:** Arduino Uno, NodeMCU ESP8266, CO Sensor, PM2.5 Sensor, Turbidity Sensor, TDS Sensor.
- **Software:** Arduino IDE, Firebase, MIT App Inventor.
- **Communication:** I2C, WiFi (ESP8266), Firebase Realtime Database.

## Installation & Setup
### Hardware Setup
1. Connect sensors to the Arduino Uno.
2. Establish a serial connection between Arduino Uno and NodeMCU.
3. Power up the system and verify sensor data using the Arduino Serial Monitor.

### Software Setup
1. **Arduino IDE**: Install necessary libraries and upload the Arduino sketch.
2. **Firebase Setup**:
   - Create a Firebase project.
   - Enable Realtime Database.
   - Obtain Firebase API credentials.
3. **Mobile App**:
   - Import the MIT App Inventor project.
   - Configure Firebase API keys.
   - Deploy the app on an Android device.

## Usage
1. Power on the system.
2. Data from sensors will be transmitted to Firebase.
3. Use the mobile app to view real-time air and water quality parameters.
4. Analyze trends and take necessary actions for environmental safety.

## Future Enhancements
- Adding more environmental parameters like **humidity** and **temperature**.
- Implementing **machine learning** for anomaly detection.
- Expanding support for **multiple sensor nodes** in a distributed environment.

## License
This project is licensed under the MIT License. See `LICENSE` for details.

## Contact
For any queries or contributions, feel free to reach out:
- **Email**: aneeshvajrapu@gmail.com
- **GitHub**: aneesh0424
