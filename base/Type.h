/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__TYPE_H
#define _DK_SDU_MIP__BASE__TYPE_H

#include <base/features.h>
#include <base/string/String.h>
#include <typeinfo>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/** 8 bit unsigned integer. */
typedef unsigned char byte;

namespace LittleEndian {

#if defined(_DK_SDU_MIP__BASE__BIG_ENDIAN)

/** 16 bit signed integer represented in little endian byte order. */
typedef struct {
  char lowByte;
  char highByte;

  inline short operator=(short eq) throw() {
    lowByte = eq & 0xff;
    highByte = eq >> 8;
    return eq;
  }

  inline operator short() const throw() {return highByte << 8 + lowByte;}
} __attribute__ ((packed)) SignedShort;

/** 16 bit unsigned integer represented in little endian byte order. */
typedef struct {
  char lowByte;
  char highByte;

  inline unsigned short operator=(unsigned short eq) throw() {
    lowByte = eq & 0xff;
    highByte = eq >> 8;
    return eq;
  }

  inline operator unsigned short() const throw() {return highByte << 8 + lowByte;}
} __attribute__ ((packed)) UnsignedShort;

/** 32 bit signed integer represented in little endian byte order. */
typedef struct {
  char a;
  char b;
  char c;
  char d;

  inline int operator=(int eq) throw() {
    a = eq & 0xff;
    b = (eq >> 8) & 0xff;
    c = (eq >> 16) & 0xff;
    d = eq >> 24;
    return eq;
  }

  inline operator int() const throw() {return ((d << 8 + c) << 8 + b) << 8 + a;}
} __attribute__ ((packed)) SignedInt;

/** 32 bit unsigned integer represented in little endian byte order. */
typedef struct {
  char a;
  char b;
  char c;
  char d;

  inline unsigned int operator=(unsigned int eq) throw() {
    a = eq & 0xff;
    b = (eq >> 8) & 0xff;
    c = (eq >> 16) & 0xff;
    d = eq >> 24;
    return eq;
  }

  inline operator unsigned int() const throw() {return ((d << 8 + c) << 8 + b) << 8 + a;}
} __attribute__ ((packed)) UnsignedInt;

#else

/** 16 bit signed integer represented in little endian byte order. */
typedef short SignedShort;
/** 16 bit unsigned integer represented in little endian byte order. */
typedef unsigned short UnsignedShort;
/** 32 bit signed integer represented in little endian byte order. */
typedef int SignedInt;
/** 32 bit unsigned integer represented in little endian byte order. */
typedef unsigned int UnsignedInt;

#endif

}; // end of namespace



namespace BigEndian {

#if defined(_DK_SDU_MIP__BASE__BIG_ENDIAN)

/** 16 bit signed integer represented in big endian byte order. */
typedef short SignedShort;
/** 16 bit unsigned integer represented in big endian byte order. */
typedef unsigned short UnsignedShort;
/** 32 bit signed integer represented in big endian byte order. */
typedef int SignedInt;
/** 32 bit unsigned integer represented in big endian byte order. */
typedef unsigned int UnsignedInt;

#else

/** 16 bit signed integer represented in big endian byte order. */
typedef struct {
  char highByte;
  char lowByte;

  inline short operator=(short eq) throw() {
    highByte = eq >> 8;
    lowByte = eq & 0xff;
    return eq;
  }

  inline operator short() const throw() {return highByte << 8 + lowByte;}
} __attribute__ ((packed)) SignedShort;

/** 16 bit unsigned integer represented in big endian byte order. */
typedef struct {
  char highByte;
  char lowByte;

  inline unsigned short operator=(unsigned short eq) throw() {
    highByte = eq >> 8;
    lowByte = eq & 0xff;
    return eq;
  }

  inline operator unsigned short() const throw() {return highByte << 8 + lowByte;}
} __attribute__ ((packed)) UnsignedShort;

/** 32 bit signed integer represented in big endian byte order. */
typedef struct {
  char d;
  char c;
  char b;
  char a;

  inline int operator=(int eq) throw() {
    d = eq >> 24;
    c = (eq >> 16) & 0xff;
    b = (eq >> 8) & 0xff;
    a = eq & 0xff;
    return eq;
  }

  inline operator int() const throw() {return ((d << 8 + c) << 8 + b) << 8 + a;}
} __attribute__ ((packed)) SignedInt;

/** 32 bit unsigned integer represented in big endian byte order. */
typedef struct {
  char d;
  char c;
  char b;
  char a;

  inline unsigned int operator=(unsigned int eq) throw() {
    d = eq >> 24;
    c = (eq >> 16) & 0xff;
    b = (eq >> 8) & 0xff;
    a = eq & 0xff;
    return eq;
  }

  inline operator unsigned int() const throw() {return ((d << 8 + c) << 8 + b) << 8 + a;}
} __attribute__ ((packed)) UnsignedInt;

#endif

}; // end of namespace

/**
  Returns the id that uniquely identifies the specified type.
*/
template<class TYPE>
inline const char* getTypeId() throw() {
  return typeid(TYPE).name();
}

/**
  Returns the id that uniquely identifies the type of the specified object.
*/
template<class TYPE>
inline const char* getTypeId(const TYPE& object) throw() {
  return typeid(object).name();
}

/**
  Demangles the specified string as returned by typeinfo::name(). You should
  not call this function directly. Use the getTypename<TYPE>() and
  getTypename<TYPE>(const TYPE&) functions.
*/
String demangleTypename(const char* mangled) throw();

/**
  Returns the demangled type name for the specified type.
*/
template<class TYPE>
inline String getTypename() throw() {
  return demangleTypename(typeid(TYPE).name());
}

/**
  Returns the demangled type name for the specified object.
*/
template<class TYPE>
inline String getTypename(const TYPE& object) throw() {
  return demangleTypename(typeid(object).name());
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
