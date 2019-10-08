/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/ui/Widget.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Split widget.
  
  @short Split widget.
  @ingroup ui
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API Split : public Widget {
private:

  /** Offset of the left or top of the splitting field. */
  int offset;
  unsigned int flags;
  unsigned int widthOfSplit;
  // Picture
  
  bool drag;
  int originalOffset;
  int originalPosition;

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
  
  Split(Window& owner) throw(UserInterfaceException);

//   unsigned int getFlags() const throw() {
//   }

  inline bool isVerticalSplit() const throw() {
    return flags & VERTICAL_SPLIT;
  }

  inline bool isHorizontalSplit() const throw() {
    return flags & HORIZONTAL_SPLIT;
  }

  inline bool getFixed() const throw() {
    return flags & FIXED_SPLIT;
  }
  
  void setFixed(bool fixed) throw() {
    //(flags & ~FIXED_SPLIT)
  }

  unsigned int getOffset(Field field) throw() {
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
  
  void setOffset(int offset, Field field) throw();
  
  unsigned int getSplitWidth() throw() {
    return widthOfSplit;
  }

  void setWidthOfSplit(unsigned int width) throw() {
    widthOfSplit = width;
  }
  
  void onResize(const Dimension& dimension) throw();

  void onMouseScope(bool scope) throw();
  
  void onMouseMove(const Position& position, unsigned int state) throw();

  void onMouseButton(const Position& position, Mouse::Button button, Mouse::Event event, unsigned int state) throw();

  void onKey(unsigned int key, unsigned int flags, unsigned int modifiers) throw();

  void onDisplay() throw();

  /**
    Destroys the split.
  */
  inline ~Split() throw() {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
