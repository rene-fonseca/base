/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__ANY_VALUE_H
#define _DK_SDU_MIP__BASE__ANY_VALUE_H

#include <base/Object.h>
#include <base/Primitives.h>
#include <base/Type.h>
#include <base/string/String.h>
#include <base/string/WideString.h>
#include <base/string/InvalidFormat.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Any value.
  
  @short Any value
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class AnyValue : public Object {
public:

  /** The supported types. */
  enum Representation {
    VOID, /**< Void. */
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
  Representation representation;
  
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
  /** Container for normal string. */
  String string;
  /** Container for wide string. */
  WideString wideString;
public:
  
  /**
    Initializes type as void.
  */
  inline AnyValue() throw();
  
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
  Type getType() const throw();

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
  
  AnyValue(char value) throw();
  AnyValue(wchar value) throw();
  AnyValue(bool value) throw();
  AnyValue(short value) throw();
  AnyValue(unsigned short value) throw();
  AnyValue(int value) throw();
  AnyValue(unsigned int value) throw();
  AnyValue(long value) throw();
  AnyValue(unsigned long value) throw();
  AnyValue(long long value) throw();
  AnyValue(unsigned long long value) throw();
  AnyValue(const String& value) throw();
  AnyValue(const WideString& value) throw();
  
  AnyValue& operator=(char value) throw();
  AnyValue& operator=(wchar value) throw();
  AnyValue& operator=(bool value) throw();
  AnyValue& operator=(short value) throw();
  AnyValue& operator=(unsigned short value) throw();
  AnyValue& operator=(int value) throw();
  AnyValue& operator=(unsigned int value) throw();
  AnyValue& operator=(long value) throw();
  AnyValue& operator=(unsigned long value) throw();
  AnyValue& operator=(long long value) throw();
  AnyValue& operator=(unsigned long long value) throw();
  AnyValue& operator=(const String& value) throw();
  AnyValue& operator=(const WideString& value) throw();
  
  char getChar() const throw();
  wchar getWideChar() const throw();
  bool getBoolean() const throw();
  short getShortInteger() const throw();
  unsigned short getUnsignedShortInteger() const throw();
  int getInteger() const throw();
  unsigned int getUnsignedInteger() const throw();
  long getLongInteger() const throw();
  unsigned long getUnsignedLongInteger() const throw();
  long long getLongLongInteger() const throw();
  unsigned long long getUnsignedLongLongInteger() const throw();
  String getString() const throw();
  WideString getWideString() const throw();

  void setChar(char value) throw();
  void setWideChar(wchar value) throw();
  void setBoolean(bool value) throw();
  void setShortInteger(short value) throw();
  void setUnsignedShortInteger(unsigned short value) throw();
  void setInteger(int value) throw();
  void setUnsignedInteger(unsigned int value) throw();
  void setLongInteger(long value) throw();
  void setUnsignedLongInteger(unsigned long value) throw();
  void setLongLongInteger(long long value) throw();
  void setUnsignedLongLongInteger(unsigned long long value) throw();
  void setString(const String& value) throw();
  void setWideString(const WideString& value) throw();
  
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const AnyValue& value) throw(IOException);
};

/**
  Writes any-value object to format output stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const AnyValue& value) throw(IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
