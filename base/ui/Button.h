/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_UI__BUTTON_H
#define _DK_SDU_MIP__BASE_UI__BUTTON_H

#include <base/ui/Widget.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Button widget.
  
  @short Button widget.
  @ingroup ui
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Button : public Widget {
private:

  // 0: enabled, disabled (ignore all other)
  // 1: focus (mark button), no focus
  // 2: over (highlight), not over
  // 3: pressed (always over), not pressed (use over/not over)
  
  String text;
  Brush brush;
//   Brush highlightBrush;
//   Brush pressedBrush;
  Color background;
  Color textColor;
  unsigned int mode;
  bool pressed;
public:

  /**
    Initializes the button widget.
  */
  Button(Window& owner) throw(UserInterfaceException);

  /**
    Returns the background color.
  */
  inline Color getBackground() const throw() {
    return background;
  }

  /**
    Sets the background color.
  */
  void setBackground(Color background) throw(UserInterfaceException);

  /**
    Returns the text color.
  */
  inline Color getTextColor() const throw() {
    return textColor;
  }

  /**
    Sets the color of the button text.
  */
  void setTextColor(Color textColor) throw(UserInterfaceException);

  /**
    Returns the button text.
  */
  inline String getText() const throw(UserInterfaceException) {
    return text;
  }

  /**
    Sets the button text.
  */
  void setText(const String& text) throw(UserInterfaceException);

  /**
    Invoked when the button is resized.
  */
  void onResize(const Dimension& dimension) throw();

  /**
    Invoked when the mouse cursor enters and exists the scope of the button.
  */
  void onMouseScope(bool scope) throw();

  /**
    Invoked on mouse motion.
  */
  void onMouseMove(const Position& position, unsigned int state) throw();

  /**
    Invoked when a mouse button is pressed or released over the button.
  */
  void onMouseButton(const Position& position, Mouse::Button button, Mouse::Event event, unsigned int state) throw();

  /**
    Invoked on keyboard events.
  */
  void onKey(unsigned int key, unsigned int flags, unsigned int modifiers) throw();

  /**
    Invoked when the button must be updated.
  */
  void onDisplay() throw();

  /**
    Destroys the button.
  */
  inline ~Button() throw() {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
