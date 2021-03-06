/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/Picture.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Picture::Picture(Window& owner)
  : Widget(owner)
{
}

void Picture::setBitmap(const Bitmap& bitmap)
{
  this->bitmap = bitmap;
  invalidate();
}

void Picture::onDisplay() noexcept
{
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
