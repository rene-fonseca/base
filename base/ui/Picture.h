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
#include <base/ui/Bitmap.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Picture widget.
  
  @short Picture widget.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Picture : public Widget {
private:

  /** The bitmap. */
  Bitmap bitmap;
public:

  /**
    Initializes the picture widget.
  */
  Picture(Window& owner) throw(UserInterfaceException);
  
  /** Pixel format. */
  enum Format {
    GRAY, /**< Gray component. */
    GRAY_ALPHA, /**< Gray and alpha components. */
    RGB, /**< Red, green, and blue components. */
    RGBA /**< Red, green, blue, and alpha components. */
  };

  /** Pixel encoding. */
  enum Encoding {
    GRAY_8, /**< Gray pixel stored in 8 bit. */
    GRAY_16, /**< Gray pixel stored in 16 bit. */
    GRAY_32, /**< Gray pixel stored in 32 bit. */
    RGB_15, /**< RGB pixel are stored in 16 bits (with most significant bit unused). */
    RGB_16, /**< RGB pixel stored in 16 bits (with green component in 6 bits). */
    RGB_24, /**< RGB pixel stored in 24 bits. */
    RGB_32, /**< RGB pixel stored in 32 bits. */
    RGB_32_LE, /**< Red, green, and blue. */
    RGB_32_BE, /**< Blue, green, and red. */
    RGBA_32, /**< Each pixel take up 32 bit. */
    RGBA_32_LE, /**< Each pixel take up 32 bit (red, green, blue, and alpha). */
    RGBA_32_BE /**< Each pixel take up 32 bit (alpha, blue, green, and red). */
  };
  
  /**
    Sets the picture.
  */
  void setPicture(const Dimension& dimension, Format format, Encoding encoding, void* data) throw();
  
  /**
    Returns the format.
  */
  Format getFormat() throw();
  
  /**
    Returns the encoding.
  */
  Encoding getEncoding() throw();
  
  /**
    Returns the bitmap data.
  */
  inline Bitmap getBitmap() const throw() {
    return bitmap;
  }
  
  /**
    Sets the bitmap.
  */
  void setBitmap(const Bitmap& bitmap) throw(UserInterfaceException);
  
  /**
    Invoked when the widget should be updated.
  */
  void onDisplay() throw();

  /**
    Destroys the picture.
  */
  inline ~Picture() throw() {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
