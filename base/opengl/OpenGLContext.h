/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_OPEN_GL__OPEN_GL_CONTEXT_H
#define _DK_SDU_MIP__BASE_OPEN_GL__OPEN_GL_CONTEXT_H

#include <base/opengl/OpenGL.h>
#include <base/Dimension.h>
#include <base/opengl/OpenGL.h>
#include <base/ui/Menu.h>
#include <base/ui/Position.h>
#include <base/SingletonException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// TAG: need wait method with timeout support

namespace OpenGLContextImpl {

  class Delegate;

  void* createContext(const String& title, const Position& position, const Dimension& dimension, unsigned int flags) throw(OpenGLException);
};

/**
  OpenGL context.

  @short OpenGL context.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OpenGLContext : public Object {
  friend class OpenGLContextImpl::Delegate;
  friend void* OpenGLContextImpl::createContext(const String& title, const Position& position, const Dimension& dimension, unsigned int flags) throw(OpenGLException);
public:

  /** Display modes. */
  enum DisplayMode {
    MODE_WINDOW, /**< Window mode. */
    MODE_FULL_SCREEN /**< Full screen mode. */
  };

  /** The cursors. */
  enum Cursor {
    NONE, /**< No cursor. */
    WORKING_IN_BACKGROUND, /**< Cursor indicating working is being the background. */
    ARROW, /**< The normal selection cursor. */
    CROSSHAIR,
    HAND,
    HELP,
    NO,
    MOVING,
    NORTHEAST_SOUTHWEST,
    NORTH_SOUTH, /**< A birectional arrow. */
    NORTHWEST_SOUTHEAST,
    WEST_EAST, /**< A birectional arrow pointer in the west and east direction. */
    VERTICAL_ARROW, /**< A vertical arrow. */
    WAITING /**< Cursor indicating blocked wait. */
  };

//   enum Cursor {
//     LEFT_ARROW,
//     RIGHT_ARROW,
//     INFO,
//     DESTROY,
//     HELP,
//     CYCLE,
//     SPRAY,
//     WAIT,
//     TEXT,
//     UP_DOWN, 
//     LEFT_RIGHT,
//     TOP_SIDE,
//     BOTTOM_SIDE,
//     LEFT_SIDE,
//     RIGHT_SIDE,
//     TOP_LEFT_CORNER,
//     TOP_RIGHT_CORNER,
//     BOTTOM_RIGHT_CORNER,
//     BOTTOM_LEFT_CORNER,
//     FULL_CROSSHAIR,
//     CROSSHAIR,
//     NONE,
//     INHERIT
//   };
private:  

  /** Specifies the current number of OpenGL contexts. */
  static unsigned int numberOfContexts;
  
  /** Opaque handle to OpenGL context. */
  void* handle;
  /** The display mode. */
  DisplayMode displayMode;

//   unsigned int numberOfOverlayPlanes;
//   unsigned int numberOfUnderlayPlanes;
  /** The minimum dimension of the window. */
  Dimension minimumSize;
  /** The maximum dimension of the window. */
  Dimension maximumSize;

  /** The current keyboard state. */
  uint8 keyboardState[256];
  /** The modifiers. */
  unsigned int modifiers;
  /** The autopreat flag. */
  bool autorepeat;
  /** Flag specifying whether the mouse is in the scope of the client area of the window. */
  bool scope;
  /** Flag specifying whether the window is active. */
  bool active;
  /** The position of the window. */
  Position position;
  /** The dimension of the window. */
  Dimension dimension;
  /** The current cursor. */
  Cursor cursor;
  
  inline void setCurrentPosition(const Position& position) throw() {
    this->position = position;
  }
  
  inline void setCurrentDimension(const Dimension& dimension) throw() {
    this->dimension = dimension;
  }
public:

  /** OpenGL implementation. */
  OpenGL openGL;
  
  enum Flag {
    COLOR_INDEX = 1 << 0,
    DOUBLE_BUFFERED = 1 << 1,
    ACCUMULATOR = 1 << 2,
    ALPHA = 1 << 3,
    DEPTH = 1 << 4,
    STENCIL = 1 << 5,
    MULTI_SAMPLE = 1 << 6,
    STEREO = 1 << 7,
    LUMINANCE = 1 << 8
  };

//     enum Visibility {INVISIBLE, VISIBLE};
//     enum Focus {LOST_FOCUS, ACQUIRED_FOCUS};
  
  // other flags
  // resizeable
  
  struct PixelFormat {
    unsigned int flags;
    //PixelType pixelType;
    uint8 colorBits;
    uint8 redBits;
    uint8 redShift;
    uint8 greenBits;
    uint8 greenShift;
    uint8 blueBits;
    uint8 blueShift;
    uint8 alphaBits;
    uint8 alphaShift;
    uint8 accumulatorBits;
    uint8 accumulatorRedBits;
    uint8 accumulatorGreenBits;
    uint8 accumulatorBlueBits;
    uint8 accumulatorAlphaBits;
    uint8 depthBits;
    uint8 stencilBits;
    uint8 auxBuffers;
  };

//     struct BufferInformation {
//       int bufferSize;
//       int stencilSize;
//       int depthSize;
//       int redSize;
//       int greenSize;
//       int blueSize;
//       int alphaSize;
//       int accumRedSize;
//       int accumGreenSize;
//       int accumBlueSize;
//       int accumAlphaSize;
//     };
  
//   inline unsigned int getCreationFlags() const throw() {
//     return creationFlags;
//   }
  
  // TAG: parent window
  // TAG: number of children
  // TAG: get pop menu
  // TAG: get fixed menu

  class Mouse {
  public:

    enum Button {
      LEFT = 1, /**< Left mouse button. */
      MIDDLE = LEFT << 1, /**< Middle mouse button. */
      RIGHT = MIDDLE << 1, /**< Right mouse button. */
      WHEEL = RIGHT << 1, /**< Mouse wheel. */
      EXTRA = WHEEL << 1, /**< Extra mouse button. */
      EXTRA2 = EXTRA << 1 /**< Secondary extra button. */
    };
  
    enum Subevent {
      PRESSED, /**< Mouse button has been pressed. */
      RELEASED, /**< Mouse button has been released. */
      DOUBLE_CLICKED, /**< Mouse button has been double-clicked. */
      TURNED /**< Mouse wheel has been turned. */
    };
  };

  class Key {
  public:

    /** Key code. */
    enum Code {
      BACK = 0x08,
      TAB = 0x09,
      RETURN = 0x0d,
      ESCAPE = 0x1b,
      SPACE = 0x20,
      INSERT = 0x100, /**< Insert. */ // does not conflict with 8-bit characters
      DELETE, /**< Delete. */
      HOME, /**< Home. */
      END, /**< End. */
      PRIOR, /**< Prior. */
      NEXT, /**< Next. */
      LEFT, /**< Left arrow. */
      RIGHT, /**< Right arrow. */
      UP, /**< Up arrow. */
      DOWN, /**< Down arrow. */
      SELECT,
      PRINT,
      EXECUTE,
      SNAPSHOT,
      HELP,
      CAPS_LOCK,
      SCROLL_LOCK,
      NUM_LOCK,
      PAUSE,
      LEFT_MANAGER,
      RIGHT_MANAGER,
      MENU,
      // TAG: add num pad keys here
      F1, /**< Function key 1. */
      F2, /**< Function key 2. */
      F3, /**< Function key 3. */
      F4, /**< Function key 4. */
      F5, /**< Function key 5. */
      F6, /**< Function key 6. */
      F7, /**< Function key 7. */
      F8, /**< Function key 8. */
      F9, /**< Function key 9. */
      F10, /**< Function key 10. */
      F11, /**< Function key 11. */
      F12, /**< Function key 12. */
      F13, /**< Function key 13. */
      F14, /**< Function key 14. */
      F15, /**< Function key 15. */
      F16, /**< Function key 16. */
      F17, /**< Function key 17. */
      F18, /**< Function key 18. */
      F19, /**< Function key 19. */
      F20, /**< Function key 20. */
      F21, /**< Function key 21. */
      F22, /**< Function key 22. */
      F23, /**< Function key 23. */
      F24 /**< Function key 24. */
    };

    /** Keyboard modifier. */
    enum Modifier {
      LEFT_CONTROL = Mouse::RIGHT << 1, /**< Left control key. */
      RIGHT_CONTROL = LEFT_CONTROL << 1, /**< Right control key. */
      CONTROL = LEFT_CONTROL | RIGHT_CONTROL, /**< Any control key. */
      LEFT_SHIFT = RIGHT_CONTROL << 1, /**< Left shift key. */
      RIGHT_SHIFT = LEFT_SHIFT << 1, /**< Right shift key. */
      SHIFT = LEFT_SHIFT | RIGHT_SHIFT, /**< Any shift key. */
      LEFT_ALT = RIGHT_SHIFT << 1, /**< Left alt key. */
      RIGHT_ALT = LEFT_ALT << 1, /**< Right alt key. */
      ALT = LEFT_ALT | RIGHT_ALT, /**< Any alt key. */
      NUM_LOCK_TOGGLED = RIGHT_ALT << 1, /**< Num lock is toggled. */
      CAPS_LOCK_TOGGLED = NUM_LOCK_TOGGLED << 1, /**< Caps lock is toggled. */
      SCROLL_LOCK_TOGGLED = CAPS_LOCK_TOGGLED << 1, /**< Scroll lock is toggled. */
      INSERT_TOGGLED = SCROLL_LOCK_TOGGLED << 1 /**< Insert is toggled. */
    };

    /** Key event flags. */
    enum Flag {
      PRESSED = 1, /**< Specifies that the key has been pressed (otherwise released). */
      FIRST_TIME = 2, /**< Specifies that the key has been pressed for the first time since last release. */
      EXTENDED = 4, /**< Specifies that the key is an extended key (e.g. CONTROL). */
      DEAD = 8, /**< Indicates a dead key (such a key is followed by another key). */
      ASCII = 16, /**< Specifies that the key is an ASCII character. */
      TOGGLED = 32, /**< Specifies that the key is toggled. */
      UNKNOWN = 64 /**< Specifies that the code is not associated with a supported key. */
    };
  };
  
  /**
    Initializes a new OpenGL context.
  */
  OpenGLContext(const String& title, const Position& position, const Dimension& dimension, unsigned int flags) throw(OpenGLException);

  /**
    Displays the specified menu at the specified position.

    @param position The position.
    @param menu The menu.
  */
  void displayMenu(const Position& position, const Menu& menu) throw(UserInterfaceException);

  /**
    Returns the autorepeat.
  */
  inline bool getAutorepeat() const throw() {
    return autorepeat;
  }

  /**
    Sets the autorepeat flag.
  */
  inline void setAutorepeat(bool autorepeat) throw() {
    this->autorepeat = autorepeat;
  }
  
  /**
    Returns the number of overlay planes.
  */
  unsigned int getNumberOfOverlayPlanes() const throw(OpenGLException);

  /**
    Returns the number of underlay planes.
  */
  unsigned int getNumberOfUnderlayPlanes() const throw(OpenGLException);
  
  /**
    Returns the current title of the window.
  */
  String getTitle() const throw(OpenGLException);
  
  /**
    Sets the title of the window.
  */
  void setTitle(const String& title) throw(OpenGLException);

  // String getIconTitle() const throw(UserInterfaceException);
  // void setIconTitle(const String& title) throw(UserInterfaceException);
  
  /**
    Returns the current position of the window.
  */
  inline Position getPosition() const throw() {
    return position;
  }

  /**
    Returns the current dimension of the window.
  */
  inline Dimension getDimension() const throw() {
    return dimension;
  }

  // Position getPosition() const throw(OpenGLException);
  void setPosition(const Position& position) throw(OpenGLException);
  
  // Dimension getSize() const throw(OpenGLException);
  void setSize(const Dimension& dimension) throw(OpenGLException);

  inline void setMinimumSize(const Dimension& dimension) throw() {
    minimumSize = dimension;
  }
  
  inline void setMaximumSize(const Dimension& dimension) throw() {
    maximumSize = dimension;
  }

  /**
    Returns the current mouse cursor for the window.
  */
  inline Cursor getCursor() const throw() {
    return cursor;
  }

  /**
    Sets the mouse cursor of the window.
  */
  void setCursor(Cursor cursor) throw(OpenGLException);

  /**
    Hides the mouse cursor.
  */
  void hideCursor() throw(OpenGLException);

  /**
    Shows the mouse cursor.
  */
  void showCursor() throw(OpenGLException);

  /**
    Returns the mouse position.
  */
  Position getCursorPosition() const throw(OpenGLException);

  /**
    Sets the mouse position.
  */
  void setCursorPosition(const Position& position) throw(OpenGLException);

  /**
    Releases the mouse confinement.
  */
  void releaseCursorConfinement() throw(OpenGLException);

  /**
    Sets the cursor confinement to the window context.
  */
  void setCursorConfinement() throw(OpenGLException);

  /**
    Sets the cursor confinement to the specified area.

    @param position The offset of the confinement area.
    @param dimension The dimension of the confinement area.
  */
  void setCursorConfinement(const Position& position, const Dimension& dimension) throw(OpenGLException);

//     bool isDoubleBuffered() const throw();
//     bool isRGBA() const throw();
//     void getBufferInformation(BufferInformation& info) const throw();
//     Window* getParent() const throw();
//     unsigned int getNumberOfChildren() const throw();

//     void show() throw();
//     void hide() throw();
//     void iconify() throw();

  /**
    Closes the window.
  */
  void close() throw(OpenGLException);
  
  // ViewState getView() throw(OpenGLException);
  bool isMaximized() throw(OpenGLException); // TAG: replace with inline
  bool isMinimized() throw(OpenGLException); // TAG: replace with inline
  
  void maximize() throw(OpenGLException);
  void minimize() throw(OpenGLException);
  void show() throw(OpenGLException);

  /**
    Hides the window.
  */
  void hide() throw(OpenGLException);
  
  /**
    Acquire focus for this window.
  */
  void acquireFocus() throw(OpenGLException);

  /**
    Returns the mouse capture state of the window.
  */
  bool getCapture() const throw(OpenGLException);

  /**
    Sets the mouse capture state of the window.
  */
  void setCapture(bool state) throw(OpenGLException);

  /**
    Returns the display mode.
  */
  inline DisplayMode getDisplayMode() const throw() {
    return displayMode;
  }

  /**
    Selects the display mode.
  */
  void setDisplayMode(DisplayMode displayMode) throw(OpenGLException);

  /**
    Returns the state of the specified key.
  */
  unsigned int getKeyState(unsigned int code) const throw();
  
  /**
    Returns the keyboard modifiers.
  */
  inline unsigned int getModifiers() const throw() {
    return modifiers;
  }
  
  /**
    Invoking when the display should be updated.
  */
  virtual void onDisplay() throw();
  
  /**
    Invoked when the window has been moved.

    @param position The new position of the window.
  */
  virtual void onMove(const Position& position) throw();

  /**
    Invoked when the window has been resized.

    @param dimension The new dimension of the window.
  */
  virtual void onResize(const Dimension& dimension) throw();

  /**
    Invoked when the mouse is moved.
    
    @param position The position of the mouse pointer.
    @param buttons The current state of the mouse buttons.
  */
  virtual void onMouseMove(const Position& position, unsigned int buttons) throw();

  /**
    Invoked then the mouse pointer enters or exits the scope of the window.

    @param scope Specified whether or not the pointer is in the scope of the window.
  */
  virtual void onMouseScope(bool scope) throw();

  /**
    Invoked when the mouse button state changes.

    @param position The position of the mouse.
    @param button The button that changed state.
    @param event The mouse button event.
  */
  virtual void onMouseButton(const Position& position, Mouse::Button button, Mouse::Subevent event) throw();

  /**
    Invoked when the mouse wheel state changes.

    @param position The position of the mouse.
    @param delta The amount of
    @param buttons The current state of the mouse buttons.
  */
  virtual void onMouseWheel(const Position& position, int delta, unsigned int buttons) throw();

  /**
    Invoked when a key is pressed or released.

    @param key The key.
    @param flags Flags describing the key event.
    @param modifiers The current state of the key modifiers (e.g. SHIFT).
  */
  virtual void onKey(unsigned int key, unsigned int flags, unsigned int modifiers) throw();

  /**
    Invoked continuously when no messages have been queued. The default
    implementation waits for an incomming message.
  */
  virtual void onIdle() throw();

  /**
    Invoked when the window is asked to be closed. This method must return true
    if the window may be closed. The default implementation returns true and
    also asks the application to exit.
  */
  virtual bool onClose() throw();

  /**
    Invoked when the visibility state changes.
  */
  virtual void onVisibility(bool visible) throw();

  /**
    Invoked when the focus state changes.
  */
  virtual void onFocus(bool focus) throw();

  /**
    Invoked when a command is send from a menu to the window.
  */
  virtual void onMenu(unsigned int identifier) throw();
  
  /**
    Invoked when a command is send to the window.
  */
  virtual void onCommand(unsigned int identifier) throw();
  
  // TAG: maximize, minimize, normal, full screen, disabled/enabled, hidden/visible

  /**
    Selects this OpenGL context as the current context of the executing thread.
  */
  void makeCurrent() throw(OpenGLException);

  /**
    Deselects the OpenGL context of the executing thread.
  */
  void deselect() throw();

  /**
    Swaps all the buffers.
  */
  void swap() throw(OpenGLException);

  /**
    Requests the window to be updated.
  */
  void update() throw(OpenGLException);
  
  /**
    Exit message dispatcher without asking the application to exit.
  */
  static void exit() throw();
  
  /**
    Singleton method responsible for dispatching incomming messages to the
    corresponding handlers.
  */
  void dispatch() throw(OpenGLException /*, SingletonException*/);  
  
  /**
    Releases the OpenGL context.
  */
  ~OpenGLContext() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
