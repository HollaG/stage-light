#pragma once
#include <string>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "widgets/BaseWidget.h"

#define MENU_HEIGHT 14 // padding included

class MenuWidget : public BaseWidget
{
public:
    int curIndex;
    int nextIndex;
    std::string presetName;
    int connectedCount;

    // Constructor
    MenuWidget(int x, int y, int curIndex, int nextIndex, std::string presetName, int connectedCount);

    // Draw method
    virtual void draw(Adafruit_SSD1306 *display);

    // Update value
    void updateIndex(int curIndex, int nextIndex);
    void updateConnectedCount(int connectedCount);
};
