#include "stdafx.h"
#include "TextRendering.h"
#include "Assert2.h"
#include "Timer.h"


IFW1Factory* createFontWrapper()
{
  IFW1Factory* fw1Factory;

  Timer timer;
  HRESULT hr = FW1CreateFactory(FW1_VERSION, &fw1Factory);

  ASSERT2(SUCCEEDED(hr), "Failed to create FW1 Factory.");

  return fw1Factory;
}

IFW1FontWrapper* createFont(ID3D11Device& device, IFW1Factory& fw1Factory, const wchar_t* fontName)
{
  Timer timer;

  IFW1FontWrapper* fontWrapper;
  HRESULT hr = fw1Factory.CreateFontWrapper(&device, fontName, &fontWrapper);

  ASSERT2(SUCCEEDED(hr), "Failed to create Consolas font wrapper.");

  return fontWrapper;
}
