#include "SaveMenuItem.h"
#include <cstdio>

// Constructor Implementation
SaveMenuItemWidget::SaveMenuItemWidget(bool isSelected, bool isDisplayed, std::string label, int x, int y) : BaseWidget(x, y), isSelected(isSelected), isDisplayed(isDisplayed), label(label)
{
}

void SaveMenuItemWidget::draw(Adafruit_SSD1306 *display)
{
    display->setCursor(x, y + 1);
    display->setTextWrap(false);





    if (isSelected)
    {
        display->fillRect(x, y, 128, 10, SSD1306_WHITE);
        display->setTextColor(SSD1306_BLACK);
        display->printf("%s", this->label.c_str());
    }
    else
    {
        display->setTextColor(SSD1306_WHITE);
        display->printf("%s", this->label.c_str());
    }
}

void SaveMenuItemWidget::updateLabel(std::string label)
{
    this->label = label;
}

void SaveMenuItemWidget::setIsDisplayed()
{
    this->isDisplayed = true;
}

// For some reason this isn't working, but still call the function. To be fixed 
void SaveMenuItemWidget::setHidden()
{
    this->isDisplayed = false;
}