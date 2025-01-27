
#include "EmailClient.h"
#include <ESP_Mail_Client.h>
#include <time.h>

EmailClient* EmailClient::instance = nullptr;

EmailClient::EmailClient(const char* smtpHost, uint16_t smtpPort,
                        const char* userEmail, const char* userPassword,
                        const char* recipientEmail, const char* userDomain,
                        float tempHigh, float tempLow,  // New parameters
                        float humHigh, float humLow) 
{
    // Initialize thresholds
    tempHighThreshold = tempHigh;
    tempLowThreshold = tempLow;
    humHighThreshold = humHigh;
    humLowThreshold = humLow;

    // Rest of existing constructor code...
    instance = this;
    
    // Configure SMTP session
    config.server.host_name = smtpHost;
    config.server.port = smtpPort;
    config.login.email = userEmail;
    config.login.password = userPassword;
    config.login.user_domain = userDomain;


    // config.login.security = esp_mail_smtp_security_tls;  // Enable TLS
    
    configureTimeSettings();  // Configure NTP and timezone

    recipient = recipientEmail;

    // Initialize SMTP
    smtp.debug(1);
    smtp.callback(smtpCallback);
    smtp.setTCPTimeout(10);
}

void EmailClient::configureTimeSettings() {
    // Configure NTP and timezone in session config
    config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
    config.time.gmt_offset = 9;       // Seoul UTC+9
    config.time.day_light_offset = 0; // No daylight saving
}

void EmailClient::smtpCallback(SMTP_Status status) {
    /* Keep existing callback implementation */
}

bool EmailClient::sendAlert(float temperature, float humidity, 
                           float tempChange, float humChange, int sent_welcome) {
    if (!smtp.connect(&config)) {
        Serial.printf("Connection error: %s\n", smtp.errorReason().c_str());
        return false;
    }

    if (!smtp.isLoggedIn()) {
        Serial.println("SMTP not logged in");
        return false;
    }

    SMTP_Message message;
    
    // Configure message headers
    message.sender.name = F("Weather Station");
    message.sender.email = config.login.email;
    message.addRecipient(F("System Admin"), recipient);

    // Set timestamp configuration
    message.timestamp.tag = "%DATE%";  // Use built-in placeholder
    message.timestamp.format = "%B %d, %Y %H:%M:%S %Z";

    // Check for initial startup condition (all zeros)
    if (sent_welcome == 0) {
        message.subject = F("System Startup Successful");
        
        String body = String(F("Weather Station Initialization Complete\n\n"));
        body += String(F("System Components Online:\n"));
        body += String(F("- WiFi Connection Established\n"));
        body += String(F("- Sensor Initialization Complete\n"));
        body += String(F("- Email Service Ready\n"));
        body += String(F("\nCurrent Conditions:\n"));
        body += String(F("Temperature: ")) + String(temperature, 1) + String(F("°C\n"));
        body += String(F("Humidity: ")) + String(humidity, 1) + String(F("%\n"));
        body += String(F("\nNext Update in 60 Seconds\n"));
        body += String(F("\nSystem Start Time: %DATE%"));
        // body += String(F("\nSent: %DATE%\n"));  // Use %DATE% placeholder\

        message.text.content = body.c_str();
        message.priority = esp_mail_smtp_priority_normal;
    } else {
        // Normal alert message
        String subject = String(F("ALERT: "));
        if (temperature > tempHighThreshold) subject += F("High Temp");
        else if (temperature < tempLowThreshold) subject += F("Low Temp");
        if (humidity > humHighThreshold) subject += F(" & High Humidity");
        else if (humidity < humLowThreshold) subject += F(" & Low Humidity");
        message.subject = subject.c_str();

        String body = String(F("Environmental Alert!\n\n"));
        body += String(F("Current Conditions:\n"));
        body += String(F("Temperature: ")) + String(temperature, 1) + String(F("°C\n"));
        body += String(F("Humidity: ")) + String(humidity, 1) + String(F("%\n\n"));
        
        body += String(F("Recent Changes:\n"));
        body += String(F("Temp Change: ")) + String(tempChange, 1) + String(F("°C\n"));
        body += String(F("Humidity Change: ")) + String(humChange, 1) + String(F("%\n\n"));


        body += String(F("Threshold Exceeded In:\n"));
        if (temperature > tempHighThreshold) 
            body += String(F("- Temperature (Above ")) + String(tempHighThreshold, 1) + F("°C)\n");
        if (temperature < tempLowThreshold) 
            body += String(F("- Temperature (Below ")) + String(tempLowThreshold, 1) + F("°C)\n");
        if (humidity > humHighThreshold) 
            body += String(F("- Humidity (Above ")) + String(humHighThreshold, 1) + F("%)\n");
        if (humidity < humLowThreshold) 
            body += String(F("- Humidity (Below ")) + String(humLowThreshold, 1) + F("%)\n");


        body += String(F("\nAlert Time: %DATE%"));
            // Then in your email content body:
        // body += String(F("\nSent: %DATE%\n"));  // Use %DATE% placeholder

        message.text.content = body.c_str();
        message.priority = esp_mail_smtp_priority_high;
    }


    if (!MailClient.sendMail(&smtp, &message)) {
        Serial.printf("Send failed: %s\n", smtp.errorReason().c_str());
        return false;
    }

    return true;
}


