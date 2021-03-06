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
#include <base/string/WideString.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  @short System logger.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API SystemLogger : public Object {
public:

  /** Specifies the type of the message. */
  enum MessageType {
    INFORMATION = 0, /**< Informational message. */
    WARNING = 1, /**< Warning message. */
    ERROR = 2 /**< Error message. */
  };

  /**
    Sends the specified message to the system logger.
  */
  static void write(MessageType type, const String& message) noexcept;

  /**
    Sends the specified message to the system logger.
  */
  static void write(MessageType type, const WideString& message) noexcept;

  /**
    Sends the specified message to the system logger.
  */
  static void write(MessageType type, const char* message) noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
