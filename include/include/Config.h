#ifndef CONFIG_H
#define CONFIG_H

#include <ESP_Mail_Client.h>  // Add this line to access port constants

// ===========================
// WiFi Configuration
// ===========================
const char* WIFI_SSID = "U+NetEFDA";
const char* WIFI_PASSWORD = "5D2A1A9EM#";

// ===========================
// Email Configuration
// ===========================
const char* SMTP_HOST = "smtp.gmail.com";
const uint16_t SMTP_PORT = 587; // esp_mail_smtp_port_587;  
const char* EMAIL_USER = "jakhon@gachon.ac.kr";
const char* EMAIL_PASSWORD = "#a123123";
const char* USER_DOMAIN = "gachon.ac.kr";

const char* EMAIL_RECIPIENT = "jakhon37@aivar.kr";



// ===========================
// DHT Sensor Configuration
// ===========================
#define DHTPIN 10
#define DHTTYPE DHT22

// ===========================
// LCD Display Configuration
// ===========================
#define LCD_ADDRESS 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

#define SDA_PIN 8
#define SCL_PIN 9

// ===========================
// Alert Thresholds
// ===========================
// In Config.h
const float TEMP_HIGH_THRESHOLD = 26.5;
const float TEMP_LOW_THRESHOLD = 22.0;
const float HUM_HIGH_THRESHOLD = 99.8;
const float HUM_LOW_THRESHOLD = 40.0;


const float TEMP_CHANGE_THRESHOLD = 1.0;       // °C
const float HUM_CHANGE_THRESHOLD = 5.0;        // %
const unsigned long ALERT_COOLDOWN = 60000;     // 1 minute in milliseconds

#endif // CONFIG_H
