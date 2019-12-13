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
#include <base/string/InvalidFormat.h>
#include <base/Type.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class provides type name demangling support.

  @short Type name
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API TypeInfo {
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
  static inline String getTypename() noexcept {
    return demangleName(Type::getType<TYPE>().getLocalName());
  }
  
  /**
    Returns the demangled type name of the type of the specified object. The
    result is the string representation of the fully qualified name of the type
    (e.g. "base::String").
  */
  template<class TYPE>
  static inline String getTypename(const TYPE& object) noexcept {
    return demangleName(Type(object).getLocalName());
  }
  
  /**
    Returns the demangled type name of the specified type. The result is the
    string representation of the fully qualified name of the type (e.g.
    "base::String").

    @param type The type.
  */
  static inline String getTypename(const Type& type) noexcept {
    return demangleName(type.getLocalName());
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
