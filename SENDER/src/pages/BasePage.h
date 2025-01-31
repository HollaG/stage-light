#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "widgets/BaseWidget.h"  // Include the header, not .cpp

class BasePage
{
public:
    // Contains an array of widgets (pointers to BaseWidget)
    BaseWidget *widgets[20];

    // Constructor
    BasePage();

    // Draw method
    void draw(Adafruit_SSD1306 *display);

    // Add a widget
    void addWidget(BaseWidget *widget);


};
