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

#include <base/ui/Bitmap.h>
#include <base/ui/Color.h>
#include <base/ui/WindowImpl.h>
#include <base/collection/Array.h>
#include <base/mem/Reference.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Graphics context.
  
  @short Graphics context.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API GraphicsContext : public WindowImpl {
public:

  /** System color. */
  enum SystemColor {
    WINDOW_BACKGROUND,
    WINDOW_FRAME,
    WINDOW_TEXT,
    MENU,
    MENU_TEXT,
    BUTTON_TEXT,
    DISABLED_TEXT,
    HIGHLIGHTED_TEXT
  };
  
  /** Pen style. */
  enum PenStyle {
    NONE, /**< None. */
    SOLID, /**< Solid. */
    DASH, /**< Dash. */
    DOT, /**< Dot. */
    DASH_DOT, /**< Dash dot. */
    DASH_DOT_DOT /**< Dash dot dot. */
  };

  class _COM_AZURE_DEV__BASE__API GraphicsContextObjectHandle : public ReferenceCountedObject {
  private:
    
    /** Opaque handle to the graphics context object. */
    void* handle = nullptr;
  public:
    
    inline GraphicsContextObjectHandle(void* _handle) noexcept
      : handle(_handle) {
    }
    
    inline void* getHandle() noexcept {
      return handle;
    }
    
    ~GraphicsContextObjectHandle() noexcept;
  };

  /**
    @short Handle to a graphics context object (e.g. pen, brush, and font).
  */
  class _COM_AZURE_DEV__BASE__API GraphicsContextObject {
  protected:
    
    Reference<GraphicsContextObjectHandle> handle;
  public:

    inline GraphicsContextObject() noexcept {
    }
    
    inline GraphicsContextObject(void* _handle) noexcept
      : handle(new GraphicsContextObjectHandle(_handle)) {
    }

    inline void* getHandle() {
      bassert(handle.isValid(), UserInterfaceException(this));
      return handle->getHandle();
    }

    inline void setHandle(void* handle) noexcept {
      this->handle = new GraphicsContextObjectHandle(handle);
    }
  };

  /**
    @short Handle to a pen.
  */
  class _COM_AZURE_DEV__BASE__API Pen : GraphicsContextObject {
    friend class GraphicsContext;
  private:
    
    inline Pen(void* handle) noexcept : GraphicsContextObject(handle) {
    }
  public:

    /**
      Initializes invalid pen.
    */
    inline Pen() noexcept : GraphicsContextObject(nullptr) {
    }

    /**
      Initializes pen as solid and with 1 in width.

      @param color The desired color.
    */
    Pen(Color color);

    /**
      Initializes pen with the specified style, color, and width.

      @param style The desired style.
      @param color The desired color.
      @param width The desired width.
    */
    Pen(
      PenStyle style,
      Color color,
      unsigned int width = 1);
  };

  /**
    @short Handle to a brush.
  */
  class _COM_AZURE_DEV__BASE__API Brush : GraphicsContextObject {
    friend class GraphicsContext;
  private:
    
    inline Brush(void* handle) noexcept : GraphicsContextObject(handle) {
    }
  public:

    inline Brush() noexcept : GraphicsContextObject(nullptr) {
    }

    /**
      Initializes brush with system color.
    */
    Brush(SystemColor color);

    /**
      Initializes brush from color.
    */
    Brush(Color color);

    /**
      Initializes brush from color.
    */
    Brush(unsigned int color);
  };
  
  /** Font flag. */
  enum FontFlags {
    ITALIC = 1, /**< Italic. */
    UNDERLINE = 2, /**< Underline. */
    STRIKE_OUT = 4 /**< Strike out. */
  };
  
  /** Font weight. */
  enum FontWeight {
    THIN, /**< Thin. */
    LIGHT, /**< Light. */
    NORMAL, /**< Normal. */
    BOLD, /**< Bold. */
    HEAVY /**< Heavy. */
  };
  
  /**
    @short Handle to a font.
  */
  class _COM_AZURE_DEV__BASE__API Font : GraphicsContextObject {
    friend class GraphicsContext;
  private:
    
    inline Font(void* handle) noexcept : GraphicsContextObject(handle) {
    }
  public:

    inline Font() noexcept : GraphicsContextObject(nullptr) {
    }

    /**
      Returns the available fonts.
    */
    static Array<String> getFonts();
    
    // static Information getInformation();
    
    /**
      Initializes the font.
      
      @param name The name of the font.
      @param weight The weight of the font.
      @param flags The flags.
    */
    Font(
      const String& name,
      unsigned short height,
      FontWeight weight = NORMAL,
      unsigned int flags = 0);
  };
  
  enum GraphicsFlag {
    FILLED = 1
  };

  /* Alignment. */
  class _COM_AZURE_DEV__BASE__API Alignment {
  public:
    
    /** Align to the left. */
    static const unsigned int LEFT = 1;
    /** Align at the center (horizontally). */
    static const unsigned int CENTER = 2;
    /** Align to the right. */
    static const unsigned int RIGHT = 4;
    /** Align at the top. */
    static const unsigned int TOP = 8;
    /** Align at the middle (vertically). */
    static const unsigned int MIDDLE = 16;
    /** Align at the bottom. */
    static const unsigned int BOTTOM = 32;
  };

  /* Text format. */
  class _COM_AZURE_DEV__BASE__API TextFormat {
  public:

    /** Align to the left. */
    static const unsigned int LEFT = 1;
    /** Align at the center (horizontally). */
    static const unsigned int CENTER = 2;
    /** Align to the right. */
    static const unsigned int RIGHT = 4;
    /** Align to top. */
    static const unsigned int TOP = 8;
    /** Align to middle (vertically). */
    static const unsigned int MIDDLE = 16;
    /** Align to bottom. */
    static const unsigned int BOTTOM = 32;
    /** Allow multiple lines. */
    static const unsigned int MULTIPLE_LINES = 64;
    /** Break words. */
    static const unsigned int BREAK_WORDS = 128;
    /** Accept '&' as underline prefix. */
    static const unsigned int PREFIX = 256;
  };
private:

  /** The current pen. */
  Pen pen;
  /** The current brush. */
  Brush brush;
  /** The current font. */
  Font font;
public:

  /**
    Initializes the graphics context.
  */
  inline GraphicsContext() {
  }

  /**
    Initializes the graphics context.
  */
  inline GraphicsContext(
    const Position& position,
    const Dimension& dimension,
    unsigned int flags)
    : WindowImpl(position, dimension, flags) {
  }

  /**
    Returns the current pen.
  */
  inline const Pen& getPen() const noexcept {
    return pen;
  }
  
  /**
    Sets the pen.
  */
  void setPen(Pen pen);

  /**
    Returns the current brush.
  */
  inline const Brush& getBrush() const noexcept {
    return brush;
  }

  /**
    Sets the brush.
  */
  void setBrush(Brush brush);

  /**
    Returns the current font.
  */
  inline const Font& getFont() const noexcept {
    return font;
  }

  /**
    Sets the font.
  */
  void setFont(Font font);

  /**
    Sets the background mode.
  */
  void setBackgroundMode(bool transparent);

  /**
    Sets the background color.
  */
  void setBackgroundColor(Color color);

  /**
    Sets the text color.
  */
  void setTextColor(Color color);

  /**
    Sets the text alignment.
  */
  void setTextAlignment(unsigned int alignment);
  
  /**
    Clears the window.
  */
  void clear();
  
  /**
    Clear the specified region.
  */
  void clear(
    const Position& position,
    const Dimension& dimension);
  
  /**
    Draws a point.

    @param position The position of the point.
    @param color The desired color.
    @param flags The flags.
  */
  void setPixel(
    const Position& position,
    Color color,
    unsigned int flags = 0);

  /**
    Returns the color of the pixel at the specified position.

    @param position The position of the pixel.
  */
  Color getPixel(const Position& position);
  
  /**
    Draws a point.
    
    @param positions The positions of the points.
    @param color The desired color.
    @param flags The flags.
  */
  void setPixels(
    const Array<Position>& positions,
    Color color,
    unsigned int flags = 0);
  
  /**
    Sets the current position.

    @param position The position.
  */
  void moveTo(const Position& position);

  /**
    Draws a line from the current position to the specified position using the
    current pen.

    @param position The position.
    @param flags The flags.
  */
  void lineTo(
    const Position& position,
    unsigned int flags = 0);

  /**
    Draws a line using the current pen.

    @param upperLeft The upper left corner.
    @param lowerRight The lower right corner.
    @param flags The flags.
  */
  void line(
    const Position& upperLeft,
    const Position& lowerRight,
    unsigned int flags = 0);
  
  /**
    Draws an arc.
    
    @param flags The flags.
  */
  void arc(
    const Position& position,
    const Dimension& dimension,
    int start,
    int stop,
    unsigned int flags = 0);

  /**
    Draws a rectangle using the current pen and brush.
    
    @param upperLeft The upper left corner.
    @param lowerRight The lower right corner.
    @param flags The flags.
  */
  void rectangle(
    const Position& upperLeft,
    const Position& b,
    unsigned int flags = 0);
  
  /**
    Draws a rectangle using the current pen and brush.
    
    @param position The upper left corner.
    @param dimension The dimension of the rectangle.
    @param flags The flags.
  */
  void rectangle(
    const Position& position,
    const Dimension& dimension,
    unsigned int flags = 0);

  /**
    Draws a rectangle with the specified brush.
    
    @param upperLeft The upper left corner.
    @param lowerRight The lower right corner.
    @param brush The brush used to fill the rectangle.
    @param flags The flags.
  */
  void rectangle(
    const Position& upperLeft,
    const Position& lowerRight,
    Brush brush,
    unsigned int flags = 0);

  /**
    Draws a rectangle with the specified brush.

    @param position The upper left corner.
    @param dimension The dimension of the rectangle.
    @param brush The brush used to fill the rectangle.
    @param flags The flags.
  */
  void rectangle(
    const Position& position,
    const Dimension& dimension,
    Brush brush,
    unsigned int flags = 0);
  
  /**
    Draws an ellipse specified by the bounding rectangle using the current pen
    and brush.
    
    @param upperLeft The upper left corner.
    @param lowerRight The lower right corner.
    @param flags The flags.
  */
  void ellipse(
    const Position& upperLeft,
    const Position& lowerRight,
    unsigned int flags = 0);

  /**
    Draws an ellipse specified by the bounding rectangle using the current pen
    and brush.
    
    @param position The upper left of the corner.
    @param dimension The dimension of the rectangle.
    @param flags The flags.
  */
  void ellipse(
    const Position& position,
    const Dimension& dimension,
    unsigned int flags = 0);

  /**
    Draws a circle specified by the bounding rectangle using the current pen
    and brush.
    
    @param position The upper left of the corner.
    @param dimension The dimension of the rectangle.
    @param flags The flags.
  */
  inline void circle(
    const Position& position,
    unsigned int dimension,
    unsigned int flags = 0) {
    ellipse(position, Dimension(dimension, dimension), flags);
  }
  
  /**
    Draw a pie.

    @param upperLeft The upper left corner of the bounding rectangle.
    @param lowerRight The lower right corner of the bounding rectangle.
    @param radialA End point of first radial.
    @param radialB End point of second radial.
    @param flags The flags.
  */
  void pie(
    const Position& upperLeft,
    const Position& lowerRight,
    const Position& radialA,
    const Position& radialB,
    unsigned int flags);

  /**
    Returns the width of the specified character.
  */
  unsigned int getWidthOf(char ch) const;
  
  /**
    Returns the dimension of the text.
  */
  Dimension getDimensionOf(
    const String& text) const;

  /**
    Draws text.
    
    @param position The upper left of the corner.
    @param dimension The dimension of the rectangle.
    @param flags The flags.
  */
  void text(
    const Position& position,
    const Dimension& dimension,
    const String& text,
    unsigned int flags = 0);

  /**
    Draws the specified image.
    
    @param position The upper left of the corner.
    @param dimension The dimension of the rectangle.
  */
  void putBitmap(
    const Position& position,
    const Dimension& dimension,
    const Bitmap& bitmap);

  /**
    Extracts the specified region as a bitmap.
    
    @param position The upper left of the corner.
    @param dimension The dimension of the rectangle.
  */
  Bitmap getBitmap(
    const Position& position,
    const Dimension& dimension);

  /**
    Destroys the graphics context.
  */
  ~GraphicsContext() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
