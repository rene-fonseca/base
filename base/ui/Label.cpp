/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/Label.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Label::Label(Window& owner) throw(UserInterfaceException)
  : Widget(owner) {
}

void Label::onDisplay() throw() {
  rectangle(Position(0, 0), getDimension(), brush);
  // set foreground
  Widget::text(Position(0, 0), getDimension(), text, 0);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
