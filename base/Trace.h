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

#include <base/features.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  @defgroup debugging Debugging
*/

/**
  This class is used for dumping tracing information to the dedicated debugging
  output stream, standard error, or system log dependent on the platform.

  @short Tracing interface
  @ingroup debugging
  @version 1.0
*/

class Trace {
private:

  /** The message to be written. */
  const char* msg = nullptr;
public:

  /**
    Write message to standard error.
  */
  static void message(const char* message) throw();

  /**
    Write pointer and message to standard error.
  */
  static void member(const void* ptr, const char* message) throw();

  /**
    Writes the message.
  */
  Trace(const char* message) throw();

  /**
    Writes the message.
  */
  ~Trace() throw();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
