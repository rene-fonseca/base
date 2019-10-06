/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Timer wrapper class.
  
  @short Timer.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Timer : public Object {
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
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class ElapsedTime {
  private:

    /** The elapsed time in nanoseconds. */
    uint64 nanoseconds;
  public:
    
    /**
      Initializes time as 0.
    */
    inline ElapsedTime() throw() : nanoseconds(0) {
    }

    /**
      Initializes the time.
    */
    inline ElapsedTime(unsigned int _nanoseconds) throw()
      : nanoseconds(_nanoseconds) {
    }

    /**
      Round to nearest microsecond. Use this after setting the time and before
      converting the time.
    */
    inline void roundToMicrosecond() throw() {
      nanoseconds += 500;
    }
    
    /**
      Round to nearest millisecond. Use this after setting the time and before
      converting the time.
    */
    inline void roundToMillisecond() throw() {
      nanoseconds += 500000;
    }
    
    /**
      Round to nearest second. Use this after setting the time and before
      converting the time.
    */
    inline void roundToSecond() throw() {
      nanoseconds += 500000000;
    }
    
    /**
      Round to nearest minute. Use this after setting the time and before
      converting the time.
    */
    inline void roundToMinute() throw() {
      nanoseconds += 60 * 500000000ULL;
    }
    
    /**
      Round to nearest hour. Use this after setting the time and before
      converting the time.
    */
    inline void roundToHour() throw() {
      nanoseconds += 60 * 60 * 500000000ULL;
    }
    
    /**
      Round to nearest day. Use this after setting the time and before
      converting the time.
    */
    inline void roundToDay() throw() {
      nanoseconds += 24 * 60 * 60 * 500000000ULL;
    }

    /**
      Returns the time in nanoseconds.
    */
    inline uint64 getNanoseconds() const throw() {
      return nanoseconds;
    }

    /**
      Sets the time in nanoseconds.
    */
    inline void setNanoseconds(uint64 nanoseconds) throw() {
      this->nanoseconds = nanoseconds;
    }
    
    /**
      Returns the time in normalized nanoseconds.
    */
    inline unsigned int getNNanoseconds() const throw() {
      return nanoseconds%1000000000;
    }
    
    /**
      Returns the time in normalized microseconds.
    */
    inline unsigned int getNMicroseconds() const throw() {
      return (nanoseconds/1000)%1000000;
    }
    
    /**
      Returns the time in normalized milliseconds.
    */
    inline unsigned int getNMilliseconds() const throw() {
      return (nanoseconds/1000000)%1000;
    }
    
    /**
      Returns the time in normalized seconds.
    */
    inline unsigned int getNSeconds() const throw() {
      return (nanoseconds/1000000000)%60;
    }
    
    /**
      Returns the time in seconds.
    */
    inline uint64 getSeconds() const throw() {
      return nanoseconds/1000000000;
    }

    /**
      Returns the time in normalized minutes.
    */
    inline unsigned int getNMinutes() const throw() {
      return (nanoseconds/(60 * 1000000000ULL))%60;
    }
    
    /**
      Returns the time in minutes.
    */
    inline unsigned int getMinutes() const throw() {
      return nanoseconds/(60 * 1000000000ULL);
    }
    
    /**
      Returns the time in normalized hours.
    */
    inline unsigned int getNHours() const throw() {
      uint64 seconds = nanoseconds/1000000000;
      uint64 minutes = seconds/60;
      uint64 hours = minutes/60;
      return hours%24;
    }
    
    /**
      Returns the time in hours.
    */
    inline unsigned int getHours() const throw() {
      return nanoseconds/(60 * 60 * 1000000000ULL);
    }

    /**
      Returns the time in days.
    */
    inline unsigned int getDays() const throw() {
      return nanoseconds/(24 * 60 * 60 * 1000000000ULL);
    }
  };
  
  /**
    Initializes the timer. The timer is started.
  */
  Timer() throw();

  /**
    Starts the timer.
  */
  void start() throw();

  /**
    Stops the timer.
  */
  void stop() throw();

  /**
    Returns the start time in microseconds.
  */
  uint64 getStartTime() const throw();

  /**
    Returns the stop time in microseconds.
  */
  uint64 getStopTime() const throw();

  /**
    Returns the number of elapsed microseconds between start and stop times.
  */
  uint64 getMicroseconds() const throw();

  /**
    Returns the number of elapsed microseconds since start.
  */
  uint64 getLiveMicroseconds() const throw();
};

/**
  Writes a string representation of the elapsed time of the specified timer.

  @relates Timer
*/
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Timer& value) throw(IOException);



class TimeScope : public Object {
private:

  Timer timer;
public:

  inline TimeScope() throw() {
  }

  inline void start() throw() {
    timer.start();
  }

  inline void stop() throw() {
    timer.stop();
  }

  void dump() const throw(IOException);

  inline ~TimeScope() throw(IOException) {
    stop();
    dump();
  }
};

/**
  Times the current scope. Writes the elapsed time to fout when the scope is exited.
*/
#define timeScope() TimeScope _dk_sdu_mip__base__timer

#define startScopeTimer() _dk_sdu_mip__base__timer.start()

#define stopScopeTimer() _dk_sdu_mip__base__timer.stop()

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
