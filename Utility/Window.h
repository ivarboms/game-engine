#pragma once

#include <string>
#include <windows.h>


struct Window
{
  HWND hWnd;
  HINSTANCE  hInstance;
  std::wstring name;
};

struct WindowCInfo
{
  WindowCInfo()
    : width(1280)
    , height(720)
  {}

  HINSTANCE hInstance;
  int nCmdShow;
  const wchar_t* className;
  const wchar_t* windowName;
  WNDPROC windowProc;
  unsigned int width;
  unsigned int height;
};

HRESULT createWindow(const WindowCInfo& cInfo, Window& windowOut);
