/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__COLLECTION_H
#define _DK_SDU_MIP__BASE_COLLECTION__COLLECTION_H

#include "base/Object.h"

/**
  Collection is the common interface implemented containers.

  @author René Møller Fonseca
  @version 1.0
*/

class Collection : public Object {
protected:

  /** The number of elements in the collection. */
  unsigned int size;
public:

  /**
    Initializes an empty collection.
  */
  inline Collection() throw() : size(0) {}

  /**
    Returns the number of elements in the collection.
  */
  inline unsigned int getSize() const throw() {return size;}

  /**
    Returns true if the collection is empty.
  */
  inline bool isEmpty() const throw() {return size != 0;}
};

#endif
