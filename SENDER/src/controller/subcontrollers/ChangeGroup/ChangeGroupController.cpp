#include "ChangeGroupController.h"

ChangeGroupController::ChangeGroupController(BaseDisplay *baseDisplay, Controller *controller) : PageController(baseDisplay, controller)
{
}

void ChangeGroupController::activate(AnyToChangeGroupData data)
{
  groupSelectionIndex = data.groupSelectionIndex;
  isInsertGroup = false;
}

void ChangeGroupController::refreshPage(Adafruit_SSD1306 *display)
{
  char groupName[GROUP_NAME_LENGTH + 1];

  strncpy(groupName, "HelloWorld12", sizeof(groupName) - 1); // Copy up to 12 characters
  groupName[sizeof(groupName) - 1] = '\0';

  baseDisplay->updateChangeGroupPage(display, this->controller->groups, this->controller->groupCount, groupSelectionIndex, isInsertGroup, this->controller->getCurrentGroup()->name);
}

void ChangeGroupController::onDown()
{
  // no looping
  if (groupSelectionIndex == this->controller->groupCount)
  {
  }
  else
  {
    if (groupSelectionIndex == -1 || groupSelectionIndex == 0)
    {
      groupSelectionIndex++;
      isInsertGroup = false;
    }
    else if (groupSelectionIndex == this->controller->groupCount - 1)
    {
      groupSelectionIndex++;
      isInsertGroup = false;
    }
    else if (!isInsertGroup)
    {
      isInsertGroup = true;
      groupSelectionIndex++;
    }
    else
    {
      isInsertGroup = false;
    }
  }

  Serial.printf("Now looking at group %d\n", groupSelectionIndex);
}

void ChangeGroupController::onUp()
{
  if (groupSelectionIndex == -1)
  {
  }
  else
  {
    if (groupSelectionIndex == this->controller->groupCount || groupSelectionIndex == 0)
    {
      groupSelectionIndex--;
      isInsertGroup = false;
    }
    else if (!isInsertGroup)
    {
      isInsertGroup = true;
    }
    else
    {
      isInsertGroup = false;

      groupSelectionIndex--;
    }
  }
  Serial.printf("Now looking at group %d\n", groupSelectionIndex);
}

void ChangeGroupController::onSend()
{
}

void ChangeGroupController::onScreenLeft()
{
  if (isInsertGroup)
  {

    // changePage(CHARACTER_INPUT_PAGE);
  }
  else
  {
    if (groupSelectionIndex != -1 && groupSelectionIndex != this->controller->groupCount)
    {
      // LOAD
      Serial.printf("Changing group to %d\n", groupSelectionIndex);
      this->controller->changeGroup(groupSelectionIndex);
      AnyToHomeData data = {0};
      this->controller->changePage(HOME_PAGE, &data);
    }
    else
    {
      // SAVE
      // changePage(CHARACTER_INPUT_PAGE);
      AnyToCharacterInputData data = {isInsertGroup, groupSelectionIndex};
      this->controller->changePage(CHARACTER_INPUT_PAGE, &data);
    }
  }
}

void ChangeGroupController::onScreenRight()
{
  // go back to settings page
  controller->changePage(SETTINGS_PAGE, NULL);
}