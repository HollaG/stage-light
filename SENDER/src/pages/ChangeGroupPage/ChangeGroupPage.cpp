#include "ChangeGroupPage.h"
#include <sstream>
#define VERTICAL_SPACING 10

#define NEW_GROUP_ENTRY_LABEL "   New Group"
#define CREATE_NEW_GROUP_BUTTON "Create"
#define CHANGE_GROUP_BUTTON "Load"
#define INSERT_BEFORE_BUTTON "InsBef"

// Constructor Implementation
ChangeGroupPage::ChangeGroupPage()
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
    actionButton = new ButtonOverlayWidget(0, 64 - BUTTON_HEIGHT, "Change");
    cancelButton = new ButtonOverlayWidget(128 - BUTTON_WIDTH, 64 - BUTTON_HEIGHT, "Cancel");

    addWidget(actionButton);
    addWidget(cancelButton);
}
void ChangeGroupPage::update(Group *groups, int groupCount, int groupSelectionIndex, bool isInsBef, char *currentGroupName, char *newGroupName)
{
    // Update header and groups
    saveMenuHeader->updateName(currentGroupName);
    // this->groups = groups;
    // this->groupCount = groupCount;

    // Lambda function to format light labels
    auto formatGroupLabel = [](const Group &group, int index) -> std::string
    {
        std::stringstream ss;
        if (index + 1 < 10)
        {
            ss << " ";
        }
        ss << index + 1;
        ss << " " << group.name;
        // ss << " " << "R:" << slot.light.r << " G:" << slot.light.g << " B:" << slot.light.b;
        return ss.str();
    };

    // Case: No groups available
    if (groupCount == 0)
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

    // Case: At the very bottom (groupSelectionIndex == groupCount)
    if (groupSelectionIndex == groupCount)
    {
        nextItem->setHidden();
        prevItem->setIsDisplayed();
        currentItem->setIsDisplayed();
        prevItem->updateLabel(formatGroupLabel(groups[groupSelectionIndex - 1], groupSelectionIndex - 1));
        currentItem->updateLabel(NEW_GROUP_ENTRY_LABEL);
        nextItem->updateLabel("");
        if (isInsBef)
        {
            actionButton->updateLabel(INSERT_BEFORE_BUTTON);
        }
        else
        {
            actionButton->updateLabel(CREATE_NEW_GROUP_BUTTON);
        }
        return;
    }

    // Case: At the very top (groupSelectionIndex == -1)
    if (groupSelectionIndex == -1)
    {
        prevItem->setHidden();
        nextItem->setIsDisplayed();
        currentItem->setIsDisplayed();
        nextItem->updateLabel(formatGroupLabel(groups[groupSelectionIndex + 1], groupSelectionIndex + 1));
        currentItem->updateLabel(NEW_GROUP_ENTRY_LABEL);
        prevItem->updateLabel("");
        if (isInsBef)
        {
            actionButton->updateLabel(INSERT_BEFORE_BUTTON);
        }
        else
        {
            actionButton->updateLabel(CREATE_NEW_GROUP_BUTTON);
        }

        return;
    }

    // Case: The slot is the only available one (groupSelectionIndex == groupCount - 1)
    if (groupSelectionIndex == groupCount - 1 && groupSelectionIndex == 0)
    {
        prevItem->setIsDisplayed();
        nextItem->setIsDisplayed();
        currentItem->setIsDisplayed();
        prevItem->updateLabel(NEW_GROUP_ENTRY_LABEL);
        nextItem->updateLabel(NEW_GROUP_ENTRY_LABEL);
        currentItem->updateLabel(formatGroupLabel(groups[groupSelectionIndex], groupSelectionIndex));
        if (isInsBef)
        {
            actionButton->updateLabel(INSERT_BEFORE_BUTTON);
        }
        else
        {
            actionButton->updateLabel(CHANGE_GROUP_BUTTON);
        }

        return;
    }

    // Case: At the top of available groups (groupSelectionIndex == 0)
    if (groupSelectionIndex == 0)
    {
        prevItem->setIsDisplayed();
        nextItem->setIsDisplayed();
        currentItem->setIsDisplayed();
        prevItem->updateLabel(NEW_GROUP_ENTRY_LABEL);
        currentItem->updateLabel(formatGroupLabel(groups[groupSelectionIndex], groupSelectionIndex));
        nextItem->updateLabel(formatGroupLabel(groups[groupSelectionIndex + 1], groupSelectionIndex + 1));
        if (isInsBef)
        {
            actionButton->updateLabel(INSERT_BEFORE_BUTTON);
        }
        else
        {
            actionButton->updateLabel(CHANGE_GROUP_BUTTON);
        }

        return;
    }

    // Case: At the bottom of available groups
    if (groupSelectionIndex == groupCount - 1)
    {
        prevItem->setIsDisplayed();
        nextItem->setIsDisplayed();
        currentItem->setIsDisplayed();
        prevItem->updateLabel(formatGroupLabel(groups[groupSelectionIndex - 1], groupSelectionIndex - 1));
        currentItem->updateLabel(formatGroupLabel(groups[groupSelectionIndex], groupSelectionIndex));
        nextItem->updateLabel(NEW_GROUP_ENTRY_LABEL);
        if (isInsBef)
        {
            actionButton->updateLabel(INSERT_BEFORE_BUTTON);
        }
        else
        {
            actionButton->updateLabel(CHANGE_GROUP_BUTTON);
        }

        return;
    }

    // Case: In the middle of the list
    prevItem->setIsDisplayed();
    nextItem->setIsDisplayed();
    currentItem->setIsDisplayed();
    prevItem->updateLabel(formatGroupLabel(groups[groupSelectionIndex - 1], groupSelectionIndex - 1));
    currentItem->updateLabel(formatGroupLabel(groups[groupSelectionIndex], groupSelectionIndex));
    nextItem->updateLabel(formatGroupLabel(groups[groupSelectionIndex + 1], groupSelectionIndex + 1));
    if (isInsBef)
    {
        actionButton->updateLabel(INSERT_BEFORE_BUTTON);
    }
    else
    {
        actionButton->updateLabel(CHANGE_GROUP_BUTTON);
    }
}
