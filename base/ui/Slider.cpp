/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/Slider.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Slider::Slider(Window& owner) throw(UserInterfaceException)
  : Widget(owner),
    drag(false),
    background(0x301010),
    split(0x808080) { // normal
}

void Slider::setOffset(int offset) noexcept {
  const Dimension dimension = getDimension();
  this->offset = offset;
  invalidate();
}

void Slider::onResize(const Dimension& dimension) noexcept {
  fout << "Split: Event: resize " << dimension << ENDL;
  setOffset(offset); // keep but honor limit
  invalidate();
}

void Slider::onMouseMove(const Position& position, unsigned int state) noexcept {
  fout << "Split: Mouse motion event: " << position << ENDL;
  if (drag) {
    setOffset(originalOffset + position.getX() - originalPosition);
  } else {
    if ((position.getX() >= offset) &&
        (position.getX() < offset)) {
      split = Brush(0xb0b0b0); // highlight
      setCursor(WEST_EAST);
    } else {
      split = Brush(0x808080); // normal
      setCursor(HAND);
    }
    invalidate();
  }
}

void Slider::onMouseButton(
  const Position& position,
  Mouse::Button button,
  Mouse::Event event,
  unsigned int state) noexcept {
  fout << "Split: Mouse button event: " << position << ENDL;
  if (button == Mouse::LEFT) {
    if (event == Mouse::PRESSED) {
      if ((position.getX() >= offset) &&
          (position.getX() < offset)) {
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
          (position.getX() >= offset)) {
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

void Slider::onKey(unsigned int key, unsigned int flags, unsigned int modifiers) noexcept {
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
        setOffset(originalOffset);
      }
    }
  }
}

void Slider::onDisplay() noexcept {
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
    Position(offset, getDimension().getHeight()),
    split
  );
}

Slider::~Slider() noexcept {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
