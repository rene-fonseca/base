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

class _COM_AZURE_DEV__BASE__API SystemInformation : public Object {
public:

  /**
    Returns the system ID (determinated at build time NOT runtime).
  */
  static String getOS() noexcept;

  /**
    Returns the vendor name of the operating system.
  */
  static String getVendor() noexcept;

  /**
    Returns a description of the operating system.
  */
  static String getSystem() noexcept;

  /**
    Returns the release of the operating system.
  */
  static String getRelease() noexcept;

  /**
    Returns the version of the operating system.
  */
  static String getVersion() noexcept;

  /**
    Returns the machine.
  */
  static String getMachine() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
