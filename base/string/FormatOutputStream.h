/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__FORMAT_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_STRING__FORMAT_OUTPUT_STREAM_H

#include "base/Object.h"
#include "base/io/BufferedOutputStream.h"
#include "base/io/BindException.h"

/**
  Format output stream.

  @author René Møller Fonseca
  @version 1.0
*/

class FormatOutputStream : public BufferedOutputStream {
public:
  /** Digits of all bases. */
  static const char DIGITS[] = "0123456789abcdef";
  /** Actions changing the format stream. */
  typedef enum {BIN, OCT, DEC, HEX, ZEROPAD, NOZEROPAD, PREFIX, NOPREFIX, EOL, FLUSH} Action;
protected:
  unsigned int defaultWidth;
  unsigned int defaultBase;
  unsigned int defaultFlags;

  unsigned int width;
  unsigned int base;
  unsigned int flags;
public:

  /**
    Initializes the format output stream.

    @param out The output stream.
    @param size The size of the buffer. Default is given by DEFAULT_BUFFER_SIZE.
  */
  FormatOutputStream(OutputStream* out, unsigned int size = DEFAULT_BUFFER_SIZE) throw(BindException);

  inline FormatOutputStream& setWidth(unsigned int width) throw() {this->width = width; return *this;};
  inline unsigned int getWidth() const throw() {return width;};

  inline FormatOutputStream& setBase(unsigned int base) throw() {this->base = base; return *this;};
  inline unsigned int getBase() const throw() {return base;};

  FormatOutputStream& operator<<(Action action) throw(IOException);

  void prepareForField();
  void addCharacterField(const char* buf, unsigned int size) throw(IOException);
  void addIntegerField(const char* buf, unsigned int size, bool isSigned) throw(IOException);
  void addDoubleField(const char* early, unsigned int earlySize, const char* late, unsigned int lateSize, bool isSigned) throw(IOException);
};

FormatOutputStream& operator<<(FormatOutputStream& stream, char value);
FormatOutputStream& operator<<(FormatOutputStream& stream, const char* value);
FormatOutputStream& operator<<(FormatOutputStream& stream, short int value);
FormatOutputStream& operator<<(FormatOutputStream& stream, int value);
FormatOutputStream& operator<<(FormatOutputStream& stream, long long int value);
FormatOutputStream& operator<<(FormatOutputStream& stream, float value);
FormatOutputStream& operator<<(FormatOutputStream& stream, double value);
FormatOutputStream& operator<<(FormatOutputStream& stream, long double value);

#endif
