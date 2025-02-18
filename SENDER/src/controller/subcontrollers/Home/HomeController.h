#pragma once
#include "controller/PageController.h"

class HomeController : PageController
{
  int slotIndex = 0;
  int groupIndex = 0;
  int groupCount = 0;

  Light light = {0, 0, 0, 0, 0, 0};
  Light frozenLight = {0, 0, 0, 0, 0, 0};

  int connectedCount = 0;
  int mode = 0; // 0 = "locked, no edit", 1 = "edit"

public:
  HomeController(BaseDisplay *baseDisplay, Controller *controller);
  HomeController() {};

  virtual void onDown();        // override
  virtual void onUp();          // override
  virtual void onSend();        // override
  virtual void onScreenLeft();  // override
  virtual void onScreenRight(); // override

  void nextSlot();
  void prevSlot();

  void refreshPage(Adafruit_SSD1306 *display);
};