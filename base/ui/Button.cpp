/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/Button.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Button::Button(Window& owner) throw(UserInterfaceException)
  : Widget(owner),
    brush(WINDOW_BACKGROUND) {
  setBackgroundMode(true);
}

void Button::setBackground(Color background) throw(UserInterfaceException) {
  if (background != this->background) {
    this->background = background;
    brush = Brush(background);
    onDisplay();
  }
}

void Button::setTextColor(Color textColor) throw(UserInterfaceException) {
  if (textColor != this->textColor) {
    this->textColor = textColor;
    setTextColor(textColor);
    onDisplay();
  }
}

void Button::setText(const String& text) throw(UserInterfaceException) {
  if (text != this->text) {
    this->text = text;
    // pre calc.
    onDisplay();
  }
}

void Button::onResize(const Dimension& dimension) throw() {
  onDisplay();
}

void Button::onMouseScope(bool scope) throw() {
  fout << MESSAGE("Button: Mouse scope: ") << scope << ENDL;
  if (scope) {
    brush = Brush(Color(0x50, 0x80, 0xff));
  } else {
    brush = Brush(Color(0x30, 0x60, 0xe0));
  }
  //highlight = scope;
  onDisplay();
}

void Button::onMouseMove(const Position& position, unsigned int state) throw() {
  fout << MESSAGE("Button: Mouse motion event: ") << position << ENDL;
}

void Button::onMouseButton(const Position& position, Mouse::Button button, Mouse::Event event, unsigned int state) throw() {
  fout << MESSAGE("Button: Mouse button event: ") << position << ENDL;
  if (button == Mouse::LEFT) {
    if (event == Mouse::PRESSED) {
      pressed = true;
      brush = Brush(Color(0x10, 0x40, 0xc0));
      setCapture(true);
    } else if (event == Mouse::RELEASED) {
      pressed = false;
      brush = Brush(Color(0x30, 0x60, 0xe0));
      if ((position.getX() >= 0) && (position.getY() >= 0)) {
        // index getDimension
      }
      // if inside window then selected onButton()
      setCapture(false);
    }
    onDisplay();
  }
}

void Button::onKey(unsigned int key, unsigned int flags, unsigned int modifiers) throw() {
  if (flags & Key::PRESSED) {
    if (flags & Key::DEAD) {
      return;
    }
    if (key == Key::SPACE) {
      // send select event to listener
    }
  }
}

void Button::onDisplay() throw() {
  // if has focus then draw box
  rectangle(Position(0, 0), Position(256, 256), brush);
  Widget::text(
    Position(0, 0),
    getDimension(),
    text,
    TextFormat::CENTER|TextFormat::MIDDLE|TextFormat::PREFIX
  );
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
