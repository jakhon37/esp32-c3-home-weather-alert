#include <Arduino.h>
#include <WiFi.h>

#include "Config.h"
#include "DhtSensor.h"
#include "EmailClient.h"
#include "WebGui.h"
#include "Utils.h"

// Instantiate classes
DhtSensor dhtSensor(DHTPIN, DHTTYPE);
EmailClient emailClient(SMTP_HOST, SMTP_PORT, EMAIL_USER, EMAIL_PASSWORD, EMAIL_RECIPIENT);
WebGui webGui;

// Variables to store current and previous sensor readings
float Temperature = 0.0;
float Humidity = 0.0;
float prevTemperature = 0.0;
float prevHumidity = 0.0;

// Timestamp to manage alert cooldown
unsigned long lastAlertTime = 0;

void setup() {
    Serial.begin(115200);
    delay(100);

    // Initialize DHT Sensor
    dhtSensor.begin();

    // Connect to WiFi
    Serial.println("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Initialize Web GUI
    webGui.begin();

    Serial.println("Setup complete.");
}

void loop() {
    // Handle incoming web clients
    webGui.handleClient();

    // Read sensor every 2 seconds
    static unsigned long lastReadTime = 0;
    unsigned long currentMillis = millis();
    if (currentMillis - lastReadTime >= 2000) { // 2 seconds interval
        lastReadTime = currentMillis;
        bool success = dhtSensor.readSensor(Temperature, Humidity);
        if (success) {
            // Update the web GUI with new sensor data
            webGui.setSensorData(Temperature, Humidity);
            Serial.printf("Temperature: %.1f°C, Humidity: %.1f%%\n", Temperature, Humidity);

            // Calculate differences from previous readings
            float tempDiff = Utils::calculateDifference(Temperature, prevTemperature);
            float humDiff = Utils::calculateDifference(Humidity, prevHumidity);

            // Check if differences exceed thresholds
            if ((tempDiff >= TEMP_CHANGE_THRESHOLD) || (humDiff >= HUM_CHANGE_THRESHOLD)) {
                unsigned long now = millis();
                if (now - lastAlertTime >= ALERT_COOLDOWN) {
                    lastAlertTime = now;
                    Serial.println("Sudden change detected. Sending email alert...");
                    bool emailSent = emailClient.sendAlert(Temperature, Humidity, tempDiff, humDiff);
                    if (emailSent) {
                        Serial.println("Email alert sent successfully.");
                    } else {
                        Serial.println("Failed to send email alert.");
                    }
                }
            }

            // Update previous readings
            prevTemperature = Temperature;
            prevHumidity = Humidity;
        } else {
            Serial.println("Failed to read from DHT sensor!");
        }
    }
}
