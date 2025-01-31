#pragma once
#include <string>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "widgets/BaseWidget.h"

class PotWidget : public BaseWidget
{
public:
    int minValue;
    int value;
    std::string label;
    int maxValue;

    // Constructor
    PotWidget(int value, int minValue, int maxValue, std::string label, int x, int y);

    // Draw method
    virtual void draw(Adafruit_SSD1306 *display);

    // Update value
    void updateValue(int value);
};


