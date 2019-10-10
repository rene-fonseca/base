/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/ui/Window.h>

using namespace com::azure::dev::base;

class WindowApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  WindowApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("simple", numberOfArguments, arguments, environment) {
  }
  
  class MyWindow : public Window {
  public:

    MyWindow(
      const String& title,
      const Position& position,
      const Dimension& dimension,
      unsigned int flags) throw(UserInterfaceException)
      : Window(position, dimension, flags) {
      setTitle(title);
      setIconTitle(title);
    }
    
    void onDisplay() throw() {
      // setBrush();
      clear();
      line(Position(10, 60), Position(64, 20));
      flush();
    }
    
    void onMove(const Position& position) throw() {
      fout << "Event: move " << position << ENDL;
    }
    
    void onResize(const Dimension& dimension) throw() {
      fout << "Event: resize " << dimension << ENDL;
      onDisplay();
    }
    
    struct Flag {
      unsigned int mask;
      Literal literal;
    };
    
    void onMouseMove(const Position& position, unsigned int state) throw() {
      static const Flag STATES[] = {
        {Mouse::LEFT, Literal("LEFT")},
        {Mouse::MIDDLE, Literal("MIDDLE")},
        {Mouse::RIGHT, Literal("RIGHT")},
        {Mouse::WHEEL, Literal("WHEEL")},
        {Mouse::EXTRA, Literal("EXTRA")},
        {Mouse::EXTRA2, Literal("EXTRA2")},
        {Key::LEFT_CONTROL, Literal("L-CTRL")},
        {Key::RIGHT_CONTROL, Literal("R-CTRL")},
        {Key::LEFT_SHIFT, Literal("L-SHFT")},
        {Key::RIGHT_SHIFT, Literal("R-SHFT")},
        {Key::LEFT_ALT, Literal("L-ALT")},
        {Key::RIGHT_ALT, Literal("R-ALT")},
        {Key::NUM_LOCK_TOGGLED, Literal("NUM")},
        {Key::CAPS_LOCK_TOGGLED, Literal("CAPS")},
        {Key::SCROLL_LOCK_TOGGLED, Literal("SCROLL")},
        {Key::INSERT_TOGGLED, Literal("INSERT")}
      };
      
      fout << "Event: mouse move " << position << ' ';
      for (unsigned int i = 0; i < getArraySize(STATES); ++i) {
        if (state & STATES[i].mask) {
          fout << STATES[i].literal << ' ';
        }
      }
      fout << ENDL;
    }

    void onMouseScope(bool scope) throw() {
      fout << "Event: mouse scope "
           << (scope ? Literal("INSIDE SCOPE") : Literal("OUT OF SCOPE")) << ENDL;
    }
    
    void onMouseButton(
      const Position& position,
      Mouse::Button button,
      Mouse::Event event,
      unsigned int state) throw() {
      
      static const Flag STATES[] = {
        {Mouse::LEFT, MESSAGE("LEFT")},
        {Mouse::MIDDLE, MESSAGE("MIDDLE")},
        {Mouse::RIGHT, MESSAGE("RIGHT")},
        {Mouse::WHEEL, MESSAGE("WHEEL")},
        {Mouse::EXTRA, MESSAGE("EXTRA")},
        {Mouse::EXTRA2, MESSAGE("EXTRA2")},
        {Key::LEFT_CONTROL, MESSAGE("L-CTRL")},
        {Key::RIGHT_CONTROL, MESSAGE("R-CTRL")},
        {Key::LEFT_SHIFT, MESSAGE("L-SHFT")},
        {Key::RIGHT_SHIFT, MESSAGE("R-SHFT")},
        {Key::LEFT_ALT, MESSAGE("L-ALT")},
        {Key::RIGHT_ALT, MESSAGE("R-ALT")},
        {Key::NUM_LOCK_TOGGLED, MESSAGE("NUM")},
        {Key::CAPS_LOCK_TOGGLED, MESSAGE("CAPS")},
        {Key::SCROLL_LOCK_TOGGLED, MESSAGE("SCROLL")},
        {Key::INSERT_TOGGLED, MESSAGE("INSERT")}
      };
      
      static const Literal EVENT_STRING[] = {
        MESSAGE("PRESSED"),
        MESSAGE("RELEASED"),
        MESSAGE("DOUBLE CLICKED"),
        MESSAGE("TURNED")
      };
      
      fout << "Event: mouse button " << getMouseButtonName(button) << SP;
      
      if (static_cast<unsigned int>(event) < getArraySize(EVENT_STRING)) {
        fout << EVENT_STRING[event];
      } else {
        fout << "[UNNAMED EVENT]" << ' ' << static_cast<unsigned int>(event);
      }
      fout << ' ';
      
      for (unsigned int i = 0; i < getArraySize(STATES); ++i) {
        if (state & STATES[i].mask) {
          fout << STATES[i].literal << ' ';
        }
      }
      
      fout << position << ENDL;
      
      if (event == Mouse::PRESSED) {
        if (button == Mouse::LEFT) {
          setCapture(true);
        }
        if (button == Mouse::RIGHT) {
          unsigned int cursor = static_cast<unsigned int>(getCursor());
          if (cursor == WAITING) {
            cursor = 0;
          } else {
            cursor += 1;
          }
          setCursor(static_cast<Cursor>(cursor));
        }
      }
      if (event == Mouse::RELEASED) {
        if (button == Mouse::LEFT) {
          setCapture(false);
        }
      }
    }
    
    void onMouseWheel(
      const Position& position, int delta, unsigned int buttons) throw() {
      fout << "Event: mouse wheel " << position << SP << delta << ENDL;
    }
    
    void onKey(
      unsigned int key, unsigned int flags, unsigned int modifiers) throw() {
      if (flags & Key::PRESSED) {
        if (flags & Key::DEAD) {
          return;
        }

        if ((flags & Key::ASCII) && ASCIITraits::isGraph(key)) {
          fout << "Key: " << PREFIX << HEX << key << SP
               << '\'' << static_cast<char>(key) << '\''
               << SP << "PRESSED";
        } else {
          fout << "Key: " << PREFIX << HEX << key << SP << "PRESSED";
        }
        if (flags & Key::FIRST_TIME) {
          fout << SP << "FIRST";
        }
        fout << ENDL;
      } else {
        fout << "Key: " << PREFIX << HEX << key << SP << "RELEASED" << ENDL;
      }
    }

    void onDestruction() throw() {
      fout << "Event: destruction" << ENDL;
      exit();
    }
    
    bool onClose() throw() {
      fout << "Event: close " << ENDL;
      return true;
    }
    
    void onVisibility(Visibility visibility) throw() {
      static const Literal VISIBILITY[] = {
        Literal("INVISIBLE"),
        Literal("PARTIALLY VISIBLE"),
        Literal("VISIBLE")
      };
      
      fout << "Event: visibility " << VISIBILITY[visibility] << ENDL;
    }
    
    void onFocus(Focus focus) throw() {
      update();
      fout << "Event: focus "
           << ((focus == ACQUIRED_FOCUS) ? Literal("ACQUIRED") : Literal("LOST")) << ENDL;
    }
    
    void onMenu(unsigned int identifier) throw() {
      fout << "Event: menu " << identifier << ENDL;
    }
    
    void onCommand(unsigned int identifier) throw() {
      fout << "Event: command " << identifier << ENDL;
    }
  };
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2003-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;

    MyWindow myWindow(
      "Hello, World",
      Position(123, 312),
      Dimension(256, 128),
      0
    );

    fout << "Server vendor: " << myWindow.getServerVendor() << ENDL;
    fout << "Server release: " << myWindow.getServerRelease() << ENDL;
    
    myWindow.show();
    myWindow.raise();
    myWindow.acquireFocus();
    Window::dispatch();
  }
};

STUB(WindowApplication);
