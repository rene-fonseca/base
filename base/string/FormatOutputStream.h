/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__FORMAT_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_STRING__FORMAT_OUTPUT_STREAM_H

#include <base/io/BufferedOutputStream.h>
#include <base/io/BindException.h>
#include <base/concurrency/Synchronizeable.h>
#include <base/concurrency/Synchronize.h>
#include <base/concurrency/SpinLock.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/*
struct DateFormatSet {
  char separator;
};

struct NumberFormatSet {
  char point;
};

struct CollectionFormatSet {
  char begin;
  char end;
  char separator;
};

class CollectionFormatSet2 {
private:
public:
  FormatSet(C)
  operator const CollectionFormatSet& throw() {return set;}
};

CollectionFormatSet defaultFormatSet {
  '{'
  '}'
  ';'
};
*/

/**
  Actions used to modify a format output stream. Use BIN, OCT, DEC, and HEX to
  select an appropriate integer base. Use ZEROPAD and NOZEROPAD to
  enable/disable zero padding. Use PREFIX and NOPREFIX to enable/disable
  prefixes for numbers. EOL writes a new line to the stream. FLUSH forces the
  internal buffers to be flushed. Use ENDL to both write a new line and flush
  the internal buffers.
*/
typedef enum {
  BIN, /**< Select binary integer base. */
  OCT, /**< Select octal integer base. */
  DEC, /**< Select decimal integer base. */
  HEX, /**< Select hexadecimal integer base. */
  ZEROPAD, /**< Enable zero padding. */
  NOZEROPAD, /**< Disable zero padding. */
  PREFIX, /**< Enable prefixes for integers. */
  NOPREFIX, /**< Disable prefixes for integers. */
  EOL, /**< Writes a new line. */
  FLUSH, /**< Flushes internal buffers. */
  ENDL /**< Write a new line and flush stream. */
} Action;

/**
  Format output stream.

  @author René Møller Fonseca
  @version 1.0
*/

class FormatOutputStream : public BufferedOutputStream, public Synchronizeable<SpinLock> {
public:

  /** Digits of all bases. */
  static const char DIGITS[]; // = "0123456789abcdef";
protected:

  unsigned int defaultWidth;
  unsigned int defaultBase;
  unsigned int defaultFlags;

  unsigned int width;
  unsigned int base;
  unsigned int flags;
public:

  typedef SpinLock LOCK;

  /**
    Sets the default field width.

    @param stream The format output stream.
    @param width The desired width.
  */
  static inline FormatOutputStream& setWidth(FormatOutputStream& stream, unsigned int width) throw() {return stream.setWidth(width);}

  /**
    Initializes the format output stream.

    @param out The output stream.
    @param size The size of the buffer. Default is given by DEFAULT_BUFFER_SIZE.
  */
  FormatOutputStream(OutputStream& out, unsigned int size = DEFAULT_BUFFER_SIZE) throw(BindException);

  /**
    Returns the default field width.
  */
  inline unsigned int getWidth() const throw() {return width;}

  /**
    Sets the default field width.

    @param width The desired width.
  */
  inline FormatOutputStream& setWidth(unsigned int width) throw() {this->width = width; return *this;}

  /**
    Sets the default base.
  */
  inline FormatOutputStream& setBase(unsigned int base) throw() {this->base = base; return *this;}

  /**
    Returns the default base.
  */
  inline unsigned int getBase() const throw() {return base;}

  /**
    Send action to stream.
  */
  FormatOutputStream& operator<<(Action action) throw(IOException);

  /**
    Prepare stream for a new field.
  */
  void prepareForField();

  /**
    Write a character to the stream.
  */
  void addCharacterField(const char* buf, unsigned int size) throw(IOException);

  /**
    Write preformated integer to stream.
  */
  void addIntegerField(const char* buf, unsigned int size, bool isSigned) throw(IOException);

  /**
    Write preformated double to stream.
  */
  void addDoubleField(const char* early, unsigned int earlySize, const char* late, unsigned int lateSize, bool isSigned) throw(IOException);

  /**
    Destroy format output stream.
  */
  ~FormatOutputStream() throw(IOException);
};

/**
  Format output stream linked to the standard output stream. This variable
  corresponds to 'cout' from the Standard Template Library.
*/
extern FormatOutputStream fout;

/**
  Format output stream linked to the standard error stream. This variable
  corresponds to 'cerr' from the Standard Template Library.
*/
extern FormatOutputStream ferr;

FormatOutputStream& operator<<(FormatOutputStream& stream, bool value);
FormatOutputStream& operator<<(FormatOutputStream& stream, char value);
FormatOutputStream& operator<<(FormatOutputStream& stream, const char* value);
FormatOutputStream& operator<<(FormatOutputStream& stream, short int value);
FormatOutputStream& operator<<(FormatOutputStream& stream, unsigned short int value);
FormatOutputStream& operator<<(FormatOutputStream& stream, int value);
FormatOutputStream& operator<<(FormatOutputStream& stream, unsigned int value);
FormatOutputStream& operator<<(FormatOutputStream& stream, long value);
FormatOutputStream& operator<<(FormatOutputStream& stream, unsigned long value);
FormatOutputStream& operator<<(FormatOutputStream& stream, long long int value);
FormatOutputStream& operator<<(FormatOutputStream& stream, unsigned long long int value);
FormatOutputStream& operator<<(FormatOutputStream& stream, float value);
FormatOutputStream& operator<<(FormatOutputStream& stream, double value);
FormatOutputStream& operator<<(FormatOutputStream& stream, long double value);



class FormatOutputStreamManipulator {
private:

  FormatOutputStream& (*function)(FormatOutputStream&, unsigned int);
  unsigned int value;
public:

  inline FormatOutputStreamManipulator(FormatOutputStream& (*f)(FormatOutputStream&, unsigned int), unsigned int v) : function(f), value(v) {}

  inline FormatOutputStream& operator()(FormatOutputStream& stream) {
    return function(stream, value);
  }
};

inline FormatOutputStreamManipulator setWidth(unsigned int width) throw() {
  return FormatOutputStreamManipulator(&FormatOutputStream::setWidth, width);
}

inline FormatOutputStream& operator<<(FormatOutputStream& stream, FormatOutputStreamManipulator manipulator) {
  return manipulator(stream);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
