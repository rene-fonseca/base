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

#include <base/Object.h>
#include <base/Primitives.h>
#include <base/Type.h>
#include <base/string/String.h>
#include <base/string/WideString.h>
#include <base/string/InvalidFormat.h>
#include <base/Literal.h>
#include <base/WideLiteral.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Any value. Containers to commonly used types.
  
  @short Any value
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API AnyValue : public ReferenceCountedObject {
  friend _COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(
    FormatOutputStream& stream, const AnyValue& value) throw(IOException);
public:

  /** The supported types. */
  enum Representation {
    VOID, /**< Void. */
    TYPE, /**< Type. */
    POINTER, /** Pointer. */
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
    FLOAT, /** Float primitive. */
    DOUBLE, /** Double primitive. */
    LONG_DOUBLE, /** Long double primitive. */
    STRING, /**< Sting. */
    WIDE_STRING, /**< Wide string. */
    REFERENCE /** Reference counted object. */
  };
private:

  /** Specifies the representation of the value. */
  Representation representation = VOID;
  
  /** Container for primitives. */
  union {
    void* p;
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
    float f;
    double d;
    long double ld;
    const std::type_info* type;
  };
  /** Container for normal string. */
  String string;
  /** Container for wide string. */
  WideString wideString;
  /** Reference counted object. */
  AnyReference reference; // it would be ok to use native pointer in this case - could be shared with string types potentially

  /** Release all non-primitive types. */
  inline void reset() {
    switch (representation) {
    case STRING:
      string = String();
      break;
    case WIDE_STRING:
      wideString = WideString();
      break;
    case REFERENCE:
      reference = nullptr;
      break;
    default:
      break;
    }
    representation = VOID;
  }
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
  inline Representation getRepresentation() const noexcept {
    return representation;
  }
  
  /**
    Returns the type of the internal representation.

    @return An uninitialized type object is returned if the representation is VOID.
  */
  Type getRepresentationType() const noexcept;
  
  /**
    Returns true if the representation is an integer type.
  */
  bool isInteger() const noexcept;

  /**
    Returns true if the representation is an unsigned type.
  */
  bool isUnsigned() const noexcept;
  
  /**
    Returns true if the representation is a floating point type.
  */
  bool isFloatingPoint() const noexcept;

  /**
    Returns true if the value represented by character(s).
  */
  bool isText() const noexcept;
  
  /**
    Initializes value as type.
  */
  AnyValue(const Type& value) throw();

  /**
    Initializes value as character.
  */
  AnyValue(void* value) throw();

  /**
    Initializes value as string.
  */
  AnyValue(const char* value) throw();

  /**
    Initializes value as wide string.
  */
  AnyValue(const wchar* value) throw();

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
    Initializes value as float.
  */
  AnyValue(float value) throw();

  /**
    Initializes value as double.
  */
  AnyValue(double value) throw();

  /**
    Initializes value as long double.
  */
  AnyValue(long double value) throw();

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
    Initializes value as reference.
  */
  AnyValue(const AnyReference& value) throw();

  /**
    Sets the value as a type.
  */
  AnyValue& operator=(const Type& value) throw();

  /**
    Sets the value as a character.
  */
  AnyValue& operator=(void* value) throw();

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
    Sets the value as a float.
  */
  AnyValue& operator=(float value) throw();

  /**
    Sets the value as a double.
  */
  AnyValue& operator=(double value) throw();

  /**
    Sets the value as a long double.
  */
  AnyValue& operator=(long double value) throw();

  /**
    Sets the value as a string.
  */
  AnyValue& operator=(const String& value) throw();

  /**
    Sets the value as a wide string.
  */
  AnyValue& operator=(const WideString& value) throw();
  
  /**
    Sets the value as a reference.
  */
  AnyValue& operator=(const AnyReference& value) throw();

  /**
    Returns value as a type.
  */
  const Type& getType() const throw();

  /**
    Returns value as a pointer.
  */
  void* getPointer() const throw();

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
    Returns value as a float.
  */
  float getFloat() const throw();

  /**
    Returns value as a double.
  */
  double getDouble() const throw();

  /**
    Returns value as a float.
  */
  long double getLongDouble() const throw();

  /**
    Returns value as a string.
  */
  String getString() const throw();

  /**
    Returns the value as a wide string.
  */
  WideString getWideString() const throw();

  /**
    Returns the reference.
  */
  AnyReference getReference() noexcept;

  /**
    Sets the value as a type.
  */
  void setType(const Type& value) throw();

  /**
    Sets the value as a pointer.
  */
  void setPointer(void* value) throw();

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

  /**
    Sets the value as a reference.
  */
  void setReference(const AnyReference& value) throw();
};

/**
  Writes any-value object to format output stream.
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const AnyValue& value) throw(IOException);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
