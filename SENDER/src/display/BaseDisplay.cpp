#include "BaseDisplay.h"

// Constructor
BaseDisplay::BaseDisplay()
{
    homePage = HomePage();
    saveSlotPage = SaveSlotPage();
    settingsPage = SettingsPage();
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

void BaseDisplay::updateHomePage(Adafruit_SSD1306 *display, int red, int green, int blue, int slotIndex, int slotCount, int connectedCount, std::string presetName, int mode)
{
    // update the values in home page
    // call a refresh
    homePage.update(red, green, blue, slotIndex, slotCount, connectedCount, presetName, mode);
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

void BaseDisplay::updateSaveSlotPage(Adafruit_SSD1306 *display, char *groupName, Slot *slots, int slotCount, int currentSlot, bool isInsert)
{

    // update the values in save slot page
    // call a refresh
    saveSlotPage.update(groupName, slots, slotCount, currentSlot, isInsert);
    showSaveSlotPage(display);
}

void BaseDisplay::showSettingsPage(Adafruit_SSD1306 *display)
{
    display->clearDisplay();
    display->setCursor(0, 0);
    settingsPage.draw(display);

    // display the updated screen
    display->display();
}

void BaseDisplay::updateSettingsPage(Adafruit_SSD1306 *display, char *groupName, int index)
{
    // Update the settings page
    settingsPage.update(groupName, index);
    showSettingsPage(display);
}

void BaseDisplay::showDeleteSlotPage(Adafruit_SSD1306 *display)
{
    display->clearDisplay();
    display->setCursor(0, 0);
    deleteSlotPage.draw(display);

    // display the updated screen
    display->display();
}

void BaseDisplay::updateDeleteSlotPage(Adafruit_SSD1306 *display, char *groupName, Slot *slots, int index, int slotCount)
{
    // Update the settings page
    deleteSlotPage.update(groupName, slots, index, slotCount);
    showDeleteSlotPage(display);
}

// Show Change Group Page
void BaseDisplay::showChangeGroupPage(Adafruit_SSD1306 *display)
{
    // Show the change group page
    display->clearDisplay();
    display->setCursor(0, 0);
    changeGroupPage.draw(display);

    // display the updated screen
    display->display();
}

void BaseDisplay::updateChangeGroupPage(Adafruit_SSD1306 *display, Group *groups, int groupCount, int groupSelectionIndex, bool isInsert, char *currentGroupName, char *newGroupName)
{
    // Update the change group page
    changeGroupPage.update(groups, groupCount, groupSelectionIndex, isInsert, currentGroupName, newGroupName);
    showChangeGroupPage(display);
}

// Show Character Input Page
void BaseDisplay::showCharacterInputPage(Adafruit_SSD1306 *display)
{
    // Show the character input page
    display->clearDisplay();
    display->setCursor(0, 0);
    characterInputPage.draw(display);

    // display the updated screen
    display->display();
}

void BaseDisplay::updateCharacterInputPage(Adafruit_SSD1306 *display, int *inputAsIndex, int inputLength, int maxInputLength, bool isEditing, int cursorPosition)
{
    // Update the character input page
    characterInputPage.update(inputAsIndex, inputLength, maxInputLength, isEditing, cursorPosition);
    showCharacterInputPage(display);
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
