/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__TYPE_INFO_H
#define _DK_SDU_MIP__BASE__TYPE_INFO_H

#include <base/features.h>
#include <base/string/String.h>
#include <typeinfo>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

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
