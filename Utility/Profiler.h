#pragma once

#include <map>
#include <vector>
#include <string>

#include "MicroSecTimer.h"


class Profiler
{
public:
  struct ProfileBlock
  {
    double beginMicroSec;
    double endMicroSec;
    std::wstring category;
    std::wstring name;
  };


	Profiler();
	~Profiler();

	void beginProfileBlock(const std::wstring& name, const std::wstring& category = L"Debug");

  void endProfileBlock(const std::wstring& name, const std::wstring& category = L"Debug");

  void endFrame();


//private:
  //typedef std::map<std::wstring, ProfileEvent> FrameData;
  typedef std::vector<ProfileBlock> FrameData;
  FrameData frame;
  std::vector<FrameData> previousFrames;

  MicroSecTimer timer;
};