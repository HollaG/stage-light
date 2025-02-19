#include "SettingsController.h"

SettingsController::SettingsController(BaseDisplay *baseDisplay, Controller *controller) : PageController(baseDisplay, controller)
{
}

void SettingsController::activate(AnyToSettingsData data = AnyToSettingsData())
{
  // no data needed
}

void SettingsController::refreshPage(Adafruit_SSD1306 *display)
{
  char groupName[GROUP_NAME_LENGTH];
  strcpy(groupName, controller->getCurrentGroup()->name);

  baseDisplay->updateSettingsPage(display, groupName, settingsIndex);
}

void SettingsController::onDown()
{
  settingsIndex = (settingsIndex + 1) % SETTINGS_COUNT;
}

void SettingsController::onUp()
{
  settingsIndex = (settingsIndex - 1 + SETTINGS_COUNT) % SETTINGS_COUNT;
}

void SettingsController::onSend()
{
}

void SettingsController::onScreenLeft()
{
  Serial.printf("Index %d\n", settingsIndex);
  if (settingsIndex == 0)
  {
    // delete item
    // deleteIndex = slotIndex;
    // changePage(DELETE_SLOT_PAGE);
    int deleteIndex = controller->slotIndex;
    AnyToDeleteSlotData data = {deleteIndex};

    controller->changePage(DELETE_SLOT_PAGE, &data);
  }
  if (settingsIndex == 1)
  {
    // change group
    // groupSelectionIndex = groupIndex;
    // changePage(CHANGE_GROUP_PAGE);
  }
  if (settingsIndex == 2)
  {
    // Scan for receivers
    // unimplemented
  }
}

void SettingsController::onScreenRight()
{
  // go back to home page
  AnyToHomeData data = {-1};
  controller->changePage(HOME_PAGE, &data);
}