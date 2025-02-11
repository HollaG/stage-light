#pragma once
#include "pages/BasePage.h"
#include "pages/HomePage/HomePage.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "structs/structs.h"
#include "pages/SaveSlotPage/SaveSlotPage.h"
#include "pages/SettingsPage/SettingsPage.h"
#include "pages/DeleteSlotPage/DeleteSlotPage.h"
#include "pages/ChangeGroupPage/ChangeGroupPage.h"
#include "pages/CharacterInputPage/CharacterInputPage.h"

class BaseDisplay
{
public:
    HomePage homePage;
    SaveSlotPage saveSlotPage;
    SettingsPage settingsPage;
    DeleteSlotPage deleteSlotPage;
    ChangeGroupPage changeGroupPage;
    CharacterInputPage characterInputPage;

    // Constructor
    BaseDisplay();

    // Home page controls
    void showHomePage(Adafruit_SSD1306 *display);
    void updateHomePage(Adafruit_SSD1306 *display, int red, int green, int blue, int slotIndex, int slotCount, int connectedCount, std::string presetName, int mode);

    void showSaveSlotPage(Adafruit_SSD1306 *display);
    void updateSaveSlotPage(Adafruit_SSD1306 *display, char *groupName, Slot *slots, int slotCount, int currentSlot, bool isInsert);

    void showSettingsPage(Adafruit_SSD1306 *display);
    void updateSettingsPage(Adafruit_SSD1306 *display, char *groupName, int index);

    void showDeleteSlotPage(Adafruit_SSD1306 *display);
    void updateDeleteSlotPage(Adafruit_SSD1306 *display, char *groupName, Slot *slots, int index, int slotCount);

    void showChangeGroupPage(Adafruit_SSD1306 *display);
    void updateChangeGroupPage(Adafruit_SSD1306 *display, Group *groups, int groupCount, int groupSelectionIndex, bool isInsert, char *currentGroupName, char *newGroupName);

    void showCharacterInputPage(Adafruit_SSD1306 *display);
    void updateCharacterInputPage(Adafruit_SSD1306 *display, int *inputAsIndex, int inputLength, int maxInputLength, bool isEditing, int cursorPosition);

    void showXXPage();
    void showYYPage();
};
