/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/Timer.h>
#include <base/string/String.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // unix
  #include <sys/time.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Timer::Timer() throw() : stopTime(0) {
  start();
}

void Timer::start() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::QueryPerformanceCounter(Cast::pointer<LARGE_INTEGER*>(&startTime));
#else // unix
  struct timeval temp;
  gettimeofday(&temp, 0);
  startTime = 1000000l * temp.tv_sec + temp.tv_usec;
#endif // flavor
}

void Timer::stop() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::QueryPerformanceCounter(Cast::pointer<LARGE_INTEGER*>(&stopTime));
#else // unix
  struct timeval temp;
  gettimeofday(&temp, 0);
  stopTime = 1000000l * temp.tv_sec + temp.tv_usec;
#endif // flavor
}

long long Timer::getStartTime() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  LARGE_INTEGER frequency; // ticks per second
  ::QueryPerformanceFrequency(&frequency); // ignore any error
  return static_cast<long long>(startTime * 1000000./frequency.QuadPart);
#else // unix
  return startTime;
#endif // flavor
}

long long Timer::getStopTime() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  LARGE_INTEGER frequency; // ticks per second
  ::QueryPerformanceFrequency(&frequency); // ignore any error
  return static_cast<long long>(stopTime * 1000000./frequency.QuadPart);
#else // unix
  return stopTime;
#endif // flavor
}

long long Timer::getMicroseconds() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  LARGE_INTEGER frequency; // ticks per second
  ::QueryPerformanceFrequency(&frequency); // ignore any error
  return static_cast<long long>((stopTime - startTime) * 1000000./frequency.QuadPart);
#else // unix
  return stopTime - startTime;
#endif // flavor
}

long long Timer::getLiveMicroseconds() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  LARGE_INTEGER now;
  ::QueryPerformanceCounter(&now);
  LARGE_INTEGER frequency; // ticks per second
  ::QueryPerformanceFrequency(&frequency); // ignore any error
  return static_cast<long long>((now.QuadPart - startTime) * 1000000./frequency.QuadPart);
#else // unix
  struct timeval temp;
  gettimeofday(&temp, 0);
  return 1000000l * temp.tv_sec + temp.tv_usec - startTime;
#endif // flavor
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Timer& value) throw(IOException) {
  FormatOutputStream::PushContext push(stream);
  long long microseconds = value.getMicroseconds();
  long long seconds = microseconds/1000000;
  long long minutes = seconds/60;
  long long hours = minutes/60;
  return stream << hours << ':' << minutes%60 << ':' << seconds%60 << '.' << microseconds%1000000;
}

void TimeScope::dump() const throw(IOException) {
  fout << MESSAGE("Elapsed time (H:M:S.microseconds): ") << timer << ENDL;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
