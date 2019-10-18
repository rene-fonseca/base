/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for copyright information.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/AnyValue.h>
#include <base/string/StringOutputStream.h>
#include <base/ShortInteger.h>
#include <base/Integer.h>
#include <base/LongInteger.h>
#include <base/TypeInfo.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

AnyValue::AnyValue() noexcept : representation(VOID) {
}

AnyValue::AnyValue(const Type& value) throw()
  : representation(TYPE), type(value) {
}

AnyValue::AnyValue(void* value) throw()
  : representation(POINTER), p(value) {
}

AnyValue::AnyValue(char value) throw()
  : representation(CHARACTER), character(value) {
}

AnyValue::AnyValue(wchar value) throw()
  : representation(WIDE_CHARACTER), wideCharacter(value) {
}

AnyValue::AnyValue(bool value) throw()
  : representation(BOOLEAN), boolean(value) {
}

AnyValue::AnyValue(short value) throw()
  : representation(INTEGER), shortInteger(value) {
}

AnyValue::AnyValue(unsigned short value) throw()
  : representation(UNSIGNED_SHORT_INTEGER), unsignedShortInteger(value) {
}

AnyValue::AnyValue(int value) throw()
  : representation(INTEGER), integer(value) {
}

AnyValue::AnyValue(unsigned int value) throw()
  : representation(UNSIGNED_INTEGER), unsignedInteger(value) {
}

AnyValue::AnyValue(long value) throw()
  : representation(LONG_INTEGER), longInteger(value) {
}

AnyValue::AnyValue(unsigned long value) throw()
  : representation(UNSIGNED_LONG_INTEGER), unsignedLongInteger(value) {
}

AnyValue::AnyValue(long long value) throw()
  : representation(LONG_LONG_INTEGER), longLongInteger(value) {
}

AnyValue::AnyValue(unsigned long long value) throw()
  : representation(UNSIGNED_LONG_LONG_INTEGER),
    unsignedLongLongInteger(value) {
}

AnyValue::AnyValue(float value) throw()
  : representation(FLOAT), f(value) {
}

AnyValue::AnyValue(double value) throw()
  : representation(DOUBLE), d(value) {
}

AnyValue::AnyValue(long double value) throw()
  : representation(LONG_DOUBLE), ld(value) {
}

AnyValue::AnyValue(const String& value) throw()
  : representation(STRING), string(value) {
}

AnyValue::AnyValue(const Literal& value) throw()
  : representation(STRING), string(value) {
}

AnyValue::AnyValue(const WideString& value) throw()
  : representation(WIDE_STRING), wideString(value) {
}

AnyValue::AnyValue(const WideLiteral& value) throw()
  : representation(WIDE_STRING), wideString(value) {
}

AnyValue::AnyValue(const AnyReference& value) throw()
  : representation(REFERENCE), reference(value) {
}

AnyValue::AnyValue(const AnyValue& copy) throw()
  : representation(copy.representation) {
  
  switch (representation) {
  case VOID:
    break;
  case TYPE:
    type = copy.type;
    break;
  case POINTER:
    p = copy.p;
    break;
  case CHARACTER:
    character = copy.character;
    break;
  case WIDE_CHARACTER:
    wideCharacter = copy.wideCharacter;
    break;
  case BOOLEAN:
    boolean = copy.boolean;
    break;
  case SHORT_INTEGER:
    shortInteger = copy.shortInteger;
    break;
  case UNSIGNED_SHORT_INTEGER:
    unsignedShortInteger = copy.unsignedShortInteger;
    break;
  case INTEGER:
    integer = copy.integer;
    break;
  case UNSIGNED_INTEGER:
    unsignedInteger = copy.unsignedInteger;
    break;
  case LONG_INTEGER:
    longInteger = copy.longInteger;
    break;
  case UNSIGNED_LONG_INTEGER:
    unsignedLongInteger = copy.unsignedLongInteger;
    break;
  case LONG_LONG_INTEGER:
    longLongInteger = copy.longLongInteger;
    break;
  case UNSIGNED_LONG_LONG_INTEGER:
    unsignedLongLongInteger = copy.unsignedLongLongInteger;
    break;
  case FLOAT:
    f = copy.f;
    break;
  case DOUBLE:
    d = copy.d;
    break;
  case LONG_DOUBLE:
    ld = copy.ld;
    break;
  case STRING:
    string = copy.string;
    break;
  case WIDE_STRING:
    wideString = copy.wideString;
    break;
  case REFERENCE:
    reference = copy.reference;
    break;
  default:
    INVALID_CONTROL_FLOW();
  }
}

AnyValue& AnyValue::operator=(const AnyValue& eq) noexcept {
  if (&eq != this) { // only if not self-assignment
    reset();
    representation = eq.representation;
    switch (representation) {
    case VOID:
      break;
    case TYPE:
      type = eq.type;
      break;
    case POINTER:
      p = eq.p;
      break;
    case CHARACTER:
      character = eq.character;
      break;
    case WIDE_CHARACTER:
      wideCharacter = eq.wideCharacter;
      break;
    case BOOLEAN:
      boolean = eq.boolean;
      break;
    case SHORT_INTEGER:
      shortInteger = eq.shortInteger;
      break;
    case UNSIGNED_SHORT_INTEGER:
      unsignedShortInteger = eq.unsignedShortInteger;
      break;
    case INTEGER:
      integer = eq.integer;
      break;
    case UNSIGNED_INTEGER:
      unsignedInteger = eq.unsignedInteger;
      break;
    case LONG_INTEGER:
      longInteger = eq.longInteger;
      break;
    case UNSIGNED_LONG_INTEGER:
      unsignedLongInteger = eq.unsignedLongInteger;
      break;
    case LONG_LONG_INTEGER:
      longLongInteger = eq.longLongInteger;
      break;
    case UNSIGNED_LONG_LONG_INTEGER:
      unsignedLongLongInteger = eq.unsignedLongLongInteger;
      break;
    case FLOAT:
      f = eq.f;
      break;
    case DOUBLE:
      d = eq.d;
      break;
    case LONG_DOUBLE:
      ld = eq.ld;
      break;
    case STRING:
      string = eq.string;
      break;
    case WIDE_STRING:
      wideString = eq.wideString;      
      break;
    default:
      INVALID_CONTROL_FLOW();
    }
  }
  return *this;
}

Type AnyValue::getRepresentationType() const noexcept
{
  switch (representation) {
  case VOID:
    return Type(); // uninitialized
  case TYPE:
    return Type::getType<Type>();
  case POINTER:
    return Type::makeType(&typeid(void*)); // Type::getType<void*>();
  case CHARACTER:
    return Type::getType<char>();
  case WIDE_CHARACTER:
    return Type::getType<wchar>();
  case BOOLEAN:
    return Type::getType<bool>();
  case SHORT_INTEGER:
    return Type::getType<short>();
  case UNSIGNED_SHORT_INTEGER:
    return Type::getType<unsigned short>();
  case INTEGER:
    return Type::getType<int>();
  case UNSIGNED_INTEGER:
    return Type::getType<unsigned int>();
  case LONG_INTEGER:
    return Type::getType<long>();
  case UNSIGNED_LONG_INTEGER:
    return Type::getType<unsigned long>();
  case LONG_LONG_INTEGER:
    return Type::getType<long long>();
  case UNSIGNED_LONG_LONG_INTEGER:
    return Type::getType<unsigned long long>();
  case FLOAT:
    return Type::getType<float>();
  case DOUBLE:
    return Type::getType<double>();
  case LONG_DOUBLE:
    return Type::getType<long double>();
  case STRING:
    return Type::getType<String>();
  case WIDE_STRING:
    return Type::getType<WideString>();
  default:
    INVALID_CONTROL_FLOW();
    return Type(); // uninitialized    
  }
}

bool AnyValue::isInteger() const noexcept {
  switch (representation) {
  case SHORT_INTEGER:
  case UNSIGNED_SHORT_INTEGER:
  case INTEGER:
  case UNSIGNED_INTEGER:
  case LONG_INTEGER:
  case UNSIGNED_LONG_INTEGER:
  case LONG_LONG_INTEGER:
  case UNSIGNED_LONG_LONG_INTEGER:
    return true;
  default:
    return false;
  }
}

bool AnyValue::isUnsigned() const noexcept {
  switch (representation) {
  case VOID:
  case CHARACTER:
  case WIDE_CHARACTER:
  case BOOLEAN:
  case UNSIGNED_SHORT_INTEGER:
  case UNSIGNED_INTEGER:
  case UNSIGNED_LONG_INTEGER:
  case UNSIGNED_LONG_LONG_INTEGER:
  case STRING:
  case WIDE_STRING:
    return true;
  default:
    return false;
  }
}

bool AnyValue::isFloatingPoint() const noexcept {
  switch (representation) {
  case FLOAT:
  case DOUBLE:
  case LONG_DOUBLE:
    return true;
  default:
    return false;
  }
}

bool AnyValue::isText() const noexcept {
  switch (representation) {
  case CHARACTER:
  case WIDE_CHARACTER:
  case STRING:
  case WIDE_STRING:
    return true;
  default:
    return false;
  }
}



AnyValue& AnyValue::operator=(const Type& value) throw() {
  reset();
  type = value;
  representation = TYPE;
  return *this;
}

AnyValue& AnyValue::operator=(void* value) throw() {
  reset();
  p = value;
  representation = POINTER;
  return *this;
}

AnyValue& AnyValue::operator=(char value) throw() {
  reset();
  character = value;
  representation = CHARACTER;
  return *this;
}

AnyValue& AnyValue::operator=(wchar value) throw() {
  reset();
  wideCharacter = value;
  representation = WIDE_CHARACTER;
  return *this;
}

AnyValue& AnyValue::operator=(bool value) throw() {
  reset();
  boolean = value;
  representation = BOOLEAN;
  return *this;
}

AnyValue& AnyValue::operator=(short value) throw() {
  reset();
  shortInteger = value;
  representation = SHORT_INTEGER;
  return *this;
}

AnyValue& AnyValue::operator=(unsigned short value) throw() {
  reset();
  unsignedShortInteger = value;
  representation = UNSIGNED_SHORT_INTEGER;
  return *this;
}

AnyValue& AnyValue::operator=(int value) throw() {
  reset();
  integer = value;
  representation = INTEGER;
  return *this;
}

AnyValue& AnyValue::operator=(unsigned int value) throw() {
  reset();
  unsignedInteger = value;
  representation = UNSIGNED_INTEGER;
  return *this;
}

AnyValue& AnyValue::operator=(long value) throw() {
  reset();
  longInteger = value;
  representation = LONG_INTEGER;
  return *this;
}

AnyValue& AnyValue::operator=(unsigned long value) throw() {
  reset();
  unsignedLongInteger = value;
  representation = UNSIGNED_LONG_INTEGER;
  return *this;
}

AnyValue& AnyValue::operator=(long long value) throw() {
  reset();
  longLongInteger = value;
  representation = LONG_LONG_INTEGER;
  return *this;
}

AnyValue& AnyValue::operator=(unsigned long long value) throw() {
  reset();
  unsignedLongLongInteger = value;
  representation = UNSIGNED_LONG_LONG_INTEGER;
  return *this;
}

AnyValue& AnyValue::operator=(float value) throw() {
  reset();
  f = value;
  representation = FLOAT;
  return *this;
}

AnyValue& AnyValue::operator=(double value) throw() {
  reset();
  d = value;
  representation = DOUBLE;
  return *this;
}

AnyValue& AnyValue::operator=(long double value) throw() {
  reset();
  ld = value;
  representation = LONG_DOUBLE;
  return *this;
}

AnyValue& AnyValue::operator=(const String& value) throw() {
  switch (representation) {
  case WIDE_STRING:
    wideString = WideString();
  default:
    string = value;
  }
  representation = STRING;
  return *this;
}

AnyValue& AnyValue::operator=(const WideString& value) throw() {
  switch (representation) {
  case STRING:
    string = String();
  default:
    wideString = value;
  }
  representation = WIDE_STRING;
  return *this;
}

AnyValue& AnyValue::operator=(const AnyReference& value) throw() {
  reset();
  reference = value;
  representation = REFERENCE;
  return *this;
}

char AnyValue::getChar() const throw() {
  switch (representation) {
  case VOID:
    return 0;
  case CHARACTER:
    return character;
  case WIDE_CHARACTER:
    return (wideCharacter <= 128) ? wideCharacter : 0; // TAG: if possible otherwise 0 - is type unsigned
  case BOOLEAN:
    return boolean ? 1 : 0;
  case SHORT_INTEGER:
    return shortInteger;
  case UNSIGNED_SHORT_INTEGER:
    return unsignedShortInteger;
  case INTEGER:
    return integer;
  case UNSIGNED_INTEGER:
    return unsignedInteger;
  case LONG_INTEGER:
    return longInteger;
  case UNSIGNED_LONG_INTEGER:
    return unsignedLongInteger;
  case LONG_LONG_INTEGER:
    return longLongInteger;
  case UNSIGNED_LONG_LONG_INTEGER:
    return unsignedLongLongInteger;
  case STRING:
    return string.isProper() ? string[0] : 0;
  case WIDE_STRING:
    if (wideString.isProper()) {
      wchar first = wideString[0];
      if (first >= 128) {
        first = 0;
      }
      return first;
    }
    return 0;
  default:
    return 0;
  }
}

wchar AnyValue::getWideChar() const throw() {
  switch (representation) {
  case VOID:
    return 0;
  case CHARACTER:
    return character; // TAG: what if >= 128
  case WIDE_CHARACTER:
    return wideCharacter;
  case BOOLEAN:
    return boolean ? 1 : 0;
  case SHORT_INTEGER:
    return shortInteger;
  case UNSIGNED_SHORT_INTEGER:
    return unsignedShortInteger;
  case INTEGER:
    return integer;
  case UNSIGNED_INTEGER:
    return unsignedInteger;
  case LONG_INTEGER:
    return longInteger;
  case UNSIGNED_LONG_INTEGER:
    return unsignedLongInteger;
  case LONG_LONG_INTEGER:
    return longLongInteger;
  case UNSIGNED_LONG_LONG_INTEGER:
    return unsignedLongLongInteger;
  case STRING:
    if (string.isProper()) {
      char first = string[0];
      if (first < 0) {
        first = 0;
      }
      return first;
    }
    return 0;
  case WIDE_STRING:
    return wideString.isProper() ? wideString[0] : 0;
  default:
    return 0;
  }
}

bool AnyValue::getBoolean() const throw() {
  switch (representation) {
  case VOID:
    return false;
  case CHARACTER:
    return character != 0;
  case WIDE_CHARACTER:
    return wideCharacter != 0;
  case BOOLEAN:
    return boolean;
  case SHORT_INTEGER:
    return shortInteger != 0;
  case UNSIGNED_SHORT_INTEGER:
    return unsignedShortInteger != 0;
  case INTEGER:
    return unsignedLongLongInteger != 0;
  case UNSIGNED_INTEGER:
    return unsignedLongLongInteger != 0;
  case LONG_INTEGER:
    return unsignedLongLongInteger != 0;
  case UNSIGNED_LONG_INTEGER:
    return unsignedLongLongInteger != 0;
  case LONG_LONG_INTEGER:
    return unsignedLongLongInteger != 0;
  case UNSIGNED_LONG_LONG_INTEGER:
    return unsignedLongLongInteger != 0;
  case STRING:
    return string.isProper();
  case WIDE_STRING:
    return wideString.isProper();
  default:
    return false;
  }
}

short AnyValue::getShortInteger() const throw() {
  switch (representation) {
  case VOID:
    return 0;
  case CHARACTER:
    return character;
  case WIDE_CHARACTER:
    return wideCharacter;
  case BOOLEAN:
    return boolean ? 1 : 0;
  case SHORT_INTEGER:
    return shortInteger;
  case UNSIGNED_SHORT_INTEGER:
    return unsignedShortInteger;
  case INTEGER:
    return integer;
  case UNSIGNED_INTEGER:
    return unsignedInteger;
  case LONG_INTEGER:
    return longInteger;
  case UNSIGNED_LONG_INTEGER:
    return unsignedLongInteger;
  case LONG_LONG_INTEGER:
    return longLongInteger;
  case UNSIGNED_LONG_LONG_INTEGER:
    return unsignedLongLongInteger;
  case STRING:
    return ShortInteger::parse(string);
  case WIDE_STRING:
    return ShortInteger::parse(wideString.getMultibyteString());
  default:
    return 0;
  }
}

unsigned short AnyValue::getUnsignedShortInteger() const throw() {
  switch (representation) {
  case VOID:
    return 0;
  case CHARACTER:
    return character;
  case WIDE_CHARACTER:
    return wideCharacter;
  case BOOLEAN:
    return boolean ? 1 : 0;
  case SHORT_INTEGER:
    return shortInteger;
  case UNSIGNED_SHORT_INTEGER:
    return unsignedShortInteger;
  case INTEGER:
    return integer;
  case UNSIGNED_INTEGER:
    return unsignedInteger;
  case LONG_INTEGER:
    return longInteger;
  case UNSIGNED_LONG_INTEGER:
    return unsignedLongInteger;
  case LONG_LONG_INTEGER:
    return longLongInteger;
  case UNSIGNED_LONG_LONG_INTEGER:
    return unsignedLongLongInteger;
  case STRING:
    return ShortInteger::parse(string, true);
  case WIDE_STRING:
    return ShortInteger::parse(wideString.getMultibyteString(), true);
  default:
    return 0;
  }
}

int AnyValue::getInteger() const throw() {
  switch (representation) {
  case VOID:
    return 0;
  case CHARACTER:
    return character;
  case WIDE_CHARACTER:
    return wideCharacter;
  case BOOLEAN:
    return boolean ? 1 : 0;
  case SHORT_INTEGER:
    return shortInteger;
  case UNSIGNED_SHORT_INTEGER:
    return unsignedShortInteger;
  case INTEGER:
    return integer;
  case UNSIGNED_INTEGER:
    return unsignedInteger;
  case LONG_INTEGER:
    return longInteger;
  case UNSIGNED_LONG_INTEGER:
    return unsignedLongInteger;
  case LONG_LONG_INTEGER:
    return longLongInteger;
  case UNSIGNED_LONG_LONG_INTEGER:
    return unsignedLongLongInteger;
  case STRING:
    return Integer::parse(string);
  case WIDE_STRING:
    return Integer::parse(wideString.getMultibyteString());
  default:
    return 0;
  }
}

unsigned int AnyValue::getUnsignedInteger() const throw() {
  switch (representation) {
  case VOID:
    return 0;
  case CHARACTER:
    return character;
  case WIDE_CHARACTER:
    return wideCharacter;
  case BOOLEAN:
    return boolean ? 1 : 0;
  case SHORT_INTEGER:
    return shortInteger;
  case UNSIGNED_SHORT_INTEGER:
    return unsignedShortInteger;
  case INTEGER:
    return integer;
  case UNSIGNED_INTEGER:
    return unsignedInteger;
  case LONG_INTEGER:
    return longInteger;
  case UNSIGNED_LONG_INTEGER:
    return unsignedLongInteger;
  case LONG_LONG_INTEGER:
    return longLongInteger;
  case UNSIGNED_LONG_LONG_INTEGER:
    return unsignedLongLongInteger;
  case STRING:
    return Integer::parse(string, true);
  case WIDE_STRING:
    return Integer::parse(wideString.getMultibyteString(), true);
  default:
    return 0;
  }
}

long AnyValue::getLongInteger() const throw() {
  switch (representation) {
  case VOID:
    return 0;
  case CHARACTER:
    return character;
  case WIDE_CHARACTER:
    return wideCharacter;
  case BOOLEAN:
    return boolean ? 1 : 0;
  case SHORT_INTEGER:
    return shortInteger;
  case UNSIGNED_SHORT_INTEGER:
    return unsignedShortInteger;
  case INTEGER:
    return integer;
  case UNSIGNED_INTEGER:
    return unsignedInteger;
  case LONG_INTEGER:
    return longInteger;
  case UNSIGNED_LONG_INTEGER:
    return unsignedLongInteger;
  case LONG_LONG_INTEGER:
    return longLongInteger;
  case UNSIGNED_LONG_LONG_INTEGER:
    return unsignedLongLongInteger;
  case STRING:
    return Integer::parse(string); // TAG: make sure long and int are compatible
  case WIDE_STRING:
    return Integer::parse(wideString.getMultibyteString()); // TAG: make sure long and int are compatible
  default:
    return 0;
  }
}

unsigned long AnyValue::getUnsignedLongInteger() const throw() {
  switch (representation) {
  case VOID:
    return 0;
  case CHARACTER:
    return character;
  case WIDE_CHARACTER:
    return wideCharacter;
  case BOOLEAN:
    return boolean ? 1 : 0;
  case SHORT_INTEGER:
    return shortInteger;
  case UNSIGNED_SHORT_INTEGER:
    return unsignedShortInteger;
  case INTEGER:
    return integer;
  case UNSIGNED_INTEGER:
    return unsignedInteger;
  case LONG_INTEGER:
    return longInteger;
  case UNSIGNED_LONG_INTEGER:
    return unsignedLongInteger;
  case LONG_LONG_INTEGER:
    return longLongInteger;
  case UNSIGNED_LONG_LONG_INTEGER:
    return unsignedLongLongInteger;
  case STRING:
    return Integer::parse(string, true); // TAG: make sure long and int are compatible
  case WIDE_STRING:
    return Integer::parse(wideString.getMultibyteString(), true); // TAG: make sure long and int are compatible
  default:
    return 0;
  }
}

long long AnyValue::getLongLongInteger() const throw() {
  switch (representation) {
  case VOID:
    return 0;
  case CHARACTER:
    return character;
  case WIDE_CHARACTER:
    return wideCharacter;
  case BOOLEAN:
    return boolean ? 1 : 0;
  case SHORT_INTEGER:
    return shortInteger;
  case UNSIGNED_SHORT_INTEGER:
    return unsignedShortInteger;
  case INTEGER:
    return integer;
  case UNSIGNED_INTEGER:
    return unsignedInteger;
  case LONG_INTEGER:
    return longInteger;
  case UNSIGNED_LONG_INTEGER:
    return unsignedLongInteger;
  case LONG_LONG_INTEGER:
    return longLongInteger;
  case UNSIGNED_LONG_LONG_INTEGER:
    return unsignedLongLongInteger;
  case STRING:
    return LongInteger::parse(string);
  case WIDE_STRING:
    return LongInteger::parse(wideString.getMultibyteString());
  default:
    return 0;
  }
}

unsigned long long AnyValue::getUnsignedLongLongInteger() const throw() {
  switch (representation) {
  case VOID:
    return 0;
  case CHARACTER:
    return character;
  case WIDE_CHARACTER:
    return wideCharacter;
  case BOOLEAN:
    return boolean ? 1 : 0;
  case SHORT_INTEGER:
    return shortInteger;
  case UNSIGNED_SHORT_INTEGER:
    return unsignedShortInteger;
  case INTEGER:
    return integer;
  case UNSIGNED_INTEGER:
    return unsignedInteger;
  case LONG_INTEGER:
    return longInteger;
  case UNSIGNED_LONG_INTEGER:
    return unsignedLongInteger;
  case LONG_LONG_INTEGER:
    return longLongInteger;
  case UNSIGNED_LONG_LONG_INTEGER:
    return unsignedLongLongInteger;
  case STRING:
    return LongInteger::parse(string, true);
  case WIDE_STRING:
    return LongInteger::parse(wideString.getMultibyteString(), true);
  default:
    return 0;
  }
}

String AnyValue::getString() const throw() {
  switch (representation) {
  case VOID:
    return String();
  case TYPE:
    return TypeInfo::getTypename(Type::makeType(type));
  case CHARACTER:
    return String(&character, 1);
  case WIDE_CHARACTER:
    return toUTF8(&wideCharacter, 1); // multi-byte character string
  case BOOLEAN:
    return String(boolean ? MESSAGE("true") : MESSAGE("false"));
  case STRING:
    return string;
  case WIDE_STRING:
    return wideString.getMultibyteString(); // multi-byte character string
  default:
    break;
  }

  StringOutputStream stream;
  switch (representation) {
  case POINTER:
    stream << p;
    break;
  case SHORT_INTEGER:
    stream << integer;
    break;
  case INTEGER:
    stream << integer;
    break;
  case UNSIGNED_INTEGER:
    stream << unsignedInteger;
    break;
  case LONG_INTEGER:
    stream << longInteger;
    break;
  case UNSIGNED_LONG_INTEGER:
    stream << unsignedLongInteger;
    break;
  case LONG_LONG_INTEGER:
    stream << longLongInteger;
    break;
  case UNSIGNED_LONG_LONG_INTEGER:
    stream << unsignedLongLongInteger;
    break;
  case FLOAT:
    stream << f;
    break;
  case DOUBLE:
    stream << d;
    break;
  case LONG_DOUBLE:
    stream << ld;
    break;
  case REFERENCE:
    stream << reference;
    break;
  default:
    INVALID_CONTROL_FLOW();
  }
  stream << FLUSH;
  return stream.getString();
}

WideString AnyValue::getWideString() const throw() {
  switch (representation) {
  case VOID:
    return WideString();
  case TYPE:
    return TypeInfo::getTypename(Type::makeType(type));
  case CHARACTER:
    return WideString(&character, 1);
  case WIDE_CHARACTER:
    return WideString(&wideCharacter, 1);
  case BOOLEAN:
    return WideString(boolean ? WIDEMESSAGE("true") : WIDEMESSAGE("false"));
  case STRING:
    return string;
  case WIDE_STRING:
    return wideString;
  default:
    break;
  }
  
  StringOutputStream stream;
  switch (representation) {
  case POINTER:
    stream << p;
    break;
  case SHORT_INTEGER:
    stream << integer;
    break;
  case INTEGER:
    stream << integer;
    break;
  case UNSIGNED_INTEGER:
    stream << unsignedInteger;
    break;
  case LONG_INTEGER:
    stream << longInteger;
    break;
  case UNSIGNED_LONG_INTEGER:
    stream << unsignedLongInteger;
    break;
  case LONG_LONG_INTEGER:
    stream << longLongInteger;
    break;
  case UNSIGNED_LONG_LONG_INTEGER:
    stream << unsignedLongLongInteger;
    break;
  case FLOAT:
    stream << f;
    break;
  case DOUBLE:
    stream << d;
    break;
  case LONG_DOUBLE:
    stream << ld;
    break;
  case REFERENCE:
    stream << reference;
    break;
  default:
    INVALID_CONTROL_FLOW();
  }
  stream << FLUSH;
  return stream.getString();
}

AnyReference AnyValue::getReference() noexcept
{
  return (representation == REFERENCE) ? reference : nullptr;
}



void AnyValue::setType(const Type& value) throw() {
  reset();
  type = value;
  representation = TYPE;
}

void AnyValue::setPointer(void* value) throw() {
  reset();
  p = value;
  representation = POINTER;
}

void AnyValue::setChar(char value) throw() {
  reset();
  character = value;
  representation = CHARACTER;
}

void AnyValue::setWideChar(wchar value) throw() {
  reset();
  wideCharacter = value;
  representation = WIDE_CHARACTER;
}

void AnyValue::setBoolean(bool value) throw() {
  reset();
  boolean = value;
  representation = BOOLEAN;
}

void AnyValue::setShortInteger(short value) throw() {
  reset();
  shortInteger = value;
  representation = SHORT_INTEGER;
}

void AnyValue::setUnsignedShortInteger(unsigned short value) throw() {
  reset();
  unsignedShortInteger = value;
  representation = UNSIGNED_SHORT_INTEGER;
}

void AnyValue::setInteger(int value) throw() {
  reset();
  integer = value;
  representation = INTEGER;
}

void AnyValue::setUnsignedInteger(unsigned int value) throw() {
  reset();
  unsignedInteger = value;
  representation = UNSIGNED_INTEGER;
}

void AnyValue::setLongInteger(long value) throw() {
  reset();
  longInteger = value;
  representation = LONG_INTEGER;
}

void AnyValue::setUnsignedLongInteger(unsigned long value) throw() {
  reset();
  unsignedLongInteger = value;
  representation = UNSIGNED_LONG_INTEGER;
}

void AnyValue::setLongLongInteger(long long value) throw() {
  reset();
  longLongInteger = value;
  representation = LONG_LONG_INTEGER;
}

void AnyValue::setUnsignedLongLongInteger(unsigned long long value) throw() {
  reset();
  unsignedLongLongInteger = value;
  representation = UNSIGNED_LONG_LONG_INTEGER;
}

void AnyValue::setString(const String& value) throw() {
  if (representation == STRING) {
    string = value;
    return;
  }
  reset();
  string = value;
  representation = STRING;
}

void AnyValue::setWideString(const WideString& value) throw() {
  if (representation == WIDE_STRING) {
    wideString = value;
    return;
  }
  reset();
  wideString = value;
  representation = WIDE_STRING;
}

void AnyValue::setReference(const AnyReference& value) throw() {
  if (representation == REFERENCE) {
    reference = value;
    return;
  }
  reset();
  reference = value;
  representation = REFERENCE;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const AnyValue& value) throw(IOException)
{
  switch (value.representation) {
  case AnyValue::TYPE:
    stream << TypeInfo::getTypename(Type::makeType(value.type));
    break;
  case AnyValue::POINTER:
    stream << value.p;
    break;
  case AnyValue::CHARACTER:
    stream << value.character;
    break;
  case AnyValue::WIDE_CHARACTER:
    stream << value.wideCharacter;
    break;
  case AnyValue::BOOLEAN:
    stream << value.boolean;
    break;
  case AnyValue::SHORT_INTEGER:
    stream << value.shortInteger;
    break;
  case AnyValue::UNSIGNED_SHORT_INTEGER:
    stream << value.unsignedShortInteger;
    break;
  case AnyValue::INTEGER:
    stream << value.integer;
    break;
  case AnyValue::UNSIGNED_INTEGER:
    stream << value.unsignedInteger;
    break;
  case AnyValue::LONG_INTEGER:
    stream << value.longInteger;
    break;
  case AnyValue::UNSIGNED_LONG_INTEGER:
    stream << value.unsignedLongInteger;
    break;
  case AnyValue::LONG_LONG_INTEGER:
    stream << value.longLongInteger;
    break;
  case AnyValue::UNSIGNED_LONG_LONG_INTEGER:
    stream << value.unsignedLongLongInteger;
    break;
  case AnyValue::FLOAT:
    stream << value.f;
    break;
  case AnyValue::DOUBLE:
    stream << value.d;
    break;
  case AnyValue::LONG_DOUBLE:
    stream << value.ld;
    break;
  case AnyValue::STRING:
    stream << value.string;
    break;
  case AnyValue::WIDE_STRING:
    stream << value.wideString;
    break;
  case AnyValue::REFERENCE:
    stream << value.reference;
    break;
  default:
    INVALID_CONTROL_FLOW();
  }
  return stream;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
