/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_UI__WINDOW_H
#define _DK_SDU_MIP__BASE_UI__WINDOW_H

#include <base/ui/WindowImpl.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Window.
  
  @short Window.
  @ingroup ui
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Window : public WindowImpl {
  friend class Widget;
public:
  
  /**
    Initializes a window.
  */
  Window(const String& title, const Position& position, const Dimension& dimension, unsigned int flags) throw(UserInterfaceException);
  
  enum GraphicsFlag {
    FILLED = 1
  };
  
  class Justify {
  public:
    
    enum Value {
      LEFT = 1,
      CENTER = 2,
      RIGHT = 4,
      TOP = 8,
      MIDDLE = 16,
      BOTTOM = 32
    };
  };
  
  /**
    Clears the window.
  */
  void clear() throw(UserInterfaceException);
  
  /**
    Clear the specified region.
  */
  void clear(const Position& position, const Dimension& dimension) throw(UserInterfaceException);
  
  /**
    Draws a point.
  */
  void point(const Position& position, unsigned int flags = 0) throw(UserInterfaceException);
  
  /**
    Draws a line.
  */
  void line(const Position& a, const Position& b, unsigned int flags = 0) throw(UserInterfaceException);

  /**
    Draws an arc.
  */
  void arc(const Position& position, const Dimension& dimension, int start, int stop, unsigned int flags = 0) throw(UserInterfaceException);

  /**
    Draws a rectangle.
  */
  void rectangle(const Position& position, const Dimension& dimension, unsigned int flags = 0) throw(UserInterfaceException);

  /**
    Returns the dimension of the text.
  */
  Dimension getDimensionOf(const String& text) const throw(UserInterfaceException);

  /**
    Draws text.
  */
  void text(const Position& position, const Dimension& dimension, const String& text, unsigned int flags = 0) throw(UserInterfaceException);

  /**
    Releases the window.
  */
  ~Window() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
