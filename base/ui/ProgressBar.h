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
  Progress bar widget.
  
  @short Progress bar widget.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API ProgressBar : public Widget {
public:

  enum TextMode {
    NO_TEXT, /**< Do not display progress as text. */
    PERCENT, /**< Display progress in percent (e.g. "48%"). */
    SLASH, /**< Display progress as "123/256". */
    PARENTHESIS /**< Display progress as "123 (256)". */
  };
private:

  Brush progressBrush;
  Pen lightPen;
  Pen darkPen;
  TextMode textMode = NO_TEXT;
  unsigned int currentValue = 0;
  unsigned int maximumValue = 0;
public:

  /**
    Initializes the button widget.
  */
  ProgressBar(Window& owner);

  /**
    Returns the text mode.
  */
  inline TextMode getTextMode() const noexcept {
    return textMode;
  }

  /**
    Sets the text mode.
  */
  void setTextMode(TextMode textMode);
  
  /**
    Returns the current value.
  */
  inline unsigned int getCurrentValue() const noexcept {
    return currentValue;
  }

  /**
    Sets the current value.
  */
  void setCurrentValue(unsigned int value) noexcept;

  /**
    Returns the maximum value.
  */
  inline unsigned int getMaximumValue() const noexcept {
    return maximumValue;
  }

  /**
    Sets the maximum value.
  */
  void setMaximumValue(unsigned int value) noexcept;

  /**
    Invoked when the button must be updated.
  */
  void onDisplay() noexcept;

  /**
    Destroys the button.
  */
  inline ~ProgressBar() noexcept {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
