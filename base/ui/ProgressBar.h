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
  Progress bar widget.
  
  @short Progress bar widget.
  @ingroup ui
  @version 1.0
*/

class ProgressBar : public Widget {
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
  TextMode textMode;
  unsigned int currentValue = 0;
  unsigned int maximumValue = 0;
public:

  /**
    Initializes the button widget.
  */
  ProgressBar(Window& owner) throw(UserInterfaceException);

  /**
    Returns the text mode.
  */
  inline TextMode getTextMode() const throw() {
    return textMode;
  }

  /**
    Sets the text mode.
  */
  void setTextMode(TextMode textMode) throw(UserInterfaceException);
  
  /**
    Returns the current value.
  */
  inline unsigned int getCurrentValue() const throw() {
    return currentValue;
  }

  /**
    Sets the current value.
  */
  void setCurrentValue(unsigned int value) throw();

  /**
    Returns the maximum value.
  */
  inline unsigned int getMaximumValue() const throw() {
    return maximumValue;
  }

  /**
    Sets the maximum value.
  */
  void setMaximumValue(unsigned int value) throw();  

  /**
    Invoked when the button must be updated.
  */
  void onDisplay() throw();

  /**
    Destroys the button.
  */
  inline ~ProgressBar() throw() {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
