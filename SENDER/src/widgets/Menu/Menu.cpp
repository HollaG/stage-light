#include "Menu.h"
#include <cstdio>

// Constructor Implementation
MenuWidget::MenuWidget(int x, int y, int curIndex, int nextIndex, std::string presetName, int connectedCount)
    : BaseWidget(x, y), curIndex(curIndex), nextIndex(nextIndex), presetName(presetName), connectedCount(connectedCount)
{
}

// Draw method implementation
void MenuWidget::draw(Adafruit_SSD1306 *display)
{
    // display->printf("%d -> %d", curIndex, nextIndex);
    display->printf("DONDON");
    display->drawFastHLine(0, y + 10, 128, SSD1306_WHITE);

    // calculate width of "connected devices" text
    int16_t x1, y1;
    uint16_t w, h;

    char buffer[4]; // Ensure buffer is large enough
    sprintf(buffer, "%d", this->connectedCount);

    display->getTextBounds(buffer, 0, 0, &x1, &y1, &w, &h);

    // draw a 3px circle right before the text 
    display->fillCircle(128 - w - 10, y + 3, 3, SSD1306_WHITE);

    display->setCursor(128 - w, y);
    display->print(this->connectedCount);

}

// Update value
void MenuWidget::updateIndex(int curIndex, int nextIndex)
{
    this->curIndex = curIndex;
    this->nextIndex = nextIndex;
}

void MenuWidget::updateConnectedCount(int connectedCount)
{
    this->connectedCount = connectedCount;
}