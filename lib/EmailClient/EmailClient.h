#ifndef EMAILCLIENT_H
#define EMAILCLIENT_H

#include <ESP_Mail_Client.h>

class EmailClient {
public:
    EmailClient(const char* smtpHost, uint16_t smtpPort, 
               const char* userEmail, const char* userPassword,
               const char* recipientEmail, const char* userDomain,
                float tempHigh, float tempLow,  // Add threshold parameters
               float humHigh, float humLow);
               
    bool sendAlert(float temperature, float humidity, 
                  float tempChange, float humChange, int sent_welcome);

private:
    // Threshold member variables
    float tempHighThreshold;
    float tempLowThreshold;
    float humHighThreshold;
    float humLowThreshold;


    void configureTimeSettings();

    static EmailClient* instance; // Static reference to instance
    static void smtpCallback(SMTP_Status status); // Static callback

    Session_Config config;
    SMTPSession smtp;
    const char* recipient;
};

#endif