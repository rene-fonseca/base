/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__COLLECTION_H
#define _DK_SDU_MIP__BASE_COLLECTION__COLLECTION_H

#include "Enumeration.h"

/**
  Collection is the common interface implemented containers.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class Collection {
public:

  /**
    Removes all elements in the collection.
  */
  virtual void removeAll() = 0;

  /**
    Returns the number of elements in the collection.
  */
  virtual unsigned int getSize() = 0;

  /**
    Returns true if the collection is empty.
  */
  virtual bool isEmpty() = 0;

//  virtual Enumeration* getEnumeration() = 0;
};

#endif
