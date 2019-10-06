/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/Primitives.h>
#include <base/Type.h>
#include <base/string/String.h>
#include <base/string/WideString.h>
#include <base/string/InvalidFormat.h>
#include <base/Literal.h>
#include <base/WideLiteral.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Any value.
  
  @short Any value
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class AnyValue : public Object {
  friend FormatOutputStream& operator<<(
    FormatOutputStream& stream, const AnyValue& value) throw(IOException);
public:

  /** The supported types. */
  enum Representation {
    VOID, /**< Void. */
    TYPE, /**< Type. */
    CHARACTER, /**< Character primitive. */
    WIDE_CHARACTER, /**< Wide character primitive. */
    BOOLEAN, /**< Boolean primitive. */
    SHORT_INTEGER, /**< Short integer primitive. */
    UNSIGNED_SHORT_INTEGER, /**< Unsigned short integer primitive. */
    INTEGER, /**< Integer primitive. */
    UNSIGNED_INTEGER, /**< Unsigned integer primitive. */
    LONG_INTEGER, /**< Long integer primitive. */
    UNSIGNED_LONG_INTEGER, /**< Unsigned long integer primitive. */
    LONG_LONG_INTEGER, /**< Long long integer primitive. */
    UNSIGNED_LONG_LONG_INTEGER, /**< Unsigned long long integer primitive. */
    STRING, /**< Sting. */
    WIDE_STRING /**< Wide string. */
  };
private:

  /** Specifies the representation of the value. */
  Representation representation = VOID;
  
  /** Container for primitives. */
  union {
    char character;
    wchar wideCharacter;
    bool boolean;
    short shortInteger;
    unsigned short unsignedShortInteger;
    int integer;
    unsigned int unsignedInteger;
    long longInteger;
    unsigned long unsignedLongInteger;
    long long longLongInteger;
    unsigned long long unsignedLongLongInteger;
  };
  /** Container for type object. */
  Type type;
  /** Container for normal string. */
  String string;
  /** Container for wide string. */
  WideString wideString;
public:
  
  /**
    Initializes type as void.
  */
  AnyValue() throw();
  
  /**
    Initializes value as copy of other value.
  */
  AnyValue(const AnyValue& copy) throw();
  
  /**
    Assignment of object to object.
  */
  AnyValue& operator=(const AnyValue& eq) throw();
  
  /**
    Returns the internal representation of the value.
  */
  inline Representation getRepresentation() const throw() {
    return representation;
  }
  
  /**
    Returns the type of the internal representation.

    @return An uninitialized type object is returned if the representation is VOID.
  */
  Type getRepresentationType() const throw();
  
  /**
    Returns true if the representation is an integer type.
  */
  bool isInteger() const throw();

  /**
    Returns true if the representation is an unsigned type.
  */
  bool isUnsigned() const throw();

  /**
    Returns true if the value represented by character(s).
  */
  bool isText() const throw();
  
  /**
    Initializes value as type.
  */
  AnyValue(const Type& value) throw();
  
  /**
    Initializes value as character.
  */
  AnyValue(char value) throw();
  
  /**
    Initializes value as wide character.
  */
  AnyValue(wchar value) throw();

  /**
    Initializes value as boolean.
  */
  AnyValue(bool value) throw();

  /**
    Initializes value as short integer.
  */
  AnyValue(short value) throw();

  /**
    Initializes value as unsigned short integer.
  */
  AnyValue(unsigned short value) throw();

  /**
    Initializes value as integer.
  */
  AnyValue(int value) throw();

  /**
    Initializes value as unsigned integer.
  */
  AnyValue(unsigned int value) throw();

  /**
    Initializes value as long integer.
  */
  AnyValue(long value) throw();

  /**
    Initializes value as unsigned long integer.
  */
  AnyValue(unsigned long value) throw();

  /**
    Initializes value as long long integer.
  */
  AnyValue(long long value) throw();

  /**
    Initializes value as unsigned long long integer.
  */
  AnyValue(unsigned long long value) throw();

  /**
    Initializes value as string.
  */
  AnyValue(const String& value) throw();

  /**
    Initializes value as string.
  */
  AnyValue(const Literal& value) throw();
  
  /**
    Initializes value as wide string.
  */
  AnyValue(const WideString& value) throw();
  
  /**
    Initializes value as wide string.
  */
  AnyValue(const WideLiteral& value) throw();
  
  /**
    Sets the value as a type.
  */
  AnyValue& operator=(const Type& value) throw();
  
  /**
    Sets the value as a character.
  */
  AnyValue& operator=(char value) throw();

  /**
    Sets the value as a wide character.
  */
  AnyValue& operator=(wchar value) throw();

  /**
    Sets the value as a boolean.
  */
  AnyValue& operator=(bool value) throw();

  /**
    Sets the value as a short integer.
  */
  AnyValue& operator=(short value) throw();

  /**
    Sets the value as an unsigned short integer.
  */
  AnyValue& operator=(unsigned short value) throw();

  /**
    Sets the value as a integer.
  */
  AnyValue& operator=(int value) throw();

  /**
    Sets the value as an unsigned integer.
  */
  AnyValue& operator=(unsigned int value) throw();

  /**
    Sets the value as a long integer.
  */
  AnyValue& operator=(long value) throw();

  /**
    Sets the value as an unsigned long integer.
  */
  AnyValue& operator=(unsigned long value) throw();

  /**
    Sets the value as a long long integer.
  */
  AnyValue& operator=(long long value) throw();

  /**
    Sets the value as an unsigned long long integer.
  */
  AnyValue& operator=(unsigned long long value) throw();

  /**
    Sets the value as a string.
  */
  AnyValue& operator=(const String& value) throw();

  /**
    Sets the value as a wide string.
  */
  AnyValue& operator=(const WideString& value) throw();
  
  /**
    Returns value as a type.
  */
  Type getType() const throw();

  /**
    Returns value as a character.
  */
  char getChar() const throw();

  /**
    Returns value as a wide character.
  */
  wchar getWideChar() const throw();

  /**
    Returns value as a boolean.
  */
  bool getBoolean() const throw();

  /**
    Returns value as a short integer.
  */
  short getShortInteger() const throw();

  /**
    Returns value as an unsigned short integer.
  */
  unsigned short getUnsignedShortInteger() const throw();

  /**
    Returns value as a integer.
  */
  int getInteger() const throw();

  /**
    Returns value as an unsiged integer.
  */
  unsigned int getUnsignedInteger() const throw();

  /**
    Returns value as a long integer.
  */
  long getLongInteger() const throw();

  /**
    Returns value as an unsigned long integer.
  */
  unsigned long getUnsignedLongInteger() const throw();

  /**
    Returns value as a long long integer.
  */
  long long getLongLongInteger() const throw();

  /**
    Returns value as an unsigned long long integer.
  */
  unsigned long long getUnsignedLongLongInteger() const throw();

  /**
    Returns value as a string.
  */
  String getString() const throw();

  /**
    Returns the value as a wide string.
  */
  WideString getWideString() const throw();

  /**
    Sets the value as a type.
  */
  void setType(const Type& value) throw();
  
  /**
    Sets the value as a character.
  */
  void setChar(char value) throw();

  /**
    Sets the value as a wide character.
  */
  void setWideChar(wchar value) throw();

  /**
    Sets the value as a boolean.
  */
  void setBoolean(bool value) throw();

  /**
    Sets the value as a short integer.
  */
  void setShortInteger(short value) throw();

  /**
    Sets the value as an unsigned short integer.
  */
  void setUnsignedShortInteger(unsigned short value) throw();

  /**
    Sets the value as a integer.
  */
  void setInteger(int value) throw();

  /**
    Sets the value as an unsigned integer.
  */
  void setUnsignedInteger(unsigned int value) throw();

  /**
    Sets the value as a long integer.
  */
  void setLongInteger(long value) throw();

  /**
    Sets the value as an unsigned long integer.
  */
  void setUnsignedLongInteger(unsigned long value) throw();

  /**
    Sets the value as a long long integer.
  */
  void setLongLongInteger(long long value) throw();

  /**
    Sets the value as an unsigned long long integer.
  */
  void setUnsignedLongLongInteger(unsigned long long value) throw();

  /**
    Sets the value as a string.
  */
  void setString(const String& value) throw();

  /**
    Sets the value as a wide string.
  */
  void setWideString(const WideString& value) throw();
};

/**
  Writes any-value object to format output stream.
*/
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const AnyValue& value) throw(IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
