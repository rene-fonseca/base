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
#include <base/string/String.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  System information.

  @short System information
  @version 1.0
*/

class SystemInformation : public Object {
public:
  
  /**
    Returns the vendor name of the operating system.
  */
  static String getVendor() throw();

  /**
    Returns a description of the operating system.
  */
  static String getSystem() throw();

  /**
    Returns the release of the operating system.
  */
  static String getRelease() throw();

  /**
    Returns the version of the operating system.
  */
  static String getVersion() throw();

  /**
    Returns the machine.
  */
  static String getMachine() throw();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
