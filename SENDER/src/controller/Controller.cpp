#include "Controller.h"
#include "bitmaps/loading.h"
#include <ArduinoJson.h>

// controllers
#include "subcontrollers/Home/HomeController.h"
#include "subcontrollers/SaveSlot/SaveSlotController.h"
#include "subcontrollers/Settings/SettingsController.h"
#include "subcontrollers/DeleteSlot/DeleteSlotController.h"
#include "subcontrollers/ChangeGroup/ChangeGroupController.h"
#include "subcontrollers/CharacterInput/CharacterInputController.h"

#include "server/ServerController.h"

Controller::Controller(BaseDisplay *baseDisplay) : baseDisplay(baseDisplay)
{

    // create initial group if not exists
    // TODO: implement saving

    // // Initialize LittleFS
    // if (REFORMAT)
    // {
    //     LittleFS.format();
    // } // first time setup

    // if (!LittleFS.begin(true))
    // { // `true` forces formatting if needed
    //     Serial.println("[error] LittleFS Mount Failed!");
    //     return;
    // }
    // Serial.println("[debug] LittleFS Mounted.");

    Group group;
    char name[GROUP_NAME_LENGTH] = "Default";
    strcpy(group.name, name);
    group.slotCount = 0;

    Light light = {0, 0, 0};
    Slot slot = {light};

    group.slots[0] = slot;
    groups[0] = group;
    groupExists[0] = true;

    // RFHOME
    // slotIndex = 0; // NOTE THAT THIS IS 0-INDEXED
    // groupIndex = 0;
    groupCount = 1; // this is a COUNT
    // END RFHOME

    // initalize page controllers
    homeController = new HomeController(baseDisplay, this);
    saveSlotController = new SaveSlotController(baseDisplay, this);
    settingsController = new SettingsController(baseDisplay, this);
    deleteSlotController = new DeleteSlotController(baseDisplay, this);
    changeGroupController = new ChangeGroupController(baseDisplay, this);
    characterInputController = new CharacterInputController(baseDisplay, this);

    load();
}

void Controller::linkToServer(ServerController *serverController)
{
    this->serverController = serverController;
}

void Controller::updateLight(int red, int green, int blue)
{
    // update the light in the current slot
    // groups[groupIndex].slots[slotIndex].light.r = red;
    // groups[groupIndex].slots[slotIndex].light.g = green;
    // groups[groupIndex].slots[slotIndex].light.b = blue;

    light = {0, red, green, blue, 0, 0};
    homeController->updateLight(red, green, blue);
}

/**
 * Change the page to the given page.
 *
 * On the next loop() iteration, the display will be updated to reflect the new page.
 * See `refreshPage()`.
 */

void Controller::changePage(Page page, void *data)
{
    currentPage = page;

    switch (page)
    {
    case HOME_PAGE:
    {
        AnyToHomeData *castedData = static_cast<AnyToHomeData *>(data);
        homeController->activate(*castedData);
        break;
    }
    case SAVE_SLOT_PAGE:
    {
        HomeToSaveSlotData *castedData = static_cast<HomeToSaveSlotData *>(data);
        Serial.println("Made it here");
        saveSlotController->activate(*castedData);
        break;
    }
    case SETTINGS_PAGE:
    {
        // Nothing needs to be done here
        break;
    }
    case DELETE_SLOT_PAGE:
    {
        AnyToDeleteSlotData *castedData = static_cast<AnyToDeleteSlotData *>(data);
        deleteSlotController->activate(*castedData);
        break;
    }

    case CHANGE_GROUP_PAGE:
    {
        AnyToChangeGroupData *castedData = static_cast<AnyToChangeGroupData *>(data);
        changeGroupController->activate(*castedData);
        break;
    }
    case CHARACTER_INPUT_PAGE:
    {
        AnyToCharacterInputData *castedData = static_cast<AnyToCharacterInputData *>(data);
        characterInputController->activate(*castedData);
        break;
    }
    }
}

void Controller::refreshPage(Adafruit_SSD1306 *display)
{
    // TODO: should we integrate the loader as a page?
    // this is a bit not-OOP as the responsibility to draw shouldn't be in the controller,
    // it should be in the page
    // perhaps we should use a "loading" page instead

    // We need to detect when we fail to load something as the save task handle does not get reset for soem reason
    if (saveTaskHandle != NULL)
    {
        eTaskState state = eTaskGetState(saveTaskHandle);

        if (state == eDeleted)
        {
            Serial.println("\n[INFO] Background worker complete.");
            saveTaskHandle = NULL; // Reset handle
        }

        display->clearDisplay();
        display->drawBitmap(2, 16, frames[loadingIndex], FRAME_WIDTH, FRAME_HEIGHT, 1);
        display->setCursor(40, 28);
        display->print(loadingLabel.c_str());
        display->display();
        loadingIndex = (loadingIndex + 1) % FRAME_COUNT;
        return;
    }

    // depending on the current page index, refresh the page by calling the relevant functions
    switch (currentPage)
    {
    case HOME_PAGE:
    {
        homeController->refreshPage(display);
        break;
    };
    case SAVE_SLOT_PAGE:
    {
        saveSlotController->refreshPage(display);
        break;
    }
    case SETTINGS_PAGE:
    {
        settingsController->refreshPage(display);
        break;
    }
    case DELETE_SLOT_PAGE:
    {
        deleteSlotController->refreshPage(display);
        break;
    }
    case CHANGE_GROUP_PAGE:
    {
        changeGroupController->refreshPage(display);
        break;
    }
    case CHARACTER_INPUT_PAGE:
    {
        characterInputController->refreshPage(display);
        break;
    }
    default:
    {
    }
    }
}

Light Controller::getLight()
{
    // if (mode == 0)
    // {
    //     return groups[groupIndex].slots[slotIndex].light;
    // }
    // else
    // {
    //     return light;
    // }
    return light;
}

// TODO: disable all buttons when saving
void Controller::onScreenLeft()
{
    // 0 --> SAVE

    Serial.println("[debug] Screen Left Button Pressed");

    switch (currentPage)
    {
    case HOME_PAGE:
    {
        homeController->onScreenLeft();
        break;
    };
    case SAVE_SLOT_PAGE:
    {
        saveSlotController->onScreenLeft();
        break;
    }

    case SETTINGS_PAGE:
    {

        settingsController->onScreenLeft();
        break;
    };
    case DELETE_SLOT_PAGE:
    {
        deleteSlotController->onScreenLeft();
        break;
    }

    case CHANGE_GROUP_PAGE:
    {
        changeGroupController->onScreenLeft();
        break;
    }
    case CHARACTER_INPUT_PAGE:
    {
        characterInputController->onScreenLeft();
        break;
    };
    }
}

void Controller::onScreenRight()
{
    Serial.println("[debug] Screen Right Button Pressed");

    switch (currentPage)
    {
    case HOME_PAGE:
    {

        homeController->onScreenRight();
        break;
    }

    case SAVE_SLOT_PAGE:
    {

        saveSlotController->onScreenRight();
        break;
    }

    case SETTINGS_PAGE:
    {

        settingsController->onScreenRight();
        break;
    }
    case DELETE_SLOT_PAGE:
    {

        deleteSlotController->onScreenRight();
        break;
    }
    case CHANGE_GROUP_PAGE:
    {

        changeGroupController->onScreenRight();
        break;
    }
    case CHARACTER_INPUT_PAGE:
    {
        characterInputController->onScreenRight();

        break;
    }
    }
}

void Controller::onSend()
{
    switch (currentPage)
    {
    case HOME_PAGE:
    {
        // send the current light settings
        // CHANGE_MESSAGE change = {0, light.r, light.g, light.b, 0, 0, 0};
        // espNowConnection.send(&change);
        break;
    }
    case CHARACTER_INPUT_PAGE:
    {
        // isEditing = !isEditing;
        characterInputController->onSend();
        break;
    }
    }
}

void Controller::onDown()
{
    switch (currentPage)
    {
    case HOME_PAGE:
    {

        homeController->onDown();
        break;
    }
    case SAVE_SLOT_PAGE:
    {

        saveSlotController->onDown();

        break;
    }
    case SETTINGS_PAGE:
    {

        settingsController->onDown();
        break;
    };
    case DELETE_SLOT_PAGE:
    {
        deleteSlotController->onDown();
        break;
    }
    case CHANGE_GROUP_PAGE:
    {

        changeGroupController->onDown();

        break;
    }
    case CHARACTER_INPUT_PAGE:
    {
        characterInputController->onDown();
        break;
    }
    }
}

void Controller::onUp()
{

    switch (currentPage)
    {
    case HOME_PAGE:
    {

        homeController->onUp();
        break;
    }
    case SAVE_SLOT_PAGE:
    {

        saveSlotController->onUp();
        break;
    }
    case SETTINGS_PAGE:
    {

        settingsController->onUp();
        break;
    }
    case DELETE_SLOT_PAGE:
    {
        deleteSlotController->onUp();
        break;
    }
    case CHANGE_GROUP_PAGE:
    {

        changeGroupController->onUp();

        break;
    }

    case CHARACTER_INPUT_PAGE:
    {
        characterInputController->onUp();

        break;
    }
    }
}

void Controller::changeGroup(int groupIndex)
{
    this->groupIndex = groupIndex;
    this->slotIndex = 0;
}

void Controller::changeSlot(int slotIndex) { 
    this->slotIndex = slotIndex;
}

void Controller::save(std::string message = "Saving slot...")
{
    isLoading = true;

    loadingLabel = message;

    xTaskCreate(
        saveHelper,
        "backgroundSave",
        16384,
        this,
        1,
        &saveTaskHandle);
    // backgroundSave();
    isLoading = false;
}

void Controller::saveHelper(void *parameter)
{
    Controller *instance = static_cast<Controller *>(parameter);
    instance->backgroundSave();
    vTaskDelete(NULL);
}

void Controller::backgroundSave()
{
    Serial.println("[debug] Saving data...");

    // 1️⃣ Save settings to Preferences (NVS)
    prefs.begin("settings");
    prefs.putBool("loadFromSave", true);
    prefs.end();

    // 2️⃣ Save group metadata to Preferences
    prefs.begin("current", false);
    prefs.putInt("slotIndex", slotIndex);
    prefs.putInt("groupIndex", groupIndex);
    prefs.putInt("groupCount", groupCount);
    prefs.end();

    // 3️⃣ Save `groups` and `groupExists` to LittleFS
    File file = LittleFS.open("/groups.bin", "w");
    if (!file)
    {
        Serial.println("[error] Failed to open /groups.bin for writing.");
        return;
    }
    file.write((uint8_t *)groups, sizeof(groups));
    file.close();
    Serial.println("[debug] Groups saved to LittleFS.");

    file = LittleFS.open("/groupExists.bin", "w");
    if (!file)
    {
        Serial.println("[error] Failed to open /groupExists.bin for writing.");
        return;
    }
    file.write((uint8_t *)groupExists, sizeof(groupExists));
    file.close();
    Serial.println("[debug] GroupExists saved to LittleFS.");
}

void Controller::load()
{

    loadingLabel = "Initializing...";
    xTaskCreate(
        loadHelper,
        "backgroundLoad",
        16384,
        this,
        1,
        &saveTaskHandle);

    isLoading = false;
    saveTaskHandle = NULL;
}

void Controller::loadHelper(void *parameter)
{
    Controller *instance = static_cast<Controller *>(parameter);
    instance->backgroundLoad();
    vTaskDelete(instance->saveTaskHandle);
    instance->saveTaskHandle = NULL;
}

void Controller::backgroundLoad()

{
    Serial.println("[init] Loading saved data...");

    // 1️⃣ Check if settings exist in Preferences
    prefs.begin("settings");
    bool exists = prefs.isKey("loadFromSave");
    prefs.end();

    if (!exists)
    {
        Serial.println("[init] No previous data found");

        return;
    }

    Serial.println("[init] Previous data found");

    // 2️⃣ Load group metadata from Preferences
    prefs.begin("current", true);
    // TODO: Don't load the indexes, always reset to default
    // slotIndex = prefs.getInt("slotIndex", 0);
    // groupIndex = prefs.getInt("groupIndex", 0);
    groupCount = prefs.getInt("groupCount", 0);
    prefs.end();

    // 3️⃣ Load `groups` from LittleFS
    File file = LittleFS.open("/groups.bin", "r");
    if (!file)
    {
        Serial.println("[error] Failed to open /groups.bin for reading.");

        return;
    }
    file.readBytes((char *)groups, sizeof(groups));
    file.close();
    Serial.println("[debug] Groups loaded from LittleFS.");

    // 4️⃣ Load `groupExists` from LittleFS
    file = LittleFS.open("/groupExists.bin", "r");
    if (!file)
    {
        Serial.println("[error] Failed to open /groupExists.bin for reading.");

        return;
    }
    file.readBytes((char *)groupExists, sizeof(groupExists));
    file.close();
    Serial.println("[debug] GroupExists loaded from LittleFS.");
}

// for server
Group *Controller::getGroups(int *groupCount)
{
    *groupCount = this->groupCount;
    return this->groups;
}

Group *Controller::getGroup(int groupIndex)
{
    return &groups[groupIndex];
}

Group *Controller::getCurrentGroup()
{
    return &groups[groupIndex];
}

Slot *Controller::getCurrentSlot()
{
    return &groups[groupIndex].slots[slotIndex];
}

/**
 * Only group names
 * TODO: abstract this out somehow
 *
 * https://arduinojson.org/v7/how-to/upgrade-from-v6/
 */
String Controller::groupOptionsToJson(Group *groups, int groupCount)
{
    JsonDocument doc; //
    JsonArray groupsArray = doc["data"].to<JsonArray>();
    for (int i = 0; i < groupCount; i++)
    {
        JsonObject groupObj = groupsArray.add<JsonObject>();
        groupObj["id"] = i; // Index as ID
        groupObj["name"] = groups[i].name;
    }

    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

/**
 * Given a slot id
 */
String Controller::groupToJson(Group *group)
{
    int slotCount = group->slotCount;
    JsonDocument doc;
    doc["data"]["name"] = group->name;
    doc["data"]["slotCount"] = slotCount;
    JsonArray slotsArray = doc["data"]["slots"].to<JsonArray>();
    for (int i = 0; i < slotCount; i++)
    {
        JsonObject slotObj = slotsArray.add<JsonObject>();
        slotObj["id"] = i; // Index as ID

        JsonObject light = slotObj["light"].to<JsonObject>();
        
        // add a key "light" and set the light to be the property
        

        light["r"] = group->slots[i].light.r;
        light["g"] = group->slots[i].light.g;
        light["b"] = group->slots[i].light.b;
        light["w"] = group->slots[i].light.w;
        light["ww"] = group->slots[i].light.ww;
        light["light__type"] = group->slots[i].light.light__type;
        light["light__transition"] = group->slots[i].light.light__transition;
    }

    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}