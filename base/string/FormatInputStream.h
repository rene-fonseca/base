/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__FORMAT_INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_STRING__FORMAT_INPUT_STREAM_H

#include <base/io/BufferedInputStream.h>
#include <base/io/BindException.h>

/**
  Format input stream. This class provides...

  @author René Møller Fonseca
  @version 1.0
*/

class FormatInputStream : public BufferedInputStream {
public:

  /**
    Initializes the string input stream.

    @param name The input stream.
  */
  FormatInputStream(InputStream& in) throw(BindException);
};

/**
  Format input stream linked to the standard input stream. This variable
  corresponds to 'cin' from the Standard Template Library.
*/
//extern FormatInputStream fin;

#endif
