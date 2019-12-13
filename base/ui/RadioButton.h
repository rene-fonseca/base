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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class _COM_AZURE_DEV__BASE__API Button : public Widget {
private:

  // 0: enabled, disabled (ignore all other)
  // 1: focus (mark button), no focus
  // 2: over (highlight), not over
  // 3: pressed (always over), not pressed (use over/not over)
  
  String text;
  Brush brush;
  Color background;
  Color textColor;
  unsigned int mode = 0;
  bool pressed = false;
public:
  
  /** The button state. */
  enum State {
    GRAYED, /**< Grayed state. */
    CHECKED, /**< Checked state. */
    UNCHECKED /**< Unchecked state. */
  };

  /**
    Initializes the button widget.
  */
  Button(Window& owner) throw(UserInterfaceException);

  /**
    Returns the background color.
  */
  inline Color getBackground() const noexcept {
    return background;
  }

  /**
    Sets the background color.
  */
  void setBackground(Color background) throw(UserInterfaceException);

  /**
    Returns the text color.
  */
  inline Color getTextColor() const noexcept {
    return textColor;
  }

  /**
    Sets the color of the button text.
  */
  void setTextColor(Color textColor) throw(UserInterfaceException);

  /**
    Returns the button text.
  */
  inline const String& getText() const throw(UserInterfaceException) {
    return text;
  }

  /**
    Sets the button text.
  */
  void setText(const String& text) throw(UserInterfaceException);

//   /**
//     Returns the current state of the button.
//   */
//   inline State getState() const throw(UserInterfaceException) {
//     return state;
//   }

//   /**
//     Sets the state of the button.
//   */
//   void setState(State state) throw(UserInterfaceException);

  /**
    Invoked when the button is resized.
  */
  void onResize(const Dimension& dimension) noexcept;

  /**
    Invoked when the mouse cursor enters and exists the scope of the button.
  */
  void onMouseScope(bool scope) noexcept;

  /**
  */
  void onMouseMove(const Position& position, unsigned int state) noexcept;

  /**
    Invoked when a mouse button is pressed or released over the button.
  */
  void onMouseButton(const Position& position, Mouse::Button button, Mouse::Event event, unsigned int state) noexcept;

  /**
    Invoked on keyboard events.
  */
  void onKey(unsigned int key, unsigned int flags, unsigned int modifiers) noexcept;

  /**
    Invoked when the button must be updated.
  */
  void onDisplay() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
