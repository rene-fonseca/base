/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/Timer.h>
#include <base/string/String.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#  include <sys/time.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/*
  // if optimized for PENTIUM (may not be allowed by system)

  unsigned int low = 0;
  unsigned int high = 0;
  asm (
    "        rdtsc\n"
    : "=a" (low), "=d" (high) // output
  );
  return (static_cast<uint64>(high) << 32) | low;
*/

Timer::Timer() noexcept {
  start();
}

void Timer::start() noexcept {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::QueryPerformanceCounter(Cast::pointer<LARGE_INTEGER*>(&startTime));
#else // unix
  struct timeval temp;
  gettimeofday(&temp, 0);
  startTime = 1000000l * temp.tv_sec + temp.tv_usec;
#endif // flavor
}

void Timer::stop() noexcept {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::QueryPerformanceCounter(Cast::pointer<LARGE_INTEGER*>(&stopTime));
#else // unix
  struct timeval temp;
  gettimeofday(&temp, 0);
  stopTime = 1000000l * temp.tv_sec + temp.tv_usec;
#endif // flavor
}

uint64 Timer::getStartTime() const noexcept {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  LARGE_INTEGER frequency; // ticks per second
  ::QueryPerformanceFrequency(&frequency); // ignore any error
  return static_cast<uint64>(startTime * 1000000./frequency.QuadPart);
#else // unix
  return startTime;
#endif // flavor
}

uint64 Timer::getStopTime() const noexcept {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  LARGE_INTEGER frequency; // ticks per second
  ::QueryPerformanceFrequency(&frequency); // ignore any error
  return static_cast<uint64>(stopTime * 1000000./frequency.QuadPart);
#else // unix
  return stopTime;
#endif // flavor
}

uint64 Timer::getMicroseconds() const noexcept {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  LARGE_INTEGER frequency; // ticks per second
  ::QueryPerformanceFrequency(&frequency); // ignore any error
  return static_cast<uint64>((stopTime - startTime) * 1000000./frequency.QuadPart);
#else // unix
  return stopTime - startTime;
#endif // flavor
}

uint64 Timer::getLiveMicroseconds() const noexcept {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  LARGE_INTEGER now;
  ::QueryPerformanceCounter(&now);
  LARGE_INTEGER frequency; // ticks per second
  ::QueryPerformanceFrequency(&frequency); // ignore any error
  return static_cast<uint64>((now.QuadPart - startTime) * 1000000./frequency.QuadPart);
#else // unix
  struct timeval temp;
  gettimeofday(&temp, 0);
  return 1000000l * temp.tv_sec + temp.tv_usec - startTime;
#endif // flavor
}

FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Timer& value) throw(IOException) {
  FormatOutputStream::PushContext push(stream);
  Timer::ElapsedTime time(value.getMicroseconds() * 1000);
  time.roundToMicrosecond();
  return stream << time.getHours() << ':'
                << time.getNMinutes() << ':'
                << time.getNSeconds() << '.'
                << setWidth(6) << ZEROPAD << time.getNMicroseconds();
}

void TimeScope::dump() const throw(IOException) {
  fout << "Elapsed time (H:M:S.microseconds): " << timer << ENDL;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
