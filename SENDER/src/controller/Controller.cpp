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
    char name[16] = "Default";
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
        display->print(loadingLabel);
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
        char groupName[16];
        strcpy(groupName, groups[groupIndex].name);

        Slot *slots = groups[groupIndex].slots;
        int slotCount = groups[groupIndex].slotCount;

        baseDisplay->updateSaveSlotPage(display, groupName, slots, slotCount, saveInSlotIndex, isInsert);
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

void Controller::onScreenLeft()
{
    // 0 --> SAVE

    Serial.println("DEBUG: Screen Left Button Pressed");

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

        save();
    }
    };
}

void Controller::onScreenRight()
{

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

        break;
    }

    case SAVE_SLOT_PAGE:
    {
        // CANCEL SAVING
        changePage(HOME_PAGE);
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
    }
}

void Controller::save()
{
    isLoading = true;
    char loading[16] = "Saving slot...";
    strcpy(loadingLabel, loading);

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
    isLoading = true;
    char loading[16] = "Initializing...";
    strcpy(loadingLabel, loading);
    xTaskCreate(
        loadHelper,
        "backgroundLoad",
        16384,
        this,
        1,
        &saveTaskHandle);

    isLoading = false;
}

void Controller::loadHelper(void *parameter)
{
    Controller *instance = static_cast<Controller *>(parameter);
    instance->backgroundLoad();
    vTaskDelete(NULL);
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