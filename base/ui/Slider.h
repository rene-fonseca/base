/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/ui/Widget.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Slider widget.
  
  @short Slider widget.
  @ingroup ui
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Slider : public Widget {
private:

  /** Offset of the left or top of the splitting field. */
  int offset = 0;
  unsigned int flags = 0;
  // Picture
  
  bool drag = false;
  int originalOffset = 0;
  int originalPosition = 0;

  Brush background;
  Brush split;
public:
  
  Slider(Window& owner) throw(UserInterfaceException);
  
  inline int getOffset() const throw() {
    return offset;
  }
  
  void setOffset(int offset) throw();
  
  void onResize(const Dimension& dimension) throw();
  
  void onMouseMove(const Position& position, unsigned int state) throw();

  void onMouseButton(const Position& position, Mouse::Button button, Mouse::Event event, unsigned int state) throw();

  void onKey(unsigned int key, unsigned int flags, unsigned int modifiers) throw();

  void onDisplay() throw();

  /**
    Destroys the split.
  */
  ~Slider() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
