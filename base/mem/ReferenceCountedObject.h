/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_H
#define _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_H

#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A reference counted object is used to count the number of references from
  a reference counting automation pointer. You should always use the automation
  pointer Reference or similar to access a ReferenceCountedObject. The
  ReferenceCountedObject is nothing more than an encapsulation of the number of
  references. To avoid any invalidation of the number of references only
  Reference can make changes to the number of references. Not even subclasses
  of ReferenceCountedObject are allowed to change the number of references. The
  number of references is not considered a part of the state of the object and
  can thus be modified for constant reference counted objects. Or to put it
  simple, you can reference count constant objects. Use ReferenceCounter to
  count the number of references to any object.
  
  To make your own reference counted class you simply inherit from this class.
  Like illustrated by this example.

  @code
  class MyClass : public virtual Object, public virtual ReferenceCountedObject {
    ...
  };
  @endcode
  
  @short Reference counted object.
  @ingroup memory
  @see Reference ReferenceCounter
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.2
*/

class ReferenceCountedObject {
  friend class ReferenceImpl;
private:

  /** The current number of references to the object. */
  mutable MemorySize references; // out of memory before overflow
public:

  /*
    This class is only introduced to give automation pointers access to the
    ReferenceCountedObject. This class musst only be used by automation pointers.
    
    @short Reference counting automation pointer implementation class.
    @see Reference
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */

  class ReferenceImpl {
  private:
    
    /** The reference counted object. */
    const ReferenceCountedObject& object;
    ReferenceImpl(const ReferenceImpl&) throw();
    ReferenceImpl& operator=(const ReferenceImpl& eq) throw();
  public:
    
    /**
      Initializes reference to object.
    */
    inline ReferenceImpl(const ReferenceCountedObject& _object) throw()
      : object(_object) {
    }
    
    /**
      Adds one reference to the object.
    */
    inline void addReference() const throw() {
      ++object.references;
    }
    
    /**
      Removes one reference to the object. The object must have at least one
      reference.
      
      @return True if all references have been released.
    */
    inline bool removeReference() const throw() {
      return --object.references == 0;
    }
    
    /**
      Returns true if the object has multiple references.
    */
    inline bool isMultiReferenced() const throw() {
      return object.references > 1;
    }
  };
  
  /**
    Initializes reference counted object with zero references.
  */
  inline ReferenceCountedObject() throw() : references(0) {
  }

  /**
    Initializes object from other reference counted object. The new object
    is initialized with zero references. This is an unusual behavior for a
    copy constructor but makes sense since a new object cannot have any
    references.
  */
  inline ReferenceCountedObject(const ReferenceCountedObject& copy) throw()
    : references(0) {
  }

  /**
    Assignment of reference counted object does not change this object. This
    is unusual behavior for the assignment operator but makes sense since
    the assignment doesn't influence the number of Reference objects that point
    to this object.
  */
  inline ReferenceCountedObject& operator=(const ReferenceCountedObject& copy) throw() {
    return *this;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
