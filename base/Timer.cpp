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
#include <base/math/Math.h>
#include <base/UnitTest.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#  include <sys/time.h>
#  include <time.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

template<class RESULT, class SOURCE>
inline const RESULT cast(SOURCE source)
{
  static_assert(std::is_pointer<RESULT>(), "Types must be pointers.");
  static_assert(std::is_pointer<SOURCE>(), "Types must be pointers.");
  static_assert(sizeof(*(RESULT)(nullptr)) == sizeof(*(SOURCE)(nullptr)), "Types must have same size.");
  return reinterpret_cast<const RESULT>(source);
}

namespace internal {

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  uint64 getFrequency() noexcept
  {
    LARGE_INTEGER frequency; // ticks per second
    BOOL status = ::QueryPerformanceFrequency(&frequency); // ignore any error
    if (!INLINE_ASSERT(status)) {
      return 0;
    }
    BASSERT(frequency.QuadPart > 0);
    return frequency.QuadPart;
  }

  inline uint64 getRealImpl() noexcept
  {
    FILETIME time;
    GetSystemTimePreciseAsFileTime(&time);
    return *cast<const uint64*>(&time);
  }

  inline uint64 getTicksImpl() noexcept
  {
    // https://docs.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps
    uint64 ticks = 0;
    ::QueryPerformanceCounter(cast<LARGE_INTEGER*>(&ticks));
    return ticks;
  }

  uint64 frequency = 0; // counts in seconds
  uint64 processStartTicks = 0;
  uint64 processStartReal = 0;

  void initializeTimer()
  {
    frequency = getFrequency(); // counts in seconds
    processStartTicks = getTicksImpl();
    processStartReal = getRealImpl();
  }
#else

  inline uint64 getRealImpl() noexcept
  {
    #if 1
      // _POSIX_C_SOURCE >= 199309L
      struct timespec time;
      int status = clock_gettime(CLOCK_REALTIME, &time);
      if (!INLINE_ASSERT(!status)) {
        return 0;
      }
      return static_cast<uint64>(1000000) * time.tv_sec + (time.tv_nsec + 500)/1000;
    #else
      struct timeval temp;
      gettimeofday(&temp, 0);
      return static_cast<uint64>(1000000) * temp.tv_sec + temp.tv_usec;
    #endif
  }

  inline uint64 getTicksImpl(bool nano) noexcept
  {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    if (nano) {
      return clock_gettime_nsec_np(CLOCK_MONOTONIC);
    }
#endif
#if 1
    // _POSIX_C_SOURCE >= 199309L
    struct timespec time;
    int status = clock_gettime(CLOCK_MONOTONIC, &time);
    if (!INLINE_ASSERT(!status)) {
      return 0;
    }
    if (nano) {
      return static_cast<uint64>(1000000 * 1000) * time.tv_sec + time.tv_nsec;
    } else {
      return static_cast<uint64>(1000000) * time.tv_sec + (time.tv_nsec + 500)/1000;
    }
#else
    struct timeval temp;
    gettimeofday(&temp, 0);
    if (nano) {
      return static_cast<uint64>(1000000 * 1000) * temp.tv_sec + temp.tv_usec * 1000;
    } else {
      return static_cast<uint64>(1000000) * temp.tv_sec + temp.tv_usec;
    }
#endif
  }

  uint64 processStartTicks = 0;
  uint64 processStartTicksNS = 0;
  uint64 processStartReal = 0;

  void initializeTimer()
  {
    processStartTicks = getTicksImpl(false);
    processStartTicksNS = getTicksImpl(true);
    processStartReal = getRealImpl();
  }
#endif
}

uint64 Timer::getMeasureFrequency() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  const auto startReal = internal::getRealImpl(); // 100ns resolution
  const uint64 startTicks = internal::getTicksImpl();
  unsigned int sleep = 10000;
  while (sleep < 1000000000) {
    Thread::nanosleep(sleep);
    const auto stopReal = internal::getRealImpl();
    const uint64 stopTicks = internal::getTicksImpl();
    const auto deltaTicks = stopTicks - startTicks;
    if (deltaTicks >= 1000000) {
      const auto elapsedTime = stopReal - startReal;
      return Math::muldiv(deltaTicks, 10 * 1000000, elapsedTime); // ticks per second
    }
    sleep *= 10;
  }
  return 0;
#else // unix
#if 1
  // _POSIX_C_SOURCE >= 199309L
  struct timespec time;
  int status = clock_getres(CLOCK_MONOTONIC, &time);
  if (!INLINE_ASSERT(!status)) {
    return 0;
  }
  uint64 resolution = static_cast<uint64>(1000000000) * time.tv_sec + time.tv_nsec;
  return 1000000000/resolution;
#else
  return 1000000;
#endif
#endif // flavor
}

uint64 Timer::getRealNow() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return (internal::getRealImpl() - internal::processStartReal)/10;
#else // unix
  return internal::getRealImpl() - internal::processStartReal;
#endif // flavor
}

uint64 Timer::getNowNS() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return Math::muldiv(internal::getTicksImpl() - internal::processStartTicks, 1000000000, internal::frequency);
#else // unix
  return internal::getTicksImpl(true) - internal::processStartTicksNS;
#endif // flavor
}

uint64 Timer::getNow() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return Math::muldiv(internal::getTicksImpl() - internal::processStartTicks, 1000000, internal::frequency);
#else // unix
  return internal::getTicksImpl(false) - internal::processStartTicks;
#endif // flavor
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Timer& value)
{
  FormatOutputStream::PushContext push(stream);
  Timer::ElapsedTime time(((value.getStopTime() == 0) ? value.getLiveMicroseconds() : value.getMicroseconds()) * 1000);
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

uint64 Timer::toTimeNS(XTime time) noexcept
{
  switch (time.exponent) {
  case 0:
    return static_cast<uint64>(time.mantissa); // ns
  case 1:
    return static_cast<uint64>(time.mantissa) * 1000; // us
  case 2:
    return static_cast<uint64>(time.mantissa) * 1000000; // ms
  case 3:
  default:
    return static_cast<uint64>(time.mantissa) * 1000000000; // s
  }
}

uint64 Timer::toTimeUS(XTime time) noexcept
{
  switch (time.exponent) {
  case 0:
    return (static_cast<uint64>(time.mantissa) + 500) / 1000; // ns
  case 1:
    return static_cast<uint64>(time.mantissa) * 1; // us
  case 2:
    return static_cast<uint64>(time.mantissa) * 1000; // ms
  case 3:
  default:
    return static_cast<uint64>(time.mantissa) * 1000000; // s
  }
}

Timer::XTime Timer::toXTimeNS(uint64 nanoseconds) noexcept
{
  constexpr uint32 MAXIMUM = (static_cast<uint32>(1) << 30) - 1;

  XTime result; // round down
  if (nanoseconds <= MAXIMUM) {
    result.exponent = 0; // ns
    result.mantissa = static_cast<uint32>(nanoseconds);
  } else if (((nanoseconds + 0 * 500) / 1000) <= MAXIMUM) {
    result.exponent = 1; // us
    result.mantissa = static_cast<uint32>((nanoseconds + 0 * 500) / 1000);
  } else if (((nanoseconds + 0 * 500000) / 1000000) <= MAXIMUM) {
    result.exponent = 2; // ms
    result.mantissa = static_cast<uint32>((nanoseconds + 0 * 500000) / 1000000);
  } else {
    result.exponent = 3; // s
    result.mantissa = static_cast<uint32>((nanoseconds + 0 * 500000000) / 1000000000);
  }
  return result;
}

Timer::XTime Timer::toXTimeUS(uint64 microseconds) noexcept
{
  constexpr uint32 MAXIMUM = (static_cast<uint32>(1) << 30) - 1;

  XTime result; // round down
  if (microseconds <= MAXIMUM) {
    result.exponent = 1; // us
    result.mantissa = static_cast<uint32>(microseconds);
  } else if (((microseconds + 0 * 500) / 1000) <= MAXIMUM) {
    result.exponent = 2; // ms
    result.mantissa = static_cast<uint32>((microseconds + 0 * 500) / 1000);
  } else {
    result.exponent = 3; // s
    result.mantissa = static_cast<uint32>((microseconds + 0 * 500000) / 1000000);
  }
  return result;
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Timer) : public UnitTest {
public:

  TEST_PRIORITY(50);
  TEST_PROJECT("base");

  void run() override
  {
    auto start = Timer::getNow();
    TEST_ASSERT(start > 0);
    Timer t1;
    Thread::nanosleep(100 * 1000);
    t1.stop();
    auto elapsed1 = Timer::getNow() - start;
    TEST_ASSERT(elapsed1 > 0);
    auto realNow = Timer::getRealNow();
    TEST_ASSERT(realNow > 0);
    auto frequency = Timer::getMeasureFrequency();
    TEST_ASSERT(frequency > 0);
    auto elapsed2 = t1.getMicroseconds();
    TEST_ASSERT(elapsed2 > 0);
  }
};

#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
TEST_REGISTER(Timer);
#endif

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
