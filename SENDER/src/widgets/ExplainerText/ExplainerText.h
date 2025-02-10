#pragma once
#include <string>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "widgets/BaseWidget.h"

class ExplainerTextWidget : public BaseWidget
{
public:
  std::string text;

  // Constructor
  ExplainerTextWidget(int x, int y, std::string text);

  void draw(Adafruit_SSD1306 *display);
};
