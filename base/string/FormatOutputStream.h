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

#include <base/io/BufferedOutputStream.h>
#include <base/io/BindException.h>
#include <base/concurrency/SpinLock.h>
#include <base/mem/Reference.h>
#include <base/string/ASCIITraits.h>
#include <base/string/String.h>
#include <base/Primitives.h>
#include <base/OutOfDomain.h>
#include <base/OutOfRange.h>
#include <base/Literal.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Date;
class String;
class Locale;

/**
  Actions used to modify a format output stream. Use BIN, OCT, DEC, and HEX to
  select an appropriate integer base. Use ZEROPAD and NOZEROPAD to
  enable/disable zero padding. Use PREFIX and NOPREFIX to enable/disable
  prefixes for numbers. EOL writes a new line to the stream. FLUSH forces the
  internal buffers to be flushed. Use ENDL to both write a new line and flush
  the internal buffers.

  @relates FormatOutputStream
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
  PREFIX, /**< Enable prefixes for integers. This is the default if the prefix is defined for the field content. */
  NOPREFIX, /**< Disable prefixes for integers. */
  NECESSARY, /**< Specifies that no garbage digits should be output for floating-point numbers. This is the default behavior. */
  UPPER, /**< Selects capital letters (both integers and real numbers). */
  LOWER, /**< Selects lower case letters (both integers and real numbers). */
  POSIX, /**< Override current locale and use POSIX formatting. */
  LOCALE, /**< Use current locale during formatting. */
  LOCAL, /**< Specifies that the date is in local time. */
  UTC, /**< Specifies that the date is in Coordinated Universal Time (UTC). This is the default. */
  DATETIME, /**< Selects both date and time. This is the default. */
  TIME, /**< Selects time only. */
  DATE, /**< Selects date only. */
  SHORT_FORMAT, /**< Selects short format. */
  MEDIUM_FORMAT, /**< Selects medium format. */
  LONG_FORMAT, /**< Selects long format. */
  RFC2822_FORMAT, /**< Selects RFC2822-conformant format (e.g. Tue, 07 May 2002 12:35:54 GMT). */
  ISO8601_FORMAT, /**< Selects ISO 8601:2000-conformant format. */
  DEPENDENT, /**< Selects default justification of the field type. This is the default. */
  LEFT, /**< Selects left justification within field. */
  RIGHT, /**< Selects right justification within field. */
  SP, /**< Writes a whitespace (SP). */
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
  @version 1.4
*/

class _DK_SDU_MIP__BASE__API FormatOutputStream : public BufferedOutputStream {
public:
  
  /** The type of the guard. */
  typedef SpinLock Guard;
  
  class Symbols {
  public:
    
    enum Base {BINARY, OCTAL, DECIMAL, HEXADECIMAL};
    enum RealStyle {SCIENTIFIC, FIXED, ENGINEERING};
    enum EndOfLine {UNIXEOL, WINDOWSEOL, MACEOL};
    enum Justification {DEPENDENT, LEFT, RIGHT, RADIX};
    enum MajorDateFormat {DATETIME, TIME, DATE, EXPLICIT_DATE_FORMAT};
    
    enum NamedDateFormat {
      SHORT_FORMAT,
      MEDIUM_FORMAT,
      LONG_FORMAT,
      RFC2822_FORMAT,
      ISO8601_FORMAT
    };
    
    enum {
      ZEROPAD = 1,
      PREFIX = 2,
      GROUPING = 4,
      PLUS = 8,
      FPLUS = 16,
      PLUSEXP = 32,
      ZEROPADEXP = 64,
      NECESSARY = 128,
      UPPER = 256,
      POSIX = 512,
      LOCAL_TIME = 1024
    };
  };
  
  static const unsigned int DEFAULT_FLAGS =
    Symbols::PREFIX | Symbols::NECESSARY | Symbols::POSIX;
  static const Symbols::EndOfLine DEFAULT_EOL = Symbols::UNIXEOL;
  static const Symbols::Base DEFAULT_INTEGER_BASE = Symbols::DECIMAL;
  static const Symbols::Base DEFAULT_REAL_BASE = Symbols::DECIMAL;
  static const Symbols::RealStyle DEFAULT_REAL_STYLE = Symbols::FIXED;
  static const int DEFAULT_RADIX_POSITION = 0;
  static const Symbols::MajorDateFormat DEFAULT_MAJOR_DATE_FORMAT =
    Symbols::DATETIME;
  static const Symbols::NamedDateFormat DEFAULT_NAMED_DATE_FORMAT =
    Symbols::MEDIUM_FORMAT;
  static const Symbols::Justification DEFAULT_JUSTIFICATION =
    Symbols::DEPENDENT;
  static const int DEFAULT_WIDTH = 0;
  static const int DEFAULT_PRECISION = 6;
  
  /** @short Context of format output stream object. */
  class Context { // TAG: need to compress structure using bit-fields
  public:

    /** The format flags. */
    unsigned int flags = DEFAULT_FLAGS;
    /** The eol. */
    Symbols::EndOfLine endOfLine = DEFAULT_EOL;
    /** The integer base. */
    Symbols::Base integerBase = DEFAULT_INTEGER_BASE;
    /** The floating-point base. */
    Symbols::Base realBase = DEFAULT_REAL_BASE;
    /** The floating-point style. */
    Symbols::RealStyle realStyle = DEFAULT_REAL_STYLE;
    /** The desired radix position. */
    int radixPosition = DEFAULT_RADIX_POSITION;
    /** The major date/time format. */
    Symbols::MajorDateFormat majorDateFormat = DEFAULT_MAJOR_DATE_FORMAT;
    /** Selects named subformat. */
    Symbols::NamedDateFormat namedDateFormat = DEFAULT_NAMED_DATE_FORMAT;
    /** Justification within field. */
    Symbols::Justification justification = DEFAULT_JUSTIFICATION;
    /** Specifies the field width. */
    int width = DEFAULT_WIDTH;
    /** Specifies the number of digits to be written after the radix character. */
    int precision = DEFAULT_PRECISION;
    /** The date format. */
    String dateFormat;
  };
  
  class Manipulator {
  public:

    typedef FormatOutputStream& (FormatOutputStream::*Method)(unsigned int);
  private:
    
    Method method;
    const unsigned int value;
  public:
    
    inline Manipulator(Method _method, unsigned int _value) throw()
      : method(_method), value(_value) {
    }
    
    inline FormatOutputStream& operator()(
      FormatOutputStream& stream) {
      return (stream.*method)(value);
    }
  };
  
  class StringManipulator {
  public:

    typedef FormatOutputStream& (FormatOutputStream::*Method)(const String&);
  private:
    
    Method method;
    const String value;
  public:
    
    inline StringManipulator(Method _method, const String& _value) throw()
      : method(_method), value(_value) {
    }
    
    inline FormatOutputStream& operator()(FormatOutputStream& stream) {
      return (stream.*method)(value);
    }
  };

  class GetContext {
  private:

    Context& context;
  public:
    
    inline GetContext(Context& _context) throw() : context(_context) {
    }
    
    inline FormatOutputStream& operator()(FormatOutputStream& stream) throw() {
      return stream.getContext(context);
    }
  };
  
  class SetContext {
  private:

    const Context& context;
  public:

    inline SetContext(const Context& _context) throw() : context(_context) {
    }
    
    inline FormatOutputStream& operator()(FormatOutputStream& stream) throw() {
      return stream.setContext(context);
    }
  };
  
  /**
    The class is used to store and restore the default context of a format
    output stream using the stack.

    Example:
    @code
    class Composite : public Object {
    private:

      int x = 0;
      int y = 0;
    public:

      Composite(int _x, int _y) throw() : x(_x), y(_y) {
      }

      friend FormatOutputStream& operator<<(FormatOutputStream& stream, const Composite& value) throw(IOException);
    };
    
    FormatOutputStream& operator<<(FormatOutputStream& stream, const Composite& value) throw(IOException) {
      FormatOutputStream::PushContext pushContext(stream); // make current context the default context
      return stream << '{' << value.x << ',' << value.y << '}';
    }
    @endcode
    
    @short Push and pop default context of format output stream
    @version 1.0
  */
  
  class PushContext : public Object {
  private:

    /** Format output stream. */
    FormatOutputStream& stream; // TAG: better if FormatOutputStream was a ReferenceCountedObject
    /** Default context of format output stream. */
    Context context;
  public:

    /**
      Stores the current default context onto the and makes the current context
      the new default context of the stream object.
    */
    PushContext(FormatOutputStream& _stream) throw()
      : stream(_stream), context(_stream.defaultContext) {
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

  class Debug {
  private:
    
    static unsigned int counter; // TAG: need atomic access
    unsigned int count = 0;
    Literal location;
  public:
    
    inline Debug(const Literal& _location) throw()
      : count(counter++), location(_location) {
    }
    
    inline const Literal& getLocation() const throw() {
      return location;
    }
    
    inline unsigned int getCount() const throw() {
      return count;
    }
  };
protected:
  
  friend class PushContext;
  /** The initial context. */
  static const Context DEFAULT_CONTEXT;
  /** Guard. */
  Guard guard;
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
    Sets the date format.

    @param format The desired date format.
  */
  FormatOutputStream& setDateFormat(const String& format) throw();
  
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
  inline unsigned int getFlags() const throw() {
    return context.flags;
  }
  
  /**
    Returns the current integer base.
  */
  inline Symbols::Base getBase() const throw() {
    return context.integerBase;
  }
  
  /**
    Returns the current floating-point base.
  */
  inline Symbols::Base getRealBase() const throw() {
    return context.realBase;
  }
  
  /**
    Returns the current field width.
  */
  inline unsigned int getWidth() const throw() {
    return context.width;
  }
  
  /**
    Returns the current precision.
  */
  inline unsigned int getPrecision() const throw() {
    return context.precision;
  }
  
  /**
    Send action to stream.
  */
  FormatOutputStream& operator<<(Action action) throw(IOException);  

  class Indent {
  private:
    
    unsigned int length = 0;
  public:
    
    inline Indent(unsigned int _length) throw()
      : length(_length) {
    }
    
    inline unsigned int getIndent() const throw() {
      return length;
    }
  };
  
  /**
    Indents the stream with the specified number of spaces. This is usually
    used immediately after an EOL.
  */
  inline FormatOutputStream& operator<<(Indent indent) throw(IOException) {
    this->indent(indent.getIndent());
    return *this;
  }
  
  /**
    Writes the specified number of spaces to the stream. The current context is
    ignored and not reset by this method.
  */
  void indent(unsigned int size) throw(IOException);
  
  /**
    Writes the specifies number of characters to the stream.
  */
  void addCharacterField(
    const char* buffer, unsigned int size) throw(IOException);

  /**
    Writes a preformated integer to the stream.
  */
  void addIntegerField(
    const char* buffer, unsigned int size, bool isSigned) throw(IOException);
  
  /**
    Writes a date object to the stream.
  */
  void addDateField(const Date& date) throw(IOException);

  /**
    Writes a preformated floating point value to stream.
  */
  void writeFloatingPointType(
    unsigned int significant,
    unsigned int* mantissa,
    unsigned int mantissaSize,
    int base2Exponent,
    unsigned int valueFlags) throw(IOException);
  
  /**
    Gets the context.
  */
  FormatOutputStream& getContext(Context& context) throw();
  
  /**
    Sets the context.
  */
  FormatOutputStream& setContext(const Context& context) throw();

  /**
    Writes the debug information to the stream.
  */
  FormatOutputStream& operator<<(const Debug& debug) throw(IOException);

  /**
    Writes the given string literal to the stream.
    
    @param literal String literal.
  */
  template<MemorySize SIZE>
  inline FormatOutputStream& operator<<(
    const char (&literal)[SIZE]) throw(IOException) {
    if (Constraint<(SIZE > 0)>::UNSPECIFIED) {}
    addCharacterField(literal, SIZE - 1);
    return *this;
  }
  
  FormatOutputStream& operator<<(bool value) throw(IOException);
  
  inline FormatOutputStream& operator<<(char value) throw(IOException) {
    addCharacterField(&value, 1);
    return *this;
  }
  
  FormatOutputStream& operator<<(short int value) throw(IOException);
  FormatOutputStream& operator<<(unsigned short int value) throw(IOException);
  FormatOutputStream& operator<<(int value) throw(IOException);
  FormatOutputStream& operator<<(unsigned int value) throw(IOException);
  FormatOutputStream& operator<<(long value) throw(IOException);
  FormatOutputStream& operator<<(unsigned long value) throw(IOException);
  FormatOutputStream& operator<<(long long value) throw(IOException);
  FormatOutputStream& operator<<(unsigned long long value) throw(IOException);
  FormatOutputStream& operator<<(float value) throw(IOException);
  FormatOutputStream& operator<<(double value) throw(IOException);
  FormatOutputStream& operator<<(long double value) throw(IOException);
  
  inline FormatOutputStream& operator<<(
    const NativeString& value) throw(IOException) {
    return *this << String(value);
  }
  
  /**
    Writes a pointer to a format output stream.
  */
  FormatOutputStream& operator<<(const void* value) throw(IOException);

  /**
    Writes a reference to a format output stream.
  */
  template<class TYPE>
  inline FormatOutputStream& operator<<(
    Reference<TYPE> value) throw(IOException) {
    return *this << value.getValue();
  }

  /**
    Writes a string literal to a format output stream.
  */
  inline FormatOutputStream& operator<<(
    const Literal& literal) throw(IOException) {
    addCharacterField(literal.getValue(), static_cast<unsigned int>(literal.getLength()));
    return *this;
  }
  
  /**
    Writes a nice description of the exception to the format output stream.
  */
  FormatOutputStream& operator<<(const Exception& e) throw(IOException);
  
  /**
    Destroy format output stream.
  */
  ~FormatOutputStream();
};

/**
  Format output stream linked to the standard output stream. This variable
  corresponds to 'cout' from the Standard Template Library.
*/
extern _DK_SDU_MIP__BASE__API FormatOutputStream fout;

/**
  Format output stream linked to the standard error stream. This variable
  corresponds to 'cerr' from the Standard Template Library.
*/
extern _DK_SDU_MIP__BASE__API FormatOutputStream ferr;

/**
  Indent.
*/
inline FormatOutputStream::Indent indent(unsigned int length) throw() {
  return FormatOutputStream::Indent(length);
}



/**
  This class describes a seqence of elements to be written to a format output
  stream.
  
  @code
  class MyClass : public Object {
  private:
  
    char guid[16];
  public:
  
    void myMethod() throw() {
      if (debugLevel >= VERY_VERBOSE) {
        fout << HEX << ZEROPAD << NOPREFIX << Sequence<unsigned char>(guid, sizeof(guid), ":") << ENDL;
      }
      // do something
    }
  };
  @endcode

  @short Sequence descriptor
  @version 1.0
*/
template<class TYPE>
class Sequence {
private:

  const TYPE* value;
  const unsigned int size;
  const Literal separator;
public:

  /**
    Initializes sequence descriptor with no separator.

    @param value The beginning of the sequence.
    @param size The number of elements in the sequence.
  */
  Sequence(const TYPE* _value, unsigned int _size) throw()
    : value(_value), size(_size), separator("") {
  }
  
  /**
    Initializes sequence descriptor.

    @param value The beginning of the sequence.
    @param size The number of elements in the sequence.
    @param separator The separator to be written between elements.
  */
  Sequence(
    const TYPE* _value,
    unsigned int _size,
    const Literal& _separator) throw()
    : value(_value), size(_size), separator(_separator) {
  }
  
  /** Returns the beginning of the sequence. */
  inline const TYPE* getValue() const throw() {
    return value;
  }

  /** Returns the number of elements in the sequence. */
  inline unsigned int getSize() const throw() {
    return size;
  }

  /** Returns the separator to be used between elements. */
  inline const Literal& getSeparator() const throw() {
    return separator;
  }
};

/**
  Writes a sequence to the format output stream.
*/
template<class TYPE>
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Sequence<TYPE>& value) throw(IOException) {
  FormatOutputStream::PushContext push(stream);
  const TYPE* src = value.getValue();
  const TYPE* end = src + value.getSize();
  const Literal& separator = value.getSeparator();
  if (separator.getLength() > 0) {
    --end;
    while (src < end) {
      stream << *src << separator;
      ++src;
    }
    if (src == end) { // if not empty
      stream << *src;
    }
  } else {
    while (src < end) {
      stream << *src;
      ++src;
    }
  }
  return stream;
}



/** Sets the desired width of the field. */
inline FormatOutputStream::Manipulator setWidth(unsigned int width) throw() {
  return FormatOutputStream::Manipulator(&FormatOutputStream::setWidth, width);
}

/**
  Sets the desired number of digits after the decimal-point for real numbers.
*/
inline FormatOutputStream::Manipulator setPrecision(
  unsigned int precision) throw() {
  return FormatOutputStream::Manipulator(
    &FormatOutputStream::setPrecision,
    precision
  );
}

/**
  Sets the desired position of the radix (and sets the justification to
  Symbols::RADIX).
*/
inline FormatOutputStream::Manipulator setRadixPosition(
  unsigned int position) throw() {
  return FormatOutputStream::Manipulator(
    &FormatOutputStream::setRadixPosition,
    position
  );
}

/** Sets the desired date format. */
inline FormatOutputStream::StringManipulator setDateFormat(
  const String& format) throw() {
  return FormatOutputStream::StringManipulator(
    &FormatOutputStream::setDateFormat,
    format
  );
}



inline FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  FormatOutputStream::Manipulator manipulator) throw(IOException) {
  return manipulator(stream);
}

inline FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  FormatOutputStream::StringManipulator manipulator) throw(IOException) {
  return manipulator(stream);
}

/** Restores the context of the stream. */
inline FormatOutputStream::SetContext setContext(
  const FormatOutputStream::Context& context) throw() {
  return FormatOutputStream::SetContext(context);
}

/** Stores the context of the stream. */
inline FormatOutputStream::GetContext getContext(
  FormatOutputStream::Context& context) throw() {
  return FormatOutputStream::GetContext(context);
}

inline FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  FormatOutputStream::SetContext setContext) throw() {
  return setContext(stream);
}

inline FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  FormatOutputStream::GetContext getContext) throw() {
  return getContext(stream);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
