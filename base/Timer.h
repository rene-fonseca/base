/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__TIMER_H
#define _DK_SDU_MIP__BASE__TIMER_H

#include <base/Object.h>
#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Timer wrapper class.

  @short Timer.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
*/

class Timer : public Object {
private:

  /** The start time in platform specific resolution. */
  long long startTime;
  /** The stop time in platform specific resolution. */
  long long stopTime;
public:

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
  long long getStartTime() const throw();

  /**
    Returns the stop time in microseconds.
  */
  long long getStopTime() const throw();

  /**
    Returns the number of elapsed microseconds between start and stop times.
  */
  long long getMicroseconds() const throw();

  /**
    Returns the number of elapsed microseconds since start.
  */
  long long getLiveMicroseconds() const throw();
};

/**
  Writes a string representation of the elapsed time of the specified timer.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Timer& value) throw(IOException);



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

#endif
