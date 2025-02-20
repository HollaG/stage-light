#pragma once
#include "controller/PageController.h"
class ChangeGroupController : PageController
{
  int groupSelectionIndex = 0; // to be set to whatever group on activate()
  bool isInsertGroup = false;

public:
  ChangeGroupController(BaseDisplay *baseDisplay, Controller *controller);
  ChangeGroupController() {};
  virtual void onDown();        // override
  virtual void onUp();          // override
  virtual void onSend();        // override
  virtual void onScreenLeft();  // override
  virtual void onScreenRight(); // override

  virtual void refreshPage(Adafruit_SSD1306 *display); // override
  virtual void activate(AnyToChangeGroupData data);
};