#pragma once
#include "controller/PageController.h"
class SettingsController : PageController
{
public:
  SettingsController(BaseDisplay *baseDisplay, Controller *controller);
  SettingsController() {};
  virtual void onDown();        // override
  virtual void onUp();          // override
  virtual void onSend();        // override
  virtual void onScreenLeft();  // override
  virtual void onScreenRight(); // override

  virtual void refreshPage(Adafruit_SSD1306 *display); // override
  virtual void activate(AnyToSettingsData data);

private:
  char groupName[GROUP_NAME_LENGTH];
  int settingsIndex = 0;
};