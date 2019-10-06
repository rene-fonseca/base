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
  Push button widget.
  
  @short Button widget.
  @ingroup ui
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Button : public Widget {
private:

  String text;
  Brush grayedBrush;
  Brush pressedBrush;
  Brush highlightBrush;
  Pen lightPen;
  Pen darkPen;
  Color normalTextColor;
  Color pressedTextColor;
  bool grayed;
  bool pressed;
  bool highlighted;
public:

  enum {
    PREFERRED_HORIZONTAL_MARGIN = 16,
    PREFERRED_VERTICAL_MARGIN = 8
  };
  
  /**
    Initializes the button widget.
  */
  Button(Window& owner) throw(UserInterfaceException);
  
  /**
    Returns the button text.
  */
  inline String getText() const throw(UserInterfaceException) {
    return text;
  }

  /**
    Returns the preferred size of the button.
  */
  Dimension getPreferredSize() throw(UserInterfaceException);
  
  /**
    Sets the button text.
  */
  void setText(const String& text) throw(UserInterfaceException);
  
  /**
    Invoked when the mouse cursor enters and exists the scope of the button.
  */
  void onMouseScope(bool scope) throw();
  
  /**
    Invoked when a mouse button is pressed or released over the button.
  */
  void onMouseButton(
    const Position& position,
    Mouse::Button button,
    Mouse::Event event,
    unsigned int state) throw();

  /**
    Invoked on keyboard events.
  */
  void onKey(
    unsigned int key, unsigned int flags, unsigned int modifiers) throw();

  /**
    Invoked when the button must be updated.
  */
  void onDisplay() throw();

  /**
    Invoked when the button is selected.
  */
  virtual void onSelection() throw();
  
  /**
    Destroys the button.
  */
  inline ~Button() throw() {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
