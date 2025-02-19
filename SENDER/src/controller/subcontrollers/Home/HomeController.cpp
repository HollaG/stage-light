#include "HomeController.h"

HomeController::HomeController(BaseDisplay *baseDisplay, Controller *controller) : PageController(baseDisplay, controller)
{
  // slotIndex = &controller->slotIndex; // NOTE THAT THIS IS 0-INDEXED
  // groupIndex = 0;
  // groupCount = 1; // this is a COUNT

  this->slotIndex = &controller->slotIndex;
  this->groupIndex = &controller->groupIndex;
  this->groupCount = &controller->groupCount;
}

void HomeController::activate(AnyToHomeData data)
{
  // if the slot index exists
  if (data.slotIndex != -1)
  {
    Serial.printf("Set slot index to %d\n", data.slotIndex);
    // set the slot index to the given slot index
    *this->slotIndex = data.slotIndex;
  }
}

void HomeController::refreshPage(Adafruit_SSD1306 *display)
{
  Group *group = (this->controller->getCurrentGroup());
  Slot *currentSlot = this->controller->getCurrentSlot();
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
    r = currentSlot->light.r;
    g = currentSlot->light.g;
    b = currentSlot->light.b;
  }

  baseDisplay->updateHomePage(display, r, g, b, *slotIndex, slotCount, connectedCount, group->name, mode);
}

void HomeController::onDown()
{
  if (this->mode == 1)
  {
    return;
  }

  Group *groups = this->controller->getGroups(this->groupCount);

  if (groups[*this->groupIndex].slotCount == 0)
  {
    return;
  }

  *this->slotIndex = (*this->slotIndex + 1) % groups[*this->groupIndex].slotCount;
  // if (controller->mode == 1) // should we make all of these public?
  //   return;
}

void HomeController::onUp()
{
  if (this->mode == 1)
  {
    return;
  }
  Group *groups = this->controller->getGroups(this->groupCount);
  if (groups[*this->groupIndex].slotCount == 0)
  {
    return;
  }
  *this->slotIndex = (*this->slotIndex - 1 + groups[*this->groupIndex].slotCount) % groups[*this->groupIndex].slotCount;
}

void HomeController::onScreenLeft()
{
  if (this->mode == 1)
  {
    // Change to the Saving Slot page
    // TODO

    int saveInSlotIndex = -1;
    if (*this->slotIndex == this->controller->getGroup(*this->groupIndex)->slotCount - 1)
    {
      saveInSlotIndex = *this->slotIndex + 1;
    }
    else
    {
      saveInSlotIndex = *this->slotIndex;
    }

    // copy this.light into frozenlight
    Light frozenLight = this->light;

    HomeToSaveSlotData data = {saveInSlotIndex, frozenLight};
    Serial.printf("save in slot index %d\n", saveInSlotIndex);
    Serial.printf("frozen light r %d, g %d, b %d\n", frozenLight.r, frozenLight.g, frozenLight.b);
    this->controller->changePage(SAVE_SLOT_PAGE, &data);
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
    this->controller->changePage(SETTINGS_PAGE, NULL);
  }
}

void HomeController::onSend()
{
  // Send the light to the ESP-NOW
  // TODO
}

void HomeController::updateLight(int red, int green, int blue)
{
  // Update the light values
  this->light.r = red;
  this->light.g = green;
  this->light.b = blue;
}