/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/io/FileDescriptorOutputStream.h>
#include <base/concurrency/Thread.h>
#include <base/FloatingPoint.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/string/Locale.h>
#include <base/Date.h>
#include <base/string/StringOutputStream.h>
#include <base/concurrency/AtomicCounter.h>
#include <base/TypeInfo.h>
#include <base/UnitTest.h>
#include <base/mathematics/Constants.h>
#include <base/mathematics/LargeInteger.h>
#include <base/UInt128.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace {

  PreferredAtomicCounter sourceCodeLocationCounter;
}

unsigned int FormatOutputStream::SourceCodeLocation::allocateCount() noexcept
{
  return ++sourceCodeLocationCounter;
}

FormatOutputStream& FormatOutputStream::operator<<(const FormatOutputStream::SourceCodeLocation& location)
{
  return *this << location.getLocation() << ' ' << '[' << DEC << location.getCount() << ']';
}

FormatOutputStream::FormatOutputStream(OutputStream& out, unsigned int size) throw(BindException)
  : BufferedOutputStream(out, size),
    defaultContext(DEFAULT_CONTEXT),
    context(DEFAULT_CONTEXT)
{
}

FormatOutputStream& FormatOutputStream::setRadixPosition(unsigned int position) throw()
{
  ExclusiveSynchronize<Guard> _guard(guard);
  context.justification = Symbols::RADIX;
  context.radixPosition = position;
  return *this;
}

FormatOutputStream& FormatOutputStream::setJustification(Symbols::Justification justification) throw()
{
  ExclusiveSynchronize<Guard> _guard(guard);
  context.justification = justification;
  return *this;
}

FormatOutputStream& FormatOutputStream::setWidth(unsigned int width) throw()
{
  ExclusiveSynchronize<Guard> _guard(guard);
  context.width = minimum(width, MAXIMUM_WIDTH);
  return *this;
}

FormatOutputStream& FormatOutputStream::setPrecision(unsigned int precision) throw()
{
  ExclusiveSynchronize<Guard> _guard(guard);
  context.flags &= ~Symbols::NECESSARY;
  context.precision = minimum(precision, MAXIMUM_PRECISION);
  return *this;
}

FormatOutputStream& FormatOutputStream::setDateFormat(const String& format)
{
  ExclusiveSynchronize<Guard> _guard(guard);
  context.majorDateFormat = Symbols::EXPLICIT_DATE_FORMAT;
  // namedDateFormat is ignored (but unchanged)
  context.dateFormat = format;
  return *this;
}

// FYI - destruct order is ok to push witin a list of pipes:
// e.g. fout << Push(justification) << 123 << Push(HEX) << 456 << Push(width) << 123 << ENDL;
// e.g. fout << Push(HEX) << 123 << Pop(HEX) << 456 << ENDL; // Pop here would restore - but this needs to know last item on stack - that could be stored via FormatOutputStream::Push* topPush; - push registers in constructor - Pop is less useful though - Pop would throw if stack is empty - and Pop would tell Push to do a NOOP in its destructor

// if Context was reference we could swap it fast
FormatOutputStream::PushContext::PushContext(FormatOutputStream& _stream) noexcept
  : stream(_stream),
    context(_stream.defaultContext)
{
  _stream.defaultContext = _stream.context;
}

FormatOutputStream::PushContext::~PushContext() noexcept
{
  stream.defaultContext = context;
  stream.context = context;
}

FormatOutputStream& FormatOutputStream::operator<<(Action action) throw(IOException)
{
  static const Literal SP_STR = " ";
  static const Literal CR_STR = "\r";
  static const Literal LF_STR = "\n";
  static const Literal LFCR_STR = "\n\r";
  static const Literal CRLF_STR = "\r\n";

  ExclusiveSynchronize<Guard> _guard(guard);
  switch (action) {
  case BIN:
    context.integerBase = Symbols::BINARY;
    break;
  case OCT:
    context.integerBase = Symbols::OCTAL;
    break;
  case DEC:
    context.integerBase = Symbols::DECIMAL;
    break;
  case HEX:
    context.integerBase = Symbols::HEXADECIMAL;
    break;
  case FBIN:
    context.realBase = Symbols::BINARY;
    break;
  case FOCT:
    context.realBase = Symbols::OCTAL;
    break;
  case FDEC:
    context.realBase = Symbols::DECIMAL;
    break;
  case FHEX:
    context.realBase = Symbols::HEXADECIMAL;
    break;
  case SCIENTIFIC:
    context.realStyle = Symbols::SCIENTIFIC;
    break;
  case FIXED:
    context.realStyle = Symbols::FIXED;
    break;
  case ENGINEERING:
    context.realStyle = Symbols::ENGINEERING;
    break;
  case ZEROPAD:
    context.flags |= Symbols::ZEROPAD;
    break;
  case NOZEROPAD:
    context.flags &= ~Symbols::ZEROPAD;
    break;
  case PREFIX:
    context.flags |= Symbols::PREFIX;
    break;
  case NOPREFIX:
    context.flags &= ~Symbols::PREFIX;
    break;
  case GROUPING:
    context.flags |= Symbols::GROUPING;
    break;
  case NOGROUPING:
    context.flags &= ~Symbols::GROUPING;
    break;
  case PLUS:
    context.flags |= Symbols::PLUS;
    break;
  case FPLUS:
    context.flags |= Symbols::FPLUS;
    break;
  case PLUSEXP:
    context.flags |= Symbols::PLUSEXP;
    break;
  case ZEROPADEXP:
    context.flags |= Symbols::ZEROPADEXP;
    break;
  case NECESSARY:
    context.flags |= Symbols::NECESSARY;
    break;
  case UPPER:
    context.flags |= Symbols::UPPER;
    break;
  case LOWER:
    context.flags &= ~Symbols::UPPER;
    break;
  case POSIX:
    context.flags |= Symbols::POSIX;
    break;
  case LOCALE:
    context.flags &= ~Symbols::POSIX;
    break;
  case LOCAL:
    context.flags |= Symbols::LOCAL_TIME;
    break;
  case UTC:
    context.flags &= ~Symbols::LOCAL_TIME;
    break;
  case DATETIME:
    context.majorDateFormat = Symbols::DATETIME;
    break;
  case TIME:
    context.majorDateFormat = Symbols::TIME;
    break;
  case DATE:
    context.majorDateFormat = Symbols::DATE;
    break;
  case SHORT_FORMAT:
    context.namedDateFormat = Symbols::SHORT_FORMAT;
    break;
  case MEDIUM_FORMAT:
    context.namedDateFormat = Symbols::MEDIUM_FORMAT;
    break;
  case LONG_FORMAT:
    context.namedDateFormat = Symbols::LONG_FORMAT;
    break;
  case RFC2822_FORMAT:
    context.namedDateFormat = Symbols::RFC2822_FORMAT;
    break;
  case ISO8601_FORMAT:
    context.namedDateFormat = Symbols::ISO8601_FORMAT;
    break;
  case DEPENDENT:
    context.justification = Symbols::DEPENDENT;
    break;
  case LEFT:
    context.justification = Symbols::LEFT;
    break;
  case RIGHT:
    context.justification = Symbols::RIGHT;
    break;
  case SP:
    write(
      Cast::pointer<const uint8*>(SP_STR.getValue()),
      SP_STR.getLength()
    ); // may throw IOException
    break;
  case UNIXEOL:
    context.endOfLine = Symbols::UNIXEOL;
    break;
  case WINDOWSEOL:
    context.endOfLine = Symbols::WINDOWSEOL;
    break;
  case MACEOL:
    context.endOfLine = Symbols::MACEOL;
    break;
  case CR:
    write(
      Cast::pointer<const uint8*>(CR_STR.getValue()),
      CR_STR.getLength()
    ); // may throw IOException
    break;
  case LF:
    write(
      Cast::pointer<const uint8*>(LF_STR.getValue()),
      LF_STR.getLength()
    ); // may throw IOException
    break;
  case CRLF:
    write(
      Cast::pointer<const uint8*>(CRLF_STR.getValue()),
      CRLF_STR.getLength()
    ); // may throw IOException
    break;
  case LFCR:
    write(
      Cast::pointer<const uint8*>(LFCR_STR.getValue()),
      LFCR_STR.getLength()
    ); // may throw IOException
    break;
  case EOL:
    switch (context.endOfLine) {
    case Symbols::UNIXEOL:
      write(
        Cast::pointer<const uint8*>(LF_STR.getValue()),
        LF_STR.getLength()
      ); // may throw IOException
      break;
    case Symbols::WINDOWSEOL:
      write(
        Cast::pointer<const uint8*>(CRLF_STR.getValue()),
        CRLF_STR.getLength()
      ); // may throw IOException
      break;
    case Symbols::MACEOL:
      write(
        Cast::pointer<const uint8*>(LFCR_STR.getValue()),
        LFCR_STR.getLength()
      ); // may throw IOException
      break;
    }
    break;
  case FLUSH:
    flush(); // may throw IOException
    break;
  case ENDL:
    switch (context.endOfLine) {
    case Symbols::UNIXEOL:
      write(
        Cast::pointer<const uint8*>(LF_STR.getValue()),
        LF_STR.getLength()
      ); // may throw IOException
      break;
    case Symbols::WINDOWSEOL:
      write(
        Cast::pointer<const uint8*>(CRLF_STR.getValue()),
        CRLF_STR.getLength()
      ); // may throw IOException
      break;
    case Symbols::MACEOL:
      write(
        Cast::pointer<const uint8*>(LFCR_STR.getValue()),
        LFCR_STR.getLength()
      ); // may throw IOException
      break;
    }
    flush(); // may throw IOException
    break;
  case PUSH:
    // push to context stack
    break;
  case POP:
    // pop to context stack
    break;
  }
  return *this;
}

FormatOutputStream& FormatOutputStream::getContext(Context& context) throw()
{
  return *this;
}

FormatOutputStream& FormatOutputStream::setContext(const Context& context) throw()
{
  return *this;
}

void FormatOutputStream::indent(unsigned int size, bool useTab) throw(IOException)
{
  static const Literal TAB_INDENT =
    "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
  static const Literal INDENT =
    "                                        "
    "                                        ";
  ExclusiveSynchronize<Guard> _guard(guard);
  
  if (useTab) {
    if (size <= TAB_INDENT.getLength()) {
      write(
        Cast::pointer<const uint8*>(TAB_INDENT.getValue()),
        size
      ); // write characters
    } else {
      unfoldValue('\t', size);
    }
  } else {
    if (size <= INDENT.getLength()) {
      write(
        Cast::pointer<const uint8*>(INDENT.getValue()),
        size
      ); // write characters
    } else {
      unfoldValue(' ', size);
    }
  }
  // context is not reset to default
}

void FormatOutputStream::addCharacterField(const char* buffer, MemorySize size) throw(IOException)
{
  ExclusiveSynchronize<Guard> _guard(guard);

  Symbols::Justification justification;
  switch (context.justification) {
  case Symbols::RIGHT:
    justification = Symbols::RIGHT;
    break;
  case Symbols::LEFT:
    justification = Symbols::LEFT;
    break;
  case Symbols::RADIX: // no radix - use default
  case Symbols::DEPENDENT:
  default:
    justification = Symbols::LEFT; // TAG: is this locale specific
  }

  if (justification == Symbols::LEFT) {
    write(Cast::pointer<const uint8*>(buffer), size); // write characters
  }
  if (size < static_cast<unsigned int>(context.width)) { // write blanks if required
    unfoldValue(' ', context.width - size);
  }
  if (context.justification == Symbols::RIGHT) {
    write(Cast::pointer<const uint8*>(buffer), size); // write characters
  }
  context = defaultContext;
}

void FormatOutputStream::addCharacterField(const wchar* buffer, MemorySize size) throw(IOException)
{
  ExclusiveSynchronize<Guard> _guard(guard);

  Symbols::Justification justification;
  switch (context.justification) {
  case Symbols::RIGHT:
    justification = Symbols::RIGHT;
    break;
  case Symbols::LEFT:
    justification = Symbols::LEFT;
    break;
  case Symbols::RADIX: // no radix - use default
  case Symbols::DEPENDENT:
  default:
    justification = Symbols::LEFT; // TAG: is this locale specific
  }

  const std::string text = toUTF8(buffer, size);

  if (justification == Symbols::LEFT) {
    write(Cast::pointer<const uint8*>(text.c_str()), text.size()); // write characters
  }
  // what if multi-wchar character - we ignore for now
  if (size < static_cast<unsigned int>(context.width)) { // write blanks if required
    unfoldValue(' ', context.width - size); // we use number of wchars
  }
  if (context.justification == Symbols::RIGHT) {
    write(Cast::pointer<const uint8*>(text.c_str()), text.size()); // write characters
  }
  context = defaultContext;
}

void FormatOutputStream::addIntegerField(const char* buffer, unsigned int size, bool isSigned) throw(IOException)
{
  ExclusiveSynchronize<Guard> _guard(guard);
  unsigned int requiredWidth = size;

  if (isSigned &&
      ((context.integerBase != Symbols::BINARY) &&
       (context.integerBase != Symbols::HEXADECIMAL))) {
    ++requiredWidth; // "-"
  }

  if ((context.flags & Symbols::PREFIX) != 0) { // is prefix requested
    switch (context.integerBase) {
    case Symbols::BINARY:
      requiredWidth += 2;
      break;
    case Symbols::OCTAL:
      requiredWidth += 1;
      break;
    case Symbols::DECIMAL:
      requiredWidth += 0;
      break;
    case Symbols::HEXADECIMAL:
      requiredWidth += 2;
      break;
    }
  }

  Symbols::Justification justification;
  switch (context.justification) {
  case Symbols::LEFT:
    justification = Symbols::LEFT;
    break;
  case Symbols::RIGHT:
    justification = Symbols::RIGHT;
    break;
  case Symbols::RADIX: // no radix - use default
  case Symbols::DEPENDENT:
  default:
    justification = Symbols::RIGHT; // TAG: is this locale specific
  }

  unsigned int pads =
    (requiredWidth >= static_cast<unsigned int>(context.width)) ? 0 : (context.width - requiredWidth);

  if (justification == Symbols::RIGHT) {
    if ((pads > 0) && ((context.flags & Symbols::ZEROPAD) == 0)) { // write blanks if required
      unfoldValue(' ', pads);
    }
  }

  if (isSigned &&
      ((context.integerBase != Symbols::BINARY) &&
       (context.integerBase != Symbols::HEXADECIMAL))) {
    write(Cast::pointer<const uint8*>("-"), 1); // write sign
  }

  if ((context.flags & Symbols::PREFIX) != 0) { // is prefix requested
    switch (context.integerBase) {
    case Symbols::BINARY:
      write(Cast::pointer<const uint8*>("0"), 1);
      write(
        Cast::pointer<const uint8*>(
          ((context.flags & FormatOutputStream::Symbols::UPPER) == 0) ? "b" : "B"
        ),
        1
      );
      break;
    case Symbols::OCTAL:
      write(Cast::pointer<const uint8*>("0"), 1);
      break;
    case Symbols::DECIMAL:
      break;
    case Symbols::HEXADECIMAL:
      write(Cast::pointer<const uint8*>("0"), 1);
      write(
        Cast::pointer<const uint8*>(
          ((context.flags & FormatOutputStream::Symbols::UPPER) == 0) ? "x" : "X"
        ),
        1
      );
      break;
    }
  }

  if ((pads > 0) && ((context.flags & Symbols::ZEROPAD) != 0)) { // do we need to extend field with digits
    unsigned int value = 0;
    if (isSigned) {
      switch (context.integerBase) {
      case Symbols::BINARY:
        value = 0x01;
        break;
      case Symbols::OCTAL:
        value = 0x00;
        break;
      case Symbols::DECIMAL:
        value = 0x00;
        break;
      case Symbols::HEXADECIMAL:
        value = 0x0f;
        break;
      }
    }
    char digit = ASCIITraits::valueToDigit(value, (context.flags & FormatOutputStream::Symbols::UPPER) != 0);
    unfoldValue(digit, pads);
  }

  write(Cast::pointer<const uint8*>(buffer), size); // write late buffer
  if (justification == Symbols::LEFT) {
    unfoldValue(' ', pads);
  }
  context = defaultContext;
}

void FormatOutputStream::addDateField(const Date& date) throw(IOException)
{
  ExclusiveSynchronize<Guard> _guard(guard);
  const bool localTime = ((context.flags & Symbols::LOCAL_TIME) != 0);
  const bool posix = ((context.flags & Symbols::POSIX) != 0);
//  const Locale* locale = posix ? &Locale::POSIX : &locale; // FIXME
  const Locale* locale = &Locale::POSIX;
  String format;
  switch (context.majorDateFormat) {
  case Symbols::DATETIME:
    switch (context.namedDateFormat) {
    case Symbols::SHORT_FORMAT:
      format = locale->getShortDateFormat() + MESSAGE(" ") + locale->getShortTimeFormat(); // TAG: optimize
      break;
    case Symbols::MEDIUM_FORMAT:
      format = locale->getMediumDateFormat() + MESSAGE(" ") + locale->getMediumTimeFormat(); // TAG: optimize
      break;
    case Symbols::LONG_FORMAT:
      format = locale->getLongDateFormat() + MESSAGE(" ") + locale->getLongTimeFormat(); // TAG: optimize
      break;
    case Symbols::RFC2822_FORMAT:
      format = "RFC2822";
      break;
    case Symbols::ISO8601_FORMAT:
      if (localTime) {
        format = "%Y-%m-%dT%H:%M:%S";
      } else {
        format = "%Y-%m-%dT%H:%M:%SZ";
      }
      break;
    }
    break;
  case Symbols::TIME:
    switch (context.namedDateFormat) {
    case Symbols::SHORT_FORMAT:
      format = locale->getShortTimeFormat();
      break;
    case Symbols::MEDIUM_FORMAT:
      format = locale->getMediumTimeFormat();
      break;
    case Symbols::LONG_FORMAT:
      format = locale->getLongTimeFormat();
      break;
    case Symbols::RFC2822_FORMAT:
      format = "RFC2822";
      break;
    case Symbols::ISO8601_FORMAT:
      if (localTime) {
        format = "%H:%M:%S";
      } else {
        format = "%H:%M:%SZ";
      }
      break;
    }
    break;
  case Symbols::DATE:
    switch (context.namedDateFormat) {
    case Symbols::SHORT_FORMAT:
      format = locale->getShortDateFormat();
      break;
    case Symbols::MEDIUM_FORMAT:
      format = locale->getMediumDateFormat();
      break;
    case Symbols::LONG_FORMAT:
      format = locale->getLongDateFormat();
      break;
    case Symbols::RFC2822_FORMAT:
      format = "<RFC2822>";
      break;
    case Symbols::ISO8601_FORMAT:
      format = "%y-%m-%d";
      break;
    }
    break;
  case Symbols::EXPLICIT_DATE_FORMAT:
    format = context.dateFormat;
    break;
  }
  String field = date.format(format, localTime);
  
  Symbols::Justification justification;
  switch (context.justification) {
  case Symbols::RIGHT:
    justification = Symbols::RIGHT;
    break;
  case Symbols::LEFT:
    justification = Symbols::LEFT;
    break;
  case Symbols::RADIX: // no radix - use default
  case Symbols::DEPENDENT:
  default:
    justification = Symbols::LEFT; // TAG: is this locale specific
  }

  const int size = field.getLength();
  if (justification == Symbols::LEFT) {
    write(Cast::pointer<const uint8*>(field.getBytes()), size); // write characters
  }
  if (size < context.width) { // write blanks if required
    unfoldValue(' ', context.width - size);
  }
  if (context.justification == Symbols::RIGHT) {
    write(Cast::pointer<const uint8*>(field.getBytes()), size); // write characters
  }
  context = defaultContext;  
}

FormatOutputStream::~FormatOutputStream() {
}

FormatOutputStream& FormatOutputStream::operator<<(bool value) throw(IOException)
{
  // TAG: need locale support
  return *this << (value ? Literal("true") : Literal("false"));
}

FormatOutputStream& FormatOutputStream::operator<<(short value) throw(IOException)
{
  char buffer[sizeof(short) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      unsigned short temp = Cast::impersonate<unsigned short>(value); // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x00000001); // get digit
        temp >>= 1; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      unsigned short temp = Cast::impersonate<unsigned short>(value); // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x00000007); // get digit
        temp >>= 3; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      unsigned short temp = (value >= 0) ? value : -value;
      do {
        *dest = ASCIITraits::valueToDigit(temp % 10); // get digit
        temp = temp/10;
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      unsigned short temp = Cast::impersonate<unsigned short>(value); // no sign
      do {
        *dest = ASCIITraits::valueToDigit(
          temp & 0x0000000f,
          (getFlags() & FormatOutputStream::Symbols::UPPER) != 0
        ); // get bits of digit
        temp >>= 4; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return *this; // do not do anything if base is unknown
  }

  addIntegerField(dest, sizeof(buffer) - (dest - buffer), value < 0);
  return *this;
}

FormatOutputStream& FormatOutputStream::operator<<(unsigned short value) throw(IOException)
{
  char buffer[sizeof(unsigned short) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000001); // get digit
        value >>= 1; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000007); // get digit
        value >>= 3; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value % 10); // get digit
        value /= 10;
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(
          value & 0x0000000f,
          (getFlags() & FormatOutputStream::Symbols::UPPER) != 0
        ); // get bits of digit
        value >>= 4; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return *this; // do not do anything if base is unknown
  }

  addIntegerField(dest, sizeof(buffer) - (dest - buffer), false);
  return *this;
}

FormatOutputStream& FormatOutputStream::operator<<(int value) throw(IOException)
{
  char buffer[sizeof(int) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      unsigned int temp = Cast::impersonate<unsigned int>(value); // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x00000001); // get digit
        temp >>= 1; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      unsigned int temp = Cast::impersonate<unsigned int>(value); // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x00000007); // get digit
        temp >>= 3; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      unsigned int temp = (value >= 0) ? value : -value;
      do {
        *dest = ASCIITraits::valueToDigit(temp % 10); // get digit
        temp = temp/10;
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      unsigned int temp = Cast::impersonate<unsigned int>(value); // no sign
      do {
        *dest = ASCIITraits::valueToDigit(
          temp & 0x0000000f,
          (getFlags() & FormatOutputStream::Symbols::UPPER) != 0
        ); // get bits of digit
        temp >>= 4; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return *this; // do not do anything if base is unknown
  }

  addIntegerField(dest, sizeof(buffer) - (dest - buffer), value < 0);
  return *this;
}

FormatOutputStream& FormatOutputStream::operator<<(unsigned int value) throw(IOException)
{
  char buffer[sizeof(unsigned int) * 8];
  char* dest = &buffer[getArraySize(buffer) - 1]; // point to least significant digit position

  switch (getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000001); // get digit
        value >>= 1; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000007); // get digit
        value >>= 3; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value % 10); // get digit
        value /= 10;
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(
          value & 0x0000000f,
          (getFlags() & FormatOutputStream::Symbols::UPPER) != 0
        ); // get bits of digit
        value >>= 4; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return *this; // do not do anything if base is unknown
  }

  addIntegerField(dest, getArraySize(buffer) - (dest - buffer), false);
  return *this;
}

FormatOutputStream& FormatOutputStream::operator<<(long value) throw(IOException)
{
  char buffer[sizeof(long) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      unsigned long temp = Cast::impersonate<unsigned long>(value); // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x00000001); // get digit
        temp >>= 1; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      unsigned long temp = Cast::impersonate<unsigned long>(value); // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x00000007); // get digit
        temp >>= 3; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      unsigned long temp = (value >= 0) ? value : -value;
      do {
        *dest = ASCIITraits::valueToDigit(temp % 10); // get digit
        temp = temp/10;
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      unsigned long temp = Cast::impersonate<unsigned long>(value); // no sign
      do {
        *dest = ASCIITraits::valueToDigit(
          temp & 0x0000000f,
          (getFlags() & FormatOutputStream::Symbols::UPPER) != 0
        ); // get bits of digit
        temp >>= 4; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return *this; // do not do anything if base is unknown
  }

  addIntegerField(dest, sizeof(buffer) - (dest - buffer), value < 0);
  return *this;
}

FormatOutputStream& FormatOutputStream::operator<<(unsigned long value) throw(IOException)
{
  char buffer[sizeof(unsigned long) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000001); // get digit
        value >>= 1; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000007); // get digit
        value >>= 3; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value % 10); // get digit
        value /= 10;
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(
          value & 0x0000000f,
          (getFlags() & FormatOutputStream::Symbols::UPPER) != 0
        ); // get bits of digit
        value >>= 4; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return *this; // do not do anything if base is unknown
  }

  addIntegerField(dest, sizeof(buffer) - (dest - buffer), false);
  return *this;
}

FormatOutputStream& FormatOutputStream::operator<<(long long value) throw(IOException)
{
  char buffer[sizeof(long long) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      unsigned long long temp = Cast::impersonate<unsigned long long>(value); // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x00000001); // get digit
        temp >>= 1; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      unsigned long long temp = Cast::impersonate<unsigned long long>(value); // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x00000007); // get digit
        temp >>= 3; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      unsigned long long temp = (value >= 0) ? value : -value;
      do {
        *dest = ASCIITraits::valueToDigit(temp % 10); // get digit
        temp = temp/10;
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      unsigned long long temp = Cast::impersonate<unsigned long long>(value); // no sign
      do {
        *dest = ASCIITraits::valueToDigit(
          temp & 0x0000000f,
          (getFlags() & FormatOutputStream::Symbols::UPPER) != 0
        ); // get bits of digit
        temp >>= 4; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return *this; // do not do anything if base is unknown
  }

  addIntegerField(dest, sizeof(buffer) - (dest - buffer), value < 0);
  return *this;
}

FormatOutputStream& FormatOutputStream::operator<<(unsigned long long value) throw(IOException)
{
  char buffer[sizeof(unsigned long long) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000001); // get digit
        value >>= 1; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000007); // get digit
        value >>= 3; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value % 10); // get digit
        value /= 10;
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(
          value & 0x0000000f,
          (getFlags() & FormatOutputStream::Symbols::UPPER) != 0
        ); // get bits of digit
        value >>= 4; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return *this; // do not do anything if base is unknown
  }

  addIntegerField(dest, sizeof(buffer) - (dest - buffer), false);
  return *this;
}

FormatOutputStream& FormatOutputStream::operator<<(int128 _value) throw(IOException)
{
  UInt128 value(_value); // no sign
  char buffer[sizeof(int128) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      do {
        *dest = ASCIITraits::valueToDigit(static_cast<unsigned int>(value) & 0x1); // get digit
        value >>= 1; // bits per digit
        --dest;
      } while (value);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(static_cast<unsigned int>(value) & 0x7); // get digit
        value >>= 3; // bits per digit
        --dest;
      } while (value);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      UInt128 temp = value.isHighBitSet() ? value : -value; // high bit is sign
      const auto BASE = static_cast<UInt128>(10);
      do {
        *dest = ASCIITraits::valueToDigit(temp % BASE); // get digit
        temp = temp/BASE;
        --dest;
      } while (temp);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(
          static_cast<unsigned int>(value) & 0xf,
          (getFlags() & FormatOutputStream::Symbols::UPPER) != 0
        ); // get bits of digit
        value >>= 4; // bits per digit
        --dest;
      } while (value);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return *this; // do not do anything if base is unknown
  }

  addIntegerField(dest, sizeof(buffer) - (dest - buffer), value.isHighBitSet()); // high bit is sign
  return *this;
}

FormatOutputStream& FormatOutputStream::operator<<(uint128 _value) throw(IOException)
{
  UInt128 value(_value);
  char buffer[sizeof(uint128) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      do {
        *dest = ASCIITraits::valueToDigit(static_cast<unsigned int>(value) & 0x1); // get digit
        value >>= 1; // bits per digit
        --dest;
      } while (value);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(static_cast<unsigned int>(value) & 0x7); // get digit
        value >>= 3; // bits per digit
        --dest;
      } while (value);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      const auto BASE = static_cast<UInt128>(10);
      do {
        *dest = ASCIITraits::valueToDigit(static_cast<uint8>(value % BASE)); // get digit
        value /= BASE;
        --dest;
      } while (value);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(
          static_cast<unsigned int>(value) & 0xf,
          (getFlags() & FormatOutputStream::Symbols::UPPER) != 0
        ); // get bits of digit
        value >>= 4; // bits per digit
        --dest;
      } while (value);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return *this; // do not do anything if base is unknown
  }

  addIntegerField(dest, sizeof(buffer) - (dest - buffer), false);
  return *this;
}

enum CutMode {
  CUT_MODE_NOGARBAGE,
  CUT_MODE_RELATIVE
};

void convertFloatingPoint(
  unsigned int significant,
  unsigned int precision,
  CutMode cutMode,
  FormatOutputStream::Symbols::RealStyle realStyle,
  unsigned int* restrict mantissa,
  unsigned int mantissaSize,
  int base2Exponent,
  uint8* restrict buffer,
  unsigned int& numberOfDigits,
  int& exponent) noexcept
{
  // TAG: there is plenty room for optimization
  static const unsigned int power[] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000
  };

  numberOfDigits = 0;
  exponent = 0; // TAG: fixme could already have been initialized (only in FASTEST mode)

  BASSERT(significant > 0);
  --significant; // TAG: why this

  bool roundUp = false;
  const unsigned int* nonZero = findPredicate(mantissa, mantissaSize, bind2Second(NotEqual<unsigned int>(), 0));
  if (!nonZero) {
    buffer[numberOfDigits++] = 0;
    return;
  }

  const unsigned int shiftS = maximum<int>(-(base2Exponent - static_cast<int>(significant)), 0); // max(0, -(e-p))
  const unsigned int shiftR = maximum<int>(base2Exponent - significant, 0); // max(e-p, 0)

  // TAG: in debug mode use worst case size
  // number of words in large integers
  unsigned int integerSize = (maximum<int>(maximum<int>(shiftS, shiftR), maximum<int>(base2Exponent, significant)) /* value bits */
                                        + 2 /* additional shifts */
                                        + 4 /* multiplication with 10, addition with 10, sum of integers */
                                        + sizeof(unsigned int)*8 - 1 /* round up */
                                       )/(sizeof(unsigned int)*8);
  BASSERT((integerSize > 0) && (integerSize <= 513));

  // allocate integers on stack (potentially 10kb)
  PrimitiveStackArray<unsigned int> S(integerSize);
  PrimitiveStackArray<unsigned int> R(integerSize);
  PrimitiveStackArray<unsigned int> Mminus(integerSize);
  PrimitiveStackArray<unsigned int> Mdouble(integerSize); // 2 * M- (only initialized if required)
  PrimitiveStackArray<unsigned int> temp(integerSize);

  LargeIntegerImpl::clear(R, integerSize);
  LargeIntegerImpl::assign(R, mantissa, mantissaSize);

  const bool unequalGap = ((nonZero - mantissa) == mantissaSize) &&
    (mantissa[mantissaSize - 1] == (1U << ((significant - 1) % (sizeof(unsigned int) * 8))));
  if (unequalGap) { // f == 1 << (p-1) // TAG: does this work for denormalized values
    LargeIntegerImpl::assignBit(S, integerSize, shiftS + 2); // S = 2*S_paper
    LargeIntegerImpl::leftShift(R, integerSize, shiftR + 2); // R = 2*R_paper
  } else {
    LargeIntegerImpl::assignBit(S, integerSize, shiftS + 1); // S = 2*S_paper
    LargeIntegerImpl::assign(temp, R, integerSize);
    LargeIntegerImpl::leftShift(R, integerSize, shiftR + 1); // R = 2*R_paper
  }
  LargeIntegerImpl::assignBit(Mminus, integerSize, shiftR); // Mminus = M-


  LargeIntegerImpl::assign(temp, S, integerSize);
  LargeIntegerImpl::add(temp, integerSize, 10 - 1);
  LargeIntegerImpl::divide(temp, integerSize, 10); // ceil(S/B)
  while (LargeIntegerImpl::lessThan(R, temp, integerSize)) { // R < ceil(S/B) => R < (S+B-1)/B
    LargeIntegerImpl::multiply(R, integerSize, 10); // R = R * B // TAG: optimize
    --exponent;
  }

  int alpha = -exponent;
  while (alpha > 0) {
    unsigned int n = minimum<int>(alpha, 9);
    alpha -= n;
    LargeIntegerImpl::multiply(Mminus, integerSize, power[n]); // M- = M- * B^n
  }

  unsigned int* Mplus = Mminus;
  if (unequalGap) { // f == 1 << (p-1) // TAG: does this work for denormalized values
    Mplus = Mdouble; // redirect to separate storage
    LargeIntegerImpl::assign(Mplus, Mminus, integerSize);
    LargeIntegerImpl::leftShift(Mplus, integerSize, 1); // Mplus = M+
  }

  int cutPlace = 0;

  bool adjusted = false;
  while (true) {
    LargeIntegerImpl::assign(temp, R, integerSize);
    LargeIntegerImpl::add(temp, Mplus, integerSize);
    while (!LargeIntegerImpl::lessThan(temp, S, integerSize)) { // 2*R + M+ >= 2*S
      adjusted = false;
      LargeIntegerImpl::multiply(S, integerSize, 10); // S = S * B // TAG: need optimization
      ++exponent;
    }

    if (adjusted) {
      break;
    }

    switch (cutMode) {
    case CUT_MODE_NOGARBAGE: // TAG: works with SCIENTIFIC and ENGINEERING but not FIXED
      cutPlace = (significant + 1)/3; // N = 2 + floor[significant/log2(10)] => N < 3 + significant/3
      break;
    case CUT_MODE_RELATIVE:
      {
        switch (realStyle) {
        case FormatOutputStream::Symbols::SCIENTIFIC:
          cutPlace = precision + 1; // one digit before radix
          break;
        case FormatOutputStream::Symbols::FIXED:
          cutPlace = precision + exponent;
          if (cutPlace < 0) {
            return; // empty buffer
          }
          break;
        case FormatOutputStream::Symbols::ENGINEERING: // 1-3 digits before radix character
          if (exponent >= 0) {
            cutPlace = precision + exponent % 3; // round towards -infinity
          } else {
            cutPlace = precision + (3 - -exponent % 3) % 3; // round towards -infinity
          }
          break;
        }

        LargeIntegerImpl::assign(temp, S, integerSize); // y = S
        int alpha = -cutPlace;
        if (alpha > 0) {
          while (alpha > 0) {
            unsigned int n = minimum<int>(alpha, 9);
            alpha -= n;
            LargeIntegerImpl::multiply(temp, integerSize, power[n]); // y = y * B^n
          }
        } else {
          while (alpha < 0) {
            unsigned int n = minimum<int>(-alpha, 9);
            alpha += n;
            LargeIntegerImpl::add(temp, integerSize, power[n] - 1);
            LargeIntegerImpl::divide(temp, integerSize, power[n]); // y = ceil(y/B^n)
          }
        }

        if (LargeIntegerImpl::lessThan(Mminus, temp, integerSize)) {
          LargeIntegerImpl::assign(Mminus, temp, integerSize); // M- = max(y, M-)
        }
        if (unequalGap) { // check if M- and M+ differ
          if (LargeIntegerImpl::lessThan(Mplus, temp, integerSize)) {
            LargeIntegerImpl::assign(Mplus, temp, integerSize); // M+ = max(y, M+)
          } else if (LargeIntegerImpl::equal(Mplus, temp, integerSize)) {
            roundUp = true;
          }
        } else {
          if (LargeIntegerImpl::equal(Mplus, temp, integerSize)) {
            roundUp = true;
          }
        }
        break;
      }
    }
    adjusted = true;
  }

  bool low = false;
  bool high = false;
  while (!low && !high) { // assert: R < S
    LargeIntegerImpl::multiply(R, integerSize, 10); // R = R * B
    LargeIntegerImpl::multiply(Mminus, integerSize, 10); // M- = M- * B
    if (unequalGap) { // only if M- and M+ differ
      LargeIntegerImpl::multiply(Mplus, integerSize, 10); // M+ = M+ * B
    }

    LargeIntegerImpl::divide(temp, R, R, S, integerSize); // U = R/S and R=R%S
    if (static_cast<int>(numberOfDigits) < cutPlace) {
      buffer[numberOfDigits++] = temp[0]; // assert: U < B
    }

    low = LargeIntegerImpl::lessThan(R, Mminus, integerSize); // 2*R < M-
    LargeIntegerImpl::assign(temp, R, integerSize); // TAG: need alternative
    LargeIntegerImpl::add(temp, Mplus, integerSize); // temp = 2*R + M+
    if (roundUp) {
      high = !LargeIntegerImpl::lessThan(temp, S, integerSize); // 2*R >= 2*S - M+ <=> 2*R + M+ >= 2*S
    } else {
      high = LargeIntegerImpl::lessThan(S, temp, integerSize); // 2*R > 2*S - M+ <=> 2*R + M+ > 2*S
    }
    if (static_cast<int>(numberOfDigits) >= cutPlace) {
      break;
    }
  }

  if (low && !high) {
  } else if (high && !low) {
    ++buffer[numberOfDigits - 1];
  } else {
    LargeIntegerImpl::leftShift(R, integerSize, 1); // R = 2*R // TAG: need ordinary compare
    if (LargeIntegerImpl::lessThan(S, R, integerSize)) { // 2*R > S
      ++buffer[numberOfDigits - 1];
    }
  }
}

FormatOutputStream& FormatOutputStream::operator<<(float _value) throw(IOException)
{
  BASSERT(sizeof(float) == sizeof(FloatingPoint::FloatRepresentation));
  const FloatingPoint::ToFloat value(_value);

  if (value.isNaN()) {
    return *this << MESSAGE("nan");
  } else if (value.isInfinity()) {
    return *this << (!value.isNegative() ? MESSAGE("inf") : MESSAGE("-inf"));
  }

  unsigned int precision = 0;
  unsigned int mantissa[(FloatingPoint::FloatRepresentation::SIGNIFICANT + (sizeof(unsigned int) * 8) - 1)/(sizeof(unsigned int) * 8)];
  int exponent = 0;
  unsigned int flags = 0;
  analyseFloatingPoint(value.value, precision, mantissa, exponent, flags);
  writeFloatingPointType(
    precision,
    mantissa,
    (FloatingPoint::FloatRepresentation::SIGNIFICANT + (sizeof(unsigned int) * 8) - 1)/(sizeof(unsigned int) * 8),
    exponent,
    flags
  );
  return *this;
}

FormatOutputStream& FormatOutputStream::operator<<(double _value) throw(IOException)
{
  BASSERT(sizeof(double) == sizeof(FloatingPoint::DoubleRepresentation));
  const FloatingPoint::ToDouble value(_value);

  if (value.isNaN()) {
    return *this << MESSAGE("nan");
  } else if (value.isInfinity()) {
    return *this << (!value.isNegative() ? MESSAGE("inf") : MESSAGE("-inf"));
  }
  
  unsigned int precision = 0;
  unsigned int mantissa[(FloatingPoint::DoubleRepresentation::SIGNIFICANT + (sizeof(unsigned int) * 8) - 1)/(sizeof(unsigned int) * 8)];
  int exponent = 0;
  unsigned int flags = 0;
  analyseFloatingPoint(value.value, precision, mantissa, exponent, flags);
  writeFloatingPointType(
    precision,
    mantissa,
    (FloatingPoint::DoubleRepresentation::SIGNIFICANT + (sizeof(unsigned int) * 8) - 1)/(sizeof(unsigned int) * 8),
    exponent,
    flags
  );
  return *this;
}

FormatOutputStream& FormatOutputStream::operator<<(long double _value) throw(IOException)
{
#if 0 && (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
  return operator<<(static_cast<double>(_value));
#endif

  BASSERT(sizeof(long double) == sizeof(FloatingPoint::LongDoubleRepresentation));
  const FloatingPoint::ToLongDouble value(_value);

  if (value.isNaN()) {
    return *this << MESSAGE("nan");
  } else if (value.isInfinity()) {
    return *this << (!value.isNegative() ? MESSAGE("inf") : MESSAGE("-inf"));
  }

  unsigned int precision = 0;
  unsigned int mantissa[(FloatingPoint::LongDoubleRepresentation::SIGNIFICANT + (sizeof(unsigned int) * 8) - 1)/(sizeof(unsigned int) * 8)];
  int exponent = 0;
  unsigned int flags = 0;
  analyseFloatingPoint(value.value, precision, mantissa, exponent, flags);

  writeFloatingPointType(
    precision,
    mantissa,
    (FloatingPoint::LongDoubleRepresentation::SIGNIFICANT + (sizeof(unsigned int) * 8) - 1)/(sizeof(unsigned int) * 8),
    exponent,
    flags
  );
  return *this;
}

FormatOutputStream& FormatOutputStream::operator<<(const void* value) throw(IOException)
{
  return *this << HEX << PREFIX << ZEROPAD << Cast::getOffset(value);
}

FormatOutputStream& FormatOutputStream::operator<<(const Exception& e) throw(IOException)
{
  StringOutputStream s;
  s << "Exception '"
    << TypeInfo::getTypename(e) << "' was raised";
  if (e.getType().isInitialized()) {
    s << " by '" << TypeInfo::getTypename(e.getType()) << '\'';
  }
  const unsigned int cause = e.getCause();
  const unsigned int nativeError = e.getError();
  const char* message = e.getMessage();
  if (message || (cause != PrimitiveTraits<unsigned int>::MAXIMUM)) {
    s << " with";
  }
  if (message) {
    s << " message '" << NativeString(message) << '\'';
  }
  if (message && (cause != PrimitiveTraits<unsigned int>::MAXIMUM)) {
    s << " and";
  }
  if (cause != PrimitiveTraits<unsigned int>::MAXIMUM) {
    s << " cause " << cause;
  } else if (nativeError != 0) {
    s << " due to native error " << nativeError;
    unsigned int error = OperatingSystem::getErrorCode(nativeError);
    if (error != OperatingSystem::UNSPECIFIED_ERROR) {
      s << ' ' << '(' << OperatingSystem::getErrorMessage(error) << ')';
    }
  }
  s << '.' << FLUSH;
  return *this << s.getString();
}

/*
  0xhex_digit_seq[.hex_digit_seq]p[+|-]binary_exponent[suffix]

*/
// void FormatOutputStream::writeFloatingHEX...() throw() {
//   if ((context.flags & Symbols::PREFIX) != 0) { // is prefix requested
//     switch (context.integerBase) {
//     case Symbols::BINARY:
//       write("0", 1);
//       write(((context.flags & FormatOutputStream::Symbols::UPPER) == 0) ? "b" : "B", 1);
//       break;
//     case Symbols::OCTAL:
//       write("0", 1);
//       break;
//     case Symbols::DECIMAL:
//       break;
//     case Symbols::HEXADECIMAL:
//       write("0", 1);
//       write(((context.flags & FormatOutputStream::Symbols::UPPER) == 0) ? "x" : "X", 1);
//       break;
//     }
//   }
// }

void FormatOutputStream::writeFloatingPointType(
  unsigned int significant,
  unsigned int* mantissa,
  unsigned int mantissaSize,
  int base2Exponent,
  unsigned int valueFlags) throw(IOException)
{
  PrimitiveStackArray<char> buffer(128 + 2 + significant/3); // N = 2 + floor[n/log2(10)] => N < 2 + n/3 // TAG: 128 should be calculated
  char* output = buffer;
  const char* radix = nullptr;
  unsigned int flags = context.flags;

  if ((valueFlags & FloatingPoint::FP_ANY_NAN) != 0) {
    if ((flags & Symbols::UPPER) == 0) {
      copy(output, "nan", sizeof("nan") - 1); // TAG: I guess this should be locale specific
      output += sizeof("nan") - 1;
    } else {
      copy(output, "NAN", sizeof("NAN") - 1); // TAG: I guess this should be locale specific
      output += sizeof("NAN") - 1;
    }
  } else {

    if ((flags & Symbols::POSIX) != 0) { // use POSIX
      if ((valueFlags & FloatingPoint::FP_NEGATIVE) != 0) {
        *output++ = '-';
      } else if ((flags & Symbols::FPLUS) != 0) { // show plus if sign is forced
        *output++ = '+';
      }
    } else { // use locale
      // numeric or currency
      // const char* str = (valueFlags & FloatingPoint::FP_NEGATIVE) ? locale->getBeginNegative() : locale->getBeginPositive();
      // copy to output
    }

    if ((valueFlags & FloatingPoint::FP_INFINITY) != 0) {
      if ((flags & Symbols::UPPER) == 0) {
        copy(output, "infinity", sizeof("infinity") - 1); // TAG: I guess this should be locale specific
        output += sizeof("infinity") - 1;
      } else {
        copy(output, "INFINITY", sizeof("INFINITY") - 1); // TAG: I guess this should be locale specific
        output += sizeof("INFINITY") - 1;
      }
    } else {

      PrimitiveStackArray<uint8> digitBuffer((significant + 1)/3); // N = 2 + floor[n/log2(10)] => N < 3 + n/3
      unsigned int numberOfDigits = 0;
      int exponent = 0;
      CutMode cutMode = CUT_MODE_NOGARBAGE;

      if ((flags & Symbols::NECESSARY) != 0) {
        cutMode = CUT_MODE_NOGARBAGE;
      } else {
        cutMode = CUT_MODE_RELATIVE;
      }

      convertFloatingPoint(
        significant,
        context.precision,
        cutMode,
        context.realStyle,
        mantissa,
        mantissaSize,
        base2Exponent,
        digitBuffer,
        numberOfDigits,
        exponent
      );
      bool showExponent = true;

      int adjustedExponent = exponent;
      switch (context.realStyle) {
      case Symbols::SCIENTIFIC: // one digit before radix character
        if ((valueFlags & FloatingPoint::FP_ANY_ZERO) == 0) {
          adjustedExponent = exponent - 1;
        }
        break;
      case Symbols::FIXED:
        if ((flags & Symbols::NECESSARY) != 0) { // NOGARBAGE - precision is not valid
          if ((exponent >= -3) && (exponent < 10)) {
            adjustedExponent = 0;
            showExponent = false;
          } else {
            adjustedExponent = exponent - 1;
          }
        } else if (exponent <= maximum<int>(context.precision, numberOfDigits)) { // TAG: need number of significant digits (base 10)
          adjustedExponent = 0;
          showExponent = false;
        } else {
          adjustedExponent = exponent - 1;
        }
        break;
      case Symbols::ENGINEERING: // 1-3 digits before radix character
        if (exponent >= 0) {
          adjustedExponent = (exponent - 1)/3*3; // round towards -infinity
        } else {
          adjustedExponent = (exponent - 3)/3*3; // round towards -infinity
        }
        break;
      }

      const uint8* digit = digitBuffer;
      const uint8* endDigit = digit + numberOfDigits;
      int digitsBeforeRadix = ((valueFlags & FloatingPoint::FP_ANY_ZERO) == 0) ? maximum(exponent - adjustedExponent, 0) : 1;
      int denormalizingZeros = minimum(maximum(adjustedExponent - exponent, 0), static_cast<int>(context.precision));

      if (digitsBeforeRadix > 0) {
        if ((flags & Symbols::POSIX) != 0) { // use posix (no grouping char specified)
          for (int i = digitsBeforeRadix - 1; i >= 0; --i) { // write all but one digit
            *output++ = (digit < endDigit) ? ASCIITraits::valueToDigit(*digit++) : ASCIITraits::valueToDigit(0);
          }
        } else {
          const char* groupingStr = ","; // = locale.getGrouping(); // TAG: fix
          for (int i = digitsBeforeRadix - 1; i >= 0; --i) { // write all but one digit
            *output++ = (digit < endDigit) ? ASCIITraits::valueToDigit(*digit++) : ASCIITraits::valueToDigit(0);
            if (i == 0) {
              break; // do not write last grouping separator (that is the radix position)
            }
            if (((flags & Symbols::GROUPING) != 0) && (i % 3 == 0)) {
              for (const char* p = groupingStr; *p != 0; ++p, ++output) {
                *output = *p;
              }
            }
          }
        }
      } else { // special case where 0 not contained in digit buffer
        *output++ = ASCIITraits::valueToDigit(0);
      }

      unsigned int digitsAfterRadix =
        (static_cast<int>(numberOfDigits) > digitsBeforeRadix) ? (numberOfDigits - digitsBeforeRadix) : 0;
      unsigned int totalDigitsAfterRadix = digitsAfterRadix + denormalizingZeros;
      int trailingZeros = 0;

      if ((flags & Symbols::NECESSARY) == 0) {
        if (totalDigitsAfterRadix < static_cast<unsigned int>(context.precision)) {
          trailingZeros += context.precision - totalDigitsAfterRadix;
          totalDigitsAfterRadix += trailingZeros;
        }
      }

      if ((totalDigitsAfterRadix > 0) || (context.justification == Symbols::RADIX)) {
        radix = output; // remember position of radix for later justification
        // only show radix char if followed by digits or used in justification
        if ((flags & Symbols::POSIX) != 0) {
          *output++ = '.';
        } else { // use locale
          //for (const char* p = locale.getDecimalPoint(); *p != 0; ++p, ++output) {
          //  *output = *p;
          //}
        }
        for (int i = denormalizingZeros; i > 0; --i) {
          *output++ = ASCIITraits::valueToDigit(0);
        }
        for (int i = digitsAfterRadix; i > 0; --i) { // write significant digits
          *output++ = ASCIITraits::valueToDigit(*digit++);
        }
        for (int i = trailingZeros; i > 0; --i) {
          *output++ = ASCIITraits::valueToDigit(0);
        }
      }

      if (showExponent) {
        *output++ = ((flags & Symbols::UPPER) == 0) ? 'e' : 'E';

        if (adjustedExponent < 0) {
          *output++ = '-';
        } else if ((flags & Symbols::PLUSEXP) != 0) { // only show positive exponent explicitly if forced
          *output++ = '+';
        }

        static const unsigned int BASE10_EXPONENTS[] = {1, 10, 100, 1000};
        bool writeDigit = ((flags & Symbols::ZEROPADEXP) != 0);
        unsigned int temp = (adjustedExponent >= 0) ? adjustedExponent : -adjustedExponent;
        for (int i = 3; i >= 0; --i) { // TAG: need limit as arg of method?
          unsigned int digitValue = temp/BASE10_EXPONENTS[i] % 10;
          if (writeDigit || (digitValue != 0) || (i == 0)) {
            writeDigit = true; // first non-zero digit forces following zero-digits to be written
            *output++ = ASCIITraits::valueToDigit(digitValue);
          }
        }
      }

      if ((flags & Symbols::POSIX) == 0) { // use locale
        // numeric or monetary
        // const char* str = (valueFlags & FloatingPoint::FP_NEGATIVE) ? locale->getEndNegative() : locale->getEndPositive();
        // copy to output
      }
    }
  }

  {
    ExclusiveSynchronize<Guard> _guard(guard);
    unsigned int length = (output - buffer);
    if (static_cast<unsigned int>(context.width) <= length) {
      write(Cast::pointer<const uint8*>(static_cast<const char*>(buffer)), length); // write characters
    } else {
      unsigned int invertedLength = context.width - length;

      Symbols::Justification justification = context.justification;
      if (context.justification == Symbols::DEPENDENT) {
        justification = Symbols::RIGHT; // TAG: is this locale specific
      }

      switch (justification) {
      case Symbols::LEFT:
        write(Cast::pointer<const uint8*>(static_cast<const char*>(buffer)), length); // write characters
        unfoldValue(' ', invertedLength);
        break;
      case Symbols::DEPENDENT:
      case Symbols::RIGHT:
        unfoldValue(' ', invertedLength);
        write(Cast::pointer<const uint8*>(static_cast<const char*>(buffer)), length); // write characters
        break;
      case Symbols::RADIX:
        BASSERT(radix);
        unsigned int beforeRadix = radix - buffer; // character before radix (excluding)
        unsigned int prefixLength = 0; // left justify by default
        if (static_cast<unsigned int>(context.radixPosition) >= beforeRadix) {
          // right justify if radix position is too big
          prefixLength = minimum(context.radixPosition - beforeRadix, invertedLength);
          unfoldValue(' ', prefixLength);
        }
        write(Cast::pointer<const uint8*>(static_cast<const char*>(buffer)), length); // write characters
        unfoldValue(' ', invertedLength - prefixLength);
        break;
      }
    }
    context = defaultContext;
  }
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Type& type) throw(IOException)
{
  return stream << "Type<" << TypeInfo::getTypename(type) << ">";
}

String FormatOutputStream::toString()
{
  return String();
}

String operator<<(FormatOutputStream& stream, const GetString& getString)
{
  stream.flush();
  return stream.toString();
}

template<typename TYPE>
class CheckType {
public:

  template<typename POLY>
  static void _checkType(decltype(&POLY::operator String)) {
    decltype(&POLY::operator String) p = &POLY::operator String;
    fout << "Got match!" << ENDL;
  }

  template<typename POLY>
  static void _checkType(...) {
    fout << "No match!" << ENDL;
  }

  static void checkType() {
    //decltype(&TYPE::operator String) p = &TYPE::operator String;
    ;
    typedef String (TYPE::*CastMember)() const;
    CastMember member = nullptr;
    _checkType<TYPE>(member);
  }
};



/** Check if TYPE has operator String() const member. https://en.cppreference.com/w/cpp/language/sfinae check. */
template<typename TYPE>
class HasStringCast {
private:

  /** Member type we are looking for. */
  typedef String (TYPE::* Member)() const; // we only want const member - and String by value - we dont want static either

  typedef bool Yes;
  typedef void No;
  static_assert(!std::is_same<Yes, No>(), "Type error.");
  static_assert(IsComplete<TYPE>(), "Type must be complete.");

  // MSC bug: decltype returns String* type instead of member function pointer type here
  template<typename POLY> static Yes hasStringCast(decltype(&POLY::operator String));
  template<typename POLY> static No hasStringCast(...);
public:

  static constexpr bool VALUE = std::is_same<decltype(hasStringCast<TYPE>(static_cast<Member>(nullptr))), Yes>();
  // static constexpr bool VALUE = std::is_same<decltype(getType<TYPE>(nullptr)), Member>();

  inline constexpr operator bool() const noexcept
  {
    return VALUE;
  }

  inline constexpr bool operator()() const noexcept
  {
    return VALUE;
  }
};

/** Check if TYPE has String toString() const member. https://en.cppreference.com/w/cpp/language/sfinae check. */
template<typename TYPE>
class HasToString {
private:

  /** Member type we are looking for. */
  typedef String (TYPE::* Member)() const; // we only want const member - and String by value - we dont want static either

  // typedef bool Yes;
  // typedef void No;
  // static_assert(!std::is_same<Yes, No>(), "Type error.");
  static_assert(IsComplete<TYPE>(), "Type must be complete.");

  template<typename POLY> static decltype(&POLY::toString) getType(decltype(&POLY::toString));
  template<typename POLY> static void getType(...);

  // template<typename POLY> static Yes hasToString(decltype(&POLY::toString));
  // template<typename POLY> static No hasToString(...);
public:

  // static constexpr bool VALUE = std::is_same<decltype(hasToString<TYPE>(static_cast<Member>(nullptr))), Yes>(); // this works
  static constexpr bool VALUE = std::is_same<decltype(getType<TYPE>(nullptr)), Member>(); // this works

  inline constexpr operator bool() const noexcept
  {
    return VALUE;
  }

  inline constexpr bool operator()() const noexcept
  {
    return VALUE;
  }
};

// TAG: priority should be: operator String() -> toString() -> no match

enum {
  USE_UNKNOWN = 0,
  USE_STRING_CAST = 1,
  USE_TO_STRING = 2
};

template<class TYPE, int match = USE_UNKNOWN>
class ForwardToString {
public:

  static inline void write(FormatOutputStream& stream, const TYPE& object)
  {
    stream << "!FORMAT UNDEFINED FOR " << Type::getType<TYPE>() << "!";
  }
};

template<class TYPE>
class ForwardToString<TYPE, USE_STRING_CAST> {
public:

  static inline void write(FormatOutputStream& stream, const TYPE& object)
  {
    stream << (object.operator String());
  }
};

template<class TYPE>
class ForwardToString<TYPE, USE_TO_STRING> {
public:

  static inline void write(FormatOutputStream& stream, const TYPE& object)
  {
    stream << object.toString();
  }
};

template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const TYPE& object) // we do NOT allow mutable object
{
  // constexpr bool useStringCast = HasStringCast<TYPE>();
  constexpr bool useToString = HasToString<TYPE>();
  ForwardToString<TYPE, /*useStringCast ? USE_STRING_CAST :*/ (useToString ? USE_TO_STRING : USE_UNKNOWN)>::write(stream, object);
  return stream;
}

template<class TYPE>
class DumpTypeInfo {
public:

  DumpTypeInfo()
  {
  }
};

template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, DumpTypeInfo<TYPE>& type)
{
  stream << Type::getType<TYPE>() << EOL
         << indent(2) << "is_constructible=" << std::is_constructible<TYPE>() << EOL
         << indent(2) << "is_copy_constructible=" << std::is_copy_constructible<TYPE>() << EOL
         << indent(2) << "is_move_constructible=" << std::is_move_constructible<TYPE>() << EOL
         << indent(2) << "is_trivially_copyable=" << std::is_trivially_copyable<TYPE>() << EOL
         << indent(2) << "is_move_assignable=" << std::is_move_assignable<TYPE>() << EOL
         << indent(2) << "is_nothrow_destructible=" << std::is_nothrow_destructible<TYPE>() << EOL
         << indent(2) << "is_destructible=" << std::is_destructible<TYPE>() << EOL;
  return stream;
}


#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(FormatOutputStream) : public UnitTest {
public:

  TEST_PRIORITY(40);
  TEST_PROJECT("base/string");

  class ClassWithToString {
  public:

    String toString() const {
      return "Using ClassWithToString::toString().";
    }
  };

  class ClassWithToString2 {
  public:

    const String& toString() const {
      static String text = "Using ClassWithToString2::toString().";
      return text;
    }
  };

  class ClassWithToString3 {
  public:

    String toString() const {
      return "Using ClassWithToString3::toString().";
    }

    operator String() const {
      return "Using ClassWithToString3::operator String().";
    }
  };

  class ClassWithMissingConstToString {
  public:

    String toString() { // missing const
      return "Using ClassWithMissingConstToString::toString().";
    }
  };

  class ClassWithStaticToString {
  public:

    static String toString() { // static
      return "Using ClassWithStaticToString::toString().";
    }
  };

  class ClassWithoutToString {
  public:
  };

  void run() override
  {
    typedef StringOutputStream f;

    TEST_EQUAL(String(f() << false), "false");
    TEST_EQUAL(String(f() << true), "true");
    TEST_EQUAL(String(f() << '!'), "!");
    TEST_EQUAL(String(f() << L'!'), "!");
    TEST_EQUAL(String(f() << -123), "-123");

    TEST_EQUAL(String(f() << static_cast<short>(-123)), "-123");
    TEST_EQUAL(String(f() << static_cast<unsigned short>(123)), "123");
    TEST_EQUAL(String(f() << -123), "-123");
    TEST_EQUAL(String(f() << 123U), "123");
    TEST_EQUAL(String(f() << static_cast<long>(-123)), "-123");
    TEST_EQUAL(String(f() << static_cast<unsigned long>(123)), "123");
    TEST_EQUAL(String(f() << static_cast<long long>(-123)), "-123");
    TEST_EQUAL(String(f() << static_cast<unsigned long long>(123)), "123");
    TEST_EQUAL(String(f() << -123.0f), "-123");
    TEST_EQUAL(String(f() << -123.0), "-123");
    TEST_EQUAL(String(f() << -123.0L), "-123");

    // pi = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117068
    const String PI_IEEE_754_SINGLE_PRECISION = "3.1415927";
    const String PI_IEEE_754_DOUBLE_PRECISION = "3.141592653589793";
    // e = 2.718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166427
    const String E_IEEE_754_SINGLE_PRECISION = "2.7182817"; // TAG: confirm if float is really rounded down to 7
    const String E_IEEE_754_DOUBLE_PRECISION = "2.718281828459045";

    switch (_COM_AZURE_DEV__BASE__FLOAT) {
    case _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION:
      TEST_EQUAL(String(f() << constant::PI_F), PI_IEEE_754_SINGLE_PRECISION);
      TEST_EQUAL(String(f() << constant::E_F), E_IEEE_754_SINGLE_PRECISION);
      // TAG: convert string back to float - and then back to string - must give the same result
      break;
    case _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION:
      TEST_EQUAL(String(f() << constant::PI_F), PI_IEEE_754_DOUBLE_PRECISION);
      TEST_EQUAL(String(f() << constant::E_F), E_IEEE_754_DOUBLE_PRECISION);
      break;
    case _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96:
    case _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16:
    case _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128:
    case _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION:
      break;
    }

    switch (_COM_AZURE_DEV__BASE__DOUBLE) {
    case _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION:
      TEST_EQUAL(String(f() << constant::PI), PI_IEEE_754_SINGLE_PRECISION);
      TEST_EQUAL(String(f() << constant::E), E_IEEE_754_SINGLE_PRECISION);
      break;
    case _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION:
      TEST_EQUAL(String(f() << constant::PI), PI_IEEE_754_DOUBLE_PRECISION);
      TEST_EQUAL(String(f() << constant::E), E_IEEE_754_DOUBLE_PRECISION);
      break;
    case _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96:
    case _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16:
    case _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128:
    case _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION:
      break;
    }

    switch (_COM_AZURE_DEV__BASE__LONG_DOUBLE) {
    case _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION:
      TEST_EQUAL(String(f() << constant::PI_L), PI_IEEE_754_SINGLE_PRECISION);
      TEST_EQUAL(String(f() << constant::E_L), E_IEEE_754_SINGLE_PRECISION);
      break;
    case _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION:
      TEST_EQUAL(String(f() << constant::PI_L), PI_IEEE_754_DOUBLE_PRECISION);
      TEST_EQUAL(String(f() << constant::E_L), E_IEEE_754_DOUBLE_PRECISION);
      break;
    case _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96:
    case _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16:
    case _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128:
    case _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION:
      break;
    }

    volatile int zero = 0;
    TEST_EQUAL(String(f() << 0.0f/zero), "nan");
    TEST_EQUAL(String(f() << 0.0/zero), "nan");
    TEST_EQUAL(String(f() << 0.0L/zero), "nan");
    TEST_EQUAL(String(f() << 123.0f/zero), "inf");
    TEST_EQUAL(String(f() << 123.0/zero), "inf");
    TEST_EQUAL(String(f() << 123.0L/zero), "inf");
    TEST_EQUAL(String(f() << -123.0f/zero), "-inf");
    TEST_EQUAL(String(f() << -123.0/zero), "-inf");
    TEST_EQUAL(String(f() << -123.0L/zero), "-inf");

    TEST_EQUAL(String(f() << ""), "");
    TEST_EQUAL(String(f() << MESSAGE("Hello, World!")), "Hello, World!");
    TEST_EQUAL(String(f() << "Hello, World!"), "Hello, World!");
    TEST_EQUAL(String(f() << WIDEMESSAGE("Hello, World!")), "Hello, World!");
    TEST_EQUAL(String(f() << L"Hello, World!"), "Hello, World!");

    TEST_EQUAL(String(f() << static_cast<void*>(nullptr)), "0x0");
    TEST_EQUAL(String(f() << BIN << 12345), "0b11000000111001");
    TEST_EQUAL(String(f() << DEC << 12345), "12345");
    TEST_EQUAL(String(f() << OCT << 12345), "030071");
    TEST_EQUAL(String(f() << HEX << 12345), "0x3039");
#if 0 // not supported
    TEST_EQUAL(String(f() << FBIN << 12345.0), "");
    TEST_EQUAL(String(f() << FDEC << 12345.0), "");
    TEST_EQUAL(String(f() << FOCT << 12345.0), "");
    TEST_EQUAL(String(f() << FHEX << 12345.0), "");
#endif
    TEST_EQUAL(String(f() << ZEROPAD << setWidth(10) << 12345), "0000012345");
    TEST_EQUAL(String(f() << indent(7)), "       ");

#if 0
    // static_assert(HasStringCast<ClassWithToString3>(), "Missing operator String().");
    // f() << ClassWithToString3() << ENDL;

    static_assert(HasToString<ClassWithToString>(), "Missing toString().");
    f() << ClassWithToString() << ENDL;
    static_assert(!HasToString<ClassWithToString2>(), "Matched bad toString()."); // const String&
    f() << ClassWithToString2() << ENDL;
    static_assert(HasToString<ClassWithToString3>(), "Missing toString().");
    f() << ClassWithToString3() << ENDL;
    static_assert(!HasToString<ClassWithMissingConstToString>(), "Matched bad toString().");
    f() << ClassWithMissingConstToString() << ENDL;
    static_assert(!HasToString<ClassWithStaticToString>(), "Matched bad toString().");
    f() << ClassWithStaticToString() << ENDL;
    static_assert(!HasToString<ClassWithoutToString>(), "Matched bad toString().");
    f() << ClassWithoutToString() << ENDL;
#endif
  }
};

TEST_REGISTER(FormatOutputStream);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
