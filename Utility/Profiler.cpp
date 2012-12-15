#include "Profiler.h"


Profiler::Profiler()
{

}

Profiler::~Profiler()
{

}

void Profiler::beginProfileBlock(const std::wstring& name, const std::wstring& category)
{
  //assert frame doesn't contain name

  ProfileBlock block;
  block.beginMicroSec = timer.getTimeMicroSeconds();
  block.endMicroSec = 0.0;
  block.name = name;
  block.category = category;

  frame.push_back(block);
}

/*  Find the block which matches the name and category.
*/
Profiler::ProfileBlock* getProfileBlock(Profiler::FrameData& frameData, const std::wstring& name,
                     const std::wstring& category)
{
  for (Profiler::ProfileBlock& block : frameData)
  {
    if (block.name == name && block.category == category)
    {
      return &block;
    }
  }

  return nullptr;
}

void Profiler::endProfileBlock(const std::wstring& name, const std::wstring& category)
{
  ProfileBlock* block = getProfileBlock(frame, name, category);
  //assert block != nullptr
  
  block->endMicroSec = timer.getTimeMicroSeconds();
}

void Profiler::endFrame()
{
  //error check
  //assert all samples have ended

  previousFrames.push_back(frame);
  frame.clear();
}
