/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_UI__BITMAP_H
#define _DK_SDU_MIP__BASE_UI__BITMAP_H

#include <base/Dimension.h>
#include <base/mem/Reference.h>
#include <base/ui/UserInterfaceException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A bitmap stored in native format.
  
  @short Bitmap.
  @ingroup ui
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Bitmap : public Object {
  friend class GraphicsContext;
private:

  class Handle : public ReferenceCountedObject {
  private:
    
    void* handle;
  public:

    inline Handle(void* _handle) throw() 
      : handle(_handle) {
    }

    inline void* getHandle() const throw() {
      return handle;
    }
    
    ~Handle() throw();
  };

  /** Handle to the bitmap. */
  Reference<Handle> handle;
public:

  /** Pixel format. */
  enum Format {
    GRAY, /**< Gray components. */
    GRAY_ALPHA, /**< Gray and alpha components. */
    RGB, /**< Red, green, and blue components. */
    RGBA /**< Red, green, blue, and alpha components. */
  };

  /** Pixel encoding. */
  enum Encoding {
    GRAY_8, /**< Pixels are stored as */
    RGB_15, /**< Pixels are stored in 16 bits (5 bit per component). */
    RGB_16, /**< Pixels are stored in 16 bits (5 bit for red and blue and 6 for green component). */
    RGB_24, /**< Pixels are stored in 24 bits (8 bits per component with the most significant octet not used). */
    RGB_24_LE, /**< Pixels are stored in 24 bits (8 bits per component with the most significant octet not used). */
    RGB_24_BE, /**< Pixels are stored in 24 bits (8 bits per component with the most significant octet not used). */
    RGB_32, /**< Pixels are stored in 32 bits (8 bits per component). */
    RGB_32_LE, /**< Pixels are stored in 32 bits (8 bits per component). */
    RGB_32_BE, /**< Pixels are stored in 32 bits (8 bits per component). */
    RGBA_32, /**< Each pixel take up 32 bit. */
    RGBA_32_LE, /**< Each pixel take up 32 bit (red, green, blue, and alpha). */
    RGBA_32_BE /**< Each pixel take up 32 bit (alpha, blue, green, and red). */
  };

  /**
    Initializes bitmap as invalid.
  */
  inline Bitmap() throw() : handle(0) {
  }

  /**
    Initializes bitmap.

    @param dimension The dimension of the bitmap.
    @param format The pixel format.
    @param encoding The encoding of the pixels.
    @param data The bitmap data.
  */
  Bitmap(const Dimension& dimension, Format format, Encoding encoding, const void* data) throw(UserInterfaceException);

  /**
    Initializes bitmap from other bitmap.
  */
  inline Bitmap(const Bitmap& copy) throw()
    : handle(copy.handle) {
  }

  /**
    Assignment of bitmap by bitmap.
  */
  inline Bitmap& operator=(const Bitmap& eq) throw() {
    handle = eq.handle;
    return *this;
  }

  /**
    Returns true if the bitmap is valid.
  */
  inline bool isValid() const throw() {
    return handle.isValid();
  }
  
  /**
    Returns the dimension of the bitmap.
  */
  Dimension getDimension() const throw(UserInterfaceException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
