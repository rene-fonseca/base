/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_UI__LABEL_H
#define _DK_SDU_MIP__BASE_UI__LABEL_H

#include <base/ui/Widget.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Label widget.
  
  @short Label widget.
  @ingroup ui
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Label : public Widget {
private:

  String text;
  Color background;
  Color foreground;
  unsigned int textFormat;
public:
  
  enum Flag {
    BORDER = 1,
    TRANSPARENT = 2
  };
  
  /**
    Initializes the label.
  */
  Label(Window& owner) throw(UserInterfaceException);

  /**
    Returns the text.
  */
  inline String getText() const throw(UserInterfaceException) {
    return text;
  }

  /**
    Sets the text.
  */
  void setText(const String& text) throw(UserInterfaceException) {
    if (text != this->text) {
      this->text = text;
      invalidate();
    }
  }

  /**
    Returns the text format.
  */
  unsigned int getTextFormat() const throw() {
    return textFormat;
  }

  /**
    Sets the text format.
  */
  void setTextFormat(unsigned int textFormat) throw() {
    this->textFormat = textFormat;
    invalidate();
  }

  /**
    Sets the background color.
  */
  inline void setBackground(Color background) throw() {
    this->background = background;
    invalidate();
  }

  /**
    Sets the text color.
  */
  inline void setForeground(Color foreground) throw() {
    this->foreground = foreground;
    invalidate();
  }

  /**
    Invoked when the widget should be repainted.
  */
  void onDisplay() throw();

  /**
    Destroys the label.
  */
  inline ~Label() throw() {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
