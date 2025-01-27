#ifndef WEBGUI_H
#define WEBGUI_H

#include <WebServer.h>

class WebGui {
public:
    /**
     * @brief Constructor to initialize the WebGui.
     * 
     * @param port Port number for the web server (default is 80).
     */
    WebGui(uint16_t port = 80);

    /**
     * @brief Initializes and starts the web server.
     */
    void begin();

    /**
     * @brief Handles incoming client requests.
     */
    void handleClient();

    /**
     * @brief Updates the current sensor data to be displayed on the web GUI.
     * 
     * @param temperature Current temperature.
     * @param humidity Current humidity.
     */
    void setSensorData(float temperature, float humidity);

private:
    WebServer server;
    float currentTemperature;
    float currentHumidity;

    /**
     * @brief Generates the HTML content for the web GUI.
     * 
     * @return String HTML content.
     */
    String generateHTML();

    // HTTP request handlers
    void handleRoot();
    void handleData();
    void handleNotFound();
};

#endif // WEBGUI_H
