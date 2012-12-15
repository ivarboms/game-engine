#pragma once

#include <bitset>


struct Mouse
{
  Mouse()
    : deltaPositionX(0)
    , deltaPositionY(0)
    , mouseWheelDelta(0.0f)
  {}

  // Returns true if the specified button is currently held down.
  bool isButtonHeld(unsigned int buttonID) const
  {
    return heldButtons[buttonID];
  }

  // Returns true if the specified button was just pressed.
  bool wasButtonPressed(unsigned int buttonID) const
  {
    return pressedButtons[buttonID];
  }

  // Returns true if the specified button was just released.
  bool wasButtonReleased(unsigned int buttonID) const
  {
    return releasedButtons[buttonID];
  }

  /*  Resets mouse delta position.
      This should be called before getting new input data.
  */
  void prepareForNewInput()
  {
    deltaPositionX = 0;
    deltaPositionY = 0;
    mouseWheelDelta = 0.0f;
    releasedButtons.reset();
    pressedButtons.reset();
  }

  /*  Set the specified button to be released.
      This should typically only be done when getting new input.
  */
  void setButtonUp(unsigned int buttonID)
  {
    pressedButtons[buttonID] = false;
    heldButtons[buttonID] = false;
    releasedButtons[buttonID] = true;
  }
  
  /*  Set the specified button to be pressed.
      This should typically only be done when getting new input.
  */
  void setButtonDown(unsigned int buttonID)
  {
    heldButtons[buttonID] = false;
    pressedButtons[buttonID] = true;
    heldButtons[buttonID] = true;
  }


  int deltaPositionX;
  int deltaPositionY;

  /*  This is provided as a float where one notch is 1.0f. For more info, see:
      http://social.msdn.microsoft.com/forums/en-US/gametechnologiesgeneral/thread/1deb5f7e-95ee-40ac-84db-58d636f601c7/
  */
  float mouseWheelDelta;

  static const unsigned int kMouseButtonCount = 5;
  // 0 = left, 1 = right, 2 = middle, 3 = XBUTTON1 (side, back), 4 = XBUTTON2 (side, forward)
  std::bitset<kMouseButtonCount> heldButtons;
  std::bitset<kMouseButtonCount> pressedButtons;
  std::bitset<kMouseButtonCount> releasedButtons;
};
