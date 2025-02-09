#include "CharacterInput.h"
#include <cstdio>

#define VERTICAL_SPACING 10

// Constructor Implementation
CharacterInputWidget::CharacterInputWidget(int x, int y)
    : BaseWidget(x, y)
{
}

// Draw method implementation
void CharacterInputWidget::draw(Adafruit_SSD1306 *display)
{
  int16_t x1, y1;
  uint16_t w, h;
  display->getTextBounds("a", 0, 0, &x1, &y1, &w, &h);

  // print

  // start point is top left
  // should look like:

  /**
   *       [empty space] 8px
   *             ^
   * [character with underline]
   *             v
   *       [empty space] 8px
   */

  // triangle of 7px width, 5px height, solid

  if (isEditing)
  {

    // top triangle
    display->drawTriangle(x, y + w / 2, x + w / 2, y, x + w, y + w / 2, SSD1306_WHITE);

    // bottom triangle
    display->drawTriangle(x, y + w + h + 2 + 4, x + w, y + w + h + 2 + 4, x + w / 2, y + w + h + 2 + 8, SSD1306_WHITE);
  }

  // bottom line
  display->drawFastHLine(x, y + w + h + 1, w, SSD1306_WHITE);
  display->setCursor(x, y + w + 1);
  display->print(character);

  // start point is given as the top left
  // corner of the button
  // only the top left and right corner should be radiused
  // if (!isButtonVisible)
  // {
  //   return;
  // }
  // display->drawRoundRect(x, y, BUTTON_WIDTH, BUTTON_HEIGHT + 10, BUTTON_RADIUS, SSD1306_WHITE);

  // int centerX = x + BUTTON_WIDTH / 2;
  // // calculate width of text

  // int16_t x1, y1;
  // uint16_t w, h;

  // display->getTextBounds(buttonLabel.c_str(), 0, 0, &x1, &y1, &w, &h);

  // display->setCursor(centerX - w / 2, y + 3);
  // display->print(buttonLabel.c_str());
};

void CharacterInputWidget::setCharacter(char character)
{
  this->character = character;
}

void CharacterInputWidget::isEditingThis(bool editing)
{
  this->isEditing = editing;
}
