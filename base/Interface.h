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

#include <base/features.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  The base class of interfaces.
  
  @short Interface.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Interface {
private:

  /* Disable the default copy constructor. */
  Interface(const Interface& copy) throw();
  /* Disable the default assignment operator. */
  Interface& operator=(const Interface& assign) throw();
public:

  /**
    Initializes the interface.
  */
  inline Interface() throw() {
  }
  
  /**
    Destroys the interface.
  */
  virtual ~Interface() throw();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
