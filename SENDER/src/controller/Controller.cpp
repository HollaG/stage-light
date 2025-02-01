#include "Controller.h"

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

void Controller::refreshPage(Adafruit_SSD1306 *display)
{
    // depending on the current page index, refresh the page by calling the relevant functions
    switch (currentPageIndex)
    {
    case 0:
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
    case 1:
    {
        char groupName[16];
        strcpy(groupName, groups[groupIndex].name);

        Slot *slots = groups[groupIndex].slots;
        int slotCount = groups[groupIndex].slotCount;

        baseDisplay->updateSaveSlotPage(display, groupName, slots, slotCount, saveInSlotIndex, false);
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

    switch (currentPageIndex)
    {
    case 0:
    {
        // go to Saving page
        if (mode == 1)
        {

            currentPageIndex = 1;

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
    case 1:
    {
        // TODO: Handle overflow (more than 100)
        // save this
        // groups[groupIndex].slots[saveInSlotIndex].light = {
        //     red
        // }
        Serial.printf("Saved in group %d, slot %d\n", groupIndex, saveInSlotIndex);
        Serial.printf("Saved R %d, G %d, B %d\n", frozenLight.r, frozenLight.g, frozenLight.b);

        currentPageIndex = 0;

        if (saveInSlotIndex == -1 || saveInSlotIndex == groups[groupIndex].slotCount)
        {
            groups[groupIndex].slots[groups[groupIndex].slotCount].light = frozenLight;
            groups[groupIndex].slotCount = groups[groupIndex].slotCount + 1;
            slotIndex = groups[groupIndex].slotCount - 1;
            Serial.printf("DEBUG: Slot count %d\n", groups[groupIndex].slotCount);
        }
        else
        {
            // overwrite mode
            // TODO allow INSERTION inbetween
            groups[groupIndex].slots[saveInSlotIndex].light = frozenLight;
            Serial.printf("DEBUG: Slot count %d\n", groups[groupIndex].slotCount);
        }

        save();
    }
    };
}

void Controller::onScreenRight()
{

    switch (currentPageIndex)
    {
    case 0:
    {
        // exit saving mode
        if (mode == 1)
        {
            // exit saving mode
            mode = 0;
        }

        break;
    }

    case 1:
    {
        // CANCEL SAVING
        currentPageIndex = 0;
        break;
    }
    }
}

void Controller::onSend()
{
}

void Controller::onDown()
{
    switch (currentPageIndex)
    {
    case 0:
    {
        if (mode == 1)
            return;
        if (groups[groupIndex].slotCount == 0)
            return;
        slotIndex = (slotIndex + 1) % groups[groupIndex].slotCount;
        break;
    }
    case 1:
    {
        // no looping
        if (saveInSlotIndex == groups[groupIndex].slotCount)
        {
        }
        else
        {

            saveInSlotIndex++;
        }

        Serial.printf("Now looking at slot %d\n", saveInSlotIndex);

        break;
    }
    }
}

void Controller::onUp()
{

    switch (currentPageIndex)
    {
    case 0:
    {
        if (mode == 1)
            return;

        if (groups[groupIndex].slotCount == 0)
            return;
        slotIndex = (slotIndex - 1 + groups[groupIndex].slotCount) % groups[groupIndex].slotCount;
        break;
    }
    case 1:
    {

        if (saveInSlotIndex == -1)
        {
        }
        else
        {

            saveInSlotIndex--;
        }
        Serial.printf("Now looking at slot %d\n", saveInSlotIndex);

        break;
    }
    }
}

void Controller::save()
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
