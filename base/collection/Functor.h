/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__FUNCTOR_H
#define _DK_SDU_MIP__BASE_COLLECTION__FUNCTOR_H

#include <base/Base.h>
#include <base/Functor.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE



// Non-modifying operations



/**
  Invocates the specified unary operation for each element (non-modifying operation).
*/
template<class COLLECTION, class UNOPR>
void forEach(COLLECTION& collection, UNOPR& function) {
  typename COLLECTION::ReadOnlyEnumeration enu(collection);
  while (enu.hasNext()) {
    function(*enu.next());
  }
}



// Modifying operations



/**
  Apply an unary operation to every element of the enumeration (modifying operation).
*/
template<class COLLECTION, class UNOPR>
inline void transform(COLLECTION& collection, const UNOPR& function) throw() {
  typename COLLECTION::Enumeration enu(collection);
  while (enu.hasNext()) {
    typename COLLECTION::Enumeration::Pointer value = enu.next();
    *value = function(*value);
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
