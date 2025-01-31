#pragma once
#include "pages/BasePage.h"
#include "string"
#include "widgets/Save/SaveMenuHeader/SaveMenuHeader.h"
#include "widgets/ButtonOverlay/ButtonOverlay.h"
#include "structs/structs.h"
#include "widgets/Save/SaveMenuItem/SaveMenuItem.h"


class SaveSlotPage : public BasePage
{
public:
    // Constructor
    SaveSlotPage();

    // variables
    char *groupName;
    Slot *slots = NULL;
    int slotCount;
    int saveInSlotIndex;



    // Widgets

    SaveMenuHeaderWidget *saveMenuHeader;

    ButtonOverlayWidget *actionButton;
    ButtonOverlayWidget *cancelButton;

    // array of save menu items
    SaveMenuItemWidget *saveMenuItems[99];

    SaveMenuItemWidget *prevItem;
    SaveMenuItemWidget *nextItem;
    SaveMenuItemWidget *currentItem;





    // update 
    void update(char *groupName, Slot *slots, int slotCount, int currentSlot, bool isInsert);
    
};


