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

#include <base/string/String.h>
#include <base/string/InvalidFormat.h>
#include <base/Type.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class provides type name demangling support.

  @short Type name
  @author Rene Moeller Fonseca
  @version 1.0
*/

class TypeInfo {
public:

  /**
    Demangles the specified NULL-terminated string. Normally, you shouldn't use
    this function directly. Instead you should use the getTypename<TYPE>() and
    getTypename<TYPE>(const TYPE&) functions. If the specified mangled string
    isn't a valid mangled name the result is unspecified.
     
    @param mangled The mangled name (compiler specific).
    
    @see getTypename
  */
  static String demangleName(const char* mangled) throw(InvalidFormat);

  /**
    Returns the demangled type name of the specified type. The result is the
    string representation of the fully qualified name of the type (e.g.
    "base::String").
  */
  template<class TYPE>
  static inline String getTypename() throw() {
    return demangleName(Type::getType<TYPE>().getLocalName());
  }
  
  /**
    Returns the demangled type name of the type of the specified object. The
    result is the string representation of the fully qualified name of the type
    (e.g. "base::String").
  */
  template<class TYPE>
  static inline String getTypename(const TYPE& object) throw() {
    return demangleName(Type(object).getLocalName());
  }
  
  /**
    Returns the demangled type name of the specified type. The result is the
    string representation of the fully qualified name of the type (e.g.
    "base::String").

    @param type The type.
  */
  static inline String getTypename(const Type& type) throw() {
    return demangleName(type.getLocalName());
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
