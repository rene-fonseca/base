/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/Slider.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Slider::Slider(Window& owner) throw(UserInterfaceException)
  : Widget(owner),
    drag(false),
    background(0x301010),
    split(0x808080) { // normal
}

void Slider::setOffset(unsigned int offset) throw() {
  const Dimension dimension = getDimension();
  this->offset = offset;
  invalidate();
}

void Split::onResize(const Dimension& dimension) throw() {
  fout << "Split: Event: resize " << dimension << ENDL;
  setOffset(offset, FIRST); // keep but honor limit
  invalidate();
}

void Split::onMouseMove(const Position& position, unsigned int state) throw() {
  fout << "Split: Mouse motion event: " << position << ENDL;
  if (drag) {
    setOffset(originalOffset + position.getX() - originalPosition, FIRST);
  } else {
    if ((position.getX() >= offset) &&
        (position.getX() < (offset + widthOfSplit))) {
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
          (position.getX() < (offset + widthOfSplit))) {
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
      if ((position.getX() < offset) ||
          (position.getX() >= (offset + widthOfSplit))) {
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

void Slider::onKey(unsigned int key, unsigned int flags, unsigned int modifiers) throw() {
  if (flags & Key::PRESSED) {
    if (flags & Key::DEAD) {
      return;
    }
    switch (key) {
    case Key::LEFT:
      break;
    case Key::RIGHT:
      
      break;
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

void Slider::onDisplay() throw() {
  Dimension dimension = getDimension();
  // left picture
  // right picture
  // background
  rectangle(
    Position(0, 0),
    Position(dimension.getWidth(), dimension.getHeight()),
    background
  );
  // slider
  rectangle(
    Position(offset, 0),
    Position(offset + widthOfSplit, getDimension().getHeight()),
    split
  );
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
