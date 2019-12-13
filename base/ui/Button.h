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

/**
  Push button widget.
  
  @short Button widget.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Button : public Widget {
private:

  String text;
  Brush grayedBrush;
  Brush pressedBrush;
  Brush highlightBrush;
  Pen lightPen;
  Pen darkPen;
  Color normalTextColor;
  Color pressedTextColor;
  bool grayed = false;
  bool pressed = false;
  bool highlighted = false;
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
  inline const String& getText() const throw(UserInterfaceException) {
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
  void onMouseScope(bool scope) noexcept;
  
  /**
    Invoked when a mouse button is pressed or released over the button.
  */
  void onMouseButton(
    const Position& position,
    Mouse::Button button,
    Mouse::Event event,
    unsigned int state) noexcept;

  /**
    Invoked on keyboard events.
  */
  void onKey(
    unsigned int key, unsigned int flags, unsigned int modifiers) noexcept;

  /**
    Invoked when the button must be updated.
  */
  void onDisplay() noexcept;

  /**
    Invoked when the button is selected.
  */
  virtual void onSelection() noexcept;
  
  /**
    Destroys the button.
  */
  inline ~Button() noexcept {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
