#include "DhtSensor.h"

DhtSensor::DhtSensor(uint8_t pin, uint8_t type) : dht(pin, type) {}

void DhtSensor::begin() {
    dht.begin();
}

bool DhtSensor::readSensor(float& temperature, float& humidity) {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    if (isnan(temperature) || isnan(humidity)) {
        return false;
    }
    return true;
}
