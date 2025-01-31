#pragma once
#include <string>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "widgets/BaseWidget.h"

#define MENU_HEIGHT 14 // Padding included

class SaveMenuHeaderWidget : public BaseWidget
{
public:
    char *name;
    // Constructor
    SaveMenuHeaderWidget(char *name);

    // Draw method
    virtual void draw(Adafruit_SSD1306 *display);

    void updateName(char *name);
};


