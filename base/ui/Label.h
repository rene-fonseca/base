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
  Brush brush;
  Color background;
  Color foreground;
  // justification
  // flags
  // font
public:

  // border color
  // border width
  // border style
  // enum {
  // };
  // TAG: transparent
  
  Label(Window& owner) throw(UserInterfaceException);
  
  inline String getText() const throw(UserInterfaceException) {
    return text;
  }
  
  void setText(const String& text) throw(UserInterfaceException) {
    if (text != this->text) {
      this->text = text;
      update();
    }
  }

  inline void setBackground(Color background) throw() {
    this->background = background;
    update();
  }
  
  inline void setForeground(Color foreground) throw() {
    this->foreground = foreground;
    update();
  }
  
  void onDisplay() throw();

  /**
    Destroys the label.
  */
  inline ~Label() throw() {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
