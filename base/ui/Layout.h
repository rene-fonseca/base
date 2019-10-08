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

#include <base/ui/Widget.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Layout widget.
  
  @short Layout widget.
  @ingroup ui
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API Layout : public Widget {
public:
  
  Layout(Window& owner) throw(UserInterfaceException);

  Dimension getMinimumSize() const throw();

  Dimension getPreferredSize() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
