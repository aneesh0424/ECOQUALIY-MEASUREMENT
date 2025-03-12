#include <SoftwareSerial.h>

// SoftwareSerial pins for communication with NodeMCU
#define RX_PIN 10  // Arduino RX (connect to NodeMCU TX)
#define TX_PIN 11  // Arduino TX (connect to NodeMCU RX)

// Sensor pins
#define TdsSensorPin A0
#define TurbidityPin A1
#define CO_SensorPin A2
#define DustSensorPin A3

// Constants for TDS calculation
#define TDS_FACTOR 0.5  // TDS conversion factor
#define ADC_RESOLUTION 1024
#define VOLTAGE_REF 5.0
float temperature = 25;  // Temperature for TDS compensation

// Variables for sensor values
float tdsValue, turbidityValue, coConcentration, pm25Concentration;

// Initialize SoftwareSerial
SoftwareSerial nodeMCUSerial(RX_PIN, TX_PIN);

void setup() {
    // Start SoftwareSerial and hardware Serial
    nodeMCUSerial.begin(9600);
    Serial.begin(115200);

    // Initialize sensors
    pinMode(TdsSensorPin, INPUT);
    pinMode(TurbidityPin, INPUT);
    pinMode(CO_SensorPin, INPUT);
    pinMode(DustSensorPin, INPUT);

    // Warm-up time for CO sensor
    delay(30000);  // 30 seconds
}

void loop() {
    // --- Read sensor data ---

    // 1. TDS Sensor
    tdsValue = readTDS();

    // 2. Turbidity Sensor
    turbidityValue = readTurbidity();

    // 3. CO Sensor
    coConcentration = readCO();

    // 4. Dust Sensor (PM2.5)
    pm25Concentration = readDust();

    // Print sensor values to serial monitor
    Serial.print("TDS: ");
    Serial.print(tdsValue, 2);
    Serial.println(" ppm");

    Serial.print("Turbidity: ");
    Serial.print(turbidityValue, 2);
    Serial.println(" NTU");

    Serial.print("CO: ");
    Serial.print(coConcentration, 2);
    Serial.println(" ppm");

    Serial.print("PM2.5: ");
    Serial.print(pm25Concentration, 2);
    Serial.println(" µg/m³");

    // --- Send data to NodeMCU via SoftwareSerial ---
    nodeMCUSerial.write((byte*)&tdsValue, sizeof(tdsValue));
    nodeMCUSerial.write((byte*)&turbidityValue, sizeof(turbidityValue));
    nodeMCUSerial.write((byte*)&coConcentration, sizeof(coConcentration));
    nodeMCUSerial.write((byte*)&pm25Concentration, sizeof(pm25Concentration));

    Serial.println("Data sent to NodeMCU");
    Serial.println("--------------------");

    delay(2000);  // Wait 2 seconds before the next reading
}

// Function to calculate TDS value
float readTDS() {
    int sensorValue = analogRead(TdsSensorPin);
    float voltage = (sensorValue * VOLTAGE_REF) / ADC_RESOLUTION;
    float compensationFactor = 1.0 + 0.02 * (temperature - 25.0);  // Temperature compensation
    float compensationVoltage = voltage / compensationFactor;
    float tds = (compensationVoltage * TDS_FACTOR) * 1000;  // Convert to ppm
    return tds;
}

// Function to calculate Turbidity value
float readTurbidity() {
    int turbidityRaw = analogRead(TurbidityPin);
    return (turbidityRaw * (VOLTAGE_REF / ADC_RESOLUTION) * 3.0) * 8;  // Replace with your formula
}

// Function to calculate CO concentration
float readCO() {
    int sensorValue = analogRead(CO_SensorPin);
    float voltage = sensorValue * (VOLTAGE_REF / 1023.0);
    float sensorResistance = (5.0 - voltage) * 10.0 / voltage;  // Assuming RLOAD = 10 kOhms
    return 9.83 * pow((sensorResistance / 9.83), -2.3);  // Adjust based on MQ sensor datasheet
}

// Function to calculate PM2.5 concentration
float readDust() {
    delayMicroseconds(280);  // Sampling time
    int voMeasured = analogRead(DustSensorPin);
    delayMicroseconds(40);  // Delta time
    delayMicroseconds(9680);  // Sleep time
    float calcVoltage = voMeasured * (VOLTAGE_REF / 1024.0);
    float pm25 = (calcVoltage - 0.3) * (12 - 4) / (1.5 - 0.3) + 6;
    return (pm25 < 0) ? 0 : pm25;  // Clamp to zero if negative
}