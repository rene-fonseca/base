/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_UI__WINDOW_H
#define _DK_SDU_MIP__BASE_UI__WINDOW_H

#include <base/ui/GraphicsContext.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Window.
  
  @short Window.
  @ingroup ui
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Window : public GraphicsContext {
  friend class Widget;
private:
  
  /**
    Releases the rendering context.
  */
  void destroy() throw();
public:
  
  /**
    Initializes a window.

    @param flags The flags.
  */
  Window(unsigned int flags) throw(UserInterfaceException);
  
  /**
    Initializes a window.

    @param position The initial position.
    @param dimension The initial dimension.
    @param flags The flags.
  */
  Window(
    const Position& position,
    const Dimension& dimension,
    unsigned int flags) throw(UserInterfaceException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
