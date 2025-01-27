#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

class Utils {
public:
    /**
     * @brief Calculates the absolute difference between two float values.
     * 
     * @param current Current value.
     * @param previous Previous value.
     * @return float Absolute difference.
     */
    static float calculateDifference(float current, float previous);
};

#endif // UTILS_H
