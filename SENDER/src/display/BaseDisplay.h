#pragma once
#include "pages/BasePage.h"
#include "pages/HomePage/HomePage.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "structs/structs.h"
#include "pages/SaveSlotPage/SaveSlotPage.h"

class BaseDisplay
{
public:
    HomePage homePage;
    SaveSlotPage saveSlotPage;

    // Constructor
    BaseDisplay();

    // Home page controls
    void showHomePage(Adafruit_SSD1306 *display);
    void updateHomePage(Adafruit_SSD1306 *display, int red, int green, int blue, int slotIndex, int slotCount, int connectedCount, std::string presetName);
    
    void showSaveSlotPage(Adafruit_SSD1306 *display);
    void updateSaveSlotPage(Adafruit_SSD1306 *display, char *groupName, Slot *slots, int slotCount, int currentSlot, bool isInsert);

    void showXXPage();
    void showYYPage();


};
