#include "CueIndicator.h"
#include <cstdio>


// Constructor Implementation
CueIndicatorWidget::CueIndicatorWidget(int x, int y, int cueIndex, int cueCount)
    : BaseWidget(x, y), cueIndex(cueIndex), cueCount(cueCount)
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
    display->printf("%d", (this->cueIndex + 1));
    display->setTextSize(1);
    display->printf("/%d", this->cueCount);



};

void CueIndicatorWidget::updateCue(int cueIndex, int cueCount)
{
    this->cueIndex = cueIndex;
    this->cueCount = cueCount;
}

void CueIndicatorWidget::updateCueIndex(int cueIndex)
{
    this->cueIndex = cueIndex;
}

void CueIndicatorWidget::updateCueCount(int cueCount)
{
    this->cueCount = cueCount;
}

void CueIndicatorWidget::nextCue()
{
    // this->cueNumber = (this->cueNumber + 1) % this->lastCueNumber;
}

void CueIndicatorWidget::prevCue()
{
    // this->cueNumber = (this->cueNumber - 1 + this->lastCueNumber) % this->lastCueNumber;
}