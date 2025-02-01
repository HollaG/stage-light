#include "SaveSlotPage.h"
#include <sstream>
#define VERTICAL_SPACING 10

// Constructor Implementation
SaveSlotPage::SaveSlotPage()
{
    // initialize widgets
    // // create the prev, current, next items
    prevItem = new SaveMenuItemWidget(false, false, "", 0, MENU_HEIGHT);
    currentItem = new SaveMenuItemWidget(true, false, "", 0, MENU_HEIGHT + VERTICAL_SPACING);
    nextItem = new SaveMenuItemWidget(false, false, "", 0, MENU_HEIGHT + VERTICAL_SPACING * 2);

    addWidget(prevItem);
    addWidget(currentItem);
    addWidget(nextItem);

    // 1) Save Menu Header
    saveMenuHeader = new SaveMenuHeaderWidget("");
    addWidget(saveMenuHeader);

    // Buttons at bottom
    actionButton = new ButtonOverlayWidget(0, 64 - BUTTON_HEIGHT, "Save");
    cancelButton = new ButtonOverlayWidget(128 - BUTTON_WIDTH, 64 - BUTTON_HEIGHT, "Cancel");

    addWidget(actionButton);
    addWidget(cancelButton);

}
void SaveSlotPage::update(char *groupName, Slot *slots, int slotCount, int saveInSlotIndex, bool isInsBef)
{
    // Update header and slots
    saveMenuHeader->updateName(groupName);
    this->slots = slots;
    this->slotCount = slotCount;

    // Lambda function to format light labels
    auto formatLightLabel = [](const Slot &slot, int index) -> std::string
    {
        std::stringstream ss;
        if (index + 1 < 10)
        {
            ss << " ";
        }
        ss << index + 1;
        ss << " " << "R:" << slot.light.r << " G:" << slot.light.g << " B:" << slot.light.b;
        return ss.str();
    };

    // Case: No slots available
    if (slotCount == 0)
    {
        nextItem->setHidden();
        prevItem->setHidden();
        currentItem->setIsDisplayed();
        currentItem->updateLabel("   Unsaved slot");
        nextItem->updateLabel("");
        prevItem->updateLabel("");
        actionButton->updateLabel("Save");
        return;
    }

    // Case: At the very bottom (saveInSlotIndex == slotCount)
    if (saveInSlotIndex == slotCount)
    {
        nextItem->setHidden();
        prevItem->setIsDisplayed();
        currentItem->setIsDisplayed();
        prevItem->updateLabel(formatLightLabel(slots[saveInSlotIndex - 1], saveInSlotIndex - 1));
        currentItem->updateLabel("   Unsaved slot");
        nextItem->updateLabel("");
        if (isInsBef)
        {
            actionButton->updateLabel("InsBef");
        }
        else
        {
            actionButton->updateLabel("Save");
        }
        return;
    }

    // Case: At the very top (saveInSlotIndex == -1)
    if (saveInSlotIndex == -1)
    {
        prevItem->setHidden();
        nextItem->setIsDisplayed();
        currentItem->setIsDisplayed();
        nextItem->updateLabel(formatLightLabel(slots[saveInSlotIndex + 1], saveInSlotIndex + 1));
        currentItem->updateLabel("   Unsaved slot");
        prevItem->updateLabel("");
        if (isInsBef)
        {
            actionButton->updateLabel("InsBef");
        }
        else
        {
            actionButton->updateLabel("Save");
        }

        return;
    }

    // Case: The slot is the only available one (saveInSlotIndex == slotCount - 1)
    if (saveInSlotIndex == slotCount - 1 && saveInSlotIndex == 0)
    {
        prevItem->setIsDisplayed();
        nextItem->setIsDisplayed();
        currentItem->setIsDisplayed();
        prevItem->updateLabel("   Unsaved slot");
        nextItem->updateLabel("   Unsaved slot");
        currentItem->updateLabel(formatLightLabel(slots[saveInSlotIndex], saveInSlotIndex));
        if (isInsBef)
        {
            actionButton->updateLabel("InsBef");
        }
        else
        {
            actionButton->updateLabel("OvWrite");
        }

        return;
    }

    // Case: At the top of available slots (saveInSlotIndex == 0)
    if (saveInSlotIndex == 0)
    {
        prevItem->setIsDisplayed();
        nextItem->setIsDisplayed();
        currentItem->setIsDisplayed();
        prevItem->updateLabel("   Unsaved slot");
        currentItem->updateLabel(formatLightLabel(slots[saveInSlotIndex], saveInSlotIndex));
        nextItem->updateLabel(formatLightLabel(slots[saveInSlotIndex + 1], saveInSlotIndex + 1));
        if (isInsBef)
        {
            actionButton->updateLabel("InsBef");
        }
        else
        {
            actionButton->updateLabel("OvWrite");
        }

        return;
    }

    // Case: At the bottom of available slots
    if (saveInSlotIndex == slotCount - 1)
    {
        prevItem->setIsDisplayed();
        nextItem->setIsDisplayed();
        currentItem->setIsDisplayed();
        prevItem->updateLabel(formatLightLabel(slots[saveInSlotIndex - 1], saveInSlotIndex - 1));
        currentItem->updateLabel(formatLightLabel(slots[saveInSlotIndex], saveInSlotIndex));
        nextItem->updateLabel("   Unsaved slot");
        if (isInsBef)
        {
            actionButton->updateLabel("InsBef");
        }
        else
        {
            actionButton->updateLabel("OvWrite");
        }

        return;
    }

    // Case: In the middle of the list
    prevItem->setIsDisplayed();
    nextItem->setIsDisplayed();
    currentItem->setIsDisplayed();
    prevItem->updateLabel(formatLightLabel(slots[saveInSlotIndex - 1], saveInSlotIndex - 1));
    currentItem->updateLabel(formatLightLabel(slots[saveInSlotIndex], saveInSlotIndex));
    nextItem->updateLabel(formatLightLabel(slots[saveInSlotIndex + 1], saveInSlotIndex + 1));
    if (isInsBef)
    {
        actionButton->updateLabel("InsBef");
    }
    else
    {
        actionButton->updateLabel("OvWrite");
    }
}
