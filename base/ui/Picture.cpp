/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/Picture.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Picture::Picture(Window& owner) throw(UserInterfaceException)
  : Widget(owner) {
}

void Picture::setBitmap(const Bitmap& bitmap) throw(UserInterfaceException) {
  this->bitmap = bitmap;
  invalidate();
}

void Picture::onDisplay() throw() {
  // TAG: handle dimension != picture dimension
  // TAG: binding point?
  
  Dimension dimension = getDimension();
  // 2 rectangles required to clear background
  rectangle(
    Position(0, 0),
    Position(dimension.getWidth(), dimension.getHeight()),
    Color(0U),
    0
  );
  putBitmap(
    Position(0, 0),
    dimension,
    bitmap
  );
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
