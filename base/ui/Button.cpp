/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/Button.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Button::Button(Window& owner) throw(UserInterfaceException)
  : Widget(owner),
    grayedBrush(Color(192, 192, 192)),
    pressedBrush(Color(64, 96, 192)),
    highlightBrush(Color(128, 160, 255)),
    lightPen(Color(192, 192, 255)),
    darkPen(Color(16, 32, 128)),
    normalTextColor(0, 0, 0),
    pressedTextColor(128, 160, 255),
    grayed(false),
    pressed(false),
    highlighted(false) {
  setBrush(Color(96, 128, 255));
  setFont(Font("Arial", 14, HEAVY));
  setBackgroundMode(true);
}

void Button::setText(const String& text) throw(UserInterfaceException) {
  if (text != this->text) {
    this->text = text;
    invalidate();
  }
}

Dimension Button::getPreferredSize() throw(UserInterfaceException) {
  Dimension dimension = getDimensionOf(text);
  return Dimension(
    dimension.getWidth() + 2 * PREFERRED_HORIZONTAL_MARGIN,
    dimension.getHeight() + 2 * PREFERRED_VERTICAL_MARGIN
  );
}

void Button::onMouseScope(bool scope) throw() {
  highlighted = scope;
  invalidate();
}

void Button::onMouseButton(
  const Position& position,
  Mouse::Button button,
  Mouse::Event event,
  unsigned int state) throw() {
  if (button == Mouse::LEFT) {
    if (event == Mouse::PRESSED) {
      pressed = true;
      setTextColor(pressedTextColor);
      setCapture(true);
    } else if (event == Mouse::RELEASED) {
      if (pressed) {
        pressed = false;
        setTextColor(normalTextColor);
        if (position.isWithin(getDimension())) {
          highlighted = true;
          onSelection();
        } else {
          highlighted = false;
        }
        setCapture(false);
      }
    }
    invalidate();
  }
}

void Button::onSelection() throw() {
  fout << "Button: Selection event" << ENDL;
}

void Button::onKey(
  unsigned int key,
  unsigned int flags,
  unsigned int modifiers) throw() {
  if (flags & Key::PRESSED) {
    if (flags & Key::DEAD) {
      return;
    }
    if (key == Key::SPACE) {
      // when released
      onSelection();
    }
  }
}

void Button::onDisplay() throw() {
  // focus, normal, pressed, highlighted
  // 0: enabled, disabled (ignore all other)
  // 1: focus (mark button), no focus
  // 2: over (highlight), not over
  // 3: pressed (always over), not pressed (use over/not over)  
  
  Position upperLeft = Position(0, 0);
  Position lowerRight = getLocalBindingOffset(LOWER_RIGHT);
  if (pressed) {
    // draw sunken frame
    setPen(darkPen);
    line(getLocalBindingOffset(LOWER_LEFT), upperLeft);
    line(upperLeft, getLocalBindingOffset(UPPER_RIGHT));
    setPen(lightPen);
    line(getLocalBindingOffset(UPPER_RIGHT), lowerRight);
    line(lowerRight, getLocalBindingOffset(LOWER_LEFT));    
  } else {
    // draw raised frame
    setPen(lightPen);
    line(getLocalBindingOffset(LOWER_LEFT), upperLeft);
    line(upperLeft, getLocalBindingOffset(UPPER_RIGHT));
    setPen(darkPen);
    line(getLocalBindingOffset(UPPER_RIGHT), lowerRight);
    line(lowerRight, getLocalBindingOffset(LOWER_LEFT));
  }
  
  upperLeft += Position(1, 1);
  lowerRight -= Position(1, 1);
  if (lowerRight.isAfter(upperLeft)) {
    if (!grayed) {
      if (pressed) {
        rectangle(upperLeft, lowerRight, pressedBrush);
      } else {
        if (highlighted) {
          rectangle(upperLeft, lowerRight, highlightBrush);
        } else {
          rectangle(upperLeft, lowerRight, getBrush()); // normal
        }
      }
      Widget::text(
        upperLeft,
        upperLeft.getDimension(lowerRight),
        text,
        TextFormat::CENTER|TextFormat::MIDDLE|TextFormat::PREFIX
      );
    } else {
      rectangle(upperLeft, lowerRight, grayedBrush);
      // TAG: draw grayed text
    }
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
