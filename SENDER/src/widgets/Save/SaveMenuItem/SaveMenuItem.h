#pragma once
#include <string>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "widgets/BaseWidget.h"

class SaveMenuItemWidget : public BaseWidget
{
public:
    bool isSelected = false;
    std::string label;
    bool isDisplayed = false;

    // Constructor
    SaveMenuItemWidget(bool isSelected, bool isDisplayed, std::string label, int x, int y);

    // Draw method
    virtual void draw(Adafruit_SSD1306 *display);

    void setIsDisplayed();
    void updateLabel(std::string label);
    
    void setHidden();

    // Update value
    void select(int value);

    void deselect(int value);
};


