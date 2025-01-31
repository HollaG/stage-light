#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "widgets/BaseWidget.h"


class CueIndicatorWidget : public BaseWidget
{
public:
    int cueIndex;
    int cueCount;

    // Constructor
    CueIndicatorWidget(int x, int y, int cueIndex, int cueCount);

    void draw(Adafruit_SSD1306 *display);

    void updateCueIndex(int cueIndex);
    void updateCueCount(int cueCount);

    void updateCue(int cueIndex, int cueCount);

    void nextCue();
    void prevCue();


};
