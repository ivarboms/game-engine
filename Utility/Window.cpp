#include "Window.h"

#include <windows.h>


HRESULT createWindow(const WindowCInfo& cInfo, Window& windowOut)
{
  WNDCLASSEX wcex = { 0 };
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = cInfo.windowProc;
  wcex.hInstance = cInfo.hInstance;
  wcex.lpszMenuName = nullptr;
  wcex.lpszClassName = cInfo.className;
  if (!RegisterClassEx(&wcex))
  {
    return E_FAIL;
  }

  windowOut.hInstance = cInfo.hInstance;

  RECT rc = { 0, 0, cInfo.width, cInfo.height };
  AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
  windowOut.hWnd = CreateWindow(cInfo.className, cInfo.windowName, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr,
    cInfo.hInstance, nullptr);
  if (!windowOut.hWnd)
  {
    return E_FAIL;
  }

  ShowWindow(windowOut.hWnd, cInfo.nCmdShow);

  return S_OK;
}
