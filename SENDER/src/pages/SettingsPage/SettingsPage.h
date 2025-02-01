#pragma once
#include "pages/BasePage.h"
#include "string"
#include "widgets/Save/SaveMenuHeader/SaveMenuHeader.h"
#include "widgets/ButtonOverlay/ButtonOverlay.h"
#include "structs/structs.h"
#include "widgets/Save/SaveMenuItem/SaveMenuItem.h"

#define SETTINGS_COUNT 3

class SettingsPage : public BasePage
{
public:
    // Constructor
    SettingsPage();

    // variables
    char *groupName;

    // create array of char[16] for each menu option
    std::string options[SETTINGS_COUNT] = {"  Delete item", "  Change group", "  Scan for receivers"};
    // 1) Delete item
    // 2) Change group

    // Widgets

    SaveMenuHeaderWidget *saveMenuHeader;

    ButtonOverlayWidget *actionButton;
    ButtonOverlayWidget *cancelButton;

    SaveMenuItemWidget *prevItem;
    SaveMenuItemWidget *nextItem;
    SaveMenuItemWidget *currentItem;

    // update
    void update(char *groupName, int index);

};
