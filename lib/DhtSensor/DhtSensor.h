#ifndef DH_SENSOR_H
#define DH_SENSOR_H

#include <Arduino.h>
#include <DHT.h>

class DhtSensor {
public:
    /**
     * @brief Constructor to initialize the DHT sensor.
     * 
     * @param pin GPIO pin number where the DHT sensor is connected.
     * @param type Type of DHT sensor (e.g., DHT11, DHT22).
     */
    DhtSensor(uint8_t pin, uint8_t type);

    /**
     * @brief Initializes the DHT sensor.
     */
    void begin();

    /**
     * @brief Reads temperature and humidity from the sensor.
     * 
     * @param temperature Reference to store the read temperature.
     * @param humidity Reference to store the read humidity.
     * @return true If reading is successful.
     * @return false If reading fails.
     */
    bool readSensor(float& temperature, float& humidity);

private:
    DHT dht;
};

#endif // DH_SENSOR_H
