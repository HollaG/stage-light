#include "Potentiometer.h"
#include <cstdio>

// Constructor Implementation
PotWidget::PotWidget(int value, int minValue, int maxValue, std::string label, int x, int y) : BaseWidget(x, y), value(value), minValue(minValue), maxValue(maxValue), label(label)
{
}

// void PotWidget::draw(Adafruit_SSD1306 *display)
// {

//     // draw a 24px outlined circle, 22px inner black, outline white
//     // assume that the coordinates given in x and y are the center of the circle
//     // add value label,
//     // then add the label

//     display->drawCircle(x, y, 12, SSD1306_WHITE);
//     display->fillCircle(x, y, 11, SSD1306_BLACK);

//     // calculate width of value text. Text should be 12px in side
//     int16_t x1, y1;
//     uint16_t w, h;

//     char buffer[4]; // Ensure buffer is large enough
//     sprintf(buffer, "%d", this->value);

//     display->getTextBounds(buffer, 0, 0, &x1, &y1, &w, &h);

//     display->setCursor(x - w / 2, y - h / 2);
//     display->print(this->value);

//     // calculate width of label text
//     int16_t x2, y2;
//     uint16_t w2, h2;

//     display->getTextBounds(label.c_str(), 0, 0, &x2, &y2, &w2, &h2);

//     display->setCursor(x - w2 / 2, y + 20);
//     display->print(label.c_str());

//     // display->setCursor(x, y);
//     // display->printf("%s: %d", label.c_str(), value);
// }

void PotWidget::draw(Adafruit_SSD1306 *display)
{
    display->setCursor(x, y);
    // write the text "label"
    display->printf("%s", this->label.c_str());
    // draw a solid rectangle 8px in height, 32px in length
    display->drawRect(x + 10, y, 34, 8, SSD1306_WHITE);
    // // cut out the "internal" space
    // display->drawRect(x + 11, y + 1, 32, 6, SSD1306_BLACK);

    float ratio = (float)(value - minValue) / (maxValue - minValue);
    display->fillRect(x + 11, y + 1, 32 * ratio, 6, SSD1306_WHITE);
    // add an additional label to the RIGHT of this rectangle
    display->setCursor(x + 10 + 34 + 2, y);
    display->printf(" %d", value);
}

void PotWidget::updateValue(int value)
{
    this->value = value;
}