/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/ProgressBar.h>
#include <base/string/StringOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

ProgressBar::ProgressBar(Window& owner) throw(UserInterfaceException)
  : Widget(owner),
    progressBrush(Color(0, 0, 192)),
    lightPen(Color(192, 192, 192)),
    darkPen(Color(32, 32, 32)),
    textMode(PERCENT),
    currentValue(0),
    maximumValue(PrimitiveTraits<unsigned int>::MAXIMUM) {
  setTextColor(Color(128, 128, 128));
  // TAG: select nice font
  setBackgroundMode(true);
}

void ProgressBar::setTextMode(TextMode textMode) throw(UserInterfaceException) {
  if (textMode != this->textMode) {
    this->textMode = textMode;
    invalidate();
  }
}

void ProgressBar::setCurrentValue(unsigned int value) throw() {
  if (value != currentValue) {
    currentValue = value;
    invalidate();
  }
}

void ProgressBar::setMaximumValue(unsigned int value) throw() {
  if (value != maximumValue) {
    maximumValue = value;
    invalidate();
  }
}

// inline Region getReducedRegion(unsigned int width) throw() {
//   Region result;
//   result.setX(getPosition().getX() + width);
//   result.setY(getPosition().getY() + width);
//   Dimension dimension = getDimension();
//   if (dimension.getWidth() > 2 * width) {
//     result.setWidth(dimension.getWidth() - 2 * width);
//   } else {
//     result.setWidth(0);
//   }
//   if (dimension.getHeight() > 2 * width) {
//     result.setHeight(dimension.getHeight() - 2 * width);
//   } else {
//     result.setHeight(0);
//   }
//   return result;
// }

void ProgressBar::onDisplay() throw() {
  const unsigned int value = minimum(currentValue, maximumValue);
  
  // draw raised/sunken frame  
  setPen(darkPen);
  line(getLocalBindingOffset(UPPER_LEFT), getLocalBindingOffset(UPPER_RIGHT));
  line(getLocalBindingOffset(UPPER_LEFT), getLocalBindingOffset(LOWER_LEFT));
  setPen(lightPen);
  line(getLocalBindingOffset(UPPER_RIGHT), getLocalBindingOffset(LOWER_RIGHT));
  line(getLocalBindingOffset(LOWER_LEFT), getLocalBindingOffset(LOWER_RIGHT));
  
  Dimension dimension = getDimension();
  if ((dimension.getWidth() > 2) && (dimension.getHeight() > 2)) {
    const Position lowerRight(dimension.getWidth() - 1, dimension.getHeight() - 1);
    
    int offset = (value * (dimension.getWidth() - 2))/maximumValue;
    rectangle(Position(1, 1), Position(offset + 1, dimension.getHeight() - 1), progressBrush);
    
    // clear background
    rectangle(Position(offset + 1, 1), lowerRight, getBrush());
    
    if (textMode != NO_TEXT) {
      StringOutputStream stream;
      switch (textMode) {
      case PERCENT:
        stream << value * 100/maximumValue << '%' << FLUSH;
        break;
      case SLASH:
        stream << value << '/' << maximumValue << FLUSH;
        break;
      case PARENTHESIS:
        stream << value << ' ' << '(' << maximumValue << ')' << FLUSH;
        break;
      default:
        break;
      }
      text(
        Position(1, 1),
        Dimension(dimension.getWidth() - 2, dimension.getHeight() - 2),
        stream.getString(),
        TextFormat::CENTER|TextFormat::MIDDLE
      );
    }
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
