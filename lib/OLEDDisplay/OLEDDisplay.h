#ifndef OLEDDISPLAY_H
#define OLEDDISPLAY_H

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

/**
 * @brief The OLEDDisplay class encapsulates functionalities to interact with an SSD1306 OLED.
 */
class OLEDDisplay {
public:
    /**
     * @brief Constructor for the OLEDDisplay class.
     * 
     * @param width OLED display width in pixels.
     * @param height OLED display height in pixels.
     * @param sda_pin SDA pin number.
     * @param scl_pin SCL pin number.
     * @param reset_pin Reset pin number (-1 if not used).
     */
    OLEDDisplay(uint16_t width, uint16_t height, uint8_t sda_pin, uint8_t scl_pin, int8_t reset_pin = -1);

    /**
     * @brief Initialize the OLED display.
     * 
     * @return true if initialization is successful, false otherwise.
     */
    bool init();

    /**
     * @brief Clear the OLED display buffer.
     */
    void clear();

    /**
     * @brief Display the current buffer on the OLED.
     */
    void displayBuffer();

    /**
     * @brief Display temperature and humidity on the OLED.
     * 
     * @param temperature Current temperature in °C.
     * @param humidity Current humidity in %.
     */
    void displayTempHum(float temperature, float humidity);

    /**
     * @brief Display a custom message on the OLED.
     * 
     * @param message1 First line message.
     * @param message2 Second line message.
     */
    void displayStatus(const char* message1, const char* message2);

    /**
     * @brief Draw a bitmap image at specified coordinates.
     * 
     * @param x X-coordinate.
     * @param y Y-coordinate.
     * @param bitmap Pointer to the bitmap data.
     * @param w Width of the bitmap.
     * @param h Height of the bitmap.
     */
    void drawBitmapImage(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h);

    /**
     * @brief Draw custom graphics or animations.
     * Implement as needed based on project requirements.
     */
    void drawCustomGraphics();

    /**
     * @brief Display a loading animation.
     */
    void displayLoadingAnimation();

    /**
     * @brief Scroll text across the OLED.
     * 
     * @param text The text to scroll.
     */
    void scrollText(const char* text);

private:
    Adafruit_SSD1306 display_; ///< Adafruit SSD1306 OLED object.
    uint16_t width_;
    uint16_t height_;
    uint8_t sda_pin_;
    uint8_t scl_pin_;
    int8_t reset_pin_;
};

#endif // OLEDDISPLAY_H
