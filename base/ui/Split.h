/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/ui/Widget.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Split widget.
  
  @short Split widget.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Split : public Widget {
private:

  /** Offset of the left or top of the splitting field. */
  int offset = 0;
  unsigned int flags = 0;
  unsigned int widthOfSplit = 0;
  // Picture
  
  bool drag = false;
  int originalOffset = 0;
  int originalPosition = 0;

  Brush background;
  Brush split;
public:

  enum {
    WIDGET_FLAG = 1,
    VERTICAL_SPLIT = WIDGET_FLAG,
    HORIZONTAL_SPLIT = VERTICAL_SPLIT << 1,
    FIXED_SPLIT
  };

  enum Field {
    FIRST, /**< The left or top window. */
    SECOND, /**< The right or bottom window. */
    RELATIVE
  };
  
  Split(Window& owner);

//   unsigned int getFlags() const noexcept {
//   }

  inline bool isVerticalSplit() const noexcept {
    return flags & VERTICAL_SPLIT;
  }

  inline bool isHorizontalSplit() const noexcept {
    return flags & HORIZONTAL_SPLIT;
  }

  inline bool getFixed() const noexcept {
    return flags & FIXED_SPLIT;
  }
  
  void setFixed(bool fixed) noexcept {
    //(flags & ~FIXED_SPLIT)
  }

  unsigned int getOffset(Field field) noexcept {
    const Dimension dimension = getDimension();
    const unsigned int total = (flags & VERTICAL_SPLIT) ? dimension.getWidth() : dimension.getHeight();
    unsigned int limit = total - widthOfSplit;
    if (offset > static_cast<int>(limit)) {
      offset = limit;
    }
    switch (field) {
    case FIRST:
      return offset; // |--F->|s|  S  |
    case SECOND:
      return limit - offset; // |  F  |s|<-S--|
    case RELATIVE:
      return 0;
    }
    return 0;
  }
  
  void setOffset(int offset, Field field) noexcept;
  
  unsigned int getSplitWidth() noexcept {
    return widthOfSplit;
  }

  void setWidthOfSplit(unsigned int width) noexcept {
    widthOfSplit = width;
  }
  
  void onResize(const Dimension& dimension) noexcept;

  void onMouseScope(bool scope) noexcept;
  
  void onMouseMove(const Position& position, unsigned int state) noexcept;

  void onMouseButton(const Position& position, Mouse::Button button, Mouse::Event event, unsigned int state) noexcept;

  void onKey(unsigned int key, unsigned int flags, unsigned int modifiers) noexcept;

  void onDisplay() noexcept;

  /**
    Destroys the split.
  */
  inline ~Split() noexcept {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
