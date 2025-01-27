#ifndef CONFIG_H
#define CONFIG_H

#include <ESP_Mail_Client.h>  // Add this line to access port constants

// ===========================
// WiFi Configuration
// ===========================
const char* WIFI_SSID = "Your_WiFi_SSID";
const char* WIFI_PASSWORD = "Your_WiFi_Password";

// ===========================
// Email Configuration
// ===========================
const char* SMTP_HOST = "smtp.gmail.com";
const uint16_t SMTP_PORT = 587; // esp_mail_smtp_port_587;  
const char* EMAIL_USER = "your_email@gmail.com";
const char* EMAIL_PASSWORD = "your_email_password_or_app_specific_password";
const char* USER_DOMAIN = "gmail.com"; // change it according to your email domain

const char* EMAIL_RECIPIENT = "recipient_email@example.com";

// ===========================
// DHT Sensor Configuration
// ===========================
#define DHTPIN 10
#define DHTTYPE DHT22

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
