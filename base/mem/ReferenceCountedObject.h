/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_H
#define _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

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
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.01
*/

class ReferenceCountedObject {

friend class ReferenceCountedObjectPointerImpl;

private:

  /** The current number of references to the object. */
  mutable unsigned long references; // out of memory before overflow
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
  inline ReferenceCountedObject(const ReferenceCountedObject& copy) throw() : references(0) {}

  /**
    Assignment of reference counted object does not change this object. This
    is unusual behaviour for the assignment operator but makes sense since
    the assignment doesn't influence the number of ReferenceCountedObjectPointer
    objects that point to this object.
  */
  inline ReferenceCountedObject& operator=(const ReferenceCountedObject& copy) throw() {
    return *this;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
