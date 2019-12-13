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
#include <base/ui/PopUpWindow.h>
#include <base/ui/Label.h>
#include <base/ui/Split.h>
#include <base/ui/Button.h>
#include <base/ui/ProgressBar.h>
#include <base/ui/Picture.h>
#include <base/opengl/OpenGLWidget.h>

using namespace com::azure::dev::base;

class WindowApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  WindowApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) noexcept
    : Application("Window", numberOfArguments, arguments, environment) {
  }

  class MyOpenGLWidget : public OpenGLWidget {
  public:
    
    MyOpenGLWidget(
      Window& owner,
      const Format& format)
      : OpenGLWidget(owner, format) {
    }
    
    void onDisplay() noexcept {
      static uint8 gray = 0;
      openGL.glClearColor(gray++/255.0, 0.0, 0.0, 1.0);
      openGL.glClear(OpenGL::COLOR_BUFFER_BIT);
      openGL.glFlush();
      swap();
    }
    
    ~MyOpenGLWidget() noexcept {
    }
  };
  
  class MyWindow : public Window {
  private:

    class MyPopUpWindow : public PopUpWindow {
    public:

      MyPopUpWindow(
        const Position& position,
        const Dimension& dimension,
        unsigned int flags = 0)
        : PopUpWindow(position, dimension, flags) {
        setBrush(Color(Color::BEIGE));
        setPen(Pen(Color(0, 0, 0 /*Color::BLACK*/)));
      }
      
      void onDisplay() noexcept {
        rectangle(Position(0, 0), getDimension());
      }

      void onMouseButton(
        const Position& position,
        Mouse::Button button,
        Mouse::Event event,
        unsigned int state) noexcept {
        return;
        
        if (event == Mouse::PRESSED) {
          if (button == Mouse::LEFT) {
            setCapture(true);
          }
        }
        if (event == Mouse::RELEASED) {
          if (button == Mouse::LEFT) {
            setCapture(false);
          }
        }
      }
    };
    
    MyPopUpWindow popUpWindow;
    
    Label label;
    Split split;
    Button button;
    ProgressBar progressBar;
    Picture picture;
    // MyOpenGLWidget openGLWidget;

    inline OpenGLWidget::Format makeFormat() noexcept {
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
      unsigned int flags)
      : Window(position, dimension, flags),
        popUpWindow(Position(0, 0), Dimension(100, 200), 0),
        label(*this),
        split(*this),
        button(*this),
        progressBar(*this),
        picture(*this)/*,
                        openGLWidget(*this, makeFormat())*/ {
      
      setTitle(title);
      setIconTitle(title);
      
      label.setPosition(Position(16, 16));
      label.setDimension(Dimension(256, 32));
      label.setText("This is a label");
      label.setTextFormat(TextFormat::RIGHT | TextFormat::MIDDLE);
      label.setBrush(Color(192, 192, 192));
      label.setFont(Font("Arial", 16));
      
      split.setPosition(Position(8, 8));
      split.setDimension(Dimension(128, 128));
      split.setOffset(64, Split::FIRST);
      
      button.setText("Accept");
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
      fout << "Bitmap: " << picture.getBitmap().getDimension() << ENDL;
      
      // openGLWidget.setPosition(Position(256, 32));
      // openGLWidget.setDimension(Dimension(128, 128));
    }
    
    void onDisplay() noexcept {
      // setBrush();
      clear();
      line(Position(10, 60), Position(64, 20));
      Bitmap bitmap = picture.getBitmap();
      putBitmap(Position(10, 10), Dimension(128, 128), bitmap);
      flush();
    }
    
    void onMove(const Position& position) noexcept {
      fout << "Event: move " << position << ENDL;
      progressBar.setCurrentValue(position.getX());
    }
    
    void onResize(const Dimension& dimension) noexcept {
      fout << "Event: resize " << dimension << ENDL;
      onDisplay();
    }
    
    struct Flag {
      unsigned int mask;
      Literal literal;
    };
    
    void onMouseMove(const Position& position, unsigned int state) noexcept {
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

    void onMouseScope(bool scope) noexcept {
      fout << "Event: mouse scope "
           << (scope ? Literal("INSIDE SCOPE") : Literal("OUT OF SCOPE")) << ENDL;
    }
    
    void onMouseButton(
      const Position& position,
      Mouse::Button button,
      Mouse::Event event,
      unsigned int state) noexcept {
      
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
      
      static const Literal EVENT_STRING[] = {
        Literal("PRESSED"),
        Literal("RELEASED"),
        Literal("DOUBLE CLICKED"),
        Literal("TURNED")
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
      popUpWindow.hide();
      
      if (event == Mouse::PRESSED) {
        if (button == Mouse::LEFT) {
          setCapture(true);
        }
        if (button == Mouse::RIGHT) {
          popUpWindow.setPosition(toGlobalPosition(position));
          popUpWindow.show();
          
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
      const Position& position, int delta, unsigned int buttons) noexcept {
      fout << "Event: mouse wheel " << position << SP << delta << ENDL;
    }
    
    void onKey(
      unsigned int key, unsigned int flags, unsigned int modifiers) noexcept {
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

    void onDestruction() noexcept {
      fout << "Event: destruction" << ENDL;
      exit();
    }
    
    bool onClose() noexcept {
      fout << "Event: close " << ENDL;
      return true;
    }
    
    void onVisibility(Visibility visibility) noexcept {
      static const Literal VISIBILITY[] = {
        Literal("INVISIBLE"),
        Literal("PARTIALLY VISIBLE"),
        Literal("VISIBLE")
      };
      
      fout << "Event: visibility " << VISIBILITY[visibility] << ENDL;
    }
    
    void onFocus(Focus focus) noexcept {
      update();
      fout << "Event: focus "
           << ((focus == ACQUIRED_FOCUS) ? Literal("ACQUIRED") : Literal("LOST")) << ENDL;
    }
    
    void onMenu(unsigned int identifier) noexcept {
      fout << "Event: menu " << identifier << ENDL;
    }
    
    void onCommand(unsigned int identifier) noexcept {
      fout << "Event: command " << identifier << ENDL;
    }
  };
  
  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;

    //Display display();
    
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

APPLICATION_STUB(WindowApplication);
