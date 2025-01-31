#include "SaveMenuHeader.h"
#include <cstdio>

// Constructor Implementation
SaveMenuHeaderWidget::SaveMenuHeaderWidget(char *name) : BaseWidget(0, 0)
{
    this->name = name;
}

void SaveMenuHeaderWidget::draw(Adafruit_SSD1306 *display)
{
    display->setCursor(x, y);
    // write the text "label"
    display->print(name);
    // draw a line below
    display->drawFastHLine(x, y + 10, 128, SSD1306_WHITE);
}

void SaveMenuHeaderWidget::updateName(char *name)
{
    this->name = name;
}