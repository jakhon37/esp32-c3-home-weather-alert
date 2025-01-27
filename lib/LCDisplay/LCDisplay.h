#ifndef DISPLAY_H
#define DISPLAY_H

#include <LiquidCrystal_I2C.h>

/**
 * @brief The Display class encapsulates functionalities to interact with an I2C LCD.
 */
class Display {
public:
    /**
     * @brief Constructor for the Display class.
     * 
     * @param address I2C address of the LCD (default is 0x27).
     * @param cols Number of columns of the LCD (default is 16).
     * @param rows Number of rows of the LCD (default is 2).
     */
    Display(uint8_t address = 0x27, uint8_t cols = 16, uint8_t rows = 2, uint8_t sda_pin = 8, uint8_t scl_pin = 9);

    /**
     * @brief Initialize the LCD display.
     */
    void init();

    /**
     * @brief Display temperature and humidity on the LCD.
     * 
     * @param temperature Current temperature in °C.
     * @param humidity Current humidity in %.
     */
    void displayTempHum(float temperature, float humidity);

private:
    LiquidCrystal_I2C lcd_; ///< LiquidCrystal_I2C object for LCD control.
    uint8_t sda_pin_;      ///< SDA pin number.
    uint8_t scl_pin_;      ///< SCL pin number.
};

#endif // DISPLAY_H
