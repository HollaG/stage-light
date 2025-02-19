#include "Controller.h"
#include "bitmaps/loading.h"
#include <ArduinoJson.h>

// controllers
#include "subcontrollers/Home/HomeController.h"
#include "subcontrollers/SaveSlot/SaveSlotController.h"
#include "subcontrollers/Settings/SettingsController.h"

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

    load();
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

// template void Controller::changePage<HomeToSaveSlotData>(Page, HomeToSaveSlotData);
// template void Controller::changePage<AnyToHomeData>(Page, AnyToHomeData);
/**d
 * Change the page to the given page.
 *
 * On the next loop() iteration, the display will be updated to reflect the new page.
 * See `refreshPage()`.
 */
// template <typename T>
void Controller::changePage(Page page, void *data)
{
    currentPage = page;

    switch (page)
    {
    case HOME_PAGE:
    {
        // AnyToHomeData saveSlotToHomeData = static_cast<AnyToHomeData>(data);
        AnyToHomeData *castedData = static_cast<AnyToHomeData *>(data);
        homeController->activate(*castedData);
        break;
    }
    case SAVE_SLOT_PAGE:
    {
        // HomeToSaveSlotData homeToSaveSlotData = static_cast<HomeToSaveSlotData>(data);
        HomeToSaveSlotData *castedData = static_cast<HomeToSaveSlotData *>(data);
        saveSlotController->activate(*castedData);
        break;
    }
    case SETTINGS_PAGE:
    {
        // AnyToSettingsData *castedData = static_cast<AnyToSettingsData *>(data);
        // settingsController->activate(*castedData);
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
        // // int red = groups[groupIndex].slots[slotIndex].light.r;
        // // int green = groups[groupIndex].slots[slotIndex].light.g;
        // // int blue = groups[groupIndex].slots[slotIndex].light.b;
        // int slotCount = groups[groupIndex].slotCount;

        // // if editing, then display potentiometer lights, if not, display slot light
        // int r, g, b;
        // if (mode == 1)
        // {
        //     r = light.r;
        //     g = light.g;
        //     b = light.b;
        // }
        // else
        // {
        //     r = groups[groupIndex].slots[slotIndex].light.r;
        //     g = groups[groupIndex].slots[slotIndex].light.g;
        //     b = groups[groupIndex].slots[slotIndex].light.b;
        // }

        // baseDisplay->updateHomePage(display, r, g, b, slotIndex, slotCount, connectedCount, groups[groupIndex].name, mode);
        break;
    };
    case SAVE_SLOT_PAGE:
    {
        // char groupName[GROUP_NAME_LENGTH];
        // strcpy(groupName, groups[groupIndex].name);

        // Slot *slots = groups[groupIndex].slots;
        // int slotCount = groups[groupIndex].slotCount;

        // baseDisplay->updateSaveSlotPage(display, groupName, slots, slotCount, saveInSlotIndex, isInsert);
        saveSlotController->refreshPage(display);
        break;
    }
    case SETTINGS_PAGE:
    {
        // char groupName[GROUP_NAME_LENGTH];
        // strcpy(groupName, groups[groupIndex].name);

        // baseDisplay->updateSettingsPage(display, groupName, settingsIndex);
        settingsController->refreshPage(display);
        break;
    }
    case DELETE_SLOT_PAGE:
    {
        // char groupName[GROUP_NAME_LENGTH];
        // strcpy(groupName, groups[groupIndex].name);

        // baseDisplay->updateDeleteSlotPage(display, groupName, groups[groupIndex].slots, deleteIndex, groups[groupIndex].slotCount);
        break;
    }
    case CHANGE_GROUP_PAGE:
    {
        // baseDisplay->updateChangeGroupPage(display, groups, groupCount, groupSelectionIndex, isInsertGroup, groups[groupIndex].name);
        break;
    }
    case CHARACTER_INPUT_PAGE:
    {
        baseDisplay->updateCharacterInputPage(display, newGroupNameAsIndex, currentNewGroupNameLength, maxNewGroupNameLength, isEditing, cursorPosition);
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
        // go to Saving page
        // if (mode == 1)
        // {

        //     // changePage(SAVE_SLOT_PAGE);

        //     // SPECIAL: if slotIndex is the last item, then we want to save in the next slot
        //     if (slotIndex == groups[groupIndex].slotCount - 1)
        //     {
        //         saveInSlotIndex = slotIndex + 1;
        //     }
        //     else
        //     {
        //         saveInSlotIndex = slotIndex;
        //     }

        //     Serial.printf("DEBUG: SaveInSlotIndex %d, slotIndex %d\n", saveInSlotIndex, slotIndex);
        //     frozenLight = light;
        // }
        // else
        // {
        //     mode = 1;
        // }

        homeController->onScreenLeft();
        break;
    };
    case SAVE_SLOT_PAGE:
    {
        // TODO: Handle overflow (more than 100)
        // save this
        // groups[groupIndex].slots[saveInSlotIndex].light = {
        //     red
        // }
        // Serial.printf("Saved in group %d, slot %d\n", groupIndex, saveInSlotIndex);
        // Serial.printf("Saved R %d, G %d, B %d\n", frozenLight.r, frozenLight.g, frozenLight.b);

        // // changePage(HOME_PAGE);

        // // case 1: isInsert = false, saveInSlotIndex = -1
        // // => insert at the head of the list
        // // case 2: isInsert = false, saveInSlotIndex = slotCount
        // // => insert at the end of the list
        // // case 3: isInsert = true, saveInSlotIndex = any except those 2
        // // => insert inbetween
        // if (isInsert == false && saveInSlotIndex == -1)
        // {
        //     // insert at the head of the list
        //     for (int i = groups[groupIndex].slotCount; i > 0; i--)
        //     {
        //         groups[groupIndex].slots[i] = groups[groupIndex].slots[i - 1];
        //     }
        //     groups[groupIndex].slots[0].light = frozenLight;
        //     groups[groupIndex].slotCount = groups[groupIndex].slotCount + 1;
        //     slotIndex = 0; // show the newly inserted slot
        // }
        // else if (isInsert == false && saveInSlotIndex == groups[groupIndex].slotCount)
        // {
        //     // insert at the end of the list
        //     groups[groupIndex].slots[groups[groupIndex].slotCount].light = frozenLight;
        //     groups[groupIndex].slotCount = groups[groupIndex].slotCount + 1;
        //     slotIndex = groups[groupIndex].slotCount - 1; // show the newly inserted slot
        // }
        // else
        // {
        //     // insert inbetween
        //     for (int i = groups[groupIndex].slotCount; i > saveInSlotIndex; i--)
        //     {
        //         groups[groupIndex].slots[i] = groups[groupIndex].slots[i - 1];
        //     }
        //     groups[groupIndex].slots[saveInSlotIndex].light = frozenLight;
        //     groups[groupIndex].slotCount = groups[groupIndex].slotCount + 1;
        //     slotIndex = saveInSlotIndex; // show the newly inserted slot
        // }

        // save("Saving slot...");
        saveSlotController->onScreenLeft();
        break;
    }

    case SETTINGS_PAGE:
    {
        // // go to HOME
        // // changePage(HOME_PAGE);
        // break;
        if (settingsIndex == 0)
        {
            // delete item
            deleteIndex = slotIndex;
            // changePage(DELETE_SLOT_PAGE);
        }
        if (settingsIndex == 1)
        {
            // change group
            groupSelectionIndex = groupIndex;
            // changePage(CHANGE_GROUP_PAGE);
        }
        if (settingsIndex == 2)
        {
            // Scan for receivers
            // unimplemented
        }

        break;
    };
    case DELETE_SLOT_PAGE:
    {
        // delete the slot
        for (int i = deleteIndex; i < groups[groupIndex].slotCount - 1; i++)
        {
            groups[groupIndex].slots[i] = groups[groupIndex].slots[i + 1];
        }
        groups[groupIndex].slotCount = groups[groupIndex].slotCount - 1;

        // // changePage(HOME_PAGE);
        if (groups[groupIndex].slotCount == 0)
        {
            // changePage(HOME_PAGE);
        }
        if (deleteIndex == groups[groupIndex].slotCount)
        {
            deleteIndex--;
        }
        if (slotIndex == groups[groupIndex].slotCount)
        {
            slotIndex--;
        }
        save("Deleting slot...");
        break;
    }

    case CHANGE_GROUP_PAGE:
    {
        // todo: handle overwrite
        // todo: cleaner way to initialize the new group name
        // what if we exit the page and should we keep the new group name when we come bac

        // initialize new group name to empty
        if (isInsertGroup)
        {

            // changePage(CHARACTER_INPUT_PAGE);
        }
        else
        {
            if (groupSelectionIndex != -1 && groupSelectionIndex != groupCount)
            {
                // LOAD
                groupIndex = groupSelectionIndex;
                // changePage(HOME_PAGE);
            }
            else
            {
                // SAVE
                // changePage(CHARACTER_INPUT_PAGE);
            }
        }
        break;
    }
    case CHARACTER_INPUT_PAGE:
    {
        if (isEditing)
        {
            cursorPosition = (cursorPosition - 1 + GROUP_NAME_LENGTH) % GROUP_NAME_LENGTH;
        }
        else
        {
            // todo: add a variable to check if this page's purpose
            // as we would like to reuse this pageas
            // save the new group name
            if (isInsertGroup)
            {
            }
            else
            {
                // save the new group name
                // groups[groupSelectionIndex].name = newGroupName;
                Group newGroup;

                // convert the name from newGroupNameAsIndex to ASCII
                for (int i = 0; i < GROUP_NAME_LENGTH; i++)
                {
                    newGroup.name[i] = *POSSIBLE_CHARS[newGroupNameAsIndex[i]];
                }
                newGroup.slotCount = 0;

                Light light = {0, 0, 0};
                Slot slot = {light};

                newGroup.slots[0] = slot;
                groups[groupSelectionIndex] = newGroup;
                groupExists[groupSelectionIndex] = true;

                groupCount++;
                groupIndex = groupSelectionIndex;

                // reset all data in character input page
                for (int i = 0; i < GROUP_NAME_LENGTH; i++)
                {
                    newGroupNameAsIndex[i] = 0;
                }
                currentNewGroupNameLength = 0;
                cursorPosition = 0;
                // changePage(HOME_PAGE);
                save("Saving new group...");
            }
        }
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
        // exit saving mode
        // if (mode == 1)
        // {
        //     // exit saving mode
        //     mode = 0;
        // }
        // else
        // {
        //     // changePage(SETTINGS_PAGE);
        // }

        homeController->onScreenRight();
        break;
    }

    case SAVE_SLOT_PAGE:
    {
        // CANCEL SAVING
        // changePage(HOME_PAGE);
        saveSlotController->onScreenRight();
        break;
    }

    case SETTINGS_PAGE:
    {
        // changePage(HOME_PAGE);
        settingsController->onScreenRight();
        break;
    }
    case DELETE_SLOT_PAGE:
    {
        // changePage(SETTINGS_PAGE);
        break;
    }
    case CHANGE_GROUP_PAGE:
    {
        // changePage(SETTINGS_PAGE);
        break;
    }
    case CHARACTER_INPUT_PAGE:
    {
        if (isEditing)
        {

            // go right
            cursorPosition = (cursorPosition + 1) % GROUP_NAME_LENGTH;
        }
        else
        {
            // changePage(CHANGE_GROUP_PAGE);
            // reset all data in character input page
            for (int i = 0; i < GROUP_NAME_LENGTH; i++)
            {
                newGroupNameAsIndex[i] = 0;
            }
            currentNewGroupNameLength = 0;
            cursorPosition = 0;
            isEditing = true;
        }
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
        isEditing = !isEditing;
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
        // if (mode == 1)
        //     return;
        // if (groups[groupIndex].slotCount == 0)
        //     return;
        // slotIndex = (slotIndex + 1) % groups[groupIndex].slotCount;
        homeController->onDown();
        break;
    }
    case SAVE_SLOT_PAGE:
    {
        // // no looping
        // if (saveInSlotIndex == groups[groupIndex].slotCount)
        // {
        // }
        // else
        // {
        //     if (saveInSlotIndex == -1 || saveInSlotIndex == 0)
        //     {
        //         saveInSlotIndex++;
        //         isInsert = false;
        //     }
        //     else if (saveInSlotIndex == groups[groupIndex].slotCount - 1)
        //     {
        //         saveInSlotIndex++;
        //         isInsert = false;
        //     }
        //     else if (!isInsert)
        //     {
        //         isInsert = true;
        //         saveInSlotIndex++;
        //     }
        //     else
        //     {
        //         isInsert = false;
        //     }
        // }

        // Serial.printf("Now looking at slot %d\n", saveInSlotIndex);

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
        deleteIndex = (deleteIndex + 1) % groups[groupIndex].slotCount;
        break;
    }
    case CHANGE_GROUP_PAGE:
    {
        // no looping
        if (groupSelectionIndex == groupCount)
        {
        }
        else
        {
            if (groupSelectionIndex == -1 || groupSelectionIndex == 0)
            {
                groupSelectionIndex++;
                isInsertGroup = false;
            }
            else if (groupSelectionIndex == groupCount - 1)
            {
                groupSelectionIndex++;
                isInsertGroup = false;
            }
            else if (!isInsertGroup)
            {
                isInsertGroup = true;
                groupSelectionIndex++;
            }
            else
            {
                isInsertGroup = false;
            }
        }

        Serial.printf("Now looking at group %d\n", groupSelectionIndex);

        break;
    }
    case CHARACTER_INPUT_PAGE:
    {
        // go down 1 character in the ASCII format
        newGroupNameAsIndex[cursorPosition] = (newGroupNameAsIndex[cursorPosition] + 1) % POSSIBLE_CHARS_LENGTH;
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
        // if (mode == 1)
        //     return;

        // if (groups[groupIndex].slotCount == 0)
        //     return;
        // slotIndex = (slotIndex - 1 + groups[groupIndex].slotCount) % groups[groupIndex].slotCount;
        homeController->onUp();
        break;
    }
    case SAVE_SLOT_PAGE:
    {

        // if (saveInSlotIndex == -1)
        // {
        // }
        // else
        // {
        //     if (saveInSlotIndex == groups[groupIndex].slotCount || saveInSlotIndex == 0)
        //     {
        //         saveInSlotIndex--;
        //         isInsert = false;
        //     }
        //     else if (!isInsert)
        //     {
        //         isInsert = true;
        //     }
        //     else
        //     {
        //         isInsert = false;

        //         saveInSlotIndex--;
        //     }
        // }
        // Serial.printf("Now looking at slot %d\n", saveInSlotIndex);

        saveSlotController->onUp();
        break;
    }
    case SETTINGS_PAGE:
    {

        // settingsIndex = (settingsIndex - 1 + SETTINGS_COUNT) % SETTINGS_COUNT;
        settingsController->onUp();
        break;
    }
    case DELETE_SLOT_PAGE:
    {
        deleteIndex = (deleteIndex - 1 + groups[groupIndex].slotCount) % groups[groupIndex].slotCount;
        break;
    }
    case CHANGE_GROUP_PAGE:
    {

        if (groupSelectionIndex == -1)
        {
        }
        else
        {
            if (groupSelectionIndex == groupCount || groupSelectionIndex == 0)
            {
                groupSelectionIndex--;
                isInsertGroup = false;
            }
            else if (!isInsertGroup)
            {
                isInsertGroup = true;
            }
            else
            {
                isInsertGroup = false;

                groupSelectionIndex--;
            }
        }
        Serial.printf("Now looking at group %d\n", groupSelectionIndex);

        break;
    }

    case CHARACTER_INPUT_PAGE:
    {
        // go up 1 character in the ASCII format
        newGroupNameAsIndex[cursorPosition] = (newGroupNameAsIndex[cursorPosition] - 1 + POSSIBLE_CHARS_LENGTH) % POSSIBLE_CHARS_LENGTH;
        break;
    }
    }
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
        slotObj["r"] = group->slots[i].light.r;
        slotObj["g"] = group->slots[i].light.g;
        slotObj["b"] = group->slots[i].light.b;
        slotObj["w"] = group->slots[i].light.w;
        slotObj["ww"] = group->slots[i].light.ww;
        slotObj["light__type"] = group->slots[i].light.light__type;
        slotObj["light__transition"] = group->slots[i].light.light__transition;
    }

    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}