#pragma once
#include "pages/BasePage.h"
#include "string"
#include "widgets/Potentiometer/Potentiometer.h"
#include "widgets/Menu/Menu.h"
#include "widgets/ButtonOverlay/ButtonOverlay.h"
#include "widgets/CueIndicator/CueIndicator.h"


class HomePage : public BasePage
{
public:
    // Constructor
    HomePage();

    // all the relevant data used in homepage should be stored here
    int curRed; int curGreen; int curBlue;
    int curIndex;
    int nextIndex;

    int connectedCount;

    std::string presetName;

    // Widgets
    PotWidget *redPot;
    PotWidget *greenPot;
    PotWidget *bluePot;

    MenuWidget *menuWidget;

    ButtonOverlayWidget *actionButton;
    ButtonOverlayWidget *cancelButton;

    CueIndicatorWidget *cueIndicator;



    // update 
    void update(int red, int green, int blue, int index, int connectedCount, std::string presetName);
    
};


