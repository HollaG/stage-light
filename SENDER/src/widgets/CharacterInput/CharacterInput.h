#pragma once
#include <string>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "widgets/BaseWidget.h"

class CharacterInputWidget : public BaseWidget
{
public:
  char character;
  bool isEditing = false;

  // Constructor
  CharacterInputWidget(int x, int y);

  void draw(Adafruit_SSD1306 *display);

  void setCharacter(char character);

  void isEditingThis(bool editing);
};
