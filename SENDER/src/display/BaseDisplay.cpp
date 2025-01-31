#include "BaseDisplay.h"

// Constructor
BaseDisplay::BaseDisplay()
{
    homePage = HomePage();
    saveSlotPage = SaveSlotPage();
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

void BaseDisplay::updateHomePage(Adafruit_SSD1306 *display, int red, int green, int blue, int slotIndex, int slotCount, int connectedCount, std::string presetName)
{
    // update the values in home page
    // call a refresh
    homePage.update(red, green, blue, slotIndex, slotCount, connectedCount, presetName);
    showHomePage(display);
}

// Show Save Slot Page
void BaseDisplay::showSaveSlotPage(Adafruit_SSD1306 *display)
{
    // Show the save slot page

    // clear display
    display->clearDisplay();
    display->setCursor(0, 0);
    saveSlotPage.draw(display);

    // display the updated screen
    display->display();
}

void BaseDisplay::updateSaveSlotPage(Adafruit_SSD1306 *display, char *groupName, Slot *slots, int slotCount, int currentSlot, bool isInsert) {

    // update the values in save slot page
    // call a refresh
    saveSlotPage.update(groupName, slots, slotCount, currentSlot, isInsert);
    showSaveSlotPage(display);
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
