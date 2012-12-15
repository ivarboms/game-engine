#pragma once

//#pragma comment(lib, "FW1FontWrapper.lib")
#pragma comment(lib, "C:\\SDKs\\FW1FontWrapper_1_1_src\\lib\\Debug\\FW1FontWrapper.lib")
#pragma comment(lib, "Gdi32.lib")

#include <FW1FontWrapper.h>

IFW1Factory* createFontWrapper();

IFW1FontWrapper* createFont(ID3D11Device& device, IFW1Factory& fw1Factory, const wchar_t* fontName);
