/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

AnyValue::AnyValue() throw() : representation(VOID) {
}

AnyValue::AnyValue(char value) throw() : representation(CHARACTER), character(value) {
}

AnyValue::AnyValue(wchar value) throw() : representation(WIDE_CHARACTER), wideCharacter(value) {
}

AnyValue::AnyValue(bool value) throw() : representation(BOOLEAN), boolean(value) {
}

AnyValue::AnyValue(short value) throw() : representation(INTEGER), shortInteger(value) {
}

AnyValue::AnyValue(unsigned short value) throw() : representation(UNSIGNED_SHORT_INTEGER), unsignedShortInteger(value) {
}

AnyValue::AnyValue(int value) throw() : representation(INTEGER), integer(value) {
}

AnyValue::AnyValue(unsigned int value) throw() : representation(UNSIGNED_INTEGER), unsignedInteger(value) {
}

AnyValue::AnyValue(long value) throw() : representation(LONG_INTEGER), longInteger(value) {
}

AnyValue::AnyValue(unsigned long value) throw() : representation(UNSIGNED_LONG_INTEGER), unsignedLongInteger(value) {
}

AnyValue::AnyValue(long long value) throw() : representation(LONG_LONG_INTEGER), longLongInteger(value) {
}

AnyValue::AnyValue(unsigned long long value) throw() : representation(UNSIGNED_LONG_LONG_INTEGER), unsignedLongLongInteger(value) {
}

AnyValue::AnyValue(const String& value) throw() : representation(STRING), string(value) {
}

AnyValue::AnyValue(const WideString& value) throw() : representation(WIDE_STRING), wideString(value) {
}

AnyValue::AnyValue(const AnyValue& copy) throw()
  : representation(copy.representation) {
  switch (representation) {
  case VOID:
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
  case STRING:
    string = copy.string;
    break;
  case WIDE_STRING:
    wideString = copy.wideString;
    break;
  }
}

AnyValue& AnyValue::operator=(const AnyValue& eq) throw() {
  if (&eq != this) { // only if not self-assignment
    switch (representation) {
    case STRING:
      if (eq.representation != STRING) {
        string = String();
      }
      break;
    case WIDE_STRING:
      if (eq.representation != WIDE_STRING) {
        wideString = WideString();
      }
      break;
    }
    representation = eq.representation;
    switch (representation) {
    case VOID:
    case CHARACTER:
      character = eq.character;
      break;
    case WIDE_CHARACTER:
      wideCharacter = eq.wideCharacter;
      break;
    case BOOLEAN:
      boolean = eq.boolean;
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
    case STRING:
      string = eq.string;
      break;
    case WIDE_STRING:
      wideString = eq.wideString;      
      break;
    }
  }
  return *this;
}

Type AnyValue::getType() const throw() {
  switch (representation) {
  case VOID:
    return Type(); // uninitialized
  case CHARACTER:
    return Type::getType<char>();
  case WIDE_CHARACTER:
    return Type::getType<wchar>();
  case BOOLEAN:
    return Type::getType<bool>();
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
  case STRING:
    return Type::getType<String>();
  case WIDE_STRING:
    return Type::getType<WideString>();
  }
}

bool AnyValue::isInteger() const throw() {
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

bool AnyValue::isUnsigned() const throw() {
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

bool AnyValue::isText() const throw() {
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



AnyValue& AnyValue::operator=(char value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  character = value;
  representation = CHARACTER;
  return *this;
}

AnyValue& AnyValue::operator=(wchar value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  wideCharacter = value;
  representation = WIDE_CHARACTER;
  return *this;
}

AnyValue& AnyValue::operator=(bool value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  boolean = value;
  representation = BOOLEAN;
  return *this;
}

AnyValue& AnyValue::operator=(short value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  shortInteger = value;
  representation = SHORT_INTEGER;
  return *this;
}

AnyValue& AnyValue::operator=(unsigned short value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  unsignedShortInteger = value;
  representation = UNSIGNED_SHORT_INTEGER;
  return *this;
}

AnyValue& AnyValue::operator=(int value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  integer = value;
  representation = INTEGER;
  return *this;
}

AnyValue& AnyValue::operator=(unsigned int value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  unsignedInteger = value;
  representation = UNSIGNED_INTEGER;
  return *this;
}

AnyValue& AnyValue::operator=(long value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  longInteger = value;
  representation = LONG_INTEGER;
  return *this;
}

AnyValue& AnyValue::operator=(unsigned long value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  unsignedLongInteger = value;
  representation = UNSIGNED_LONG_INTEGER;
  return *this;
}

AnyValue& AnyValue::operator=(long long value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  longLongInteger = value;
  representation = LONG_LONG_INTEGER;
  return *this;
}

AnyValue& AnyValue::operator=(unsigned long long value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  unsignedLongLongInteger = value;
  representation = UNSIGNED_LONG_LONG_INTEGER;
  return *this;
}

AnyValue& AnyValue::operator=(const String& value) throw() {
  switch (representation) {
  case WIDE_STRING:
    wideString = WideString();
  case VOID:
  case CHARACTER:
  case WIDE_CHARACTER:
  case BOOLEAN:
  case SHORT_INTEGER:
  case UNSIGNED_SHORT_INTEGER:
  case INTEGER:
  case UNSIGNED_INTEGER:
  case LONG_INTEGER:
  case UNSIGNED_LONG_INTEGER:
  case LONG_LONG_INTEGER:
  case UNSIGNED_LONG_LONG_INTEGER:
  case STRING:
    string = value;
  }
  representation = STRING;
  return *this;
}

AnyValue& AnyValue::operator=(const WideString& value) throw() {
  switch (representation) {
  case STRING:
    string = String();
  case VOID:
  case CHARACTER:
  case WIDE_CHARACTER:
  case BOOLEAN:
  case SHORT_INTEGER:
  case UNSIGNED_SHORT_INTEGER:
  case INTEGER:
  case UNSIGNED_INTEGER:
  case LONG_INTEGER:
  case UNSIGNED_LONG_INTEGER:
  case LONG_LONG_INTEGER:
  case UNSIGNED_LONG_LONG_INTEGER:
  case WIDE_STRING:
    wideString = value;
  }
  representation = WIDE_STRING;
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
  }
}

String AnyValue::getString() const throw() {
  switch (representation) {
  case VOID:
    return String();
  case CHARACTER:
    return String(&character, 1);
  case WIDE_CHARACTER:
    return String(); // String(&wideCharacter, 1); // multi-byte character string
  case BOOLEAN:
    {
      StringOutputStream stream;
      stream << boolean << FLUSH;
      return stream.getString();
    }
  case STRING:
    return string;
  case WIDE_STRING:
    return wideString.getMultibyteString(); // multi-byte character string
  }

  StringOutputStream stream;
  switch (representation) {
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
  }
  stream << FLUSH;
  return stream.getString();
}

WideString AnyValue::getWideString() const throw() {
  switch (representation) {
  case VOID:
    return WideString();
  case CHARACTER:
    return WideString(&character, 1);
  case WIDE_CHARACTER:
    return WideString(&wideCharacter, 1);
  case BOOLEAN:
    return boolean ? WideString(WIDEMESSAGE("false")) : WideString(WIDEMESSAGE("false")); // TAG: locale support
  case STRING:
    return string;
  case WIDE_STRING:
    return wideString;
  }
  
  StringOutputStream stream;
  switch (representation) {
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
  }
  stream << FLUSH;
  return stream.getString();
}



void AnyValue::setChar(char value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  character = value;
  representation = CHARACTER;
}

void AnyValue::setWideChar(wchar value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  wideCharacter = value;
  representation = WIDE_CHARACTER;
}

void AnyValue::setBoolean(bool value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  boolean = value;
  representation = BOOLEAN;
}

void AnyValue::setShortInteger(short value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  shortInteger = value;
  representation = SHORT_INTEGER;
}

void AnyValue::setUnsignedShortInteger(unsigned short value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  unsignedShortInteger = value;
  representation = UNSIGNED_SHORT_INTEGER;
}

void AnyValue::setInteger(int value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  integer = value;
  representation = INTEGER;
}

void AnyValue::setUnsignedInteger(unsigned int value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  unsignedInteger = value;
  representation = UNSIGNED_INTEGER;
}

void AnyValue::setLongInteger(long value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  longInteger = value;
  representation = LONG_INTEGER;
}

void AnyValue::setUnsignedLongInteger(unsigned long value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  unsignedLongInteger = value;
  representation = UNSIGNED_LONG_INTEGER;
}

void AnyValue::setLongLongInteger(long long value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  longLongInteger = value;
  representation = LONG_LONG_INTEGER;
}

void AnyValue::setUnsignedLongLongInteger(unsigned long long value) throw() {
  switch (representation) {
  case STRING:
    string = String();
    break;
  case WIDE_STRING:
    wideString = WideString();
    break;
  }
  unsignedLongLongInteger = value;
  representation = UNSIGNED_LONG_LONG_INTEGER;
}

void AnyValue::setString(const String& value) throw() {
  switch (representation) {
  case WIDE_STRING:
    wideString = String();
  default:
    string = value;
  }
  representation = STRING;
}

void AnyValue::setWideString(const WideString& value) throw() {
  switch (representation) {
  case STRING:
    string = String();
  default:
    wideString = value;
  }
  representation = WIDE_STRING;
}


FormatOutputStream& operator<<(FormatOutputStream& stream, const AnyValue& value) throw(IOException) {
  switch (value.representation) {
  case AnyValue::CHARACTER:
    return stream << value.character;
  case AnyValue::WIDE_CHARACTER:
    return stream << value.wideCharacter;
  case AnyValue::BOOLEAN:
    return stream << value.boolean;
  case AnyValue::SHORT_INTEGER:
    return stream << value.shortInteger;
  case AnyValue::UNSIGNED_SHORT_INTEGER:
    return stream << value.unsignedShortInteger;
  case AnyValue::INTEGER:
    return stream << value.integer;
  case AnyValue::UNSIGNED_INTEGER:
    return stream << value.unsignedInteger;
  case AnyValue::LONG_INTEGER:
    return stream << value.longInteger;
  case AnyValue::UNSIGNED_LONG_INTEGER:
    return stream << value.unsignedLongInteger;
  case AnyValue::LONG_LONG_INTEGER:
    return stream << value.longLongInteger;
  case AnyValue::UNSIGNED_LONG_LONG_INTEGER:
    return stream << value.unsignedLongLongInteger;
  case AnyValue::STRING:
    return stream << value.string;
  case AnyValue::WIDE_STRING:
    return stream << value.wideString;
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
