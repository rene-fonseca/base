/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_H
#define _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_H

#include "../Object.h"

/**
  A Reference Counted Object is used to count the number of references to itself. Beware, the ReferenceCountedObject does not do any validation checking. You should always use the template interface PReferenceCountedObject to access a ReferenceCountedObject.

  @author René Møller Fonseca
  @version 1.0
*/

  class ReferenceCountedObject : public Object {
  private:

    /** The current number of references to the object. */
    unsigned int references; // its not likely that we get an overflow
  public:

    /**
      Initializes the object. Initially the object has zero references.
    */
    inline ReferenceCountedObject() throw() : references(0) {}

    /**
      Add one reference to the object.
    */
    inline void addReferenceToObject() throw() {
      ++references; // no overflow checking
    }

    /**
      Remove one reference to the object.

      @return True if the object has no more references (i.e. the object should be destroyed).
    */
    inline bool removeReferenceToObject() throw() {
      return !--references;
    }

    /**
      Returns the number of references to the object.
    */
    inline unsigned int getReferencesToObject() const throw() {
      return references;
    }

    /** Destroys the object. */
    inline ~ReferenceCountedObject() throw() {}
  };

#endif
