/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/Timer.h>

#if defined(__win32__)
  #include <windows.h>
#else // __unix__
  #include <sys/time.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Timer::Timer() throw() : stopTime(0) {
  start();
}

void Timer::start() throw() {
#if defined(__win32__)
  ASSERT(sizeof(LARGE_INTEGER) == sizeof(long long));
  QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
#else // __unix__
  struct timeval temp;
  gettimeofday(&temp, 0);
  startTime = 1000000l * temp.tv_sec + temp.tv_usec;
#endif
}

void Timer::stop() throw() {
#if defined(__win32__)
  ASSERT(sizeof(LARGE_INTEGER) == sizeof(long long));
  QueryPerformanceCounter((LARGE_INTEGER*)&stopTime);
#else // __unix__
  struct timeval temp;
  gettimeofday(&temp, 0);
  stopTime = 1000000l * temp.tv_sec + temp.tv_usec;
#endif
}

long long Timer::getStartTime() const throw() {
#if defined(__win32__)
  LARGE_INTEGER frequency; // ticks per second
  QueryPerformanceFrequency(&frequency); // ignore any error
  return static_cast<long long>(startTime * 1000000./frequency.QuadPart);
#else // __unix__
  return startTime;
#endif
}

long long Timer::getStopTime() const throw() {
#if defined(__win32__)
  LARGE_INTEGER frequency; // ticks per second
  QueryPerformanceFrequency(&frequency); // ignore any error
  return static_cast<long long>(stopTime * 1000000./frequency.QuadPart);
#else // __unix__
  return stopTime;
#endif
}

long long Timer::getMicroseconds() const throw() {
#if defined(__win32__)
  LARGE_INTEGER frequency; // ticks per second
  QueryPerformanceFrequency(&frequency); // ignore any error
  return static_cast<long long>((stopTime - startTime) * 1000000./frequency.QuadPart);
#else // __unix__
  return stopTime - startTime;
#endif
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Timer& value) throw(IOException) {
  long long microseconds = value.getMicroseconds();
  long long seconds = microseconds/1000000;
  long long minutes = seconds/60;
  long long hours = minutes/60;
  return stream << hours << ":" << minutes%60 << ":" << seconds%60 << "." << microseconds%1000000;
}

void TimeScope::dump() const throw(IOException) {
  fout << "Elapsed time (H:M:S.microseconds): " << timer << ENDL;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
