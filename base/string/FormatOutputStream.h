/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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
#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class binds together a string literal and its length. Use the macro
  MESSAGE to generate an object of this class for a given string literal (e.g.
  MESSAGE("Hello World")). Do not call the constructor directly.

  @short String literal
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/

class StringLiteral {
private:

  /** The number of characters occupied by the message without the terminator. */
  const unsigned int length;
  /** NULL-terminated message. */
  const char* message;
public:

  /** Initializes message. Automatically invocated by the macro MESSAGE. */
  inline StringLiteral(unsigned int _length, const char* _message) throw() : length(_length), message(_message) {}
  /** Cast to the usual message type. */
  inline operator const char*() const throw() {return message;}
  /** Returns the length of the string literal. */
  inline unsigned int getLength() const throw() {return length;}
};

/** This macro returns a StringLiteral object from a string literal (e.g. MESSAGE("Hello, World")). */
#define MESSAGE(msg) StringLiteral(sizeof(msg) - 1, msg) // TAG: replace with symbol LITERAL



class Locale; // forward declaration

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
  DEPENDENT, /**< Selects default justification of the field type. This is the default. */
  LEFT, /**< Selects left justification within field. */
  RIGHT, /**< Selects right justification within field. */
  UNIXEOL, /**< Selects line-feed (LF) as EOL. This is the default for all platforms. */
  WINDOWSEOL, /**< Selects carriage-return followed by line-feed (LF) as EOL. */
  MACEOL, /**< Selects line-feed (LF) followed by carriage-return (CR) as EOL. */
  CR, /**< Writes a carriage-return (CR). */
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
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.3.1
*/

class FormatOutputStream : public BufferedOutputStream, public Synchronizeable<SpinLock> {
public:

  class Symbols {
  public:
    enum Base {BINARY, OCTAL, DECIMAL, HEXADECIMAL};
    enum RealStyle {SCIENTIFIC, FIXED, ENGINEERING};
    enum EndOfLine {UNIXEOL, WINDOWSEOL, MACEOL};
    enum Justification {DEPENDENT, LEFT, RIGHT, RADIX};
    enum {ZEROPAD = 1, PREFIX = 2, GROUPING = 4, PLUS = 8, FPLUS = 16, PLUSEXP = 32, ZEROPADEXP = 64, NECESSARY = 128, UPPER = 256, POSIX = 512};
  };
  
  static const unsigned int DEFAULT_FLAGS = Symbols::PREFIX | Symbols::NECESSARY | Symbols::POSIX;
  static const Symbols::EndOfLine DEFAULT_EOL = Symbols::UNIXEOL;
  static const Symbols::Base DEFAULT_INTEGER_BASE = Symbols::DECIMAL;
  static const Symbols::Base DEFAULT_REAL_BASE = Symbols::DECIMAL;
  static const Symbols::RealStyle DEFAULT_REAL_STYLE = Symbols::FIXED;
  static const int DEFAULT_RADIX_POSITION = 0;
  static const Symbols::Justification DEFAULT_JUSTIFICATION = Symbols::DEPENDENT;
  static const int DEFAULT_WIDTH = 0;
  static const int DEFAULT_PRECISION = 6;
  
  /** Context of format output stream object. */
  struct Context {
    /** The format flags. */
    unsigned int flags;
    /** The eol. */
    Symbols::EndOfLine endOfLine;
    /** The integer base. */
    Symbols::Base integerBase;
    /** The floating-point base. */
    Symbols::Base realBase;
    /** The floating-point style. */
    Symbols::RealStyle realStyle;
    /** The desired radix position. */
    int radixPosition;
    /** Justification within field. */
    Symbols::Justification justification;
    /** Specifies the field width. */
    int width;
    /** Specifies the number of digits to be written after the radix character. */
    int precision;
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
  
  /**
    The class is used to store and restore the default context of a format
    output stream using the stack.

    Example:
    <pre>
    class Composite : public Object {
    private:

      int x;
      int y;
    public:

      Composite(int _x, int _y) throw() : x(_x), y(_y) {
      }

      friend FormatOutputStream& operator<<(FormatOutputStream& stream, const Composite& value) throw(IOException);
    };
    
    FormatOutputStream& operator<<(FormatOutputStream& stream, const Composite& value) throw(IOException) {
      FormatOutputStream::PushContext pushContext(stream); // make current context the default context
      return stream << '{' << value.x << ',' << value.y << '}';
    }
    </pre>
    
    @short Push and pop default context of format output stream
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  
  class PushContext : public Object {
  private:

    /** Default context of format output stream. */
    Context context;
    /** Format output stream. */
    FormatOutputStream& stream; // TAG: better if FormatOutputStream was a ReferenceCountedObject
  public:

    /**
      Stores the current default context onto the and makes the current context the new default context of the
      stream object.
    */
    PushContext(FormatOutputStream& _stream) throw() : stream(_stream), context(_stream.defaultContext) {
      _stream.defaultContext = _stream.context;
    }
    
    /**
      Restores the default context of the format output stream. The current
      context of the stream object is reset to the default context.
    */
    ~PushContext() throw() {
      stream.defaultContext = context;
      stream.context = context;
    }
  };
protected:
  
  friend class PushContext;
  /** The initial context. */
  static const Context DEFAULT_CONTEXT;
  /** The current default context. */
  Context defaultContext;
  /** The current context. */
  Context context;
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
  
  FormatOutputStream(const FormatOutputStream& copy) throw();
  
  /**
    Sets the desired position of the decimal-point within the field. Please
    note that the position is only advisory. Please not that this methods sets
    the justification state to Symbols::RADIX as a side effect.

    @param position The desired position.
  */
  FormatOutputStream& setRadixPosition(unsigned int position) throw();
  
  /**
    Sets the value justification within the field.

    @param justification The desired justification.
  */
  FormatOutputStream& setJustification(Symbols::Justification justification) throw();
  
  /**
    Sets the current field width. The width silently reduced to MAXIMUM_WIDTH.
    
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
    Sets the locale of the stream. The locale is global for the stream. The
    stream uses POSIX formatting until the locale is set explicitly using this
    method. The action POSIX can be used to override the locale (see ISO/IEC
    WG14 N884, NCITS J11/99-019).

    @param locale The desired locale.
  */
  void setLocale(const Locale& locale) throw();
  
  /**
    Returns the flags for the next field.
  */
  inline unsigned int getFlags() const throw() {return context.flags;}
  
  /**
    Returns the current integer base.
  */
  inline Symbols::Base getBase() const throw() {return context.integerBase;}
  
  /**
    Returns the current floating-point base.
  */
  inline Symbols::Base getRealBase() const throw() {return context.realBase;}
  
  /**
    Returns the current field width.
  */
  inline unsigned int getWidth() const throw() {return context.width;}
  
  /**
    Returns the current precision.
  */
  inline unsigned int getPrecision() const throw() {return context.precision;}
  
  /**
    Send action to stream.
  */
  FormatOutputStream& operator<<(Action action) throw(IOException);  
  
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

/**
  Writes a NULL-terminated string literal to a format output stream (you are advised against using this function).
*/
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

/**
  Writes a pointer to a format output stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const void* value) throw(IOException);

/**
  Writes a string literal to a format output stream.
*/
inline FormatOutputStream& operator<<(FormatOutputStream& stream, const StringLiteral& value) throw(IOException) {
  stream.addCharacterField(value, value.getLength());
  return stream;
}



/** Sets the desired width of the field. */
inline FormatOutputStream::Manipulator setWidth(unsigned int width) throw() {
  return FormatOutputStream::Manipulator(&FormatOutputStream::setWidth, width);
}

/** Sets the desired number of digits after the decimal-point for real numbers. */
inline FormatOutputStream::Manipulator setPrecision(unsigned int precision) throw() {
  return FormatOutputStream::Manipulator(&FormatOutputStream::setPrecision, precision);
}

/** Sets the desired position of the radix (and sets the justification to Symbols::RADIX). */
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
