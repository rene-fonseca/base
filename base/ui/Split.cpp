/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/Split.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// TAG: need grow to owner client area flag
// owner class children resize and position and dimension

// bind the position
// allow position to change
// bind dimension
// allow dimension to changed

Split::Split(Window& owner) throw(UserInterfaceException)
  : Widget(owner),
    widthOfSplit(10),
    drag(false),
    background(0x301010),
    split(0x808080) { // normal
}

// void Split::setState(unsigned int state) throw() {
//   if (state != this->state) {
//     this->state = state;
//     invalidate();
//   }
// }

void Split::setOffset(int offset, Field field) throw() {
  const Dimension dimension = getDimension();
  int total;
  if (flags & VERTICAL_SPLIT) {
    total = dimension.getWidth();
  } else {
    total = dimension.getHeight();
  }
  int limit = total - widthOfSplit;
  if (offset > limit) {
    offset = limit;
  }
  if (offset < 0) {
    offset = 0;
  }
  switch (field) {
  case FIRST:
    this->offset = offset;
    break;
  case SECOND:
    this->offset = limit - offset; // translate to FIRST
    break;
  case RELATIVE:
    break;
  }
  // first widget setDimension();
  // second widget setDimension();
  invalidate();
}

void Split::onResize(const Dimension& dimension) throw() {
  fout << "Split: Event: resize " << dimension << ENDL;
  setOffset(offset, FIRST); // keep but honor limit
  invalidate();
}

void Split::onMouseScope(bool scope) throw() {
  if (!scope) {
    split = Brush(0x808080); // normal
    setCursor(HAND);
    invalidate();
  }
}

void Split::onMouseMove(const Position& position, unsigned int state) throw() {
  fout << "Split: Mouse motion event: " << position << ENDL;
  if (drag) {
    setOffset(originalOffset + position.getX() - originalPosition, FIRST);
  } else {
    if ((position.getX() >= offset) &&
        (position.getX() < static_cast<int>(offset + widthOfSplit))) {
      split = Brush(0xb0b0b0); // highlight
      setCursor(WEST_EAST);
    } else {
      split = Brush(0x808080); // normal
      setCursor(HAND);
    }
    invalidate();
  }
}

void Split::onMouseButton(
  const Position& position,
  Mouse::Button button,
  Mouse::Event event,
  unsigned int state) throw() {
  
  fout << "Split: Mouse button event: " << position << ENDL;
  if (button == Mouse::LEFT) {
    if (event == Mouse::PRESSED) {
      if ((position.getX() >= offset) &&
          (position.getX() < static_cast<int>(offset + widthOfSplit))) {
        drag = true;
        split = Brush(0x404040); // select
        setCapture(true);
        originalPosition = position.getX();
        originalOffset = offset;
        setCursor(WEST_EAST);
      }
    } else if (event == Mouse::RELEASED) {
      drag = false;
      setCapture(false);
      if ((position.getX() < offset) || (position.getX() >= static_cast<int>(offset + widthOfSplit))) {
        split = Brush(0x808080); // normal
        setCursor(HAND);
      } else {
        split = Brush(0xb0b0b0); // highlight
        setCursor(WEST_EAST);
      }
    }
    invalidate();
  }
}

void Split::onKey(unsigned int key, unsigned int flags, unsigned int modifiers) throw() {
  if (flags & Key::PRESSED) {
    if (flags & Key::DEAD) {
      return;
    }
    if (key == Key::ESCAPE) {
      if (drag) {
        drag = false;
        setCapture(false);
        setOffset(originalOffset, FIRST);
      }
    }
  }
}

void Split::onDisplay() throw() {
  Dimension dimension = getDimension();
  rectangle(
    Position(0, 0),
    Position(dimension.getWidth(), dimension.getHeight()),
    background
  );
  rectangle(
    Position(offset, 0),
    Position(offset + widthOfSplit, getDimension().getHeight()),
    split
  );
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
