/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__FORMAT_INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_STRING__FORMAT_INPUT_STREAM_H

#include <base/io/BufferedInputStream.h>
#include <base/io/BindException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Format input stream. This class provides...

  @author René Møller Fonseca
  @version 1.0
*/

class FormatInputStream : public BufferedInputStream {
public:

  /**
    Initializes the format input stream.

    @param in The input stream.
    @param size The size of the buffer. Default is given by DEFAULT_BUFFER_SIZE.
  */
  FormatInputStream(InputStream& in, unsigned int size = DEFAULT_BUFFER_SIZE) throw(BindException);

  /**
    Read a character from the stream.
  */
  char getCharacter() throw(IOException);

  /**
    Destroy format input stream.
  */
  ~FormatInputStream();
};

/**
  Format input stream linked to the standard input stream. This variable
  corresponds to 'cin' from the Standard Template Library.
*/
extern FormatInputStream fin;

FormatInputStream& operator>>(FormatInputStream& stream, bool& value);
FormatInputStream& operator>>(FormatInputStream& stream, char& value);
FormatInputStream& operator>>(FormatInputStream& stream, char*& value);
//FormatInputStream& operator>>(FormatInputStream& stream, short int value);
//FormatInputStream& operator>>(FormatInputStream& stream, unsigned short int value);
//FormatInputStream& operator>>(FormatInputStream& stream, int value);
//FormatInputStream& operator>>(FormatInputStream& stream, unsigned int value);
//FormatInputStream& operator>>(FormatInputStream& stream, long long int value);
//FormatInputStream& operator>>(FormatInputStream& stream, unsigned long long int value);
//FormatInputStream& operator>>(FormatInputStream& stream, float value);
//FormatInputStream& operator>>(FormatInputStream& stream, double value);
//FormatInputStream& operator>>(FormatInputStream& stream, long double value);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
