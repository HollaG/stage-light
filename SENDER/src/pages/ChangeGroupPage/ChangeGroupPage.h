#pragma once
#include "pages/BasePage.h"
#include "string"
#include "widgets/Save/SaveMenuHeader/SaveMenuHeader.h"
#include "widgets/ButtonOverlay/ButtonOverlay.h"
#include "structs/structs.h"
#include "widgets/Save/SaveMenuItem/SaveMenuItem.h"

class ChangeGroupPage : public BasePage
{
public:
    // Constructor
    ChangeGroupPage();

    // variables
    Slot *slots = NULL;
    int slotCount;
    int saveInSlotIndex;

    Group *groups;

    // Widgets

    SaveMenuHeaderWidget *saveMenuHeader;

    ButtonOverlayWidget *actionButton;
    ButtonOverlayWidget *cancelButton;

    SaveMenuItemWidget *prevItem;
    SaveMenuItemWidget *nextItem;
    SaveMenuItemWidget *currentItem;

    // update
    void update(Group *groups, int groupCount, int groupSelectionIndex, bool isInsert, char *currentGroupName, char *newGroupName);
};
