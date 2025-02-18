#include "HomeController.h"

HomeController::HomeController(BaseDisplay *baseDisplay, Controller *controller) : PageController(baseDisplay, controller)
{
  slotIndex = 0; // NOTE THAT THIS IS 0-INDEXED
  groupIndex = 0;
  groupCount = 1; // this is a COUNT
}

void HomeController::refreshPage(Adafruit_SSD1306 *display)
{
  Group *group = (this->controller->getGroup(groupIndex));
  int slotCount = group->slotCount;

  // if editing, then display potentiometer lights, if not, display slot light
  int r, g, b;
  if (mode == 1)
  {
    r = light.r;
    g = light.g;
    b = light.b;
  }
  else
  {
    r = group->slots[slotIndex].light.r;
    g = group->slots[slotIndex].light.g;
    b = group->slots[slotIndex].light.b;
  }

  baseDisplay->updateHomePage(display, r, g, b, slotIndex, slotCount, connectedCount, group->name, mode);
}

void HomeController::onDown()
{
  if (this->mode == 1)
  {
    return;
  }

  Group *groups = this->controller->getGroups(&this->groupCount);

  if (groups[this->groupIndex].slotCount == 0)
  {
    return;
  }

  this->slotIndex = (this->slotIndex + 1) % groups[this->groupIndex].slotCount;
  // if (controller->mode == 1) // should we make all of these public?
  //   return;
}

void HomeController::onUp()
{
  if (this->mode == 1)
  {
    return;
  }
  Group *groups = this->controller->getGroups(&this->groupCount);
  if (groups[this->groupIndex].slotCount == 0)
  {
    return;
  }
  this->slotIndex = (this->slotIndex - 1 + groups[this->groupIndex].slotCount) % groups[this->groupIndex].slotCount;
}

void HomeController::onScreenLeft()
{
  if (this->mode == 1)
  {
    // Change to the Saving Slot page
    // TODO
  }
  else
  {
    this->mode = 1;
  }
}

void HomeController::onScreenRight()
{
  if (this->mode == 1)
  {
    this->mode = 0;
  }
  else
  {
    // Change to the Settings page
    // TODO
  }
}

void HomeController::onSend()
{
  // Send the light to the ESP-NOW
  // TODO
}