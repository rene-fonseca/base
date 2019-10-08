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
#include <base/ui/UserInterfaceException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @defgroup ui User interface
*/

/**
  Dialog.
  
  @short Dialog
  @ingroup ui
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API Dialog : public Object {
public:
  
  /**
    Executes the dialog.
  */
  virtual bool execute() throw(UserInterfaceException) = 0;

  /**
    Destroys the dialog.
  */
  virtual ~Dialog() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
