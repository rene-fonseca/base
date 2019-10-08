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

#include <base/string/String.h>
#include <base/string/WideString.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Object Request Broker (ORB) response to a method invocation.
  
  @short Object Request Broker (ORB) response.
  @ingroup rmi
  @version 1.0
*/

class OrbResponse : public Object {
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
  ResponseType responseType;
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
  inline OrbResponse(uint8 value) throw() : octet(value), responseType(OCTET) {
  }
  
  /**
    Returns the response type.
  */
  inline ResponseType getResponseType() const throw() {
    return responseType;
  }
  
  inline uint8 getOctet() const throw() {
    return octet;
  }

  inline char getCharacter() const throw() {
    return character;
  }
  
  inline ucs2 getWideCharacter() const throw() {
    return wideCharacter;
  }
  
  inline String getString() const throw() {
    return string;
  }
  
  inline WideString getWideString() const throw() {
    return wideString;
  }
  
  // set value
  
  // read response() throw(OrbException, IOException) {
  // }
  
  // write response() throw(OrbException, IOException) {
  // }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
