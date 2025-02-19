#pragma once
#include "controller/PageController.h"
class SaveSlotController : PageController
{

  int saveInSlotIndex = -1;
  int saveInGroupIndex = -1;
  bool isInsert = false;
  Light frozenLight = {0, 0, 0, 0, 0, 0};

public:
  SaveSlotController(BaseDisplay *baseDisplay, Controller *controller);
  SaveSlotController() {};
  virtual void onDown();        // override
  virtual void onUp();          // override
  virtual void onSend();        // override
  virtual void onScreenLeft();  // override
  virtual void onScreenRight(); // override

  virtual void refreshPage(Adafruit_SSD1306 *display); // override
  virtual void activate(HomeToSaveSlotData data);
};