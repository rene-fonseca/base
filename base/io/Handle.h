/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__HANDLE_H
#define _DK_SDU_MIP__BASE_IO__HANDLE_H

#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/OperatingSystem.h>
#include <base/io/IOException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A generic descriptor which serves as a handle to a source or sink of bytes
  within the operatingsystem (e.g. file, socket and pipe). This class is
  normally not used directly by the application.

  @short Generic descriptor
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Handle : public ReferenceCountedObject {
private:

  /** Handle to resource. */
  OperatingSystem::Handle handle;

  /* Disable the default copy constructor. */
  Handle(const Handle& copy);
  /* Disable the default assignment operator. */
  Handle& operator=(const Handle& eq);
public:

  /**
    Initializes an invalid handle.
  */
  inline Handle() throw() : handle(OperatingSystem::INVALID_HANDLE) {}

  /**
    Initializes handle using the specified value.
  */
  explicit inline Handle(OperatingSystem::Handle _handle) throw() : handle(_handle) {}

  /**
    Returns the handle.
  */
  inline OperatingSystem::Handle getHandle() const throw() {return handle;}

  /**
    Returns true if the handle is valid.
  */
  inline bool isValid() const throw() {
    return handle != OperatingSystem::INVALID_HANDLE;
  }

  /**
    Destroys the handle.
  */
  virtual ~Handle() throw(IOException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
