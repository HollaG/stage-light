#include "DeleteSlotController.h"

DeleteSlotController::DeleteSlotController(BaseDisplay *baseDisplay, Controller *controller) : PageController(baseDisplay, controller)
{
}

void DeleteSlotController::activate(AnyToDeleteSlotData data)
{
  deleteIndex = data.deleteIndex;
}

void DeleteSlotController::refreshPage(Adafruit_SSD1306 *display)
{
  char groupName[GROUP_NAME_LENGTH];
  strcpy(groupName, controller->getCurrentGroup()->name);

  baseDisplay->updateDeleteSlotPage(display, groupName, controller->getCurrentGroup()->slots, deleteIndex, controller->getCurrentGroup()->slotCount);
}

void DeleteSlotController::onDown()
{
  if (deleteIndex == -1)
  {
    return;
  }

  if (deleteIndex == controller->getCurrentGroup()->slotCount - 1)
  {
    deleteIndex = -1;
  }
  else
  {
    deleteIndex++;
  }
}

void DeleteSlotController::onUp()
{
  if (deleteIndex == -1)
  {
    deleteIndex = controller->getCurrentGroup()->slotCount - 1;
  }
  else
  {
    deleteIndex--;
  }
}

void DeleteSlotController::onSend()
{
}

void DeleteSlotController::onScreenLeft()
{
  // delete the slot
  Group *groups = controller->groups;
  int groupIndex = controller->groupIndex;
  for (int i = deleteIndex; i < groups[groupIndex].slotCount - 1; i++)
  {
    groups[groupIndex].slots[i] = groups[groupIndex].slots[i + 1];
  }
  groups[groupIndex].slotCount = groups[groupIndex].slotCount - 1;

  // no more slots for this group
  if (groups[groupIndex].slotCount == 0)
  {
    AnyToHomeData data = {0};
    this->controller->changePage(HOME_PAGE, &data);
  }
  if (deleteIndex == groups[groupIndex].slotCount)
  {
    deleteIndex--;
  }
  if (this->controller->slotIndex == groups[groupIndex].slotCount)
  {
    this->controller->slotIndex--;
  }
  this->controller->save("Deleting slot...");

  // change back to home
}

void DeleteSlotController::onScreenRight()
{
  // change back to settings page
  this->controller->changePage(SETTINGS_PAGE, NULL);
}
