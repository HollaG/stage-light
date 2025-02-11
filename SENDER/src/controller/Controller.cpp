#include "Controller.h"
#include "bitmaps/loading.h"

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

    slotIndex = 0; // NOTE THAT THIS IS 0-INDEXED
    groupIndex = 0;
    groupCount = 1; // this is a COUNT

    // load();
}

void Controller::updateLight(int red, int green, int blue)
{
    // update the light in the current slot
    // groups[groupIndex].slots[slotIndex].light.r = red;
    // groups[groupIndex].slots[slotIndex].light.g = green;
    // groups[groupIndex].slots[slotIndex].light.b = blue;

    light = {0, red, green, blue, 0, 0};
}

void Controller::nextSlot()
{
    int slotCount = groups[groupIndex].slotCount;
    slotIndex = (slotIndex + 1) % slotCount;
}

void Controller::prevSlot()
{
    int slotCount = groups[groupIndex].slotCount;
    slotIndex = (slotIndex - 1 + slotCount) % slotCount;
}

/**
 * Change the page to the given page.
 *
 * On the next loop() iteration, the display will be updated to reflect the new page.
 * See `refreshPage()`.
 */
void Controller::changePage(Page page)
{
    currentPage = page;
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
        // int red = groups[groupIndex].slots[slotIndex].light.r;
        // int green = groups[groupIndex].slots[slotIndex].light.g;
        // int blue = groups[groupIndex].slots[slotIndex].light.b;
        int slotCount = groups[groupIndex].slotCount;

        // if editing, then display potentiometer lights, if not, display slot light
        int r, g, b;
        if (mode == 1)
        {
            r = light.r;
            g = light.g;
            b = light.b;
        }
        else
        {
            r = groups[groupIndex].slots[slotIndex].light.r;
            g = groups[groupIndex].slots[slotIndex].light.g;
            b = groups[groupIndex].slots[slotIndex].light.b;
        }

        baseDisplay->updateHomePage(display, r, g, b, slotIndex, slotCount, connectedCount, "", mode);
        break;
    };
    case SAVE_SLOT_PAGE:
    {
        char groupName[GROUP_NAME_LENGTH];
        strcpy(groupName, groups[groupIndex].name);

        Slot *slots = groups[groupIndex].slots;
        int slotCount = groups[groupIndex].slotCount;

        baseDisplay->updateSaveSlotPage(display, groupName, slots, slotCount, saveInSlotIndex, isInsert);
        break;
    }
    case SETTINGS_PAGE:
    {
        char groupName[GROUP_NAME_LENGTH];
        strcpy(groupName, groups[groupIndex].name);

        baseDisplay->updateSettingsPage(display, groupName, settingsIndex);
        break;
    }
    case DELETE_SLOT_PAGE:
    {
        char groupName[GROUP_NAME_LENGTH];
        strcpy(groupName, groups[groupIndex].name);

        baseDisplay->updateDeleteSlotPage(display, groupName, groups[groupIndex].slots, deleteIndex, groups[groupIndex].slotCount);
        break;
    }
    case CHANGE_GROUP_PAGE:
    {
        baseDisplay->updateChangeGroupPage(display, groups, groupCount, groupSelectionIndex, isInsertGroup, groups[groupIndex].name, newGroupName);
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
    if (mode == 0)
    {
        return groups[groupIndex].slots[slotIndex].light;
    }
    else
    {
        return light;
    }
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
        if (mode == 1)
        {

            changePage(SAVE_SLOT_PAGE);

            // SPECIAL: if slotIndex is the last item, then we want to save in the next slot
            if (slotIndex == groups[groupIndex].slotCount - 1)
            {
                saveInSlotIndex = slotIndex + 1;
            }
            else
            {
                saveInSlotIndex = slotIndex;
            }

            Serial.printf("DEBUG: SaveInSlotIndex %d, slotIndex %d\n", saveInSlotIndex, slotIndex);
            frozenLight = light;
        }
        else
        {
            mode = 1;
        }

        break;
    };
    case SAVE_SLOT_PAGE:
    {
        // TODO: Handle overflow (more than 100)
        // save this
        // groups[groupIndex].slots[saveInSlotIndex].light = {
        //     red
        // }
        Serial.printf("Saved in group %d, slot %d\n", groupIndex, saveInSlotIndex);
        Serial.printf("Saved R %d, G %d, B %d\n", frozenLight.r, frozenLight.g, frozenLight.b);

        changePage(HOME_PAGE);

        // case 1: isInsert = false, saveInSlotIndex = -1
        // => insert at the head of the list
        // case 2: isInsert = false, saveInSlotIndex = slotCount
        // => insert at the end of the list
        // case 3: isInsert = true, saveInSlotIndex = any except those 2
        // => insert inbetween
        if (isInsert == false && saveInSlotIndex == -1)
        {
            // insert at the head of the list
            for (int i = groups[groupIndex].slotCount; i > 0; i--)
            {
                groups[groupIndex].slots[i] = groups[groupIndex].slots[i - 1];
            }
            groups[groupIndex].slots[0].light = frozenLight;
            groups[groupIndex].slotCount = groups[groupIndex].slotCount + 1;
            slotIndex = 0; // show the newly inserted slot
        }
        else if (isInsert == false && saveInSlotIndex == groups[groupIndex].slotCount)
        {
            // insert at the end of the list
            groups[groupIndex].slots[groups[groupIndex].slotCount].light = frozenLight;
            groups[groupIndex].slotCount = groups[groupIndex].slotCount + 1;
            slotIndex = groups[groupIndex].slotCount - 1; // show the newly inserted slot
        }
        else
        {
            // insert inbetween
            for (int i = groups[groupIndex].slotCount; i > saveInSlotIndex; i--)
            {
                groups[groupIndex].slots[i] = groups[groupIndex].slots[i - 1];
            }
            groups[groupIndex].slots[saveInSlotIndex].light = frozenLight;
            groups[groupIndex].slotCount = groups[groupIndex].slotCount + 1;
            slotIndex = saveInSlotIndex; // show the newly inserted slot
        }

        save("Saving slot...");
        break;
    }

    case SETTINGS_PAGE:
    {
        // // go to HOME
        // changePage(HOME_PAGE);
        // break;
        if (settingsIndex == 0)
        {
            // delete item
            deleteIndex = slotIndex;
            changePage(DELETE_SLOT_PAGE);
        }
        if (settingsIndex == 1)
        {
            // change group
            groupSelectionIndex = groupIndex;
            changePage(CHANGE_GROUP_PAGE);
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

        // changePage(HOME_PAGE);
        if (groups[groupIndex].slotCount == 0)
        {
            changePage(HOME_PAGE);
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
        changePage(CHARACTER_INPUT_PAGE);
        break;
    }
    case CHARACTER_INPUT_PAGE:
    {
        // todo: handle long press
        // go left
        cursorPosition = (cursorPosition - 1 + GROUP_NAME_LENGTH) % GROUP_NAME_LENGTH;
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
        if (mode == 1)
        {
            // exit saving mode
            mode = 0;
        }
        else
        {
            changePage(SETTINGS_PAGE);
        }

        break;
    }

    case SAVE_SLOT_PAGE:
    {
        // CANCEL SAVING
        changePage(HOME_PAGE);
        break;
    }

    case SETTINGS_PAGE:
    {
        changePage(HOME_PAGE);
        break;
    }
    case DELETE_SLOT_PAGE:
    {
        changePage(SETTINGS_PAGE);
        break;
    }
    case CHANGE_GROUP_PAGE:
    {
        changePage(SETTINGS_PAGE);
        break;
    }
    case CHARACTER_INPUT_PAGE:
    {
        // go right
        cursorPosition = (cursorPosition + 1) % GROUP_NAME_LENGTH;
        break;
    }
    }
}

void Controller::onSend()
{
}

void Controller::onDown()
{
    switch (currentPage)
    {
    case HOME_PAGE:
    {
        if (mode == 1)
            return;
        if (groups[groupIndex].slotCount == 0)
            return;
        slotIndex = (slotIndex + 1) % groups[groupIndex].slotCount;
        break;
    }
    case SAVE_SLOT_PAGE:
    {
        // no looping
        if (saveInSlotIndex == groups[groupIndex].slotCount)
        {
        }
        else
        {
            if (saveInSlotIndex == -1 || saveInSlotIndex == 0)
            {
                saveInSlotIndex++;
                isInsert = false;
            }
            else if (saveInSlotIndex == groups[groupIndex].slotCount - 1)
            {
                saveInSlotIndex++;
                isInsert = false;
            }
            else if (!isInsert)
            {
                isInsert = true;
                saveInSlotIndex++;
            }
            else
            {
                isInsert = false;
            }
        }

        Serial.printf("Now looking at slot %d\n", saveInSlotIndex);

        break;
    }
    case SETTINGS_PAGE:
    {
        // go to HOME
        settingsIndex = (settingsIndex + 1) % SETTINGS_COUNT;
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
            else if (groupSelectionIndex == groups[groupIndex].slotCount - 1)
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
        if (mode == 1)
            return;

        if (groups[groupIndex].slotCount == 0)
            return;
        slotIndex = (slotIndex - 1 + groups[groupIndex].slotCount) % groups[groupIndex].slotCount;
        break;
    }
    case SAVE_SLOT_PAGE:
    {

        if (saveInSlotIndex == -1)
        {
        }
        else
        {
            if (saveInSlotIndex == groups[groupIndex].slotCount || saveInSlotIndex == 0)
            {
                saveInSlotIndex--;
                isInsert = false;
            }
            else if (!isInsert)
            {
                isInsert = true;
            }
            else
            {
                isInsert = false;

                saveInSlotIndex--;
            }
        }
        Serial.printf("Now looking at slot %d\n", saveInSlotIndex);

        break;
    }
    case SETTINGS_PAGE:
    {

        settingsIndex = (settingsIndex - 1 + SETTINGS_COUNT) % SETTINGS_COUNT;
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
    slotIndex = prefs.getInt("slotIndex", 0);
    groupIndex = prefs.getInt("groupIndex", 0);
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