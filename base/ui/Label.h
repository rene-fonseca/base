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
  Label widget.
  
  @short Label widget.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Label : public Widget {
private:

  String text;
  Color background;
  Color foreground;
  unsigned int textFormat = 0;
public:
  
  enum Flag {
    BORDER = 1,
    TRANSPARENT = 2
  };
  
  /**
    Initializes the label.
  */
  Label(Window& owner);

  /**
    Returns the text.
  */
  inline const String& getText() const {
    return text;
  }

  /**
    Sets the text.
  */
  void setText(const String& text) {
    if (text != this->text) {
      this->text = text;
      invalidate();
    }
  }

  /**
    Returns the text format.
  */
  unsigned int getTextFormat() const noexcept
  {
    return textFormat;
  }

  /**
    Sets the text format.
  */
  void setTextFormat(unsigned int textFormat) noexcept
  {
    this->textFormat = textFormat;
    invalidate();
  }

  /**
    Sets the background color.
  */
  inline void setBackground(Color background) noexcept
  {
    this->background = background;
    invalidate();
  }

  /**
    Sets the text color.
  */
  inline void setForeground(Color foreground) noexcept
  {
    this->foreground = foreground;
    invalidate();
  }

  /**
    Invoked when the widget should be repainted.
  */
  void onDisplay() noexcept;

  /**
    Destroys the label.
  */
  inline ~Label() noexcept
  {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
