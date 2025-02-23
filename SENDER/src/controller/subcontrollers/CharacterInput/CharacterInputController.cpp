#include "CharacterInputController.h"

CharacterInputController::CharacterInputController(BaseDisplay *baseDisplay, Controller *controller) : PageController(baseDisplay, controller)
{
}

void CharacterInputController::activate(AnyToCharacterInputData data)
{
  // reset all data in character input page
  // TODO
  for (int i = 0; i < GROUP_NAME_LENGTH; i++)
  {
    newGroupNameAsIndex[i] = 0;
  }

  isInsertGroup = data.isInsertGroup;
  groupSelectionIndex = data.groupSelectionIndex;
  currentNewGroupNameLength = 0;
  cursorPosition = 0;
  isEditing = true;
}

void CharacterInputController::refreshPage(Adafruit_SSD1306 *display)
{
  baseDisplay->updateCharacterInputPage(display, newGroupNameAsIndex, currentNewGroupNameLength, maxNewGroupNameLength, isEditing, cursorPosition);
}

void CharacterInputController::onDown()
{
  newGroupNameAsIndex[cursorPosition] = (newGroupNameAsIndex[cursorPosition] + 1) % POSSIBLE_CHARS_LENGTH;
}

void CharacterInputController::onUp()
{
  newGroupNameAsIndex[cursorPosition] = (newGroupNameAsIndex[cursorPosition] - 1 + POSSIBLE_CHARS_LENGTH) % POSSIBLE_CHARS_LENGTH;
}

void CharacterInputController::onSend()
{
  isEditing = !isEditing;
}

void CharacterInputController::onScreenLeft()
{
  if (isEditing)
  {
    cursorPosition = (cursorPosition - 1 + GROUP_NAME_LENGTH) % GROUP_NAME_LENGTH;
  }
  else
  {
    // todo: add a variable to check if this page's purpose
    // as we would like to reuse this pageas
    // save the new group name
    if (isInsertGroup)
    {
    }
    else
    {
      // save the new group name
      // groups[groupSelectionIndex].name = newGroupName;
      Group newGroup;

      // convert the name from newGroupNameAsIndex to ASCII
      for (int i = 0; i < GROUP_NAME_LENGTH; i++)
      {
        newGroup.name[i] = *POSSIBLE_CHARS[newGroupNameAsIndex[i]];
      }
      newGroup.name[GROUP_NAME_LENGTH] = '\0';
      newGroup.slotCount = 0;

      Light light = {0, 0, 0};
      Slot slot = {light};

      newGroup.slots[0] = slot;
      this->controller->groups[groupSelectionIndex] = newGroup;
      this->controller->groupExists[groupSelectionIndex] = true;

      this->controller->groupCount++;
      this->controller->groupIndex = groupSelectionIndex;

      // reset all data in character input page
      for (int i = 0; i < GROUP_NAME_LENGTH; i++)
      {
        newGroupNameAsIndex[i] = 0;
      }
      currentNewGroupNameLength = 0;
      cursorPosition = 0;
      // changePage(HOME_PAGE);
      this->controller->save("Saving new group...");

      this->controller->changeGroup(groupSelectionIndex);
      AnyToHomeData data = {0};
      this->controller->changePage(HOME_PAGE, &data);
    }
  }
}

void CharacterInputController::onScreenRight()
{
  if (isEditing)
  {

    // go right
    cursorPosition = (cursorPosition + 1) % GROUP_NAME_LENGTH;
  }
  else
  {
    // go back to change group page
    AnyToChangeGroupData data = {groupSelectionIndex};
    this->controller->changePage(CHANGE_GROUP_PAGE, &data);

    // changePage(CHANGE_GROUP_PAGE);
    // reset all data in character input page
    // for (int i = 0; i < GROUP_NAME_LENGTH; i++)
    // {
    //   newGroupNameAsIndex[i] = 0;
    // }
    // currentNewGroupNameLength = 0;
    // cursorPosition = 0;
    // isEditing = true;
  }
}