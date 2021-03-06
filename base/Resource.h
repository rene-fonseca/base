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

#include <base/string/String.h>
#include <base/mem/Reference.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Base class for resource handles.
*/
class _COM_AZURE_DEV__BASE__API Resource : public virtual Object {
protected:

  /** Internal handle for resource. */
  AnyReference handle;
public:

  /** Returns the description of the resource. */
  String getDescription() const;

  /** Sets the description of the resource. */
  void setDescription(const String& description);

  /** Returns the unique resource ID. */
  unsigned int getResourceId() const;

  /** Returns the thread that created the resource. */
  unsigned int getCreatedById() const;

  /** Returns the handle. */
  inline AnyReference getHandle() const noexcept
  {
    return handle;
  }
  
  /** Returns the handle. */
  template<class TYPE>
  inline TYPE& getInternalHandle() const
  {
    return *handle.cast<TYPE>(); // throws NullPointer
  }

  /** Returns true if handle is set. */
  inline operator bool() const noexcept
  {
    return handle;
  }
};

/**
  Writes a string representation of the resource.
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Resource& resource);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
