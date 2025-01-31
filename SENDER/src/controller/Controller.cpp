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

        baseDisplay->updateSaveSlotPage(display, groupName, slots, slotCount, slotIndex, false);
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

        saveInGroupIndex = slotIndex;

        break;
    };
    case 1: 
    {
        // save this 
        // groups[groupIndex].slots[saveInSlotIndex].light = { 
        //     red
        // }
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

void Controller::onSend() {

}

void Controller::onDown() {
  
}

void Controller::onUp() {
    // prevSlot();
}