/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__FORMAT_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_STRING__FORMAT_OUTPUT_STREAM_H

#include <base/io/BufferedOutputStream.h>
#include <base/io/BindException.h>
#include <base/concurrency/Synchronizeable.h>
#include <base/concurrency/Synchronize.h>
#include <base/concurrency/SpinLock.h>
#include <base/string/ASCIITraits.h>
#include <base/Type.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Locale {}; // TAG: temp fix

/**
  Actions used to modify a format output stream. Use BIN, OCT, DEC, and HEX to
  select an appropriate integer base. Use ZEROPAD and NOZEROPAD to
  enable/disable zero padding. Use PREFIX and NOPREFIX to enable/disable
  prefixes for numbers. EOL writes a new line to the stream. FLUSH forces the
  internal buffers to be flushed. Use ENDL to both write a new line and flush
  the internal buffers.
*/
enum Action {
  BIN, /**< Selects binary base integer types. */
  OCT, /**< Selects octal base integer types. */
  DEC, /**< Selects decimal base integer types. */
  HEX, /**< Selects hexadecimal base integer types. */
  FBIN, /**< Selects binary base for floating-point types. */
  FOCT, /**< Selects octal base for floating-point types. */
  FDEC, /**< Selects decimal base for floating-point types. */
  FHEX, /**< Selects hexadecimal base for floating-point types. */
  SCIENTIFIC, /**< Selects scientific style for floating-point types ([-]d.ddde[-]dd). One digit before radix character and exponent is always present. The number of digits after the radix character is equal to the selected precision. */
  FIXED, /**< Selects default floating-point style ([-]ddd.ddd). The exponent is adjusted to zero if close to zero. */
  ENGINEERING, /**< Select engineering style. Same as scientific style but the exponent is adjusted such that exponent%3=0. */
  ZEROPAD, /**< Enables zero padding for integer types. */
  NOZEROPAD, /**< Disables zero padding for integer types. */
  GROUPING, /**< Enables thousands grouping for floating-point types (only if the current locale has a grouping character defined). */
  NOGROUPING, /**< Disables thousands grouping for floating-point types. */
  PLUS, /**< Forces plus sign to be written for signed integer types (unsigned integer types never has a sign). */
  FPLUS, /**< Forces plus sign to be written for floating-point types. */
  PLUSEXP, /**< Show positive exponent for floating-point types explicitly. */
  ZEROPADEXP, /**< Zero pads the exponent of floating-point types to the maximum number of digits. */
  PREFIX, /**< Enable prefixes for integers. */
  NOPREFIX, /**< Disable prefixes for integers. */
  NECESSARY, /**< Specifies that no garbage digits should be output for floating-point numbers. This is the default behaviour. */
  UPPER, /**< Selects capital letters (both integers and real numbers). */
  LOWER, /**< Selects lower case letters (both integers and real numbers). */
  POSIX, /**< Override current locale and use POSIX formatting. */
  LOCALE, /**< Use current locale during formatting. */
  LEFT, /**< Selects left justification within field. */
  RIGHT, /**< Selects right justification within field. This is the default. */
  UNIXEOL, /**< Selects line-feed (LF) as EOL. This is the default for all platforms. */
  WINDOWSEOL, /**< Selects carriage-return followed by line-feed (LF) as EOL. */
  MACEOL, /**< Selects line-feed (LF) followed by carriage-return (CR) as EOL. */
  LF, /**< Writes a line-feed (LF). */
  CRLF, /**< Write both a carriage-return (CR) and a line-feed (LF). */
  LFCR, /**< Write both a line-feed (LF) and carriage-return (CR). */
  EOL, /**< Writes a new line as requested by UNIXEOL, WINDOWSEOL, and MACEOL. */
  FLUSH, /**< Flushes the stream. */
  ENDL, /**< Write an EOL and flushes the stream. */
  PUSH, /**< Pushes the current context. */
  POP /**< Restores a previous context. */
};

/**
  Stream used to stringify types.

  @short Format output stream.
  @author René Møller Fonseca
  @version 1.2
*/

class FormatOutputStream : public BufferedOutputStream, public Synchronizeable<SpinLock> {
public:

  class Symbols {
  public:
    enum Base {BINARY, OCTAL, DECIMAL, HEXADECIMAL};
    enum RealStyle {SCIENTIFIC, FIXED, ENGINEERING};
    enum EndOfLine {UNIXEOL, WINDOWSEOL, MACEOL};
    enum Justification {LEFT, RIGHT, RADIX};
    enum {ZEROPAD = 1, PREFIX = 2, GROUPING = 4, PLUS = 8, FPLUS = 16, PLUSEXP = 32, ZEROPADEXP = 64, NECESSARY = 128, UPPER = 256, POSIX = 512};
  };

  class Manipulator {
  public:
    typedef FormatOutputStream& (FormatOutputStream::*Method)(unsigned int);
  private:
    Method method;
    unsigned int value;
  public:
    inline Manipulator(Method m, unsigned int v) : method(m), value(v) {}
    inline FormatOutputStream& operator()(FormatOutputStream& stream) {
      return (stream.*method)(value);
    }
  };

  class Context {
  private:
    Symbols::EndOfLine endOfLine;
    Symbols::Base integerBase;
    Symbols::Base realBase;
    Symbols::RealStyle realStyle;
    Symbols::Justification justification;
    unsigned int flags;
    int width;
    int precision;
  };

  class GetContext {
  private:
    Context& context;
  public:
    inline GetContext(Context& c) throw() : context(c) {}
    inline FormatOutputStream& operator()(FormatOutputStream& stream) throw() {
      return stream.getContext(context);
    }
  };

  class SetContext {
  private:
    const Context& context;
  public:
    inline SetContext(const Context& c) throw() : context(c) {}
    inline FormatOutputStream& operator()(FormatOutputStream& stream) throw() {
      return stream.setContext(context);
    }
  };
protected:

  Context defaultContext;
  Context context;

  /** The default integer base. */
  Symbols::Base defaultIntegerBase;
  /** The default floating-point base. */
  Symbols::Base defaultRealBase;
  /** The default floating-point style. */
  Symbols::RealStyle defaultRealStyle;
  /** The default eol. */
  Symbols::EndOfLine endOfLine;
  /** The default format flags. */
  unsigned int defaultFlags;
  /** The default width. */
  int defaultWidth;
  /** Specifies the number of digits to be written after the radix character. */
  int defaultPrecision;

  /** The format flags of the next field. */
  unsigned int flags;
  /** The precision for the next field. */
  int precision;
  /** The width of the next field. */
  int width;
  /** The desired radix position. */
  int radixPosition;
  /** The base of integers. */
  Symbols::Base integerBase;
  /** The base of floating-point numers. */
  Symbols::Base realBase;
  /** The style of floating-point numbers. */
  Symbols::RealStyle realStyle;
  /** Justification. */
  Symbols::Justification justification;
public:

  /** Specifies the maximum field width. */
  static const unsigned int MAXIMUM_WIDTH = 128;
  /** Specifies the maximum precision. */
  static const unsigned int MAXIMUM_PRECISION = 64;
  /** Specifies the maximum number of characters for real numbers. */
  static const unsigned int MAXIMUM_SIZE = 256;

  typedef SpinLock LOCK;

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
    Sets the default field width. The width silently reduced to MAXIMUM_WIDTH.

    @param width The desired width.
  */
  FormatOutputStream& setWidth(unsigned int width) throw();

  /**
    Sets the precision for floating-point numbers (i.e. the number of digits
    after the decimal-point). The requested precision is silently reduced to
    MAXIMUM_PRECISION.

    @param precision The desired precision.
  */
  FormatOutputStream& setPrecision(unsigned int precision) throw();

  /**
    Sets the desired position of the decimal-point within the field. Please
    note that the position is only advisory.

    @param position The desired position.
  */
  FormatOutputStream& setRadixPosition(unsigned int position) throw();

  /**
    Sets the locale of the stream. The locale is global for the stream. The
    stream uses POSIX formatting until the locale is set explicitly using this
    method. The action POSIX can be used to override the locale (see ISO/IEC
    WG14 N884, NCITS J11/99-019).

    @param locale The desired locale.
  */
  void setLocale(const Locale& locale) throw();

  /**
    Returns the integer base.
  */
  inline Symbols::Base getBase() const throw() {return integerBase;}

  /**
    Returns the flags for the next field.
  */
  inline unsigned int getFlags() const throw() {return flags;}

  /**
    Returns the precision for the next field.
  */
  inline unsigned int getPrecision() const throw() {return precision;}

  /**
    Send action to stream.
  */
  FormatOutputStream& operator<<(Action action) throw(IOException);

  /**
    Prepare stream for a new field.
  */
  void prepareForField();

  /**
    Writes a character to the stream.
  */
  void addCharacterField(const char* buffer, unsigned int size) throw(IOException);

  /**
    Writes a preformated integer to stream.
  */
  void addIntegerField(const char* buffer, unsigned int size, bool isSigned) throw(IOException);

  /**
    Writes a preformated floating point value to stream.
  */
  void writeFloatingPointType(unsigned int significant, unsigned int* mantissa, unsigned int mantissaSize, int base2Exponent, unsigned int valueFlags) throw(IOException);

  /**
    Gets the context.
  */
  FormatOutputStream& getContext(Context& context) throw();

  /**
    Sets the context.
  */
  FormatOutputStream& setContext(const Context& context) throw();

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

FormatOutputStream& operator<<(FormatOutputStream& stream, bool value) throw(IOException);
FormatOutputStream& operator<<(FormatOutputStream& stream, char value) throw(IOException);
FormatOutputStream& operator<<(FormatOutputStream& stream, const char* value) throw(IOException);
FormatOutputStream& operator<<(FormatOutputStream& stream, short int value) throw(IOException);
FormatOutputStream& operator<<(FormatOutputStream& stream, unsigned short int value) throw(IOException);
FormatOutputStream& operator<<(FormatOutputStream& stream, int value) throw(IOException);
FormatOutputStream& operator<<(FormatOutputStream& stream, unsigned int value) throw(IOException);
FormatOutputStream& operator<<(FormatOutputStream& stream, long value) throw(IOException);
FormatOutputStream& operator<<(FormatOutputStream& stream, unsigned long value) throw(IOException);
FormatOutputStream& operator<<(FormatOutputStream& stream, long long int value) throw(IOException);
FormatOutputStream& operator<<(FormatOutputStream& stream, unsigned long long int value) throw(IOException);
FormatOutputStream& operator<<(FormatOutputStream& stream, float value) throw(IOException);
FormatOutputStream& operator<<(FormatOutputStream& stream, double value) throw(IOException);
FormatOutputStream& operator<<(FormatOutputStream& stream, long double value) throw(IOException);



/** Sets the desired width of the field. */
inline FormatOutputStream::Manipulator setWidth(unsigned int width) throw() {
  return FormatOutputStream::Manipulator(&FormatOutputStream::setWidth, width);
}

/** Sets the desired number of digits after the decimal-point for real numbers. */
inline FormatOutputStream::Manipulator setPrecision(unsigned int precision) throw() {
  return FormatOutputStream::Manipulator(&FormatOutputStream::setPrecision, precision);
}

/** Sets the desired position of the radix. */
inline FormatOutputStream::Manipulator setRadixPosition(unsigned int position) throw() {
  return FormatOutputStream::Manipulator(&FormatOutputStream::setRadixPosition, position);
}

inline FormatOutputStream& operator<<(FormatOutputStream& stream, FormatOutputStream::Manipulator manipulator) {
  return manipulator(stream);
}

/** Restores the context of the stream. */
inline FormatOutputStream::SetContext setContext(const FormatOutputStream::Context& context) throw() {
  return FormatOutputStream::SetContext(context);
}

/** Stores the context of the stream. */
inline FormatOutputStream::GetContext getContext(FormatOutputStream::Context& context) throw() {
  return FormatOutputStream::GetContext(context);
}

inline FormatOutputStream& operator<<(FormatOutputStream& stream, FormatOutputStream::SetContext setContext) throw() {
  return setContext(stream);
}

inline FormatOutputStream& operator<<(FormatOutputStream& stream, FormatOutputStream::GetContext getContext) throw() {
  return getContext(stream);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
