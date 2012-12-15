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
class MicroSecTimer
{
public:
  MicroSecTimer()
  {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    oneOverFrequency = 1.0 / frequency.QuadPart;

    QueryPerformanceCounter(&start);
  }

  double getTimeMicroSeconds() const
  {
    LARGE_INTEGER end;
    QueryPerformanceCounter(&end);
    const double million = 1000.0 * 1000.0;
    return (million * (end.QuadPart - start.QuadPart)) * oneOverFrequency;
  }


private:
  LARGE_INTEGER start;
  double oneOverFrequency;
};
