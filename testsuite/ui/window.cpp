/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/ui/Window.h>
#include <base/ui/Label.h>
#include <base/ui/Split.h>
#include <base/ui/Button.h>
#include <base/ui/ProgressBar.h>
#include <base/ui/Picture.h>
#include <base/opengl/OpenGLWidget.h>

using namespace base;

class WindowApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  WindowApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("Window"), numberOfArguments, arguments, environment) {
  }

  class MyOpenGLWidget : public OpenGLWidget {
  public:
    
    MyOpenGLWidget(Window& owner, const Format& format) throw(OpenGLException, UserInterfaceException)
      : OpenGLWidget(owner, format) {
    }
    
    void onDisplay() throw() {
      static uint8 gray = 0;
      openGL.glClearColor(gray++/255.0, 0.0, 0.0, 1.0);
      openGL.glClear(OpenGL::COLOR_BUFFER_BIT);
      openGL.glFlush();
      swap();
    }
    
    ~MyOpenGLWidget() throw() {
    }
  };
  
  class MyWindow : public Window {
  private:
    
    Label label;
    Split split;
    Button button;
    ProgressBar progressBar;
    Picture picture;
    MyOpenGLWidget openGLWidget;

    inline OpenGLWidget::Format makeFormat() throw() {
      OpenGLWidget::Format format;
      format.id = 1;
      format.flags = OpenGLWidget::RGB|OpenGLWidget::DEPTH;
      format.colorBits = 32;
      format.redBits = 8;
      format.greenBits = 8;
      format.blueBits = 8;
      format.alphaBits = 8;
      format.accumulatorBits = 64;
      format.accumulatorRedBits = 16;
      format.accumulatorGreenBits = 16;
      format.accumulatorBlueBits = 16;
      format.accumulatorAlphaBits = 16;
      format.depthBits = 24;
      format.stencilBits = 0;
      format.auxBuffers = 0;
      return format;
    }
  public:

    MyWindow(
      const String& title,
      const Position& position,
      const Dimension& dimension,
      unsigned int flags) throw(UserInterfaceException)
      : Window(position, dimension, flags),
        label(*this),
        split(*this),
        button(*this),
        progressBar(*this),
        picture(*this),
        openGLWidget(*this, makeFormat()) {
      
      setTitle(title);
      setIconTitle(title);
      
      label.setPosition(Position(16, 16));
      label.setDimension(Dimension(256, 32));
      label.setText(MESSAGE("This is a label"));
      label.setTextFormat(TextFormat::RIGHT | TextFormat::MIDDLE);
      label.setBrush(Color(192, 192, 192));
      label.setFont(Font(MESSAGE("Arial"), 16));
      
      split.setPosition(Position(8, 8));
      split.setDimension(Dimension(128, 128));
      split.setOffset(64, Split::FIRST);
      
      button.setText(MESSAGE("Accept"));
      button.setPosition(Position(512, 64));
      button.setDimension(button.getPreferredSize());

      progressBar.setPosition(Position(16, 256));
      progressBar.setDimension(Dimension(256, 16));
      progressBar.setMaximumValue(256);
      
      picture.setPosition(Position(128, 128));
      picture.setDimension(Dimension(128, 128));

      Color data[128][128];
      for (unsigned int y = 0; y < 128; ++y) {
        for (unsigned int x = 0; x < 128; ++x) {
          data[y][x] = Color(y, x, y + x);
        }
      }
      
      Bitmap bitmap(Dimension(128, 128), Bitmap::RGB, Bitmap::RGB_32, data);
      picture.setBitmap(bitmap);
      fout << MESSAGE("Bitmap: ") << picture.getBitmap().getDimension() << ENDL;
      
      openGLWidget.setPosition(Position(256, 32));
      openGLWidget.setDimension(Dimension(128, 128));
    }
    
    void onDisplay() throw() {
      // setBrush();
      clear();
      line(Position(10, 60), Position(64, 20));
      flush();
    }
    
    void onMove(const Position& position) throw() {
      fout << MESSAGE("Event: move ") << position << ENDL;
      progressBar.setCurrentValue(position.getX());
    }
    
    void onResize(const Dimension& dimension) throw() {
      fout << MESSAGE("Event: resize ") << dimension << ENDL;
      onDisplay();
    }
    
    struct Flag {
      unsigned int mask;
      StringLiteral literal;
    };
    
    void onMouseMove(const Position& position, unsigned int state) throw() {
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
      
      fout << MESSAGE("Event: mouse move ") << position << ' ';
      for (unsigned int i = 0; i < getArraySize(STATES); ++i) {
        if (state & STATES[i].mask) {
          fout << STATES[i].literal << ' ';
        }
      }
      fout << ENDL;
    }

    void onMouseScope(bool scope) throw() {
      fout << MESSAGE("Event: mouse scope ")
           << (scope ? MESSAGE("INSIDE SCOPE") : MESSAGE("OUT OF SCOPE")) << ENDL;
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
      
      static const StringLiteral EVENT_STRING[] = {
        MESSAGE("PRESSED"),
        MESSAGE("RELEASED"),
        MESSAGE("DOUBLE CLICKED"),
        MESSAGE("TURNED")
      };
      
      fout << MESSAGE("Event: mouse button ")
           << getMouseButtonName(button) << ' ';
      
      if (event < getArraySize(EVENT_STRING)) {
        fout << EVENT_STRING[event];
      } else {
        fout << MESSAGE("[UNNAMED EVENT]") << ' ' << static_cast<unsigned int>(event);
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
    
    void onMouseWheel(const Position& position, int delta, unsigned int buttons) throw() {
      fout << MESSAGE("Event: mouse wheel ") << position << ' ' << delta << ENDL;
    }
    
    void onKey(unsigned int key, unsigned int flags, unsigned int modifiers) throw() {
      if (flags & Key::PRESSED) {
        if (flags & Key::DEAD) {
          return;
        }

        if ((flags & Key::ASCII) && ASCIITraits::isGraph(key)) {
          fout << MESSAGE("Key: ") << PREFIX << HEX << key << ' '
               << '\'' << static_cast<char>(key) << '\''
               << ' ' << MESSAGE("PRESSED");
        } else {
          fout << MESSAGE("Key: ") << PREFIX << HEX << key << ' ' << MESSAGE("PRESSED");
        }
        if (flags & Key::FIRST_TIME) {
          fout << ' ' << MESSAGE("FIRST");
        }
        fout << ENDL;
      } else {
        fout << MESSAGE("Key: ") << PREFIX << HEX << key << ' ' << MESSAGE("RELEASED") << ENDL;
      }
    }

    void onDestruction() throw() {
      fout << MESSAGE("Event: destruction") << ENDL;
      exit();
    }
    
    bool onClose() throw() {
      fout << MESSAGE("Event: close ") << ENDL;
      return true;
    }
    
    void onVisibility(Visibility visibility) throw() {
      static const StringLiteral VISIBILITY[] = {
        MESSAGE("INVISIBLE"),
        MESSAGE("PARTIALLY VISIBLE"),
        MESSAGE("VISIBLE")
      };
      
      fout << MESSAGE("Event: visibility ") << VISIBILITY[visibility] << ENDL;
    }
    
    void onFocus(Focus focus) throw() {
      update();
      fout << MESSAGE("Event: focus ")
           << ((focus == ACQUIRED_FOCUS) ? MESSAGE("ACQUIRED") : MESSAGE("LOST")) << ENDL;
    }
    
    void onMenu(unsigned int identifier) throw() {
      fout << MESSAGE("Event: menu ") << identifier << ENDL;
    }
    
    void onCommand(unsigned int identifier) throw() {
      fout << MESSAGE("Event: command ") << identifier << ENDL;
    }
  };
  
  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;

    //Display display();
    
    MyWindow myWindow(MESSAGE("Hello, World"), Position(123, 312), Dimension(256, 128), 0);

    fout << MESSAGE("Server vendor: ") << myWindow.getServerVendor() << ENDL;
    fout << MESSAGE("Server release: ") << myWindow.getServerRelease() << ENDL;
    
    myWindow.show();
    myWindow.raise();
    myWindow.acquireFocus();
    Window::dispatch();
  }
};

STUB(WindowApplication);
