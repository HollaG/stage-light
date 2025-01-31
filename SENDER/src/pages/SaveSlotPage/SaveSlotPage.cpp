#include "SaveSlotPage.h"
#include <sstream>
#define VERTICAL_SPACING 10

// Constructor Implementation
SaveSlotPage::SaveSlotPage()
{
    // initialize widgets

    // 1) Save Menu Header
    saveMenuHeader = new SaveMenuHeaderWidget("");
    addWidget(saveMenuHeader);

    // Buttons at bottom
    actionButton = new ButtonOverlayWidget(0, 64 - BUTTON_HEIGHT, "Save");
    cancelButton = new ButtonOverlayWidget(128 - BUTTON_WIDTH, 64 - BUTTON_HEIGHT, "Cancel");

    addWidget(actionButton);
    addWidget(cancelButton);

    // // create 99 save menu items
    // for (int i = 0; i < 99; i++)
    // {
    //     saveMenuItems[i] = new SaveMenuItemWidget(false, false, "", 0, 0);
    //     addWidget(saveMenuItems[i]);
    // }

    // // create the prev, current, next items
    prevItem = new SaveMenuItemWidget(false, false, "", 0, MENU_HEIGHT);
    currentItem = new SaveMenuItemWidget(true, false, "", 0, MENU_HEIGHT + VERTICAL_SPACING);
    nextItem = new SaveMenuItemWidget(false, false, "", 0, MENU_HEIGHT + VERTICAL_SPACING * 2);

    addWidget(prevItem);
    addWidget(currentItem);
    addWidget(nextItem);
}

void SaveSlotPage::update(char *groupName, Slot *slots, int slotCount, int saveInSlotIndex, bool isInsert)
{
    // update the values in save slot page
    // call a refresh
    saveMenuHeader->updateName(groupName);
    this->slots = slots;
    this->slotCount = slotCount;

    // we can only display 3 items on screens at one time
    //  items on screen should be
    // saveInSlotIndex - 1, saveInSlotIndex, saveInSlotIndex + 1

    // TODO: ENSURE ITS NOT FULL!!!!
    if (slotCount == 0)
    {
        nextItem->setHidden();
        prevItem->setHidden();
        currentItem->setIsDisplayed();
        currentItem->updateLabel("Unsaved slot");
    }
    else if (saveInSlotIndex == slotCount)
    {
        // we are at the bottom of the list
        // hide the next item
        nextItem->setHidden();
        prevItem->setIsDisplayed();
        currentItem->setIsDisplayed();
        std::stringstream ss;
        ss << "R:" << slots[saveInSlotIndex - 1].light.r << " G:" << slots[saveInSlotIndex - 1].light.g << " B:" << slots[saveInSlotIndex - 1].light.b;
        prevItem->updateLabel(ss.str());

        currentItem->updateLabel("Unsaved slot");

    }
    else

        if (saveInSlotIndex == -1)
    {
        // we are at the top of the list
        // hide the prev item
        prevItem->setHidden();
        nextItem->setIsDisplayed();
        currentItem->setIsDisplayed();

        std::stringstream ss;
        ss << "R:" << slots[saveInSlotIndex + 1].light.r << " G:" << slots[saveInSlotIndex + 1].light.g << " B:" << slots[saveInSlotIndex + 1].light.b;
        nextItem->updateLabel(ss.str());

        currentItem->updateLabel("Unsaved slot");
    }
    else
    {
        // we are in the middle of the list
        prevItem->setIsDisplayed();
        nextItem->setIsDisplayed();
        currentItem->setIsDisplayed();

        std::stringstream ss;
        ss <<  "R:" <<slots[saveInSlotIndex - 1].light.r << " G:" << slots[saveInSlotIndex - 1].light.g << " B:" << slots[saveInSlotIndex - 1].light.b;
        prevItem->updateLabel(ss.str());

        std::stringstream ss2;
        ss2 <<  "R:" <<slots[saveInSlotIndex].light.r << " G:" << slots[saveInSlotIndex].light.g << " B:" << slots[saveInSlotIndex].light.b;
        currentItem->updateLabel(ss2.str());

        std::stringstream ss3;
        ss3 <<  "R:" <<slots[saveInSlotIndex + 1].light.r << " G:" << slots[saveInSlotIndex + 1].light.g << " B:" << slots[saveInSlotIndex + 1].light.b;
        nextItem->updateLabel(ss3.str());
    }


}