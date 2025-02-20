#pragma once
#include "controller/PageController.h"

class DeleteSlotController : PageController
{
  int deleteIndex = -1;

public:
  DeleteSlotController(BaseDisplay *baseDisplay, Controller *controller);
  DeleteSlotController() {};

  virtual void onDown();        // override
  virtual void onUp();          // override
  virtual void onSend();        // override
  virtual void onScreenLeft();  // override
  virtual void onScreenRight(); // override

  virtual void refreshPage(Adafruit_SSD1306 *display); // override
  virtual void activate(AnyToDeleteSlotData data);
};