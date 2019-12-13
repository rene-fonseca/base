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

#include <base/string/String.h>
#include <base/string/WideString.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Object Request Broker (ORB) response to a method invocation.
  
  @short Object Request Broker (ORB) response.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OrbResponse : public Object {
public:

  /** Response type. */
  enum ResponseType {
    EXCEPTION, /**< Exception. */
    REFERENCE, /**< Object reference. */
    CHARACTER, /**< Character. */
    WIDE_CHARACTER, /**< Wide character. */
    STRING, /**< String. */
    WIDE_STRING, /**< Wide string. */
    OCTET, /**< Octet primitive. */
    SHORT, /**< Short integer primitive. */
    UNSIGNED_SHORT, /**< Unsigned short integer primitive. */
    INT, /**< Integer primitive. */
    UNSIGNED_INT, /**< Unsigned integer primitive. */
    LONG, /**< Long integer primitive. */
    UNSIGNED_LONG, /**< Unsigned long integer primitive. */
    LONG_LONG, /**< Long long integer primitive. */
    UNSIGNED_LONG_LONG, /**< Unsigned long long integer primitive. */
    ENUMERATION, /**< Enumeration. */
    STRUCTURE, /**< Structure. */
    OBJECT /**< Object. */
  };
private:

  /** The type of the response. */
  ResponseType responseType = OCTET;
  union {
    uint64 reference;
    uint8 octetValue;
    char character;
    ucs2 wideCharacter;
    short shortValue;
    unsigned short unsignedShortValue;
    int intValue;
    unsigned int unsignedIntValue;
    long longValue;
    unsigned long unsignedLongValue;
    long long longLongValue;
    unsigned long long unsignedLongLongValue;
  };
  String string;
  WideString wideString;
  // object;
public:
  
  /**
    Initializes the response.
  */
  inline OrbResponse(uint8 value) noexcept : octet(value), responseType(OCTET) {
  }
  
  /**
    Returns the response type.
  */
  inline ResponseType getResponseType() const noexcept {
    return responseType;
  }
  
  inline uint8 getOctet() const noexcept {
    return octet;
  }

  inline char getCharacter() const noexcept {
    return character;
  }
  
  inline ucs2 getWideCharacter() const noexcept {
    return wideCharacter;
  }
  
  inline const String& getString() const noexcept {
    return string;
  }
  
  inline const WideString& getWideString() const noexcept {
    return wideString;
  }
  
  // set value
  
  // read response() {
  // }
  
  // write response() {
  // }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
