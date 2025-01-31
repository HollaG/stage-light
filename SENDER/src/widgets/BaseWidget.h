#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ALL widgets must implement an update function

// BaseWidget: A drawable component for the display
class BaseWidget
{
public:
    int x, y;

    // Constructor
    BaseWidget(int x, int y);

    void updatePos(int x, int y);
    

    // Pure virtual function (must be implemented in derived classes)
    virtual void draw(Adafruit_SSD1306 *display) = 0;
};
