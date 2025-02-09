#pragma once
#include "pages/BasePage.h"
#include "widgets/ButtonOverlay/ButtonOverlay.h"
#include "widgets/CharacterInput/CharacterInput.h"
#include "structs/common.h"

class CharacterInputPage : public BasePage
{

public:
  // Constructor
  CharacterInputPage();

  // variables
  char *input;
  int inputLength;
  int maxInputLength;
  bool isEditing;
  int cursorPosition;

  // Widgets
  ButtonOverlayWidget *actionButton;
  ButtonOverlayWidget *cancelButton;

  // array of characterinputs
  CharacterInputWidget *characterInputs[GROUP_NAME_LENGTH];

  // update
  void update(int *inputAsIndexes, int inputLength, int maxInputLength, bool isEditing, int cursorPosition);
};