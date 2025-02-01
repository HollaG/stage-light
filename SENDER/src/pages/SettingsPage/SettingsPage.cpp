#include "SettingsPage.h"
#include <sstream>
#define VERTICAL_SPACING 10

// Constructor Implementation
SettingsPage::SettingsPage()
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
    actionButton = new ButtonOverlayWidget(0, 64 - BUTTON_HEIGHT, "Enter");
    cancelButton = new ButtonOverlayWidget(128 - BUTTON_WIDTH, 64 - BUTTON_HEIGHT, "Cancel");

    addWidget(actionButton);
    addWidget(cancelButton);
}
void SettingsPage::update(char *groupName, int index)
{
    // Update header and slots
    saveMenuHeader->updateName("Settings");


    // looping lis
    int previousIndex = (index - 1 + SETTINGS_COUNT) % SETTINGS_COUNT;
    int nextIndex = (index + 1) % SETTINGS_COUNT;



    // update the labels
    prevItem->updateLabel(options[previousIndex]);
    currentItem->updateLabel(options[index]);
    nextItem->updateLabel(options[nextIndex]);

}

