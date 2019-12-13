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
  Slider widget.
  
  @short Slider widget.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Slider : public Widget {
private:

  /** Offset of the left or top of the splitting field. */
  int offset = 0;
  unsigned int flags = 0;
  // Picture
  
  bool drag = false;
  int originalOffset = 0;
  int originalPosition = 0;

  Brush background;
  Brush split;
public:
  
  Slider(Window& owner);
  
  inline int getOffset() const noexcept {
    return offset;
  }
  
  void setOffset(int offset) noexcept;
  
  void onResize(const Dimension& dimension) noexcept;
  
  void onMouseMove(const Position& position, unsigned int state) noexcept;

  void onMouseButton(const Position& position, Mouse::Button button, Mouse::Event event, unsigned int state) noexcept;

  void onKey(unsigned int key, unsigned int flags, unsigned int modifiers) noexcept;

  void onDisplay() noexcept;

  /**
    Destroys the split.
  */
  ~Slider() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
