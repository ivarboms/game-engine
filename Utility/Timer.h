#pragma once

#include <windows.h>

/*  Simple timer.
  Use Timer::getTimeMilliSeconds to get millisecond intervals.
  Uses no CPU cycles when idle.
  Will likely be inaccurate if power saving or similar is active.

  Usage looks something like this:
  Timer timer;
  float start = timer.getTimeMilliSeconds();
  doSomeWork();
  float duration = timer.getTimeMilliSeconds() - start;
  //duration now contains time doSomeWork() took in milliseconds.
*/
class Timer
{
public:
  Timer()
  {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    oneOverFrequency = (float)(1.0f / (float)frequency.QuadPart);

    QueryPerformanceCounter(&start);
  }

  float getTimeMilliSeconds() const
  {
    LARGE_INTEGER end;
    QueryPerformanceCounter(&end);
    return (1e3f * (end.QuadPart - start.QuadPart)) * oneOverFrequency;
  }


private:
  LARGE_INTEGER start;
  float oneOverFrequency;
};
