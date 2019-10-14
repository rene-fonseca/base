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

#include <base/Object.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  The base class of all dynamically allocated classes.

  @short Dynamic object.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API DynamicObject {
private:

#if defined(DEBUG) /*|| defined(_COM_AZURE_DEV__BASE__DEBUG)*/
  bool valid = true;
#endif
public:

  /**
    Initializes the object.
  */
  inline DynamicObject() throw() {
  }

  /**
    Returns true if the object is valid.
  */
  bool isValidObject() const throw();

  /**
    Destroys the object.
  */
  virtual inline ~DynamicObject() noexcept(false) {
#if defined(DEBUG) /*|| defined(_COM_AZURE_DEV__BASE__DEBUG)*/
    ASSERT(valid);
    valid = false;
#endif
  }
};

template<>
class Uninitializeable<DynamicObject> {
public:

  static const bool IS_UNINITIALIZEABLE = Uninitializeable<bool>::IS_UNINITIALIZEABLE;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
