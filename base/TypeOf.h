/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__TYPE_OF_H
#define _DK_SDU_MIP__BASE__TYPE_OF_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short TypeOf operator.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class TypeOf {
private:

  /** Default assignment is prohibited. */
  TypeOf& operator=(const TypeOf& eq) throw();
public:

  /** The type. */
  typedef TYPE Type;
  
  /** Initializes type object. */
  inline TypeOf() throw() {
  }
  
  /** Initializes type object from other object. */
  inline TypeOf(const TypeOf& copy) throw() {
  }
  
  /** Initializes type object from value. */
  inline TypeOf(TYPE value) throw() {
  }
  
  /** Returns true is the types are the same. */
  inline bool operator==(const TypeOf& value) const throw() {
    return true;
  }
  
  /** Returns true is the types are the different. */
  inline bool operator!=(const TypeOf& value) const throw() {
    return false;
  }
  
  /** Returns true is the types are the same. */
  template<class POLY>
  inline bool operator==(const TypeOf<POLY>& value) const throw() {
    return false;
  }
  
  /** Returns true is the types are the different. */
  template<class POLY>
  inline bool operator!=(const TypeOf<POLY>& value) const throw() {
    return true;
  }
};

/**
  Returns the specified type.
  
  @relates TypeOf
*/
template<class TYPE>
inline TypeOf<TYPE> type() throw() {
  return TypeOf<TYPE>();
}

/**
  Returns the specified type.
  
  @relates TypeOf
*/
template<class TYPE>
inline TypeOf<TYPE> type(TYPE) throw() {
  return TypeOf<TYPE>();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif