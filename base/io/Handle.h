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

#include <base/mem/Reference.h>
#include <base/OperatingSystem.h>
#include <base/io/IOException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A generic descriptor which serves as a handle to a source or sink of bytes
  within the operatingsystem (e.g. file, socket and pipe). This class is
  normally not used directly by the application.

  @short Generic descriptor
  @ingroup io
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Handle : public ReferenceCountedObject {
private:

  /** Handle to resource. */
  OperatingSystem::Handle handle = OperatingSystem::INVALID_HANDLE;

  /* Disable the default copy constructor. */
  Handle(const Handle& copy) noexcept;
  /* Disable the default assignment operator. */
  Handle& operator=(const Handle& assign) noexcept;
public:

  /**
    Initializes an invalid handle.
  */
  inline Handle() noexcept
  {
  }

  /**
    Initializes handle using the specified value.
  */
  explicit inline Handle(OperatingSystem::Handle _handle) noexcept
    : handle(_handle)
  {
  }

  /**
    Returns the handle.
  */
  inline OperatingSystem::Handle getHandle() const noexcept
  {
    return handle;
  }

  /**
    Returns true if the handle is valid.
  */
  inline bool isValid() const noexcept
  {
    return handle != OperatingSystem::INVALID_HANDLE;
  }

  /**
    Destroys the handle.
  */
  virtual ~Handle();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
