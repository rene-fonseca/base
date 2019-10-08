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

#include <base/Base.h>
#include <base/Functor.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE



// Non-modifying operations



/**
  Invocates the specified unary operation for each element (non-modifying
  operation).
*/
template<class COLLECTION, class UNOPR>
void forEach(COLLECTION collection, UNOPR function) /*throw(...)*/ {
  typename COLLECTION::ReadEnumerator enu = collection.getReadEnumerator();
  while (enu.hasNext()) {
    function(*enu.next());
  }
}



// Modifying operations



/**
  Apply an unary operation to every element of the enumeration (modifying
  operation).
*/
template<class COLLECTION, class UNOPR>
inline void transform(COLLECTION& collection, UNOPR function) throw() {
  typename COLLECTION::Enumerator enu = collection.getEnumerator();
  while (enu.hasNext()) {
    typename COLLECTION::Enumerator::Pointer value = enu.next();
    *value = function(*value);
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
