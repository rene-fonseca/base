/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_H
#define _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_H

#include "base/Object.h"

//template<class TYPE> class ReferenceCountedObjectPointer;

/**
  A reference counted object is used to count the number of references from
  ReferenceCountedObjectPointer objects to itself. You should always use the
  automation pointer ReferenceCountedObjectPointer to access a
  ReferenceCountedObject. The ReferenceCountedObject is nothing more than an
  encapsulation of the number of references. To avoid any invalidation of the
  number of references only ReferenceCountedObjectPointer can make changes to
  the number of references. Not even subclasses of ReferenceCountedObject are
  allowed to change the number of references. The number of references is not
  considered a part of the state of the object and can thus be modified for
  constant reference counted objects. Or to put it simple, you can reference
  count constant objects.

  To make your own reference counted class you simply inherit from this class.
  Like illustrated by this example.

  <pre>
  class MyClass : public virtual Object, public virtual ReferenceCountedObject {
    ...
  };
  </pre>

  @short Reference counted object.
  @see ReferenceCountedObjectPointer
  @author René Møller Fonseca
  @version 1.01
*/

  class ReferenceCountedObject : public Object {

  friend class ReferenceCountedObjectFriend;
//  friend ReferenceCountedObjectPointer<const ReferenceCountedObject>;

  private:

    /** The current number of references to the object. */
    mutable unsigned int references; // its not likely that we get an overflow or is it
  public:

    /**
      Initializes reference counted object with zero references.
    */
    inline ReferenceCountedObject() throw() : references(0) {}

    /**
      Initializes object from other reference counted object. The new object
      is initialized with zero references. This is an unusual behaviour for a
      copy constructor but makes sense since a new object cannot have any
      references.
    */
    inline ReferenceCountedObject(const ReferenceCountedObject& copy) throw() : references(0) {};

    /**
      Assignment of reference counted object does not change this object. This
      is unusual behaviour for the assignment operator but makes sense since
      the assignment doesn't influence the number of ReferenceCountedObjectPointer
      objects that point to this object.
    */
    inline ReferenceCountedObject& operator=(const ReferenceCountedObject& copy) throw() {return *this;};

    /**
      Destroys the reference counted object.
    */
    inline ~ReferenceCountedObject() throw() {};
  };

#endif
