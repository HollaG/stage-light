#include "DeleteSlotPage.h"
#include <sstream>
#define VERTICAL_SPACING 10

// Constructor Implementation
DeleteSlotPage::DeleteSlotPage()
{
    // initialize widgets
    // // create the prev, current, next items
    prevItem = new SaveMenuItemWidget(false, false, "", 0, MENU_HEIGHT);
    currentItem = new SaveMenuItemWidget(true, false, "", 0, MENU_HEIGHT + VERTICAL_SPACING);
    nextItem = new SaveMenuItemWidget(false, false, "", 0, MENU_HEIGHT + VERTICAL_SPACING * 2);

    addWidget(prevItem);
    addWidget(currentItem);
    addWidget(nextItem);

    // 1) Delete Menu Header
    saveMenuHeader = new SaveMenuHeaderWidget("");
    addWidget(saveMenuHeader);

    // Buttons at bottom
    actionButton = new ButtonOverlayWidget(0, 64 - BUTTON_HEIGHT, "Delete");
    cancelButton = new ButtonOverlayWidget(128 - BUTTON_WIDTH, 64 - BUTTON_HEIGHT, "Cancel");

    addWidget(actionButton);
    addWidget(cancelButton);
}
void DeleteSlotPage::update(char *groupName, Slot* slots, int index, int slotCount)
{
    // Update header and slots
    saveMenuHeader->updateName(groupName);
    this->slots = slots;
    this->slotCount = slotCount;
    actionButton->updateLabel("Delete");
    actionButton->showButton();

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
        currentItem->updateLabel("   No slots saved");
        nextItem->updateLabel("");
        prevItem->updateLabel("");
        actionButton->hideButton();

        return;
    }

    // Case: The slot is the only available one (index == slotCount - 1)
    if (index == slotCount - 1 && index == 0)
    {
        prevItem->setHidden();
        nextItem->setHidden();
        currentItem->setIsDisplayed();
        prevItem->updateLabel("");
        nextItem->updateLabel("");
        currentItem->updateLabel(formatLightLabel(slots[index], index));

        return;
    }

    // Case: At the top of available slots (index == 0)
    if (index == 0)
    {
        prevItem->setHidden();
        nextItem->setIsDisplayed();
        currentItem->setIsDisplayed();
        prevItem->updateLabel("");
        currentItem->updateLabel(formatLightLabel(slots[index], index));
        nextItem->updateLabel(formatLightLabel(slots[index + 1], index + 1));

        return;
    }

    // Case: At the bottom of available slots
    if (index == slotCount - 1)
    {
        prevItem->setIsDisplayed();
        nextItem->setHidden();
        currentItem->setIsDisplayed();
        prevItem->updateLabel(formatLightLabel(slots[index - 1], index - 1));
        currentItem->updateLabel(formatLightLabel(slots[index], index));
        nextItem->updateLabel("   ");

        return;
    }

    // Case: In the middle of the list
    prevItem->setIsDisplayed();
    nextItem->setIsDisplayed();
    currentItem->setIsDisplayed();
    prevItem->updateLabel(formatLightLabel(slots[index - 1], index - 1));
    currentItem->updateLabel(formatLightLabel(slots[index], index));
    nextItem->updateLabel(formatLightLabel(slots[index + 1], index + 1));
}
