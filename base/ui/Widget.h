/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_UI__WIDGET_H
#define _DK_SDU_MIP__BASE_UI__WIDGET_H

#include <base/ui/Window.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

struct Color {
  union {
    uint32 value;
    struct {
      uint8 reserved;
      uint8 red;
      uint8 green;
      uint8 blue;
    } _DK_SDU_MIP__BASE__PACKED;
  };

  inline bool operator==(const Color color) const throw() {
    return value == color.value;
  }
  
  inline bool operator!=(const Color color) const throw() {
    return value != color.value;
  }
} _DK_SDU_MIP__BASE__PACKED;

inline Color makeColor(uint8 gray) throw() {
  Color color;
  color.value = (static_cast<uint32>(gray) << 16) |
    (static_cast<uint32>(gray) << 8) |
    static_cast<uint32>(gray);
  return color;
}

inline Color makeColor(uint8 red, uint8 green, uint8 blue) throw() {
  Color color;
  color.value = (static_cast<uint32>(red) << 16) |
    (static_cast<uint32>(green) << 8) |
    static_cast<uint32>(blue);
  return color;
}

inline Color makeColor(unsigned int rgb) throw() {
  Color color;
  color.value = rgb;
  return color;
}

/**
  The base class of all widgets.
  
  @short Widget base class.
  @ingroup ui
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Widget : public WindowImpl {
public:

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
  
  enum Style {
    NONE,
    SOLID,
    DASH,
    DOT,
    DASH_DOT,
    DASH_DOT_DOT
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
    friend class Widget;
  private:
    
    inline Pen(void* handle) throw() : GraphicsContextObject(handle) {
    }
  public:

    inline Pen() throw() : GraphicsContextObject(0) {
    }
    
    Pen(Style style, Color color, unsigned int width = 1) throw(UserInterfaceException);
  };

  /**
    Brush.
  */
  class Brush : GraphicsContextObject {
    friend class Widget;
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
    friend class Widget;
  private:
    
    inline Font(void* handle) throw() : GraphicsContextObject(handle) {
    }
  public:

    inline Font() throw() : GraphicsContextObject(0) {
    }
    
    Font(const String& name) throw(UserInterfaceException);
  };
private:
  
  bool enabled;
  Pen pen;
  Brush brush;
  Font font;
public:
  
  /* named binding points (could also have offsets default is (0,0)) */
  enum Binding {
    UPPER_LEFT, /**< Upper left corner. */
    UPPER_CENTER, /**< Upper center. */
    UPPER_RIGHT, /**< Upper rigth corner. */
    MIDDLE_LEFT, /**< Middle to the left. */
    MIDDLE_CENTER, /**< Middle at the center. */
    MIDDLE_RIGHT, /**< Middle to the right . */
    LOWER_LEFT, /**< Lower left corner. */
    LOWER_CENTER, /**< Lower center. */
    LOWER_RIGHT /**< Lower right corner. */
  };

  enum NamedColor {
    ALMOND_BLANCHED
  };

  static Color getNamedColor(NamedColor color) throw();
  
  Widget(Window& owner) throw(UserInterfaceException);
  
  // full reference counting!
  
  // TAG: need support for relative to client area
  /**
    Returns the position of the binding point relative to the owner.
  */
  Position getBindingOffset(Binding binding) const throw();
  
  inline bool isEnabled() const throw() {
    return enabled;
  }
  
  inline void disable() throw() {
    enabled = false;
  }
  
  inline void enable() throw() {
    enabled = true;
  }
  
//   inline Window getOwner() const throw() {
//     return owner;
//   }

  enum GraphicsFlag {
    FILLED = 1
  };
  
  class Alignment {
  public:
    
    static const unsigned int LEFT = 1;
    static const unsigned int CENTER = 2;
    static const unsigned int RIGHT = 4;
    static const unsigned int TOP = 8;
    static const unsigned int MIDDLE = 16;
    static const unsigned int BOTTOM = 32;
  };

  class TextFormat {
  public:
    
    static const unsigned int LEFT = 1;
    static const unsigned int CENTER = 2;
    static const unsigned int RIGHT = 4;
    static const unsigned int TOP = 8;
    static const unsigned int MIDDLE = 16;
    static const unsigned int BOTTOM = 32;
    static const unsigned int MULTIPLE_LINES = 64;
    static const unsigned int BREAK_WORDS = 128;
    static const unsigned int PREFIX = 256;
  };  

  inline Pen Widget::getPen() const throw() {
    return pen;
  }
  
  void setPen(Pen pen) throw(UserInterfaceException);

  inline Brush Widget::getBrush() const throw() {
    return brush;
  }
  
  void setBrush(Brush brush) throw(UserInterfaceException);
  
  inline Font Widget::getFont() const throw() {
    return font;
  }
  
  void setFont(Font font) throw(UserInterfaceException);
  
  void setBackgroundMode(bool transparent) throw(UserInterfaceException);
  void setBackgroundColor(Color color) throw(UserInterfaceException);
  void setTextColor(Color color) throw(UserInterfaceException);
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
    Returns the dimension of the text.
  */
  Dimension getDimensionOf(const String& text) const throw(UserInterfaceException);

  /**
    Draws text.
  */
  void text(const Position& position, const Dimension& dimension, const String& text, unsigned int flags = 0) throw(UserInterfaceException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
