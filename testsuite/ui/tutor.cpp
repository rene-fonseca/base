#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/opengl/OpenGLContext.h>

using namespace com::azure::dev::base;

class TorusApplication : public Application {
public:
  
  TorusApplication()
    : Application("torus")
  {
  }
  
  class MyOpenGLContext : public OpenGLContext {
  private:
    
    Vector3D<double> rotation;
    double scale = 0;
    
    Vector3D<double> rotationBegin;
    double scaleBegin = 0;
    
    Position mouseButtonPosition;
    bool mouseLeftButtonPressed = false;
    bool mouseMiddleButtonPressed = false;
    bool mouseRightButtonPressed = false;
  public:
    
    MyOpenGLContext(
      const String& title,
      const Position& position,
      const Dimension& dimension,
      const Format& format)
      : OpenGLContext(position, dimension, format) {
      setTitle(title);
      setIconTitle(title);
      
      openGL.glColorMaterial(OpenGL::FRONT_AND_BACK, OpenGL::DIFFUSE);
      openGL.glEnable(OpenGL::COLOR_MATERIAL);
      
      openGL.glDisable(OpenGL::DEPTH_TEST);
      
      openGL.glHint(OpenGL::PERSPECTIVE_CORRECTION_HINT, OpenGL::NICEST);
      openGL.glHint(OpenGL::POINT_SMOOTH_HINT, OpenGL::NICEST);
      openGL.glHint(OpenGL::LINE_SMOOTH_HINT, OpenGL::NICEST);
      openGL.glHint(OpenGL::POLYGON_SMOOTH_HINT, OpenGL::NICEST);
      openGL.glEnable(OpenGL::LINE_SMOOTH);
      openGL.glShadeModel(OpenGL::SMOOTH);
      openGL.glPolygonMode(OpenGL::FRONT_AND_BACK, OpenGL::LINE);
      openGL.glLineWidth(2.5);
      
      openGL.glEnable(OpenGL::BLEND);
      openGL.glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      openGL.glBlendFunc(OpenGL::SRC_ALPHA, OpenGL::ONE);
      
      rotation = Vector3D<double>(0, 0, 0);
      scale = 1;
      
      mouseMiddleButtonPressed = false;
      mouseLeftButtonPressed = false;
      mouseRightButtonPressed = false;
    }

    ~MyOpenGLContext() noexcept {
    }

    void onDisplay() noexcept {
      openGL.glClearColor(0.0, 0.0, 0.0, 1.0);
      openGL.glClear(OpenGL::COLOR_BUFFER_BIT | OpenGL::DEPTH_BUFFER_BIT);
      
      openGL.glMatrixMode(OpenGL::MODELVIEW);
      openGL.glLoadIdentity();
      openGL.glRotatef(rotation.getX(), 0.0, 1.0, 0.0);
      openGL.glRotatef(rotation.getY(), 1.0, 0.0, 0.0);
      openGL.glRotatef(rotation.getZ(), 0.0, 0.0, 1.0);
      openGL.glScalef(scale * 0.005, scale * 0.005, scale * 0.005);
      openGL.glColor4f(0.25, 0.5, 0.75, 0.5);
      openGL.torus(4.0, 8.0, 64, 16);
      
      openGL.glFlush();
      swap();
    }

    void onResize(const Dimension& dimension) noexcept {
      openGL.glViewport(0, 0, dimension.getWidth(), dimension.getHeight());
      invalidate();
    }
    
    void onMouseMove(const Position& position, unsigned int state) noexcept {
      const Position difference = position - mouseButtonPosition;
      setCursor(OpenGLContext::HAND);
      
      unsigned int modifiers = getModifiers();
      
      if ((mouseMiddleButtonPressed) ||
          (mouseLeftButtonPressed && mouseRightButtonPressed)) {
        double scale = scaleBegin *
          Math::exp(static_cast<double>(difference.getY() * 0.01));
        if (scale < 0.00001) {
          scale = 0.0001;
        } else if (scale >= 100.0) {
          scale = 100.0;
        }
        this->scale = scale;
        invalidate();
      } else if (mouseLeftButtonPressed) {
        if (modifiers & Key::CONTROL) {
          rotation.setZ(
            rotationBegin.getZ() + difference.getX()/(256/180.0)
          );
        } else {
          rotation.setX(
            rotationBegin.getX() + difference.getX()/(256/180.0)
          );
          rotation.setY(
            rotationBegin.getY() + difference.getY()/(256/180.0)
          );
        }
        invalidate();
      }
    }
    
    void onMouseButton(
      const Position& position,
      Mouse::Button button,
      Mouse::Event event,
      unsigned int state) noexcept {
      
      switch (button) {
      case Mouse::LEFT:
        mouseLeftButtonPressed = event == Mouse::PRESSED;
        setCapture(mouseLeftButtonPressed);
        break;
      case Mouse::MIDDLE:
        mouseMiddleButtonPressed = event == Mouse::PRESSED;
        setCapture(mouseMiddleButtonPressed);
        break;
      case Mouse::RIGHT:        
        mouseRightButtonPressed = event == Mouse::PRESSED;
        setCapture(false);
        break;
      case Mouse::WHEEL:
        break;
      case Mouse::EXTRA:
        break;
      case Mouse::EXTRA2:
        break;
      }
      mouseButtonPosition = position;
      
      if (event == Mouse::PRESSED) {
        rotationBegin = rotation;
        scaleBegin = scale;
      }
    }
  };
  
  void main() {
    MyOpenGLContext::Format desiredFormat;
    int formatId = -1;
    
    Array<MyOpenGLContext::Format> formats = MyOpenGLContext::getFormats(
      OpenGLContext::RGB |
      OpenGLContext::DOUBLE_BUFFERED |
      OpenGLContext::DEPTH |
      OpenGLContext::DIRECT
    );
    
    Array<MyOpenGLContext::Format>::ReadEnumerator enu =
      formats.getReadEnumerator();
    for (unsigned int i = 0; enu.hasNext(); ++i) {
      const MyOpenGLContext::Format* format = enu.next();
      if ((formatId == -1) ||
          (format->alphaBits && (!desiredFormat.alphaBits))) {
        formatId = i;
        desiredFormat = *format;
      }
    }
    
    if (formatId == -1) {
      ferr << "Error: Format not available" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    MyOpenGLContext myOpenGLContext(
      MESSAGE("OpenGL rules"),
      Position(123, 312),
      Dimension(512, 512),
      desiredFormat
    );

    myOpenGLContext.show();
    myOpenGLContext.raise();
    myOpenGLContext.dispatch();
  }
};

APPLICATION_STUB(TorusApplication);
