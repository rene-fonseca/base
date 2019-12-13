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
#include <base/ui/Menu.h>
#include <base/ui/Position.h>
#include <base/ui/Drawable.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// TAG: need reference counting
// TAG: full screen support

/**
  Generic window implementation. This class cannot be instantiated.
  
  @short Generic window implementation.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API WindowImpl : public Object, virtual public Drawable {
  friend class Backend<WindowImpl>;
public:

  /** Display modes. */
  enum DisplayMode {
    MODE_WINDOW, /**< Window mode. */
    MODE_FULL_SCREEN /**< Full screen mode. */
  };

  /** The cursors. */
  enum Cursor {
    INHERIT, /**< Inherit cursor. */
    NONE, /**< No cursor. */
    WORKING_IN_BACKGROUND, /**< Cursor indicating working is being the background. */
    ARROW, /**< The normal selection cursor. */
    CROSSHAIR, /**< Cross hair. */
    HAND, /**< Hand. */
    HELP, /**< Help. */
    NO, /**< No. */
    MOVING, /**< Moving. */
    NORTHEAST_SOUTHWEST, /**< Northeast and southwest pointing arrow. */
    NORTH_SOUTH, /**< A birectional arrow. */
    NORTHWEST_SOUTHEAST, /**< Northwest and southeast pointing arrow. */
    WEST_EAST, /**< A birectional arrow pointer in the west and east direction. */
    VERTICAL_ARROW, /**< A vertical arrow. */
    WAITING /**< Cursor indicating blocked wait. */
  };

  /** Visibility state. */
  enum Visibility {
    INVISIBLE, /**< The window is invisible. */
    OBSCURED, /**< The window is partially visible. */
    VISIBLE /**< The window is fully visible. */
  };
private:

  enum Message {
    DESTROY_MESSAGE = 0, /**< Request destruction of window. */
    PING_MESSAGE = 1, /**< Request response from window. */
    QUIT_MESSAGE /**< Exit dispatch loop. */
  };
  
  /** Resource allocation and release lock. */
  static SpinLock spinLock;
  
  /** The display mode. */
  DisplayMode displayMode = MODE_WINDOW;
  Position lastMousePosition;
  
  /** The current keyboard state. */
  uint8 keyboardState[256];
  /** The modifiers. */
  unsigned int modifiers = 0;
  /** The autopreat flag. */
  bool autorepeat = false;
  /** Flag specifying whether the mouse is in the scope of the client area of the window. */
  bool scope = false;
  /** Flag specifying whether the window is active. */
  bool active = false;
  /** Specifies whether to window is visible or not. */
  Visibility visibility = VISIBLE;
  /** Specifies whether to window is enabled or not. */
  bool enabled = false;
  /** The current cursor. */
  Cursor cursor = ARROW;

  /** The current number of locks on the user interface component. */
  static unsigned int numberOfLocks;
  
  /**
    Loads or unloads the user interface component.
  */
  static bool loadModule(bool load) noexcept;
protected:
  
  /** The normal title of the window. */
  String title;
  /** The icon title of the window. */
  String iconTitle;
  /** The position of the window. */
  Position position;
  /** The dimension of the window. */
  Dimension dimension;
  /** Flags. */
  unsigned int flags = 0;
  /** The minimum dimension of the window. */
  Dimension minimumSize;
  /** The maximum dimension of the window. */
  Dimension maximumSize;
  /** Opaque handle to the display. */
  static void* displayHandle;
  /** Opaque handle to the screen. */
  void* screenHandle = nullptr;
  /** Opaque handle to the graphics context of the window. */
  void* graphicsContextHandle = nullptr;
  
  /**
    Invoked after initial context specific initialization to finalize the
    generic initialization.
  */
  void construct() noexcept;

  /**
    Invoked when the window is destroyed.
  */
  virtual void destroy() noexcept;
public:

  /** Window flags. */
  enum Flag {
    AUTO_CLEAR = 1 << 0, /**< Specifies that the window should be cleared automatically. */
    FIXED_SIZE = 1 << 1 /**< Specifies that the window cannot be resized. */
  };

  /** Focus state. */
  enum Focus {
    LOST_FOCUS,
    ACQUIRED_FOCUS
  };
  
  class _COM_AZURE_DEV__BASE__API Mouse {
  public:

    /** Mouse button. */
    enum Button {
      LEFT = 1, /**< Left mouse button. */
      MIDDLE = LEFT << 1, /**< Middle mouse button. */
      RIGHT = MIDDLE << 1, /**< Right mouse button. */
      WHEEL = RIGHT << 1, /**< Mouse wheel. */
      EXTRA = WHEEL << 1, /**< Extra mouse button. */
      EXTRA2 = EXTRA << 1 /**< Secondary extra button. */
    };

    /** Mouse button event. */
    enum Event {
      PRESSED, /**< Mouse button has been pressed. */
      RELEASED, /**< Mouse button has been released. */
      DOUBLE_CLICKED, /**< Mouse button has been double-clicked. */
      TURNED /**< Mouse wheel has been turned. */
    };
  };

  class _COM_AZURE_DEV__BASE__API Key {
  public:

    /** Key code. */
    enum Code {
      BACK = 0x08, /**< Backspace. */
      TAB = 0x09, /**< Tab. */
      RETURN = 0x0d, /**< Return. */
      ESCAPE = 0x1b, /**< Escape. */
      SPACE = 0x20, /**< Space. */
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
      SELECT, /**< Select. */
      PRINT, /**< Print. */
      EXECUTE, /**< Execute. */
      SNAPSHOT, /**< Snapshot. */
      HELP, /**< Help. */
      CAPS_LOCK, /**< Caps lock. */
      SCROLL_LOCK, /**< Scroll lock. */
      NUM_LOCK, /**< Num lock. */
      PAUSE, /**< Pause. */
      LEFT_MANAGER, /**< Left manager. */
      RIGHT_MANAGER, /**< Right manager. */
      MENU, /**< Menu. */
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
      F24, /**< Function key 24. */
      // TAG: new from here
      SHIFT_LEFT, // TAG: fix name
      SHIFT_RIGHT, // TAG: fix name
      CONTROL_LEFT, // TAG: fix name
      CONTROL_RIGHT, // TAG: fix name
      ALT_LEFT, // TAG: fix name
      ALT_RIGHT, // TAG: fix name
      META_LEFT,
      META_RIGHT,
      SUPER_LEFT,
      SUPER_RIGHT,
      HYPER_LEFT,
      HYPER_RIGHT
    };

    /** Keyboard modifier. */
    enum Modifier {
      LEFT_CONTROL = Mouse::EXTRA2 << 1, /**< Left control key. */
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

  /* Named binding points. */
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
  
  /**
    Returns the zero-based index of the specified mouse button.
  */
  static unsigned int getMouseButtonIndex(Mouse::Button button) noexcept;

  /**
    Returns the name of the specified mouse button.
  */
  static Literal getMouseButtonName(Mouse::Button button) noexcept;
  
  /**
    Initializes a new window with position (0, 0) and dimension (0, 0).
  */
  WindowImpl() throw(UserInterfaceException);
  
  /**
    Initializes a new window.
  */
  WindowImpl(
    const Position& position,
    const Dimension& dimension,
    unsigned int flags) throw(UserInterfaceException);

  /**
    Returns the position of the binding point relative to the owner.
  */
  Position getBindingOffset(Binding binding) const throw();
  
  /**
    Flushes the window requests to the server.
  */
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  inline void flush() throw(UserInterfaceException) {
  }
#else // unix
  void flush() throw(UserInterfaceException);
#endif // flavor

  /**
    Asynchronous update request.
  */
  void invalidate() throw(UserInterfaceException);
  
  /**
    Returns the vendor of the server.
  */
  String getServerVendor() const throw(UserInterfaceException);

  /**
    Returns the release of the server.
  */
  unsigned int getServerRelease() const throw(UserInterfaceException);
  
  /**
    Returns the window flags.
  */
  inline unsigned int getFlags() const throw(UserInterfaceException) {
    return flags;
  }
  
  /**
    Displays the specified menu at the specified position.

    @param position The position.
    @param menu The menu.
  */
  void displayMenu(const Position& position, const Menu& menu) throw(UserInterfaceException);

  /**
    Returns the autorepeat.
  */
  inline bool getAutorepeat() const throw(UserInterfaceException) {
    return autorepeat;
  }

  /**
    Sets the autorepeat flag.
  */
  inline void setAutorepeat(bool autorepeat) throw(UserInterfaceException) {
    this->autorepeat = autorepeat;
  }  
  
  /**
    Returns the current title of the window.
  */
  String getTitle() const throw(UserInterfaceException);
  
  /**
    Sets the title of the window.
  */
  void setTitle(const String& title) throw(UserInterfaceException);

  /**
    Returns the icon title.
  */
  String getIconTitle() const throw(UserInterfaceException);

  /**
    Sets the icon title.
  */
  void setIconTitle(const String& title) throw(UserInterfaceException);
  
  /**
    Returns the current position of the window.
  */
  inline Position getPosition() const throw(UserInterfaceException) {
    return position;
  }
  
  void setPosition(const Position& position) throw(UserInterfaceException);
  
  /**
    Returns the size of the window.
  */
  inline Dimension getDimension() const throw(UserInterfaceException) {
    return dimension;
  }
  
  /**
    Sets the size of the window.
  */
  void setDimension(const Dimension& dimension) throw(UserInterfaceException);

  /**
    Sets the position and dimension of the window.
  */
  void setRegion(const Position& position, const Dimension& dimension) throw(UserInterfaceException);
  
  /**
    Sets the minimum size of the window.
  */
  inline void setMinimumSize(const Dimension& dimension) throw(UserInterfaceException) {
    minimumSize = dimension;
  }

  /**
    Sets the maximum size of the window.
  */
  inline void setMaximumSize(const Dimension& dimension) throw(UserInterfaceException) {
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
  void setCursor(Cursor cursor) throw(UserInterfaceException);
  
  /**
    Returns the position relative to the root window.
    
    @param position The position relative to this window.
  */
  Position toGlobalPosition(const Position& position) const throw(UserInterfaceException);
  
  /**
    Returns the mouse position.
  */
  Position getCursorPosition() const throw(UserInterfaceException);

  /**
    Sets the mouse position.
  */
  void setCursorPosition(const Position& position) throw(UserInterfaceException);

  /**
    Releases the mouse confinement.
  */
  void releaseCursorConfinement() throw(UserInterfaceException);

  /**
    Sets the cursor confinement to the window context.
  */
  void setCursorConfinement() throw(UserInterfaceException);

  /**
    Sets the cursor confinement to the specified area.

    @param position The offset of the confinement area.
    @param dimension The dimension of the confinement area.
  */
  void setCursorConfinement(const Position& position, const Dimension& dimension) throw(UserInterfaceException);

  /**
    Disable clipping.
  */
  void disableClipping() throw(UserInterfaceException);

  struct Region {
    // TAG: fixme
  };
  
  /**
    Returns the clipping region.
  */
  Region getClipping() const throw(UserInterfaceException);
  
  /**
    Sets the clipping region.
  */
  void setClipping(const Position& position, const Dimension& dimension) throw(UserInterfaceException);
  
  /**
    Closes the window.
  */
  void close() throw(UserInterfaceException);
  
  // ViewState getView() throw(UserInterfaceException);

  /**
    Returns true if this object is a child of the specified object.
  */
  bool isChildOf(const WindowImpl& object) throw(UserInterfaceException);
  
  /**
    Returns true if this object is the parent of the specified object.
  */
  bool isParentOf(const WindowImpl& object) throw(UserInterfaceException);
  
  /**
    Returns true if the window is maximized.
  */
  bool isMaximized() throw(UserInterfaceException); // TAG: replace with inline

  /**
    Returns true if the window is minimized.
  */
  bool isMinimized() throw(UserInterfaceException); // TAG: replace with inline

  /**
    Maximizes the window.
  */
  void maximize() throw(UserInterfaceException);

  /**
    Minimizes the window.
  */
  void minimize() throw(UserInterfaceException);

  /**
    Restores the original position and dimension of the window if currently
    minimized or maximized.
  */
  void normalize() throw(UserInterfaceException);

  /**
    Returns true if the window is visible.
  */
  inline bool isVisible() const throw() {
    return visibility != INVISIBLE;
  }

  inline Visibility getVisibility() const throw() {
    return visibility;
  }
  
  /**
    Shows the window if currently hidden.
  */
  void show() throw(UserInterfaceException);

  /**
    Hides the window.
  */
  void hide() throw(UserInterfaceException);

  /**
    Returns true if the window is enabled.
  */
  inline bool isEnabled() const throw() {
    return enabled;
  }
  
  /**
    Enables the window.
  */
  void enable() throw(UserInterfaceException);

  /**
    Disable the window.
  */
  void disable() throw(UserInterfaceException);

  /**
    Raises the window.
  */
  void raise() throw(UserInterfaceException);
  
  /**
    Acquire focus for this window.
  */
  void acquireFocus() throw(UserInterfaceException);

  /**
    Returns the mouse capture state of the window.
  */
  bool getCapture() const throw(UserInterfaceException);

  /**
    Sets the mouse capture state of the window.
  */
  void setCapture(bool state) throw(UserInterfaceException);

  /**
    Returns the display mode.
  */
  inline DisplayMode getDisplayMode() const throw(UserInterfaceException) {
    return displayMode;
  }

  /**
    Selects the display mode.
  */
  void setDisplayMode(DisplayMode displayMode) throw(UserInterfaceException);

  /**
    Returns the state of the specified key.
  */
  unsigned int getKeyState(unsigned int code) const throw(UserInterfaceException);
  
  /**
    Returns the keyboard modifiers.
  */
  inline unsigned int getModifiers() const throw(UserInterfaceException) {
    return modifiers;
  }

  /**
    Invoked when the window is destroyed.
  */
  virtual void onDestruction() noexcept;
  
  /**
    Invoked when the display should be updated.
  */
  virtual void onDisplay() noexcept;
  
  /**
    Invoked when the window has been moved.

    @param position The new position of the window.
  */
  virtual void onMove(const Position& position) noexcept;

  /**
    Invoked when the window has been resized.

    @param dimension The new dimension of the window.
  */
  virtual void onResize(const Dimension& dimension) noexcept;

  /**
    Invoked when the mouse is moved.
    
    @param position The position of the mouse pointer.
    @param buttons The current state of the mouse buttons.
  */
  virtual void onMouseMove(const Position& position, unsigned int buttons) noexcept;

  /**
    Invoked then the mouse pointer enters or exits the scope of the window.

    @param scope Specified whether or not the pointer is in the scope of the window.
  */
  virtual void onMouseScope(bool scope) noexcept;

  /**
    Invoked when the mouse button state changes.

    @param position The position of the mouse.
    @param button The button that changed state.
    @param event The mouse button event.
  */
  virtual void onMouseButton(const Position& position, Mouse::Button button, Mouse::Event event, unsigned int state) noexcept;

  /**
    Invoked when the mouse wheel state changes.

    @param position The position of the mouse.
    @param delta The amount of
    @param buttons The current state of the mouse buttons.
  */
  virtual void onMouseWheel(const Position& position, int delta, unsigned int buttons) noexcept;

  /**
    Invoked when a key is pressed or released.

    @param key The key.
    @param flags Flags describing the key event.
    @param modifiers The current state of the key modifiers (e.g. SHIFT).
  */
  virtual void onKey(unsigned int key, unsigned int flags, unsigned int modifiers) noexcept;

  /**
    Invoked continuously when no messages have been queued. The default
    implementation waits for an incoming message.
  */
  virtual void onIdle() noexcept;

  /**
    Invoked when the window is asked to be closed. This method must return true
    if the window may be closed. The default implementation returns true and
    also asks the application to exit.
  */
  virtual bool onClose() noexcept;

  /**
    Invoked when the visibility state changes.
  */
  virtual void onVisibility(Visibility visibility) noexcept;

  /**
    Invoked when the focus state changes.
  */
  virtual void onFocus(Focus focus) noexcept;

  /**
    Invoked when a command is send from a menu to the window.
  */
  virtual void onMenu(unsigned int identifier) noexcept;
  
  /**
    Invoked when a command is send to the window.
  */
  virtual void onCommand(unsigned int identifier) noexcept;
  
  /**
    Requests the window to be updated.
  */
  void update() throw(UserInterfaceException);
  
  /**
    Exit message dispatcher without asking the application to exit.
  */
  static void exit() noexcept;
  
  /**
    Waits for a message.
  */
  void wait() throw(UserInterfaceException);

  /**
    Waits for a message.

    @param milliseconds The timeout period.
  */
  bool wait(unsigned int milliseconds) throw(UserInterfaceException);
  
  /**
    This method is responsible for dispatching incoming messages to the
    corresponding message handlers.

    @return False if dispatch got an exit message.
  */
  bool openDispatch() throw(UserInterfaceException);

  /**
    This method is responsible for dispatching incoming messages to the
    corresponding message handlers.
  */
  static void dispatch() throw(UserInterfaceException);

  /**
    Returns true if a mouse is present.
  */
  bool hasMouse() noexcept;

  /**
    Returns the available mouse buttons.
  */
  unsigned int getMouseButtons() noexcept;
  
  /**
    Returns the number of monitors.
  */
  unsigned int getNumberOfMonitors() noexcept;

  /**
    Returns the dimension of the display.
  */
  Dimension getDisplayDimension() noexcept;

  /**
    Returns true if the window is reponsing within the specified timeout period.

    @param milliseconds The timeout period (silently reduced to 999999999).
  */
  bool isResponding(unsigned int milliseconds) throw(UserInterfaceException);
  
  /**
    Releases the window.
  */
  virtual ~WindowImpl() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
