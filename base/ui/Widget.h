/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_UI__WIDGET_H
#define _DK_SDU_MIP__BASE_UI__WIDGET_H

#include <base/ui/Window.h>
#include <base/ui/GraphicsContext.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  The base class of all widgets.
  
  @short Widget base class.
  @ingroup ui
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Widget : public GraphicsContext {
private:
  
  /**
    Releases the widget.
  */
  void destroy() throw();
public:

  /**
    Initializes the widget.

    @param owner The owner of the widget.
  */
  Widget(Window& owner) throw(UserInterfaceException);

  /**
    Destroys the widget.
  */
  ~Widget() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
