#include "Controller.h"

Controller::Controller(BaseDisplay *baseDisplay) : baseDisplay(baseDisplay)
{
    // create initial group if not exists
    // TODO: implement saving

    Group group;
    char name[16] = "Default";
    strcpy(group.name, name);
    group.slotCount = 0;

    Light light = {127, 127, 127};
    Slot slot = {light};

    group.slots[0] = slot;
    groups[0] = group;
    groupExists[0] = true;

    slotIndex = 0; // NOTE THAT THIS IS 0-INDEXED
    groupIndex = 0;
    groupCount = 1; // this is a COUNT
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

        baseDisplay->updateHomePage(display, light.r, light.g, light.b, slotIndex, slotCount, connectedCount, "");
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
    return groups[groupIndex].slots[slotIndex].light;
}

void Controller::onScreenLeft()
{
    // 0 --> SAVE

    switch (currentPageIndex)
    {
    case 0:
    {
        // go to Saving page
        currentPageIndex = 1;

        this->saveInSlotIndex = slotIndex;
        Serial.printf("DEBUG: SaveInSlotIndex %d, slotIndex %d\n", saveInSlotIndex, slotIndex);
        frozenLight = light;

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
    }
    };
}

void Controller::onScreenRight()
{

    switch (currentPageIndex)
    {
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