#include "ExplainerText.h"

// Constructor
ExplainerTextWidget::ExplainerTextWidget(int x, int y, std::string text) : BaseWidget(x, y), text(text)
{
}

// Draw method
void ExplainerTextWidget::draw(Adafruit_SSD1306 *display)
{
  display->setCursor(x, y);
  // enable wrap
  display->setTextWrap(true);
  display->print(text.c_str());
}