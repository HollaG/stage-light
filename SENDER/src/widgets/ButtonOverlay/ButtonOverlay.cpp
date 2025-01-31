#include "ButtonOverlay.h"
#include <cstdio>

// Constructor Implementation
ButtonOverlayWidget::ButtonOverlayWidget(int x, int y, std::string buttonLabel)
    : BaseWidget(x, y), buttonLabel(buttonLabel)
{
}

// Draw method implementation
void ButtonOverlayWidget::draw(Adafruit_SSD1306 *display)
{
    // start point is given as the top left
    // corner of the button
    // only the top left and right corner should be radiused
    if (!isButtonVisible)
    {
        return;
    }
    display->drawRoundRect(x, y, BUTTON_WIDTH, BUTTON_HEIGHT + 10, BUTTON_RADIUS, SSD1306_WHITE);

    int centerX = x + BUTTON_WIDTH / 2;
    // calculate width of text

    int16_t x1, y1;
    uint16_t w, h;

    display->getTextBounds(buttonLabel.c_str(), 0, 0, &x1, &y1, &w, &h);

    display->setCursor(centerX - w / 2, y + 2);
    display->print(buttonLabel.c_str());
};

void ButtonOverlayWidget::updateLabel(std::string label)
{
    this->buttonLabel = label;
}

void ButtonOverlayWidget::hideButton()
{
    this->isButtonVisible = false;
}

void ButtonOverlayWidget::showButton()
{
    this->isButtonVisible = true;
}