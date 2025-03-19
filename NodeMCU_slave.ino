#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

#include <Wire.h>

// Firebase and Wi-Fi credentials
#define WIFI_SSID "wifi name"                          
#define WIFI_PASSWORD "wifi password"                  
#define DATABASE_URL "firebase link"
#define API_KEY "your API key"               
#define USER_EMAIL "your gmail"            
#define USER_PASSWORD "your password"               

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

String receivedData = ""; // Buffer for incoming data from Arduino
unsigned long lastSendTime = 0;

// Function to parse received data
bool parseData(const String& data, float& tds, float& turbidity, float& co, float& pm25) {
    int tdsIndex = data.indexOf("TDS:");
    int turbidityIndex = data.indexOf("Turbidity:");
    int coIndex = data.indexOf("CO:");
    int pm25Index = data.indexOf("PM2.5:");

    if (tdsIndex == -1 || turbidityIndex == -1 || coIndex == -1 || pm25Index == -1) {
        return false;
    }

    tds = data.substring(tdsIndex + 4, data.indexOf(",", tdsIndex)).toFloat();
    turbidity = data.substring(turbidityIndex + 10, data.indexOf(",", turbidityIndex)).toFloat();
    co = data.substring(coIndex + 3, data.indexOf(",", coIndex)).toFloat();
    pm25 = data.substring(pm25Index + 6).toFloat();

    return true;
}

// I²C data reception handler
void receiveData(int byteCount) {
    while (Wire.available()) {
        char c = Wire.read();
        receivedData += c;
    }
    Serial.println("Received data: " + receivedData);
}

void setup() {
    Serial.begin(115200);
    Wire.begin(8); // Initialize I²C as slave with address 8
    Wire.onReceive(receiveData);

    // Connect to Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi connected!");

    // Configure Firebase
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    Serial.println("Firebase connected!");
}

void loop() {
    // Process received data
    if (receivedData.length() > 0) {
        float tds, turbidity, co, pm25;

        if (parseData(receivedData, tds, turbidity, co, pm25)) {
            Serial.println("Parsed Data:");
            Serial.println("TDS: " + String(tds));
            Serial.println("Turbidity: " + String(turbidity));
            Serial.println("CO: " + String(co));
            Serial.println("PM2.5: " + String(pm25));

            // Upload data to Firebase
            if (Firebase.setFloat(fbdo, "/SensorData/TDS", tds)) {
                Serial.println("TDS uploaded!");
            } else {
                Serial.println("Failed to upload TDS: " + fbdo.errorReason());
            }

            if (Firebase.setFloat(fbdo, "/SensorData/Turbidity", turbidity)) {
                Serial.println("Turbidity uploaded!");
            } else {
                Serial.println("Failed to upload Turbidity: " + fbdo.errorReason());
            }

            if (Firebase.setFloat(fbdo, "/SensorData/CO", co)) {
                Serial.println("CO uploaded!");
            } else {
                Serial.println("Failed to upload CO: " + fbdo.errorReason());
            }

            if (Firebase.setFloat(fbdo, "/SensorData/PM2.5", pm25)) {
                Serial.println("PM2.5 uploaded!");
            } else {
                Serial.println("Failed to upload PM2.5: " + fbdo.errorReason());
            }
        } else {
            Serial.println("Error parsing data.");
        }

        // Clear received data buffer
        receivedData = "";
    }

    delay(1000);
}
