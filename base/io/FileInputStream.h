/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_IO_FILE_INPUT_STREAM_H
#define _DK_SDU_MIP_BASE_IO_FILE_INPUT_STREAM_H

/**
  Interface.

  @author René Møller Fonseca
  @version 1.0
*/

class InputStream {
public:

  /**
    Returns
  */
  virtual unsigned int available() {return 0;};

  /**
    Closes the stream.
  */
  virtual void close() {};

  virtual char read() = 0;

  virtual unsigned int read(char* buffer, unsigned int count) {
    char* head = &buffer[0];
    char* tail = &buffer[count];
    while (head != tail) {
      buffer[head] = read();
      ++head;
    }
  }

  /**
    Skips a specified number of ... blocks...
  */
  void skip(unsigned int count) {
    while (count) {
      read();
      --count;
    }
  }
};

#endif
