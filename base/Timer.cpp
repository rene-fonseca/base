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
#include <base/concurrency/Thread.h>
#include <base/UInt128.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#  include <sys/time.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace {

  // TAG: move to Math
  inline uint64 muldiv(uint64 value, uint64 mul, uint64 div)
  {
    if (!(value >> 32) && !(mul >> 32)) {
      return value * mul / div;
    }
    if (!(value >> (32+16)) && !(mul >> 16)) {
      return value * mul / div;
    }
    if (!(value >> (32 + 16 + 8)) && !(mul >> 8)) {
      return value * mul / div;
    }
    // handle div or mul are power of 2
    if ((mul % div) == 0) {
      return value * (mul / div);
    }
    // TAG: div can be very small in many cases
    BASSERT(!"Integer overflow"); // implement
    return UInt128(value) * UInt128(mul) / UInt128(div);
    // return value * mul / div; // ignore overflow
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  inline uint64 getFrequency() noexcept
  {
    LARGE_INTEGER frequency; // ticks per second
    BOOL status = ::QueryPerformanceFrequency(&frequency); // ignore any error
    BASSERT(status);
    BASSERT(frequency.QuadPart > 0);
    return frequency.QuadPart;
  }

  const uint64 frequency = getFrequency(); // counts in seconds
#endif

  // TAG: const uint64 processStartTime = 0; // reduce overflow
}



template<class RESULT, class SOURCE>
inline const RESULT cast(SOURCE source)
{
  static_assert(std::is_pointer<RESULT>(), "Types must be pointers.");
  static_assert(std::is_pointer<SOURCE>(), "Types must be pointers.");
  static_assert(sizeof(*(RESULT)(nullptr)) == sizeof(*(SOURCE)(nullptr)), "Types must have same size.");
  return reinterpret_cast<const RESULT>(source);
}



uint64 Timer::getMeasureFrequency() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  auto f = frequency; // TAG: temp test

  unsigned int sleep = 10000;
  while (sleep < 1000000000) {
    FILETIME startReal;
    GetSystemTimePreciseAsFileTime(&startReal);
    uint64 startTicks = 0;
    ::QueryPerformanceCounter(cast<LARGE_INTEGER*>(&startTicks));
    Thread::nanosleep(sleep);
    FILETIME stopReal;
    GetSystemTimePreciseAsFileTime(&stopReal);
    uint64 stopTicks = 0;
    ::QueryPerformanceCounter(cast<LARGE_INTEGER*>(&stopTicks));
    auto deltaTicks = stopTicks - startTicks;
    if (deltaTicks >= 1000000) {
      auto elapsedTime = *cast<const uint64*>(&stopReal) - *cast<const uint64*>(&startReal);
      return muldiv(deltaTicks, 10 * 1000000, elapsedTime); // ticks per second
    }
    sleep *= 10;
  }
  return 0;
#else // unix
#if 1
  // _POSIX_C_SOURCE >= 199309L
  struct timespec time;
  int status = clock_getres(CLOCK_REALTIME, &time);
  if (!INLINE_ASSERT(!status)) {
    return 0;
  }
  uint64 resolution = static_cast<uint64>(1000000000)* temp.tv_sec + temp.tv_nsec;
  return 1000000000/resolution;
#else
  return 1000000;
#endif
#endif // flavor
}

uint64 Timer::getRealNow() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  FILETIME time;
  GetSystemTimePreciseAsFileTime(&time);
  return *cast<const uint64*>(&time)/10;
#else // unix
#if 1
  // _POSIX_C_SOURCE >= 199309L
  struct timespec time;
  int status = clock_getres(CLOCK_REALTIME, &time);
  if (!INLINE_ASSERT(!status)) {
    return 0;
  }
  return static_cast<uint64>(1000000) * temp.tv_sec + (temp.tv_nsec + 500)/1000;
#else
  struct timeval temp;
  gettimeofday(&temp, 0);
  return static_cast<uint64>(1000000) * temp.tv_sec + temp.tv_usec;
#endif
#endif // flavor
}

uint64 Timer::getNowNS() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  uint64 time = 0;
  ::QueryPerformanceCounter(cast<LARGE_INTEGER*>(&time));
  return muldiv(time, 1000000000, frequency);
#else // unix
#if 1
  // _POSIX_C_SOURCE >= 199309L
  struct timespec time;
  int status = clock_getres(CLOCK_MONOTONIC, &time);
  if (!INLINE_ASSERT(!status)) {
    return 0;
  }
  return static_cast<uint64>(1000000000) * temp.tv_sec + temp.tv_nsec;
#else
  struct timeval temp;
  gettimeofday(&temp, 0);
  return static_cast<uint64>(100000000) * temp.tv_sec + temp.tv_usec * 1000;
#endif
#endif // flavor
}

uint64 Timer::getNow() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // https://docs.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps
  uint64 time = 0;
  ::QueryPerformanceCounter(cast<LARGE_INTEGER*>(&time));
  return muldiv(time, 1000000, frequency);
#else // unix
#if 1
  // _POSIX_C_SOURCE >= 199309L
  struct timespec time;
  int status = clock_getres(CLOCK_MONOTONIC, &time);
  if (!INLINE_ASSERT(!status)) {
    return 0;
  }
  return static_cast<uint64>(1000000)* temp.tv_sec + (temp.tv_nsec + 500)/1000;
#else
  struct timeval temp;
  gettimeofday(&temp, 0);
  return static_cast<uint64>(1000000) * temp.tv_sec + temp.tv_usec;
#endif
#endif // flavor
}

FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Timer& value) throw(IOException)
{
  FormatOutputStream::PushContext push(stream);
  Timer::ElapsedTime time(value.getMicroseconds() * 1000);
  time.roundToMicrosecond();
  return stream << time.getHours() << ':'
                << time.getNMinutes() << ':'
                << time.getNSeconds() << '.'
                << setWidth(6) << ZEROPAD << time.getNMicroseconds();
}

void TimeScope::dump() const
{
  fout << "Elapsed time (H:M:S.microseconds): " << timer << ENDL;
}

// TAG: add test

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
