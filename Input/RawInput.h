#pragma once

#include <Windows.h>

struct Mouse;
struct Keyboard;



/*  
    Loop structure to get raw input properly:

    while (true)
    {
      keyboard.prepareForNewInput();
      mouse.prepareForNewInput();
      PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
    }
    
    ...

    WinProc(..., UINT message, ...)
    {
      if (message == WM_INPUT)
      {
        getRawInput(lParam, mouse, keyboard);
        DefWindowProc(...);
      }
    }
*/


/*  Should be called on WM_INPUT messages.
    The provided mouse and keyboard may be modified, depending on what lParam contains.
*/
void getRawInput(LPARAM lParam, Mouse& mouse, Keyboard& keyboard);

/*  Register listening to raw keyboard input.
    targetWindow is the window for which to listen to input. If it is null, it follows the
    keyboard focus.
*/
bool registerRawKeyboard(HWND targetWindow);

/*  Register listening to raw mouse input.
    targetWindow is the window for which to listen to input. If it is null, it follows the
    keyboard focus.
*/
bool registerRawMouse(HWND targetWindow);
