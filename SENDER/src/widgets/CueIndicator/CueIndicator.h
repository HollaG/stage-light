#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "widgets/BaseWidget.h"


class CueIndicatorWidget : public BaseWidget
{
public:
    int cueNumber;
    int lastCueNumber;

    // Constructor
    CueIndicatorWidget(int x, int y, int cueNumber, int lastCueNumber);

    void draw(Adafruit_SSD1306 *display);

    void updateCueNumber(int cueNumber);
    void updateLastCueNumber(int lastCueNumber);

    void nextCue();
    void prevCue();


};
