#include "./CharacterInputPage.h"
#include "../../controller/Controller.h"

// Constructor Implementation
CharacterInputPage::CharacterInputPage()
{

  // Buttons at bottom
  actionButton = new ButtonOverlayWidget(0, 64 - BUTTON_HEIGHT, "<");
  cancelButton = new ButtonOverlayWidget(128 - BUTTON_WIDTH, 64 - BUTTON_HEIGHT, ">");

  addWidget(actionButton);
  addWidget(cancelButton);

  // create 16 character input widgets
  for (int i = 0; i < GROUP_NAME_LENGTH; i++)
  {
    characterInputs[i] = new CharacterInputWidget(i * 10, 12);
    addWidget(characterInputs[i]);
  }
}

void CharacterInputPage::update(int *inputAsIndexes, int inputLength, int maxInputLength, bool isEditing, int cursorPosition)
{
  // for each character input widget, update the character
  Serial.println("-");

  for (int i = 0; i < GROUP_NAME_LENGTH; i++)
  {
    Serial.print(inputAsIndexes[i]);
    Serial.print(" ");
    if (i == cursorPosition)
    {
      characterInputs[i]->isEditingThis(true);
    }
    else
    {
      characterInputs[i]->isEditingThis(false);
    }

    characterInputs[i]->setCharacter(*POSSIBLE_CHARS[inputAsIndexes[i]]);

    // characterInputs[i]->isEditingThis(isEditing);
  }

  // Serial.printf("Cursor position: %d\n", cursorPosition);
  // // print input as index
  // for (int i = 0; i < maxInputLength; i++)
  // {
  //   Serial.printf("%d ", inputAsIndexes[i]);
  // }
}