/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__SYSTEM_LOGGER_H
#define _DK_SDU_MIP__BASE__SYSTEM_LOGGER_H

#include <base/Object.h>
#include <base/string/String.h>

// protect against the evil programmers
#undef INFORMATION
#undef WARNING
#undef ERROR

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short System logger.
  @author René Møller Fonseca
  @version 1.0
*/

class SystemLogger : public Object {
public:

  /** Specifies the type of the message. */
  typedef enum {
    INFORMATION = 0, /**< Informational message. */
    WARNING = 1, /**< Warning message. */
    ERROR = 2 /**< Error message. */
  } MessageType;

  /**
    Sends the specified message to the system logger.
  */
  static void write(MessageType type, const String& message) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
