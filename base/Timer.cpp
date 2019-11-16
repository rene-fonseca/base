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

namespace {

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  inline uint64 getFrequency() noexcept
  {
    LARGE_INTEGER frequency; // ticks per second
    BOOL status = ::QueryPerformanceFrequency(&frequency); // ignore any error
    BASSERT(status);
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

  const uint64 frequency = getFrequency(); // counts in seconds
#endif

  uint64 processStartTicks = 0;
  uint64 processStartReal = 0;
}






uint64 Timer::getMeasureFrequency() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  const auto startReal = getRealImpl(); // 100ns resolution
  const uint64 startTicks = getTicksImpl();
  unsigned int sleep = 10000;
  while (sleep < 1000000000) {
    Thread::nanosleep(sleep);
    const auto stopReal = getRealImpl();
    const uint64 stopTicks = getTicksImpl();
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
  int status = clock_getres(CLOCK_REALTIME, &time);
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
  return getRealImpl()/10;
#else // unix
#if 1
  // _POSIX_C_SOURCE >= 199309L
  struct timespec time;
  int status = clock_getres(CLOCK_REALTIME, &time);
  if (!INLINE_ASSERT(!status)) {
    return 0;
  }
  return static_cast<uint64>(1000000) * time.tv_sec + (time.tv_nsec + 500)/1000;
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
  return Math::muldiv(getTicksImpl(), 1000000000, frequency);
#else // unix
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  return clock_gettime_nsec_np(CLOCK_MONOTONIC);
#elif 1
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
  return Math::muldiv(getTicksImpl(), 1000000, frequency);
#else // unix
#if 1
  // _POSIX_C_SOURCE >= 199309L
  struct timespec time;
  int status = clock_getres(CLOCK_MONOTONIC, &time);
  if (!INLINE_ASSERT(!status)) {
    return 0;
  }
  return static_cast<uint64>(1000000) * time.tv_sec + (time.tv_nsec + 500)/1000;
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

TEST_REGISTER(Timer);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
