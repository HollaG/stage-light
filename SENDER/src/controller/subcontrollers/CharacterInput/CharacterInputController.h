#pragma once
#include "controller/PageController.h"

class CharacterInputController : PageController
{
  int newGroupNameAsIndex[GROUP_NAME_LENGTH] = {0};
  int currentNewGroupNameLength = 0;
  int maxNewGroupNameLength = GROUP_NAME_LENGTH;
  bool isEditing = true;
  int cursorPosition = 0;

  int isInsertGroup = false;
  int groupSelectionIndex = 0;

public:
  CharacterInputController(BaseDisplay *baseDisplay, Controller *controller);
  CharacterInputController() {};
  virtual void onDown();        // override
  virtual void onUp();          // override
  virtual void onSend();        // override
  virtual void onScreenLeft();  // override
  virtual void onScreenRight(); // override

  virtual void refreshPage(Adafruit_SSD1306 *display); // override
  virtual void activate(AnyToCharacterInputData data);
};
