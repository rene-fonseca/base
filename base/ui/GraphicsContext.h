/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_UI__GRAPHICS_CONTEXT_H
#define _DK_SDU_MIP__BASE_UI__GRAPHICS_CONTEXT_H

#include <base/ui/Bitmap.h>
#include <base/ui/Color.h>
#include <base/ui/WindowImpl.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Graphics context.
  
  @short Graphics context.
  @ingroup ui
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class GraphicsContext : public WindowImpl {
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

  class GraphicsContextObjectHandle : public ReferenceCountedObject {
  private:
    
    /** Opaque handle to the graphics context object. */
    void* handle;
  public:
    
    inline GraphicsContextObjectHandle(void* _handle) throw()
      : handle(_handle) {
    }
    
    inline void* getHandle() throw() {
      return handle;
    }
    
    ~GraphicsContextObjectHandle() throw();
  };

  /** Handle to a graphics context object. */
  class GraphicsContextObject {
  protected:
    
    ReferenceCountedObjectPointer<GraphicsContextObjectHandle> handle;
  public:

    inline GraphicsContextObject() throw() {
    }
    
    inline GraphicsContextObject(void* _handle) throw()
      : handle(new GraphicsContextObjectHandle(_handle)) {
    }

    inline void* getHandle() throw(UserInterfaceException) {
      assert(handle.isValid(), UserInterfaceException(this));
      return handle->getHandle();
    }

    inline void setHandle(void* handle) throw() {
      this->handle = new GraphicsContextObjectHandle(handle);
    }
  };

  /**
    Pen.
  */
  class Pen : GraphicsContextObject {
    friend class GraphicsContext;
  private:
    
    inline Pen(void* handle) throw() : GraphicsContextObject(handle) {
    }
  public:

    inline Pen() throw() : GraphicsContextObject(0) {
    }
    
    Pen(PenStyle style, Color color, unsigned int width = 1) throw(UserInterfaceException);
  };

  /**
    Brush.
  */
  class Brush : GraphicsContextObject {
    friend class GraphicsContext;
  private:
    
    inline Brush(void* handle) throw() : GraphicsContextObject(handle) {
    }
  public:

    inline Brush() throw() : GraphicsContextObject(0) {
    }
    
    Brush(SystemColor color) throw(UserInterfaceException);
    
    Brush(Color color) throw(UserInterfaceException);

    Brush(unsigned int color) throw(UserInterfaceException);
  };

  /**
    Font.
  */
  class Font : GraphicsContextObject {
    friend class GraphicsContext;
  private:
    
    inline Font(void* handle) throw() : GraphicsContextObject(handle) {
    }
  public:

    inline Font() throw() : GraphicsContextObject(0) {
    }
    
    Font(const String& name) throw(UserInterfaceException);
  };
  
  enum GraphicsFlag {
    FILLED = 1
  };

  /** Alignment. */
  class Alignment {
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

  /** Text format. */
  class TextFormat {
  public:

    /**< Align to the left. */
    static const unsigned int LEFT = 1;
    /**< Align at the center (horizontally). */
    static const unsigned int CENTER = 2;
    /**< Align to the right. */
    static const unsigned int RIGHT = 4;
    /**< Align to top. */
    static const unsigned int TOP = 8;
    /**< Align to middle (vertically). */
    static const unsigned int MIDDLE = 16;
    /**< Align to bottom. */
    static const unsigned int BOTTOM = 32;
    /**< Allow multiple lines. */
    static const unsigned int MULTIPLE_LINES = 64;
    /**< Break words. */
    static const unsigned int BREAK_WORDS = 128;
    /**< Accept '&' as underline prefix. */
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
  inline GraphicsContext() throw(UserInterfaceException) {
  }

  /**
    Initializes the graphics context.
  */
  inline GraphicsContext(
    const Position& position,
    const Dimension& dimension,
    unsigned int flags) throw(UserInterfaceException)
    : WindowImpl(position, dimension, flags) {
  }

  /**
    Returns the current pen.
  */
  inline Pen getPen() const throw() {
    return pen;
  }
  
  /**
    Sets the pen.
  */
  void setPen(Pen pen) throw(UserInterfaceException);

  /**
    Returns the current brush.
  */
  inline Brush getBrush() const throw() {
    return brush;
  }

  /**
    Sets the brush.
  */
  void setBrush(Brush brush) throw(UserInterfaceException);

  /**
    Returns the current font.
  */
  inline Font getFont() const throw() {
    return font;
  }

  /**
    Sets the font.
  */
  void setFont(Font font) throw(UserInterfaceException);

  /**
    Sets the background mode.
  */
  void setBackgroundMode(bool transparent) throw(UserInterfaceException);

  /**
    Sets the background color.
  */
  void setBackgroundColor(Color color) throw(UserInterfaceException);

  /**
    Sets the text color.
  */
  void setTextColor(Color color) throw(UserInterfaceException);

  /**
    Sets the text alignment.
  */
  void setTextAlignment(unsigned int alignment) throw(UserInterfaceException);
  
  /**
    Clears the window.
  */
  void clear() throw(UserInterfaceException);
  
  /**
    Clear the specified region.
  */
  void clear(const Position& position, const Dimension& dimension) throw(UserInterfaceException);
  
  /**
    Draws a point.
  */
  void point(const Position& position, unsigned int flags = 0) throw(UserInterfaceException);
  
  /**
    Draws a line.
  */
  void line(const Position& a, const Position& b, unsigned int flags = 0) throw(UserInterfaceException);

  /**
    Draws an arc.
  */
  void arc(const Position& position, const Dimension& dimension, int start, int stop, unsigned int flags = 0) throw(UserInterfaceException);

  /**
    Draws a rectangle.
  */
  void rectangle(const Position& a, const Position& b, unsigned int flags = 0) throw(UserInterfaceException);
  
  /**
    Draws a rectangle.
  */
  void rectangle(const Position& position, const Dimension& dimension, unsigned int flags = 0) throw(UserInterfaceException);

  /**
    Draws a rectangle with the specified brush.
  */
  void rectangle(const Position& a, const Position& b, Brush brush, unsigned int flags = 0) throw(UserInterfaceException);
  
  /**
    Draws an ellipse.

    @param a Corner.
    @param b Corner.
  */
  void ellipse(const Position& a, const Position& b, unsigned int flags = 0) throw(UserInterfaceException);

  /**
    Draw a pie.

    @param a Corner.
    @param b Corner.
    @param radialA End point of first radial.
    @param radialB End point of second radial.
  */
  void pie(
    const Position& a,
    const Position& b,
    const Position& radialA,
    const Position& radialB,
    unsigned int flags) throw(UserInterfaceException);

  /**
    Returns the dimension of the text.
  */
  Dimension getDimensionOf(const String& text) const throw(UserInterfaceException);

  /**
    Draws text.
  */
  void text(
    const Position& position,
    const Dimension& dimension,
    const String& text,
    unsigned int flags = 0) throw(UserInterfaceException);

  /**
    Draws the specified image.
  */
  void putBitmap(
    const Position& position,
    const Dimension& dimension,
    Bitmap bitmap) throw(UserInterfaceException);

  /**
    Extracts the specified region as a bitmap.
  */
  Bitmap getBitmap(
    const Position& position,
    const Dimension& dimension) throw(UserInterfaceException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
