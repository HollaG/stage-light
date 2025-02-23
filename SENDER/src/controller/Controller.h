// Controls everything that happens. Has knowledge of all relevant data in the app.
#pragma once
#include "display/BaseDisplay.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Preferences.h> // save
#include "FS.h"
#include <LittleFS.h>
#include "structs/structs.h"
#include "structs/common.h"
#include "EspNowConnection/EspNowConnection.h"

// page controllers
// #include "subcontrollers/SaveSlot/SaveSlotController.h"
// #include "subcontrollers/Home/HomeController.h"
class HomeController; // forward declaration
class SaveSlotController;
class SettingsController;
class DeleteSlotController;
class ChangeGroupController;
class CharacterInputController;

class ServerController; // forward declaration

class Controller
{
    Preferences prefs;
    BaseDisplay *baseDisplay;
    EspNowConnection espNowConnection;
    Page currentPage = HOME_PAGE;

    // initialze subcontrollers
    HomeController *homeController;
    SaveSlotController *saveSlotController;
    SettingsController *settingsController;
    DeleteSlotController *deleteSlotController;
    ChangeGroupController *changeGroupController;
    CharacterInputController *characterInputController;

    ServerController *serverController;

    // ------ HOME PAGE
    Light light = {0, 0, 0, 0, 0, 0}; // light used in EDITING
    // when we're not in EDIT mode, show the light from preset

    // TODO decide if we want to do this
    // Light frozenLight = {0, 0, 0, 0, 0, 0}; // When in saving screen, we should disable any modifications

    // always init to R:0 G:0 B:0
    // int slotIndex = 0;

    // we can have up to 20 "loadouts" of 100 slots each

    // 0 "home"
    // 1 "save_slot"
    // 2 "new_preset"

    // int connectedCount = 0;

    // int mode = 0; // 0 = "locked, no edit", 1 = "edit"

    // ----- END HOME PAGE

    // int saveInSlotIndex = -1;
    // int saveInGroupIndex = -1;
    // bool isInsert = false;

    // Loading mode
    bool isLoading = false;
    std::string loadingLabel;
    int loadingIndex = 0;

    TaskHandle_t saveTaskHandle = NULL;

    // Settings
    // int settingsIndex = 0;

    // Delete Slot Page
    // int deleteIndex = 0;

    // Change Group page
    // int groupSelectionIndex = 0;
    // bool isInsertGroup = false;

    // Character Input Page
    int newGroupNameAsIndex[GROUP_NAME_LENGTH] = {0};
    int currentNewGroupNameLength = 0;
    int maxNewGroupNameLength = GROUP_NAME_LENGTH;
    bool isEditing = true;
    int cursorPosition = 0;

public:
    Group groups[20];
    bool groupExists[20] = {false};
    int groupIndex = 0;
    int slotIndex = 0;
    int groupCount = 0;

    Controller(BaseDisplay *baseDisplay);

    void linkToServer(ServerController *serverController);

    void refreshPage(Adafruit_SSD1306 *display);

    void updateLight(int red, int green, int blue);
    Light getLight();

    // BUTTON ACTIONS
    void onScreenLeft();
    void onScreenRight();
    void onSend();
    void onDown();
    void onUp();

    // template <typename T>
    void changePage(Page page, void *data);
    void save(std::string message);
    void load();

    void changeGroup(int groupIndex);
    void changeSlot(int slotIndex);

    // for server
    Group *getGroups(int *groupCount);
    Group *getGroup(int groupIndex);
    Group *getCurrentGroup();
    Slot *getCurrentSlot();

    static String groupOptionsToJson(Group *groups, int groupCount);
    static String groupToJson(Group *group);

    // Getters and Setters
    // int getEditMode();
    // Group* getGroups();

private:
    void backgroundSave();
    void backgroundLoad();

    static void saveHelper(void *parameter);
    static void loadHelper(void *parameter);
};
