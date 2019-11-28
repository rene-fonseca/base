/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Timer.
  
  @short Timer.
*/

class _COM_AZURE_DEV__BASE__API Timer : public Object {
private:

  // TAG: we need nano resolution timer support also

  /** The start time in microseconds. */
  uint64 startTime = 0;
  /** The stop time in microseconds. */
  uint64 stopTime = 0;
public:

  /** 32-bit duration/elapsed time. ns/us/ms/s. Used when memory footprint is important. */
  struct XTime {
    uint32 exponent : 2; // 0~ns, 1~us, 2~ms: 3~s
    uint32 mantissa : 30;
  };
  
  /** Converts XTime to normal 64-bit time (nanoseconds). */
  static uint64 toTimeNS(XTime time) noexcept;

  /** Converts XTime to normal 64-bit time (microseconds). */
  static uint64 toTimeUS(XTime time) noexcept;

  /** Returns XTime from nanoseconds. */
  static XTime toXTimeNS(uint64 nanoseconds) noexcept;

  /** Returns XTime from microseconds. */
  static XTime toXTimeUS(uint64 microseconds) noexcept;

  struct Time {
    uint64 seconds;
    uint32 nanoseconds;
  };
  
  /**
    This class provides methods for converting nanoseconds into other time
    units.
    
    @short Elapsed time conversion.
  */
  class _COM_AZURE_DEV__BASE__API ElapsedTime {
  private:

    /** The elapsed time in nanoseconds. */
    uint64 nanoseconds = 0;
  public:
    
    /**
      Initializes time as 0.
    */
    inline ElapsedTime() noexcept
    {
    }

    /**
      Initializes the time.
    */
    inline ElapsedTime(uint64 _nanoseconds) noexcept
      : nanoseconds(_nanoseconds)
    {
    }

    /**
      Round to nearest microsecond. Use this after setting the time and before
      converting the time.
    */
    inline void roundToMicrosecond() noexcept
    {
      nanoseconds += 500;
    }
    
    /**
      Round to nearest millisecond. Use this after setting the time and before
      converting the time.
    */
    inline void roundToMillisecond() noexcept
    {
      nanoseconds += 500000;
    }
    
    /**
      Round to nearest second. Use this after setting the time and before
      converting the time.
    */
    inline void roundToSecond() noexcept
    {
      nanoseconds += 500000000;
    }
    
    /**
      Round to nearest minute. Use this after setting the time and before
      converting the time.
    */
    inline void roundToMinute() noexcept
    {
      nanoseconds += 60 * 500000000ULL;
    }
    
    /**
      Round to nearest hour. Use this after setting the time and before
      converting the time.
    */
    inline void roundToHour() noexcept
    {
      nanoseconds += 60 * 60 * 500000000ULL;
    }
    
    /**
      Round to nearest day. Use this after setting the time and before
      converting the time.
    */
    inline void roundToDay() noexcept
    {
      nanoseconds += 24 * 60 * 60 * 500000000ULL;
    }

    /**
      Returns the time in nanoseconds.
    */
    inline uint64 getNanoseconds() const noexcept
    {
      return nanoseconds;
    }

    /**
      Sets the time in nanoseconds.
    */
    inline void setNanoseconds(uint64 nanoseconds) noexcept
    {
      this->nanoseconds = nanoseconds;
    }
    
    /**
      Returns the time in normalized nanoseconds.
    */
    inline unsigned int getNNanoseconds() const noexcept
    {
      return nanoseconds % 1000000000;
    }
    
    /**
      Returns the time in normalized microseconds.
    */
    inline unsigned int getNMicroseconds() const noexcept
    {
      return (nanoseconds/1000) % 1000000;
    }
    
    /**
      Returns the time in normalized milliseconds.
    */
    inline unsigned int getNMilliseconds() const noexcept
    {
      return (nanoseconds/1000000) % 1000;
    }
    
    /**
      Returns the time in normalized seconds.
    */
    inline unsigned int getNSeconds() const noexcept
    {
      return (nanoseconds/1000000000) % 60;
    }
    
    /**
      Returns the time in seconds.
    */
    inline uint64 getSeconds() const noexcept
    {
      return nanoseconds/1000000000;
    }

    /**
      Returns the time in normalized minutes.
    */
    inline unsigned int getNMinutes() const noexcept
    {
      return (nanoseconds/(60 * 1000000000ULL)) % 60;
    }
    
    /**
      Returns the time in minutes.
    */
    inline unsigned int getMinutes() const noexcept
    {
      return nanoseconds/(60 * 1000000000ULL);
    }
    
    /**
      Returns the time in normalized hours.
    */
    inline unsigned int getNHours() const noexcept
    {
      uint64 seconds = nanoseconds/1000000000;
      uint64 minutes = seconds/60;
      uint64 hours = minutes/60;
      return hours % 24;
    }
    
    /**
      Returns the time in hours.
    */
    inline unsigned int getHours() const noexcept
    {
      return nanoseconds/(60 * 60 * 1000000000ULL);
    }

    /**
      Returns the time in days.
    */
    inline unsigned int getDays() const noexcept
    {
      return nanoseconds/(24 * 60 * 60 * 1000000000ULL);
    }
  };

  /**
    Measure frequency (ticks per second).
  */
  static uint64 getMeasureFrequency() noexcept;

  /**
    Returns the current clock time in microseconds. Time can go backwards.
  */
  static uint64 getRealNow() noexcept;

  /**
    Returns the current time in nanoseconds.
  */
  static uint64 getNowNS() noexcept;

  /**
    Returns the current time in microseconds.
  */
  static uint64 getNow() noexcept;

  /**
    Initializes the timer. The timer is started.
  */
  inline Timer() noexcept
  {
    start();
  }

  /**
    Starts the timer.
  */
  inline void start() noexcept
  {
    startTime = getNow();
  }

  /**
    Stops the timer.
  */
  inline void stop() noexcept
  {
    stopTime = getNow();
  }

  /**
    Returns the start time in microseconds.
  */
  inline uint64 getStartTime() const noexcept
  {
    return startTime;
  }

  /**
    Returns the stop time in microseconds.
  */
  inline uint64 getStopTime() const noexcept
  {
    return stopTime;
  }

  /**
    Returns the number of elapsed microseconds between start and stop times.
  */
  inline uint64 getMicroseconds() const noexcept
  {
    return stopTime - startTime;
  }

  /**
    Returns the number of elapsed microseconds since start.
  */
  inline uint64 getLiveMicroseconds() const noexcept
  {
    auto now = getNow();
    return now - startTime;
  }
};

/**
  Writes a string representation of the elapsed time of the specified timer.

  @relates Timer
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Timer& value) throw(IOException);



class _COM_AZURE_DEV__BASE__API TimeScope : public Object {
private:

  Timer timer;
public:

  inline TimeScope() noexcept
  {
  }

  inline void start() noexcept
  {
    timer.start();
  }

  inline void stop() noexcept
  {
    timer.stop();
  }

  void dump() const;

  inline ~TimeScope()
  {
    stop();
    dump();
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
