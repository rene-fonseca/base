/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/ProgressBar.h>
#include <base/string/StringOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

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

void ProgressBar::setCurrentValue(unsigned int value) noexcept {
  if (value != currentValue) {
    currentValue = value;
    invalidate();
  }
}

void ProgressBar::setMaximumValue(unsigned int value) noexcept {
  if (value != maximumValue) {
    maximumValue = value;
    invalidate();
  }
}

// inline Region getReducedRegion(unsigned int width) noexcept {
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

void ProgressBar::onDisplay() noexcept {
  const unsigned int value = minimum(currentValue, maximumValue);

  // draw raised frame
  setPen(darkPen);
  line(getLocalBindingOffset(LOWER_LEFT), Position(0, 0));
  line(Position(0, 0), getLocalBindingOffset(UPPER_RIGHT));
  Position lowerRight = getLocalBindingOffset(LOWER_RIGHT);
  setPen(lightPen);
  line(getLocalBindingOffset(UPPER_RIGHT), lowerRight);
  line(lowerRight, getLocalBindingOffset(LOWER_LEFT));
  
  Dimension dimension = getDimension();
  if ((dimension.getWidth() > 2) && (dimension.getHeight() > 2)) {
    lowerRight -= Position(1, 1);
    
    int offset = (value * (dimension.getWidth() - 2))/maximumValue;
    rectangle(Position(1, 1), Position(offset + 1, lowerRight.getY()), progressBrush);
    
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
