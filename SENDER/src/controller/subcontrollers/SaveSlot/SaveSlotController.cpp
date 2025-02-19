#include "SaveSlotController.h"

SaveSlotController::SaveSlotController(BaseDisplay *baseDisplay, Controller *controller) : PageController(baseDisplay, controller)
{
}

void SaveSlotController::activate(HomeToSaveSlotData data)
{
  saveInSlotIndex = data.saveInSlotIndex;
  frozenLight = data.frozenLight;
}

void SaveSlotController::refreshPage(Adafruit_SSD1306 *display)
{
  char groupName[GROUP_NAME_LENGTH];
  strcpy(groupName, this->controller->groups[this->controller->groupIndex].name);

  baseDisplay->updateSaveSlotPage(display, groupName, this->controller->groups[this->controller->groupIndex].slots, this->controller->groups[this->controller->groupIndex].slotCount, saveInSlotIndex, isInsert);
}

// void SaveSlotController::onScreenLeft(Controller *controller)
// {
//   controller->prevSlot();
// }

void SaveSlotController::onDown()
{
  // no looping
  if (saveInSlotIndex == this->controller->groups[this->controller->groupIndex].slotCount)
  {
  }
  else
  {
    if (saveInSlotIndex == -1 || saveInSlotIndex == 0)
    {
      saveInSlotIndex++;
      isInsert = false;
    }
    else if (saveInSlotIndex == this->controller->groups[this->controller->groupIndex].slotCount - 1)
    {
      saveInSlotIndex++;
      isInsert = false;
    }
    else if (!isInsert)
    {
      isInsert = true;
      saveInSlotIndex++;
    }
    else
    {
      isInsert = false;
    }
  }

  Serial.printf("Now looking at slot %d\n", saveInSlotIndex);
}
void SaveSlotController::onUp()
{
  if (saveInSlotIndex == -1)
  {
  }
  else
  {
    if (saveInSlotIndex == this->controller->groups[this->controller->groupIndex].slotCount || saveInSlotIndex == 0)
    {
      saveInSlotIndex--;
      isInsert = false;
    }
    else if (!isInsert)
    {
      isInsert = true;
    }
    else
    {
      isInsert = false;

      saveInSlotIndex--;
    }
  }
  Serial.printf("Now looking at slot %d\n", saveInSlotIndex);
}
void SaveSlotController::onSend() {}
void SaveSlotController::onScreenLeft()
{
  // TODO: Handle overflow (more than 100)
  // save this
  Group *groups = this->controller->groups;
  int groupIndex = this->controller->groupIndex;

  // groups[groupIndex].slots[saveInSlotIndex].light = {
  //     red} Serial.printf("Saved in group %d, slot %d\n", groupIndex, saveInSlotIndex);
  // Serial.printf("Saved R %d, G %d, B %d\n", frozenLight.r, frozenLight.g, frozenLight.b);

  // changePage(HOME_PAGE);

  // case 1: isInsert = false, saveInSlotIndex = -1
  // => insert at the head of the list
  // case 2: isInsert = false, saveInSlotIndex = slotCount
  // => insert at the end of the list
  // case 3: isInsert = true, saveInSlotIndex = any except those 2
  // => insert inbetween

  Serial.printf("Saving frozen light as %d, %d, %d", frozenLight.r, frozenLight.g, frozenLight.b);

  int toDisplaySlotIndex = 0;

  if (isInsert == false && saveInSlotIndex == -1)
  {
    // insert at the head of the list
    for (int i = groups[groupIndex].slotCount; i > 0; i--)
    {
      groups[groupIndex].slots[i] = groups[groupIndex].slots[i - 1];
    }
    groups[groupIndex].slots[0].light = frozenLight;
    groups[groupIndex].slotCount = groups[groupIndex].slotCount + 1;

    toDisplaySlotIndex = 0; // show the newly inserted slot
  }
  else if (isInsert == false && saveInSlotIndex == groups[groupIndex].slotCount)
  {
    // insert at the end of the list
    groups[groupIndex].slots[groups[groupIndex].slotCount].light = frozenLight;
    groups[groupIndex].slotCount = groups[groupIndex].slotCount + 1;
    toDisplaySlotIndex = groups[groupIndex].slotCount - 1; // show the newly inserted slot
  }
  else
  {
    // insert inbetween
    for (int i = groups[groupIndex].slotCount; i > saveInSlotIndex; i--)
    {
      groups[groupIndex].slots[i] = groups[groupIndex].slots[i - 1];
    }
    groups[groupIndex].slots[saveInSlotIndex].light = frozenLight;
    groups[groupIndex].slotCount = groups[groupIndex].slotCount + 1;
    toDisplaySlotIndex = saveInSlotIndex; // show the newly inserted slot
  }

  this->controller->save("Saving slot...");

  // change page back to home page
  SaveSlotToHomeData data = {toDisplaySlotIndex};
  this->controller->changePage(HOME_PAGE, &data);
}
void SaveSlotController::onScreenRight()
{
  // cancel saving
  SaveSlotToHomeData data = {-1};
  this->controller->changePage(HOME_PAGE, &data);
}