#include "OLEDDisplay.h"
#include <Wire.h>
#include <Fonts/FreeSerif9pt7b.h>

/**
 * @brief Constructor implementation.
 * Initializes the Adafruit_SSD1306 object with the provided width, height, and reset pin.
 */
OLEDDisplay::OLEDDisplay(uint16_t width, uint16_t height, uint8_t sda_pin, uint8_t scl_pin, int8_t reset_pin)
    : display_(width, height, &Wire, reset_pin), width_(width), height_(height), sda_pin_(sda_pin), scl_pin_(scl_pin), reset_pin_(reset_pin) {
    // Constructor body can be empty as initialization is handled in the initializer list.
}

/**
 * @brief Initialize the OLED display.
 * Sets up the I2C communication and initializes the display.
 * 
 * @return true if initialization is successful, false otherwise.
 */
bool OLEDDisplay::init() {
    Serial.println("Initializing I2C for OLED...");
    Wire.begin(sda_pin_, scl_pin_); // Initialize I2C with custom SDA and SCL pins
    Serial.println("I2C Initialized.");

    Serial.println("Initializing SSD1306 OLED...");
    if(!display_.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for most SSD1306 OLEDs
        Serial.println("SSD1306 allocation failed");
        return false; // Initialization failed
    }
    Serial.println("SSD1306 OLED Initialized Successfully.");

    display_.clearDisplay();
    display_.display(); // Show the cleared buffer
    delay(1000); // Pause for a second

    // Initial Message
    display_.setTextSize(1);             // Normal 1:1 pixel scale
    display_.setTextColor(SSD1306_WHITE);        // Draw white text
    display_.setCursor(0, 0);            // Start at top-left corner
    display_.println(F("ESP32 Weather"));
    display_.setCursor(0, 10);
    display_.println(F("Initializing..."));
    display_.display(); // Update the display with the initial message
    delay(2000); // Pause for 2 seconds

    display_.clearDisplay();
    display_.display(); // Clear the display buffer

    Serial.println("OLED Display Ready.");
    return true; // Initialization successful
}

/**
 * @brief Clear the OLED display buffer.
 */
void OLEDDisplay::clear() {
    display_.clearDisplay();
}

/**
 * @brief Display the current buffer on the OLED.
 */
void OLEDDisplay::displayBuffer() {
    display_.display();
}

/**
 * @brief Display temperature and humidity on the OLED.
 * Clears the display and updates both lines with current readings.
 * 
 * @param temperature Current temperature in °C.
 * @param humidity Current humidity in %.
 */
void OLEDDisplay::displayTempHum(float temperature, float humidity) {
    display_.clearDisplay();

    // Display Temperature on the first line
    display_.setTextSize(2);             // 2X-scale text
    display_.setTextColor(SSD1306_WHITE);
    display_.setCursor(0, 0);             // First line
    display_.print("Temp:");
    display_.print(temperature, 1);       // One decimal place
    //display_.print((char)223);            // Degree symbol
    display_.print("C");

    // Display Humidity on the second line
    display_.setTextSize(2);             // 2X-scale text
    display_.setCursor(0, 16);            // Second line
    display_.print("Humd:");
    display_.print(humidity, 1);          // One decimal place
    display_.print("%");

    display_.display(); // Update the display with the new readings
}

/**
 * @brief Display a custom message on the OLED.
 * 
 * @param message1 First line message.
 * @param message2 Second line message.
 */
void OLEDDisplay::displayStatus(const char* message1, const char* message2) {
    //display_.setFont(&FreeSerif9pt7b); // Set custom font
    display_.clearDisplay();
    display_.setTextSize(1);             // Normal 1:1 pixel scale
    display_.setTextColor(SSD1306_WHITE);
    display_.setCursor(0, 0);             // First line
    display_.println(message1);
    display_.setCursor(0, 13);            // Second line
    display_.println(message2);
    display_.display(); // Update the display with the status message
}

/**
 * @brief Draw a bitmap image at specified coordinates.
 * 
 * @param x X-coordinate.
 * @param y Y-coordinate.
 * @param bitmap Pointer to the bitmap data.
 * @param w Width of the bitmap.
 * @param h Height of the bitmap.
 */
void OLEDDisplay::drawBitmapImage(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h) {
    display_.drawBitmap(x, y, bitmap, w, h, SSD1306_WHITE);
    display_.display(); // Update the display with the bitmap
}

/**
 * @brief Draw custom graphics or animations.
 * Implement as needed based on project requirements.
 */
void OLEDDisplay::drawCustomGraphics() {
    // Example: Drawing a rectangle
    display_.drawRect(10, 10, 50, 30, SSD1306_WHITE);
    display_.display();
}

/**
 * @brief Display a loading animation.
 */
void OLEDDisplay::displayLoadingAnimation() {
    display_.clearDisplay();
    display_.setTextSize(1);
    display_.setTextColor(SSD1306_WHITE);
    display_.setCursor(0, 0);
    display_.println(F("Loading..."));
    display_.display();

    for(int i = 0; i < 100; i++) {
        display_.fillRect(0, 20, i, 10, SSD1306_WHITE);
        display_.display();
        delay(20);
    }

    delay(500);
    display_.clearDisplay();
    display_.display();
}

/**
 * @brief Scroll text across the OLED.
 * 
 * @param text The text to scroll.
 */
void OLEDDisplay::scrollText(const char* text) {
    display_.clearDisplay();
    display_.setTextSize(1);
    display_.setTextColor(SSD1306_WHITE);
    display_.setCursor(width_, 10);
    display_.print(text);
    display_.display();

    for(int16_t x = width_; x >= -strlen(text)*6; x--) { // Approximate width per character
        display_.clearDisplay();
        display_.setCursor(x, 10);
        display_.print(text);
        display_.display();
        delay(50);
    }
}
