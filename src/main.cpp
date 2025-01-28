#include <Arduino.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

#include "Config.h"
#include "DhtSensor.h"
#include "EmailClient.h"
#include "LCDisplay.h"
#include "OLEDDisplay.h"
#include "WebGui.h"
#include "Utils.h"


#include <time.h>
const char* NTP_SERVER = "pool.ntp.org";
const long GMT_OFFSET_SEC = 32400;  // Korea Standard Time (UTC+9)
const int DAYLIGHT_OFFSET_SEC = 0;

// Instantiate OLEDDisplay
OLEDDisplay oled(OLED_WIDTH, OLED_HEIGHT, OLED_SDA_PIN, OLED_SCL_PIN, OLED_RESET_PIN);

// Instantiate classes
DhtSensor dhtSensor(DHTPIN, DHTTYPE);
// EmailClient emailClient(SMTP_HOST, SMTP_PORT, EMAIL_USER, EMAIL_PASSWORD, EMAIL_RECIPIENT, USER_DOMAIN);
EmailClient emailClient(
    SMTP_HOST, 
    SMTP_PORT,
    EMAIL_USER,
    EMAIL_PASSWORD,
    EMAIL_RECIPIENT,
    USER_DOMAIN,
    TEMP_HIGH_THRESHOLD,  // From Config.h
    TEMP_LOW_THRESHOLD,
    HUM_HIGH_THRESHOLD,
    HUM_LOW_THRESHOLD
);

WebGui webGui;

// Instantiate Display
Display displayLCD(LCD_ADDRESS, LCD_COLS, LCD_ROWS, SDA_PIN, SCL_PIN); // I2C address 0x27, 16 columns, 2 rows
// Display displayLCD(0x27, 16, 2); // I2C address 0x27, 16 columns, 2 rows


// Variables to store current and previous sensor readings
float Temperature = 0.0;
float Humidity = 0.0;
float prevTemperature = 0.0;
float prevHumidity = 0.0;
int sent_welcome = 0;
float tempDiff = 0;
float humDiff = 0;
// Timestamp to manage alert cooldown
unsigned long lastAlertTime = 0;
// WiFi Connection Timeout (e.g., 30 seconds)
const unsigned long WIFI_TIMEOUT = 30000; // 30,000 milliseconds
unsigned long wifiStartTime = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Starting     ESP32 Weather Alert System...");

    delay(100);

    // Initialize OLED Display
    Serial.println("Initializing OLED Display...");
    if (oled.init()) {
        Serial.println("OLED Display Initialized Successfully.");
    } else {
        Serial.println("Failed to Initialize OLED Display.");
        // Optionally, halt the system or attempt re-initialization
        while (1);
    }
    // Initialize LCD Display
    displayLCD.init();
    Serial.println("LCD Display Initialized.");

    // Initialize DHT Sensor
    dhtSensor.begin();
    Serial.println("DHT Sensor Initialized.");

    // Connect to WiFi
    Serial.println("Connecting to WiFi...");
    oled.displayStatus("Connecting WiFi", "Please Wait...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    wifiStartTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");

        // Optionally, update OLED to show connection progress
        static int dotCount = 0;
        dotCount = (dotCount + 1) % 4;
        char statusMessage[32];
        snprintf(statusMessage, sizeof(statusMessage), "Connecting WiFi");
        oled.displayStatus(statusMessage, "Please Wait...");
        // Implement timeout
        if (millis() - wifiStartTime > WIFI_TIMEOUT) {
            Serial.println("\nWiFi connection timed out!");
            oled.displayStatus("WiFi Timeout", "Retrying...");
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Retry
            wifiStartTime = millis();
        }
    }
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
   // Update OLED with WiFi connection status
    String ipAddress = String(WiFi.localIP());
    oled.displayStatus("WiFi Connected", ("IP: " + ipAddress).c_str());
    delay(2000); // Display IP address for 2 seconds

    // Initialize Time (NTP)
    Serial.println("Syncing time with NTP server...");
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        oled.displayStatus("Time Error", "Failed to sync");
    } else {
        char timeString[32];
        strftime(timeString, sizeof(timeString), "%A, %B %d %Y %H:%M:%S", &timeinfo);
        Serial.println(timeString);
        oled.displayStatus("Current Time:", timeString);
        delay(2000); // Display time for 2 seconds
        oled.clear();
    }
    // Wait for time synchronization
    time_t now = 0;
    while (time(&now) < ESP_MAIL_CLIENT_VALID_TS) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nTime synchronized!");


        
    // Initialize Web GUI
    Serial.println("Initializing Web GUI...");
    webGui.begin();
    Serial.println("Web GUI Initialized.");



    // // Initialize Email Client
    // Serial.println("Initializing Email Client...");
    // bool emailInit = emailClient.init();
    // if (emailInit) {
    //     Serial.println("Email Client Initialized Successfully.");
    //     oled.displayStatus("Email", "Client Ready");
    // } else {
    //     Serial.println("Failed to Initialize Email Client.");
    //     oled.displayStatus("Email Error", "Client Failed");
    // }

    Serial.println("Setup Complete.");

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
        if (sent_welcome == 0) {
                bool emailSent = emailClient.sendAlert(Temperature, Humidity, tempDiff, humDiff, sent_welcome);
                Serial.println("Welcome email sent successfully.");
                oled.displayStatus("Email", "Welcome email sent successfully.");
                sent_welcome = 1;
        }
        if (success) {
            // Update the web GUI with new sensor data
            webGui.setSensorData(Temperature, Humidity);
            Serial.printf("Temperature: %.1f°C, Humidity: %.1f%%\n", Temperature, Humidity);
            // Update the LCD with new sensor data
            displayLCD.displayTempHum(Temperature, Humidity);

            // Update the OLED with new sensor data
            oled.displayTempHum(Temperature, Humidity);

            // Calculate differences from previous readings
            float tempDiff = Utils::calculateDifference(Temperature, prevTemperature);
            float humDiff = Utils::calculateDifference(Humidity, prevHumidity);
            Serial.printf("Temperature change: %.1f°C, Humidity change: %.1f%%\n", tempDiff, humDiff);

            // Check if differences exceed thresholds
            if ((tempDiff >= TEMP_CHANGE_THRESHOLD) || (humDiff >= HUM_CHANGE_THRESHOLD) || (Temperature > TEMP_HIGH_THRESHOLD) || (Temperature < TEMP_LOW_THRESHOLD) || (Humidity > HUM_HIGH_THRESHOLD)  || (Humidity < HUM_LOW_THRESHOLD )) {
                unsigned long now = millis();
                if (now - lastAlertTime >= ALERT_COOLDOWN) {
                    lastAlertTime = now;
                    Serial.println("Sudden change detected. Sending email alert...");
                    bool emailSent = emailClient.sendAlert(Temperature, Humidity, tempDiff, humDiff, sent_welcome);
                    if (emailSent) {
                        Serial.println("Email alert sent successfully.");
                        oled.displayStatus("Alert Sent", "Check Email");
                    } else {
                        Serial.println("Failed to send email alert.");
                        oled.displayStatus("Email Error", "Failed to Send");
                    }
                }
                // Update previous readings
                prevTemperature = Temperature;
                prevHumidity = Humidity;
            }

            // // Update previous readings
            // prevTemperature = Temperature;
            // prevHumidity = Humidity;
        } else {
            Serial.println("Failed to read from DHT sensor!");
            displayLCD.displayTempHum(0.0, 0.0); // Optional: Display error message
            oled.displayStatus("Sensor Error", "Read Failed");

        }
    }
}
