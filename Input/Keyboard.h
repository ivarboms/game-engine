#pragma once

#include "KeyCode.h"

#include <bitset>


struct Keyboard
{
  // Returns true if the specified key is held down.
  bool isKeyHeld(unsigned char keyCode) const
  {
    return heldKeys[keyCode];
  }

  // Returns true if the key was just pressed.
  bool wasKeyPressed(unsigned char keyCode) const
  {
    return pressedKeys[keyCode];
  }

  // Returns true if the key was just released.
  bool wasKeyReleased(unsigned char keyCode) const
  {
    return releasedKeys[keyCode];
  }

  // Returns true if the specified key is held down.
  bool isKeyHeld(KeyCode keyCode) const
  {
    return isKeyHeld((unsigned char)keyCode);
  }

  // Returns true if the key was just pressed.
  bool wasKeyPressed(KeyCode keyCode) const
  {
    return wasKeyPressed((unsigned char)keyCode);
  }

  // Returns true if the key was just released.
  bool wasKeyReleased(KeyCode keyCode) const
  {
    return wasKeyReleased((unsigned char)keyCode);
  }

  /*  Resets key pressed/released state (not key held state).
      This should be called before getting new input data.
  */
  void prepareForNewInput()
  {
    releasedKeys.reset();
    pressedKeys.reset();
  }

  /*  Set the state of a key code to be released.
      This should typically only be done when getting new input.
  */
  void setKeyStateReleased(unsigned char keyCode)
  {
    heldKeys[keyCode] = false;
    releasedKeys[keyCode] = true;
  }

  /*  Set the state of a key code to be pressed.
      This should typically only be done when getting new input.
  */
  void setKeyStatePressed(unsigned char keyCode)
  {
    if (heldKeys[keyCode])
    {
      // Already held, don't set it to pressed again.
      // This happens when the key repeat is active.
      return;
    }
    heldKeys[keyCode] = true;
    pressedKeys[keyCode] = true;
  }


  const static size_t kNumKeys = 256;
  std::bitset<kNumKeys> heldKeys;
  std::bitset<kNumKeys> releasedKeys;
  std::bitset<kNumKeys> pressedKeys;
};
