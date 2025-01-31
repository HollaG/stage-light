#include "SaveMenuItem.h"
#include <cstdio>

// Constructor Implementation
SaveMenuItemWidget::SaveMenuItemWidget(bool isSelected,  bool isDisplayed, std::string label, int x, int y) : BaseWidget(x, y), isSelected(isSelected), isDisplayed(isDisplayed), label(label)
{
}

void SaveMenuItemWidget::draw(Adafruit_SSD1306 *display)
{
    display->setCursor(x, y + 1);
    display->setTextWrap(false);

    std::string text = "  Empty slot";
    if (label.length() > 0)
    {
        text = "  " + label;
    }

    if (isSelected)
    {
        display->fillRect(x, y, 128, 10, SSD1306_WHITE);
        display->setTextColor(SSD1306_BLACK);
        display->printf("  %s", this->label.c_str());
    }
    else
    {
        display->setTextColor(SSD1306_WHITE);
        display->printf("  %s", this->label.c_str());
    }

    // display->setCursor(x, y);
    // // write the text "label"
    // display->printf("%s", this->label.c_str());
    // // draw a solid rectangle 8px in height, 32px in length
    // display->drawRect(x + 10, y, 34, 8, SSD1306_WHITE);
    // // // cut out the "internal" space
    // // display->drawRect(x + 11, y + 1, 32, 6, SSD1306_BLACK);

    // float ratio = (float)(value - minValue) / (maxValue - minValue);
    // display->fillRect(x + 11, y + 1, 32 * ratio, 6, SSD1306_WHITE);
    // // add an additional label to the RIGHT of this rectangle
    // display->setCursor(x + 10 + 34 + 2, y);
    // display->printf(" %d", value);
}

void SaveMenuItemWidget::updateLabel(std::string label)
{
    this->label = label;
}

void SaveMenuItemWidget::setIsDisplayed() { 
    this->isDisplayed = true;
}

void SaveMenuItemWidget::setHidden() { 
    this->isDisplayed = false;
}