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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Privilege.
  
  @short Privilege.
  @ingroup security
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API Privilege : public Object {
public:

  enum PrivilegeType {
    BACKUP /**< Backup privilege. */
  };

  /**
    Returns the name of the specified privilege.
  */
  static String getName(PrivilegeType privilege) throw();

  /**
    Returns the value of the specified privilege.
  */
  static PrivilegeType getValue(const String& name) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
