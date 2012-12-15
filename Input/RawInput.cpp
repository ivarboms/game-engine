#include "RawInput.h"
#include "Mouse.h"
#include "Keyboard.h"

#include <windows.h>


void getMouseInput(Mouse& mouse, const RAWMOUSE& rawMouse)
{
  mouse.deltaPositionX = rawMouse.lLastX;
  mouse.deltaPositionY = rawMouse.lLastY;

  const unsigned short buttonFlags = rawMouse.usButtonFlags;

  // Left mouse button.
  if (buttonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
  {
    mouse.setButtonDown(0);
  }
  else if (buttonFlags & RI_MOUSE_LEFT_BUTTON_UP)
  {
    mouse.setButtonUp(0);
  }

  // Right mouse button.
  if (buttonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
  {
    mouse.setButtonDown(1);
  }
  else if (buttonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
  {
    mouse.setButtonUp(1);
  }

  // Middle mouse button.
  if (buttonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
  {
    mouse.setButtonDown(2);
  }
  else if (buttonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)
  {
    mouse.setButtonUp(2);
  }

  // XBUTTON1 (side, back)
  if (buttonFlags & RI_MOUSE_BUTTON_4_DOWN)
  {
    mouse.setButtonDown(3);
  }
  else if (buttonFlags & RI_MOUSE_BUTTON_4_UP)
  {
    mouse.setButtonUp(3);
  }

  // XBUTTON2 (side, forward)
  if (buttonFlags & RI_MOUSE_BUTTON_5_DOWN)
  {
    mouse.setButtonDown(4);
  }
  else if (buttonFlags & RI_MOUSE_BUTTON_5_UP)
  {
    mouse.setButtonUp(4);
  }

  // Mouse wheel.
  if (buttonFlags & RI_MOUSE_WHEEL)
  {
    /*
    Mouse wheel deltas are represented as multiples of 120.
    MSDN: The delta was set to 120 to allow Microsoft or other vendors to build
    finer-resolution wheels (a freely-rotating wheel with no notches) to send more
    messages per rotation, but with a smaller value in each message.

    Because of this, the value is converted to a float in case a mouse's wheel
    reports a value other than 120, in which case dividing by 120 would produce
    a very incorrect value.

    More info: http://social.msdn.microsoft.com/forums/en-US/gametechnologiesgeneral/thread/1deb5f7e-95ee-40ac-84db-58d636f601c7/
    */


    // One wheel notch is represented as this delta (WHEEL_DELTA).
    const float oneNotch = (float)WHEEL_DELTA;

    // Mouse wheel delta in multiples of WHEEL_DELTA (120).
    const float mouseWheelDelta = (float)((short)LOWORD(rawMouse.usButtonData));

    // Convert each notch from [-120, 120] to [-1, 1].
    mouse.mouseWheelDelta = mouseWheelDelta / oneNotch;
  }
}

void getKeyboardInput(Keyboard& keyboard, const RAWKEYBOARD& rawKeyboard)
{
  const unsigned short keyCode = rawKeyboard.VKey;
  const bool released = rawKeyboard.Flags & RI_KEY_BREAK;
  if (keyCode < Keyboard::kNumKeys)
  {
    if (released)
    {
      keyboard.setKeyStateReleased((unsigned char)keyCode);
    }
    else
    {
      keyboard.setKeyStatePressed((unsigned char)keyCode);
    }
  }
}

void getRawInput(LPARAM lParam, Mouse& mouse, Keyboard& keyboard)
{
  //Get raw input, then delegate it to the correct function based on the header.

  unsigned int size;
  GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
  
  unsigned char* buffer = (unsigned char*)alloca(size);
  if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER))
    != size)
  {
    __debugbreak();
  }

  RAWINPUT* rawInput = (RAWINPUT*)buffer;

  if (rawInput->header.dwType == RIM_TYPEKEYBOARD)
  {
    getKeyboardInput(keyboard, rawInput->data.keyboard);
  }
  else if (rawInput->header.dwType == RIM_TYPEMOUSE)
  {
    getMouseInput(mouse, rawInput->data.mouse);
  }
}

bool registerRawKeyboard(HWND targetWindow)
{
  RAWINPUTDEVICE rawKeyboard;
  rawKeyboard.usUsagePage = 1;
  rawKeyboard.usUsage = 6;//Keyboard.
  rawKeyboard.dwFlags = 0;
  rawKeyboard.hwndTarget = targetWindow;

  return RegisterRawInputDevices(&rawKeyboard, 1, sizeof(rawKeyboard)) == TRUE;
}

bool registerRawMouse(HWND targetWindow)
{
  RAWINPUTDEVICE rawMouse;
  rawMouse.usUsagePage = 1;
  rawMouse.usUsage = 2;//Mouse.
  rawMouse.dwFlags = 0;
  rawMouse.hwndTarget = targetWindow;

  return RegisterRawInputDevices(&rawMouse, 1, sizeof(rawMouse)) == TRUE;
}
