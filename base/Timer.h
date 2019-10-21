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
  Timer wrapper class.
  
  @short Timer.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Timer : public Object {
private:

  /** The start time in platform specific resolution. */
  uint64 startTime = 0;
  /** The stop time in platform specific resolution. */
  uint64 stopTime = 0;
public:

  /**
    This class provides methods for converting nanoseconds into other time
    units.
    
    @short Elapsed time conversion.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API ElapsedTime {
  private:

    /** The elapsed time in nanoseconds. */
    uint64 nanoseconds = 0;
  public:
    
    /**
      Initializes time as 0.
    */
    inline ElapsedTime() noexcept {
    }

    /**
      Initializes the time.
    */
    inline ElapsedTime(unsigned int _nanoseconds) noexcept
      : nanoseconds(_nanoseconds) {
    }

    /**
      Round to nearest microsecond. Use this after setting the time and before
      converting the time.
    */
    inline void roundToMicrosecond() noexcept {
      nanoseconds += 500;
    }
    
    /**
      Round to nearest millisecond. Use this after setting the time and before
      converting the time.
    */
    inline void roundToMillisecond() noexcept {
      nanoseconds += 500000;
    }
    
    /**
      Round to nearest second. Use this after setting the time and before
      converting the time.
    */
    inline void roundToSecond() noexcept {
      nanoseconds += 500000000;
    }
    
    /**
      Round to nearest minute. Use this after setting the time and before
      converting the time.
    */
    inline void roundToMinute() noexcept {
      nanoseconds += 60 * 500000000ULL;
    }
    
    /**
      Round to nearest hour. Use this after setting the time and before
      converting the time.
    */
    inline void roundToHour() noexcept {
      nanoseconds += 60 * 60 * 500000000ULL;
    }
    
    /**
      Round to nearest day. Use this after setting the time and before
      converting the time.
    */
    inline void roundToDay() noexcept {
      nanoseconds += 24 * 60 * 60 * 500000000ULL;
    }

    /**
      Returns the time in nanoseconds.
    */
    inline uint64 getNanoseconds() const noexcept {
      return nanoseconds;
    }

    /**
      Sets the time in nanoseconds.
    */
    inline void setNanoseconds(uint64 nanoseconds) noexcept {
      this->nanoseconds = nanoseconds;
    }
    
    /**
      Returns the time in normalized nanoseconds.
    */
    inline unsigned int getNNanoseconds() const noexcept {
      return nanoseconds%1000000000;
    }
    
    /**
      Returns the time in normalized microseconds.
    */
    inline unsigned int getNMicroseconds() const noexcept {
      return (nanoseconds/1000)%1000000;
    }
    
    /**
      Returns the time in normalized milliseconds.
    */
    inline unsigned int getNMilliseconds() const noexcept {
      return (nanoseconds/1000000)%1000;
    }
    
    /**
      Returns the time in normalized seconds.
    */
    inline unsigned int getNSeconds() const noexcept {
      return (nanoseconds/1000000000)%60;
    }
    
    /**
      Returns the time in seconds.
    */
    inline uint64 getSeconds() const noexcept {
      return nanoseconds/1000000000;
    }

    /**
      Returns the time in normalized minutes.
    */
    inline unsigned int getNMinutes() const noexcept {
      return (nanoseconds/(60 * 1000000000ULL))%60;
    }
    
    /**
      Returns the time in minutes.
    */
    inline unsigned int getMinutes() const noexcept {
      return nanoseconds/(60 * 1000000000ULL);
    }
    
    /**
      Returns the time in normalized hours.
    */
    inline unsigned int getNHours() const noexcept {
      uint64 seconds = nanoseconds/1000000000;
      uint64 minutes = seconds/60;
      uint64 hours = minutes/60;
      return hours%24;
    }
    
    /**
      Returns the time in hours.
    */
    inline unsigned int getHours() const noexcept {
      return nanoseconds/(60 * 60 * 1000000000ULL);
    }

    /**
      Returns the time in days.
    */
    inline unsigned int getDays() const noexcept {
      return nanoseconds/(24 * 60 * 60 * 1000000000ULL);
    }
  };
  
  /**
    Initializes the timer. The timer is started.
  */
  Timer() noexcept;

  /**
    Starts the timer.
  */
  void start() noexcept;

  /**
    Stops the timer.
  */
  void stop() noexcept;

  /**
    Returns the start time in microseconds.
  */
  uint64 getStartTime() const noexcept;

  /**
    Returns the stop time in microseconds.
  */
  uint64 getStopTime() const noexcept;

  /**
    Returns the number of elapsed microseconds between start and stop times.
  */
  uint64 getMicroseconds() const noexcept;

  /**
    Returns the number of elapsed microseconds since start.
  */
  uint64 getLiveMicroseconds() const noexcept;
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

  inline TimeScope() noexcept {
  }

  inline void start() throw() {
    timer.start();
  }

  inline void stop() throw() {
    timer.stop();
  }

  void dump() const throw(IOException);

  inline ~TimeScope() {
    stop();
    dump();
  }
};

/**
  Times the current scope. Writes the elapsed time to fout when the scope is exited.
*/
#define timeScope() TimeScope _COM_AZURE_DEV__BASE__timer

#define startScopeTimer() _COM_AZURE_DEV__BASE__timer.start()

#define stopScopeTimer() _COM_AZURE_DEV__BASE__timer.stop()

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
