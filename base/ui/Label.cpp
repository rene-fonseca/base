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
  : Widget(owner),
    textFormat(TextFormat::LEFT|TextFormat::MIDDLE) {
  setBackgroundMode(true);
  setTextColor(Color(0, 0, 0));
}

void Label::onDisplay() throw() {
  const Dimension dimension = getDimension();
  if (dimension.isProper()) {
    const Position lowerRight(dimension.getWidth() - 1, dimension.getHeight() - 1);
    // if not transparent do
    if (true) {
      rectangle(Position(0, 0), dimension, getBrush());
    }
    Widget::text(Position(0, 0), dimension, text, textFormat);
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
