/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__STRING_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_STRING__STRING_OUTPUT_STREAM_H

#include "base/Object.h"
#include "base/string/String.h"

/**
  A string output stream is a format output stream that directs the stream data into a string.

  @author René Møller Fonseca
  @version 1.0
*/

class StringOutputStream : public FormatOutputStream {
public:

  /** Specifies the default amount of memory by which the capacity of the string is increased. */
  static const unsigned int DEFAULT_INCREMENT = 1024;
protected:

  /** The string holding the stream data. */
  String<> str;
public:

  /**
    Initializes string output stream.

    @param increment Specifies the default amount of memory by which the capacity of the string is increased. Default is given by DEFAULT_INCREMENT.
  */
  explicit StringOutputStream(unsigned int increment = DEFAULT_INCREMENT) throw(BindException);

  /**
    Returns the string associated with the stream.
  */
  const String<>& getString() const throw();
};

#endif
