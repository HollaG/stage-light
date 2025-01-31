#include "HomePage.h"
#include "widgets/Menu/Menu.h" // Include the header, not the .cpp file
#include "widgets/Potentiometer/Potentiometer.h"
#include "widgets/ButtonOverlay/ButtonOverlay.h"
#include "widgets/CueIndicator/CueIndicator.h"

#define VERTICAL_SPACING 10

// Constructor Implementation
HomePage::HomePage()
{
    // Initialize the home page and add widgets

    // 1.) Menu widget
    menuWidget = new MenuWidget(0, 0, 0, 1, "", 0);
    addWidget(menuWidget);

    // 1) Potentiometer widgets
    // *redPot = new PotWidget(0, 255, "Red", 30, 64 / 2);
    redPot = new PotWidget(curRed, 0, 255, "R", 0, MENU_HEIGHT);
    // redPot = new PotWidget(curRed, 255, "Red", 30, 64 / 2);
    addWidget(redPot);

    greenPot = new PotWidget(curGreen, 0, 255, "G", 0, MENU_HEIGHT + VERTICAL_SPACING);
    // greenPot = new PotWidget(curGreen, 255, "Green", 128 / 2, 64 / 2);
    addWidget(greenPot);

    bluePot = new PotWidget(curBlue, 0, 255, "B", 0, MENU_HEIGHT + VERTICAL_SPACING * 2);
    // bluePot = new PotWidget(curBlue, 255, "Blue", 128 - 30, 64 / 2);
    addWidget(bluePot);

    // Buttons at bottom
    actionButton = new ButtonOverlayWidget(0, 64 - BUTTON_HEIGHT, "Save");
    cancelButton = new ButtonOverlayWidget(128 - BUTTON_WIDTH, 64 - BUTTON_HEIGHT, "Cancel");

    addWidget(actionButton);
    addWidget(cancelButton);

    // cue number
    cueIndicator = new CueIndicatorWidget(70, MENU_HEIGHT + 5, 0, 0);
    addWidget(cueIndicator);
}

void HomePage::update(int red, int green, int blue, int slotIndex, int slotCount, int connectedCount, std::string presetName, int mode)
{
    // Update the values in the home page
    // This function will be called from the main loop
    // to update the values in the home page
    // and then call the draw function to refresh the screen
    // with the updated values

    // only update if each param is not NULL --> NULL indicates no change
    // if (red != curRed) {
    // curRed = red;
    redPot->updateValue(red);
    // }

    // if (green != curGreen)
    // {
    // curGreen = green;
    greenPot->updateValue(green);
    // }

    // if (blue != curBlue)
    // {
    // curBlue = blue;
    bluePot->updateValue(blue);
    // }

    // if (slotIndex != curIndex)
    // {
    // curIndex = index;
    menuWidget->updateIndex(curIndex, nextIndex);

    // }

    // if (connectedCount != this->connectedCount)
    // {
    // this->connectedCount = connectedCount;
    menuWidget->updateConnectedCount(connectedCount);
    // }

    // if (presetName != "")
    // {
    this->presetName = presetName;
    // }

    cueIndicator->updateCue(slotIndex, slotCount);

    this->mode = mode;

    if (mode == 1)
    {
        actionButton->updateLabel("Save");
        cancelButton->showButton();
    }
    else
    {
        actionButton->updateLabel("Edit");
        cancelButton->hideButton();
    }
}
