#include "LCDisplay.h"
#include <WiFi.h>

/**
 * @brief Constructor implementation.
 * Initializes the LiquidCrystal_I2C object with the provided address, columns, and rows.
 * LCD_ADDRESS, LCD_COLS, LCD_ROWS, SDA_PIN, SDL_PIN
 */
Display::Display(uint8_t address, uint8_t cols, uint8_t rows, uint8_t sda_pin, uint8_t scl_pin) 
: lcd_(address, cols, rows), sda_pin_(sda_pin), scl_pin_(scl_pin) {
    // Constructor body can be empty as initialization is handled in the initializer list.
}

/**
 * @brief Initialize the LCD.
 * Sets up the LCD and turns on the backlight.
 */
void Display::init() {
    // Wire.begin(sda_pin_, scl_pin_);     // Initialize I2C communication   
    lcd_.init();                      // Initialize the LCD
    lcd_.backlight();                 // Turn on the backlight
    lcd_.clear();                     // Clear any residual content
    lcd_.setCursor(0, 0);             // Set cursor to first line
    lcd_.print("ESP32 Weather");      // Initial message
    lcd_.setCursor(0, 1);             // Set cursor to second line
    lcd_.print("Initializing...");
}

/**
 * @brief Display temperature and humidity on the LCD.
 * Clears the display and updates both lines with current readings.
 * 
 * @param temperature Current temperature in °C.
 * @param humidity Current humidity in %.
 */
void Display::displayTempHum(float temperature, float humidity) {
    lcd_.clear();
    if (temperature == 0.0 && humidity == 0.0) {
        lcd_.setCursor(0, 0);             // First line
        lcd_.print("Failed to read");
        lcd_.setCursor(0, 1);             // Second line
        lcd_.print("from sensor!");
        return;
    }
    else {
            
        // Display Temperature on the first line
        lcd_.setCursor(0, 0);             // First line
        lcd_.print("Temp: ");
        lcd_.print(temperature, 1);       // One decimal place
        lcd_.print((char)223);            // Degree symbol
        lcd_.print("C");
        
        // Display Humidity on the second line
        lcd_.setCursor(0, 1);             // Second line
        lcd_.print("Hum: ");
        lcd_.print(humidity, 1);          // One decimal place
        lcd_.print("%");
    }

}
