#pragma once
#include "controller/Controller.h"

struct HomeToSaveSlotData
{
  int saveInSlotIndex;
  Light frozenLight;
};

struct SaveSlotToHomeData
{
  int slotIndex;
};
class PageController
{

protected:
  BaseDisplay *baseDisplay;
  Controller *controller;

public:
  PageController(BaseDisplay *baseDisplay, Controller *controller) : baseDisplay(baseDisplay), controller(controller) {}
  PageController();

  virtual void onScreenLeft() {};
  virtual void onScreenRight() {};
  virtual void onSend() {};
  virtual void onDown() {};
  virtual void onUp() {};

  virtual void refreshPage(Adafruit_SSD1306 *display) {};

  template <typename T>
  void activate(T data) {}
};