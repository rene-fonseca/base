/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_UI__LAYOUT_H
#define _DK_SDU_MIP__BASE_UI__LAYOUT_H

#include <base/ui/Widget.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Layout widget.
  
  @short Layout widget.
  @ingroup ui
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Layout : public Widget {
public:
  
  Layout(Window& owner) throw(UserInterfaceException);

  Dimension getMinimumSize() const throw();

  Dimension getPreferredSize() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
