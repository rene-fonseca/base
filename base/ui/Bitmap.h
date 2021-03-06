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

#include <base/Dimension.h>
#include <base/mem/Reference.h>
#include <base/ui/UserInterfaceException.h>
#include <base/ui/Position.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A bitmap stored in native format.
  
  @short Bitmap.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Bitmap : public Object {
  friend class GraphicsContext;
private:

  class _COM_AZURE_DEV__BASE__API Handle : public ReferenceCountedObject {
  private:
    
    void* handle = nullptr;
  public:

    inline Handle(void* _handle) noexcept 
      : handle(_handle) {
    }

    inline void* getHandle() const noexcept {
      return handle;
    }
    
    ~Handle() noexcept;
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
  inline Bitmap() noexcept {
  }

  /**
    Initializes bitmap.

    @param dimension The dimension of the bitmap.
    @param format The pixel format.
    @param encoding The encoding of the pixels.
    @param data The bitmap data.
  */
  Bitmap(
    const Dimension& dimension,
    Format format,
    Encoding encoding,
    const void* data);

  /**
    Initializes bitmap from other bitmap.
  */
  inline Bitmap(const Bitmap& copy) noexcept
    : handle(copy.handle) {
  }

  /**
    Assignment of bitmap by bitmap.
  */
  inline Bitmap& operator=(const Bitmap& assign) noexcept {
    handle = assign.handle;
    return *this;
  }

  /**
    Returns true if the bitmap is valid.
  */
  inline bool isValid() const noexcept {
    return handle.isValid();
  }
  
  /**
    Returns the dimension of the bitmap.
  */
  Dimension getDimension() const;

  /**
    Returns the value of the specified pixel.
  */
  uint32 getPixel(
    const Position& position) const;

  /**
    Sets the value of the specified pixel.
  */
  void setPixel(
    const Position& position,
    uint32 value) const;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
