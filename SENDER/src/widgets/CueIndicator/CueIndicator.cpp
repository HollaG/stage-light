#include "CueIndicator.h"
#include <cstdio>


// Constructor Implementation
CueIndicatorWidget::CueIndicatorWidget(int x, int y, int cueNumber, int lastCueNumber)
    : BaseWidget(x, y), cueNumber(cueNumber), lastCueNumber(lastCueNumber)
{
}

// Draw method implementation
void CueIndicatorWidget::draw(Adafruit_SSD1306 *display)
{
    // start point is given as the top left 
    // corner of the button
    // only the top left and right corner should be radiused
    display->setCursor(x, y);
    display->setTextSize(3);
    display->printf("%d", this->cueNumber);
    display->setTextSize(1);
    display->printf("/%d", this->lastCueNumber);



};

void CueIndicatorWidget::updateCueNumber(int cueNumber)
{
    this->cueNumber = cueNumber;
}

void CueIndicatorWidget::updateLastCueNumber(int lastCueNumber)
{
    this->lastCueNumber = lastCueNumber;
}

void CueIndicatorWidget::nextCue()
{
    this->cueNumber = (this->cueNumber + 1) % this->lastCueNumber;
}

void CueIndicatorWidget::prevCue()
{
    this->cueNumber = (this->cueNumber - 1 + this->lastCueNumber) % this->lastCueNumber;
}