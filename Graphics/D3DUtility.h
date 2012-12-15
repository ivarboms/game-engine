#pragma once

#include <string>

#include <D3Dcommon.h>


template<typename D3DObject>
void setDebugName(D3DObject& d3dObject, const std::string& name)
{
  d3dObject.SetPrivateData(WKPDID_D3DDebugObjectName, name.size(), name.c_str());
}
