#pragma once
#include <string>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "widgets/BaseWidget.h"

#define BUTTON_WIDTH 48
#define BUTTON_HEIGHT 12
#define BUTTON_RADIUS 3

class ButtonOverlayWidget : public BaseWidget
{
public:
    std::string buttonLabel;

    bool isButtonVisible = true;

    // Constructor
    ButtonOverlayWidget(int x, int y, std::string buttonLabel);

    void draw(Adafruit_SSD1306 *display);

    void updateLabel(std::string label);

    void hideButton();
    void showButton();
};
