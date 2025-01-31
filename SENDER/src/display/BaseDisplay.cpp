#include "BaseDisplay.h"

// Constructor
BaseDisplay::BaseDisplay() : homePage(HomePage()) // Proper initialization
{
    // Initialize the display
}

// Show Home Page
void BaseDisplay::showHomePage(Adafruit_SSD1306 *display)
{
    // Clear display
    display->clearDisplay();
    display->setCursor(0, 0);

    // Draw the home page
    homePage.draw(display);

    // Display the updated screen
    display->display();
}

void BaseDisplay::updateHomePage(Adafruit_SSD1306 *display, int red, int green, int blue, int index, int connectedCount, std::string presetName)
{
    // update the values in home page
    // call a refresh
    homePage.update(red, green, blue, index, connectedCount, presetName);
    showHomePage(display);
}

// Show XX Page
void BaseDisplay::showXXPage()
{
    // Show the XX page
}

// Show YY Page
void BaseDisplay::showYYPage()
{
    // Show the YY page
}
