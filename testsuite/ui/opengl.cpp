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
#include <base/opengl/OpenGLContext.h>
#include <base/Version.h>
#include <base/ui/MessageDialog.h>

using namespace com::azure::dev::base;

/** Verbosity levels. */
class Verbosity {
public:
      
  enum Value {
    NO_INFORMATION,
    WARNINGS,
    NORMAL,
    COMMANDS,
    ACTIVE_MOUSE_EVENTS,
    ALL_MOUSE_EVENTS,
    EVERYTHING,
    DEFAULT = EVERYTHING // NORMAL
  };
};

/** Internal commands. */
class Command {
public:

  enum {
    NOTHING,
        
    SELECT_VERBOSITY_NO_INFORMATION,
    SELECT_VERBOSITY_WARNINGS,
    SELECT_VERBOSITY_NORMAL,
    SELECT_VERBOSITY_COMMANDS,
    SELECT_VERBOSITY_ACTIVE_MOUSE_EVENTS,
    SELECT_VERBOSITY_ALL_MOUSE_EVENTS,
    SELECT_VERBOSITY_EVERYTHING,
        
    RENDER_DEFAULT,
    RENDER_INTENSITY,
    RENDER_RGB_CLOUD,
    RESET_VIEW_PARAMETERS,
    SHOW_OPENGL_INFORMATION, /**< Show OpenGL specific information. */
    HELP, /**< Show help information. */
    ABOUT, /**< Show information about the application. */
    QUIT, /**< Quit the application. */
    
    SELECT_LOW_DETAIL,
    SELECT_MEDIUM_DETAIL,
    SELECT_HIGH_DETAIL,
        
    SHOW_FRAME_RATE,
        
    ENTER_FULL_SCREEN_MODE,
        
    CYCLE_SHADING_MODEL,
    SELECT_SHADING_MODEL_FLAT,
    SELECT_SHADING_MODEL_SMOOTH,
        
    CYCLE_POLYGON_MODE,
    SELECT_POLYGON_MODE_FILL,
    SELECT_POLYGON_MODE_LINE,
    SELECT_POLYGON_MODE_POINT,
        
    BLENDING_ENABLE,
    BLENDING_DISABLE,
        
    LIGHTING_ENABLE,
    LIGHTING_DISABLE,
        
    ROTATE_AROUND_X_AXIS_NEG,
    ROTATE_AROUND_X_AXIS_POS,
    ROTATE_AROUND_Y_AXIS_NEG,
    ROTATE_AROUND_Y_AXIS_POS,
    ROTATE_AROUND_Z_AXIS_NEG,
    ROTATE_AROUND_Z_AXIS_POS,
        
    TRANSLATE_ALONG_X_NEG,
    TRANSLATE_ALONG_X_POS,
    TRANSLATE_ALONG_Y_NEG,
    TRANSLATE_ALONG_Y_POS,
    TRANSLATE_ALONG_Z_NEG,
    TRANSLATE_ALONG_Z_POS,

    SELECT_MODE_SYSTEM,
    SELECT_MODE_FLOOR,
    SELECT_MODE_CUBE,
    SELECT_MODE_CONE,
    SELECT_MODE_TORUS,

    SELECT_QUALITY_WORST,
    SELECT_QUALITY_NORMAL,
    SELECT_QUALITY_BEST
  };
};
    
/** The supported shading models. */
class ShadingModel {
public:
      
  enum Model {
    FLAT,
    SMOOTH,
    FIRST_MODEL = FLAT,
    LAST_MODEL = SMOOTH
  };
};
    
/** The supported polygon modes. */
class PolygonMode {
public:

  enum Mode {
    FILL,
    LINE,
    POINT,
    FIRST_MODE = FILL,
    LAST_MODE = POINT
  };
};

/** Quality. */
enum Quality {
  QUALITY_WORST,
  QUALITY_NORMAL,
  QUALITY_BEST
};

/** The mode. */
enum Mode {
  MODE_SYSTEM,
  MODE_FLOOR,
  MODE_CUBE,
  MODE_CONE,
  MODE_TORUS
};

class MyMenu : public Menu {
public:
  
  class PolygonModeMenu : public Menu {
  public:
    
    PolygonModeMenu() {
      append(MESSAGE("&Fill\tf"), Command::SELECT_POLYGON_MODE_FILL);
      append(MESSAGE("&Line\tl"), Command::SELECT_POLYGON_MODE_LINE);
      append(MESSAGE("&Point\tp"), Command::SELECT_POLYGON_MODE_POINT);
    }
  };
  
  class ShadingModelMenu : public Menu {
  public:
    
    ShadingModelMenu() {
      append(MESSAGE("&Flat\tf"), Command::SELECT_SHADING_MODEL_FLAT);
      append(MESSAGE("&Smooth\ts"), Command::SELECT_SHADING_MODEL_SMOOTH);
    }
  };
  
  class ModeMenu : public Menu {
  public:
    
    ModeMenu() {
      append(MESSAGE("&System"), Command::SELECT_MODE_SYSTEM);
      append(MESSAGE("&Floor"), Command::SELECT_MODE_FLOOR);
      append(MESSAGE("&Cube"), Command::SELECT_MODE_CUBE);
      append(MESSAGE("C&one"), Command::SELECT_MODE_CONE);
      append(MESSAGE("&Torus"), Command::SELECT_MODE_TORUS);
    }
  };
  
  class QualityMenu : public Menu {
  public:
    
    QualityMenu() {
      append(MESSAGE("&Worst"), Command::SELECT_QUALITY_WORST);
      append(MESSAGE("&Normal"), Command::SELECT_QUALITY_NORMAL);
      append(MESSAGE("&Best"), Command::SELECT_QUALITY_BEST);
    }
  };
  
  class VerbosityMenu : public Menu {
  public:
    
    VerbosityMenu() {
      append(MESSAGE("&No information"), Command::SELECT_VERBOSITY_NO_INFORMATION);
      append(MESSAGE("&Warnings"), Command::SELECT_VERBOSITY_WARNINGS);
      append(MESSAGE("&Normal"), Command::SELECT_VERBOSITY_NORMAL);
      append(MESSAGE("&Commands"), Command::SELECT_VERBOSITY_COMMANDS);
      append(MESSAGE("&Active mouse events"), Command::SELECT_VERBOSITY_ACTIVE_MOUSE_EVENTS);
      append(MESSAGE("&All mouse events"), Command::SELECT_VERBOSITY_ALL_MOUSE_EVENTS);
      append(MESSAGE("&Everything"), Command::SELECT_VERBOSITY_EVERYTHING);
    }
  };
private:
  
  PolygonModeMenu polygonModeMenu;
  ShadingModelMenu shadingModelMenu;
  ModeMenu modeMenu;
  QualityMenu qualityMenu;
  VerbosityMenu verbosityMenu;
public:
  
  MyMenu() {
    appendMenu(MESSAGE("S&hading model"), shadingModelMenu);
    appendMenu(MESSAGE("&Polygon mode"), polygonModeMenu);
    appendMenu(MESSAGE("&Mode"), modeMenu);
    appendMenu(MESSAGE("&Quality"), qualityMenu);
    appendMenu(MESSAGE("&Verbosity"), verbosityMenu);
    append(MESSAGE("&About\t(C-a)"), Command::ABOUT);
    append(MESSAGE("&Reset\t(C-r)"), Command::RESET_VIEW_PARAMETERS);
    append(MESSAGE("&Quit\t(C-q)"), Command::QUIT);
  }
};

class View {
public:
    
  Vector3D<double> translation;
  Vector3D<double> rotation;
  double scale = 1;
public:
    
  inline View() noexcept {
    resetParameters();
  }
      
  inline void resetParameters() noexcept {
    setTranslation(Vector3D<double>(0, 0, 0));
    setRotation(Vector3D<double>(0, 0, 0));
    setScale(1);
  }
    
  inline const Vector3D<double>& getTranslation() const noexcept {
    return translation;
  }
    
  inline void setTranslation(const Vector3D<double>& translation) noexcept {
    this->translation = translation;
  }
    
  inline const Vector3D<double>& getRotation() const noexcept {
    return rotation;
  }
    
  inline void setRotation(const Vector3D<double>& rotation) noexcept {
    this->rotation = rotation;
  }
    
  inline double getScale() const noexcept {
    return scale;
  }
      
  inline void setScale(double scale) noexcept {
    this->scale = scale;
  }
};

class OpenGLApplication : public Application {
private:
  
  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:
  
  OpenGLApplication()
    : Application("opengl")
  {
  }
  
  class MyOpenGLContext : public OpenGLContext {
  private:

    Verbosity::Value verbosity = Verbosity::DEFAULT;
    MyMenu menu;
    Mode mode = MODE_TORUS;
    View view;
    
    unsigned int displayMode = MODE_WINDOW;
    ShadingModel::Model shadingModel = ShadingModel::FLAT;
    PolygonMode::Mode polygonMode = PolygonMode::FILL;
    bool blending = false;
    bool lighting = true;
    
    Vector3D<double> translationBegin;
    Vector3D<double> rotationBegin;
    double scaleBegin = 0;
    
    double xTranslation = 0;
    double yTranslation = 0;
    double zTranslation = 0;
    double xAngle = 0;
    double yAngle = 0;
    double zAngle = 0;
    double scale = 20;
    
    double orthoLeft = 0;
    double orthoRight = 0;
    double orthoBottom = 0;
    double orthoTop = 0;
    double orthoNear = 0;
    double orthoFar = 0;
    
    Position mouseButtonPosition;
    bool mouseLeftButtonPressed = false;
    bool mouseMiddleButtonPressed = false;
    bool mouseRightButtonPressed = false;
    
    Vector3D<double> drag;
  public:

    enum Object {
      OBJECT_SYSTEM = 1,
      OBJECT_FLOOR,
      OBJECT_CUBE,
      OBJECT_CONE,
      OBJECT_TORUS
    };
    
    MyOpenGLContext(
      const String& title,
      const Position& position,
      const Dimension& dimension,
      const Format& format)
      : OpenGLContext(position, dimension, format)
    {
      setTitle(title);
      setIconTitle(title);
      
      openGL.glPolygonMode(OpenGL::FRONT_AND_BACK, OpenGL::FILL);
      
      static const OpenGL::GLfloat lightDiffuse[] = {1.0, 0.5, 1.0, 1.0};
      static const OpenGL::GLfloat lightPosition[] = {10.0, 10.0, 10.0, 0.0};

      // param: AMBIENT, DIFFUSE, SPECULAR, POSITION, SPOT_DIRECTION, SPOT_EXPONENT, SPOT_CUTOFF, CONSTANT_ATTENUATION, LINEAR_ATTENUATION, QUADRATIC_ATTENUATION
      
      // openGL.glMateriali(OpenGL::FRONT_AND_BACK, OpenGL::SHININESS, ?);
      // param: LIGHT_MODEL_AMBIENT, LIGHT_MODEL_LOCAL_VIEWER, LIGHT_MODEL_TWO_SIDE, LIGHT_MODEL_COLOR_CONTROL
      //openGL.glLightModel();

      openGL.glLightfv(OpenGL::LIGHT0, OpenGL::DIFFUSE, lightDiffuse);
      openGL.glLightfv(OpenGL::LIGHT0, OpenGL::POSITION, lightPosition); // uses current model-view matrix
      openGL.glEnable(OpenGL::LIGHT0);

      // FRONT, BACK, or FRONT_AND_BACK; EMISSION, AMBIENT, DIFFUSE, SPECULAR, or AMBIENT AND DIFFUSE
      //openGL.glColorMaterial(OpenGL::FRONT, OpenGL::AMBIENT);
      openGL.glColorMaterial(OpenGL::FRONT_AND_BACK, OpenGL::DIFFUSE);
      openGL.glEnable(OpenGL::COLOR_MATERIAL);

      //openGL.glEnable(OpenGL::MULTISAMPLE); // OpenGL 1.3
      
      openGL.glEnable(OpenGL::DEPTH_TEST);
      openGL.glDepthFunc(OpenGL::LEQUAL);

      mode = MODE_TORUS;
      openGL.glHint(OpenGL::PERSPECTIVE_CORRECTION_HINT, OpenGL::DONT_CARE);
      openGL.glHint(OpenGL::POINT_SMOOTH_HINT, OpenGL::DONT_CARE);
      openGL.glHint(OpenGL::LINE_SMOOTH_HINT, OpenGL::DONT_CARE);
      openGL.glHint(OpenGL::POLYGON_SMOOTH_HINT, OpenGL::DONT_CARE);
      openGL.glDisable(OpenGL::LINE_SMOOTH);
      shadingModel = ShadingModel::FLAT;
      openGL.glShadeModel(OpenGL::FLAT);
      polygonMode = PolygonMode::FILL;
      openGL.glPolygonMode(OpenGL::FRONT_AND_BACK, OpenGL::FILL);
      blending = false;
      openGL.glDisable(OpenGL::BLEND);
      lighting = true;
      openGL.glEnable(OpenGL::LIGHTING);
        
      const String vendor = reinterpret_cast<const char*>(openGL.glGetString(OpenGL::VENDOR));
      const String version = reinterpret_cast<const char*>(openGL.glGetString(OpenGL::VERSION));
      const String renderer = reinterpret_cast<const char*>(openGL.glGetString(OpenGL::RENDERER));
      const String extensions = reinterpret_cast<const char*>(openGL.glGetString(OpenGL::EXTENSIONS));
      fout << "OpenGL context information: " << EOL
        << indent(2) << "Vendor: " << vendor << EOL
        << indent(2) << "Version: " << version << EOL
        << indent(2) << "Renderer: " << renderer << EOL
        << indent(2) << "Extensions: " << extensions << ENDL;

      makeSystem();
      makeFloor();
      makeCube();
      makeCone();
      makeTorus();
    }

    ~MyOpenGLContext() noexcept {
    }
    
    void setQuality(Quality quality) noexcept {
      // TAG: need attribute
      switch (quality) {
      case QUALITY_WORST:
        // fastest
        openGL.glHint(OpenGL::PERSPECTIVE_CORRECTION_HINT, OpenGL::FASTEST);
        openGL.glHint(OpenGL::POINT_SMOOTH_HINT, OpenGL::FASTEST);
        openGL.glHint(OpenGL::LINE_SMOOTH_HINT, OpenGL::FASTEST);
        openGL.glHint(OpenGL::POLYGON_SMOOTH_HINT, OpenGL::FASTEST);
        openGL.glDisable(OpenGL::LINE_SMOOTH);
        break;
      case QUALITY_NORMAL:
        // ok
        openGL.glHint(OpenGL::PERSPECTIVE_CORRECTION_HINT, OpenGL::DONT_CARE);
        openGL.glHint(OpenGL::POINT_SMOOTH_HINT, OpenGL::DONT_CARE);
        openGL.glHint(OpenGL::LINE_SMOOTH_HINT, OpenGL::DONT_CARE);
        openGL.glHint(OpenGL::POLYGON_SMOOTH_HINT, OpenGL::DONT_CARE);
        openGL.glDisable(OpenGL::LINE_SMOOTH);
        break;
      case QUALITY_BEST:
        openGL.glHint(OpenGL::PERSPECTIVE_CORRECTION_HINT, OpenGL::NICEST);
        openGL.glHint(OpenGL::POINT_SMOOTH_HINT, OpenGL::NICEST);
        openGL.glHint(OpenGL::LINE_SMOOTH_HINT, OpenGL::NICEST);
        openGL.glHint(OpenGL::POLYGON_SMOOTH_HINT, OpenGL::NICEST);
        openGL.glEnable(OpenGL::LINE_SMOOTH);
        break;
      }
    }

    inline void setVerbosity(Verbosity::Value verbosity) noexcept {
      this->verbosity = verbosity;
    }
      
    void setTranslation(const Vector3D<double>& translation) noexcept {
      view.setTranslation(translation);
    }
      
    void setRotation(const Vector3D<double>& rotation) noexcept {
      view.setRotation(rotation);
    }
      
    void setScale(double scale) noexcept {
      view.setScale(scale);
    }
      
    void resetViewParameters() noexcept {
      setTranslation(Vector3D<double>(0, 0, 0));
      setRotation(Vector3D<double>(0, 0, 0));
      setScale(1);
    }
      
    void setShadingModel(ShadingModel::Model shadingModel) noexcept {
      if (shadingModel != this->shadingModel) {
        this->shadingModel = shadingModel;
        switch (shadingModel) {
        case ShadingModel::SMOOTH:
          openGL.glShadeModel(OpenGL::SMOOTH);
          break;
        case ShadingModel::FLAT:
          openGL.glShadeModel(OpenGL::FLAT);
          break;
        }
        invalidate();
      }
    }
    
    void setPolygonMode(PolygonMode::Mode polygonMode) noexcept {
      if (polygonMode != this->polygonMode) {
        this->polygonMode = polygonMode;
        switch (polygonMode) {
        case PolygonMode::FILL:
          openGL.glPolygonMode(OpenGL::FRONT_AND_BACK, OpenGL::FILL);
          break;
        case PolygonMode::LINE:
          openGL.glPolygonMode(OpenGL::FRONT_AND_BACK, OpenGL::LINE);
          break;
        case PolygonMode::POINT:
          openGL.glPolygonMode(OpenGL::FRONT_AND_BACK, OpenGL::POINT);
          break;
        }
        invalidate();
      }
    }
      
    void setBlending(bool blending) noexcept {
      if (blending != this->blending) {
        this->blending = blending;
        if (blending) {
          openGL.glEnable(OpenGL::BLEND);
          openGL.glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
          openGL.glBlendFunc(OpenGL::SRC_ALPHA, OpenGL::ONE);
        } else {
          openGL.glDisable(OpenGL::BLEND);
        }
        invalidate();
      }
    }
      
    void setLighting(bool lighting) noexcept {
      if (lighting != this->lighting) {
        this->lighting = lighting;
        if (lighting) {
          openGL.glEnable(OpenGL::LIGHTING);
        } else {
          openGL.glDisable(OpenGL::LIGHTING);
        }
        invalidate();
      }
    }

    void setMode(Mode mode) noexcept {
      if (mode != this->mode) {
        this->mode = mode;
        invalidate();
      }
    }

    /** Maps the (x,y)-position into world coordinates. */
    Vector3D<double> getPosition(const Position& position, const int viewPort[4]) const noexcept {
      double tempX = static_cast<double>(position.getX() - viewPort[0])/static_cast<double>(viewPort[2]);
      double tempY = static_cast<double>(position.getY() - viewPort[1])/static_cast<double>(viewPort[3]);
      return Vector3D<double>(
        orthoLeft + tempX * (orthoRight - orthoLeft),
        orthoTop + tempY * (orthoBottom - orthoTop),
        orthoNear
      );
    }

    void makeSystem() noexcept {
      OpenGL::DisplayList displayList(openGL, OBJECT_SYSTEM);
      //         openGL.glColor4f(0.8, 0.8, 0.8, 0.5);
      //         OpenGL::Block block(openGL, OpenGL::LINES);
      //         openGL.glVertex3i(-10, 0, 0);
      //         openGL.glVertex3i(10, 0, 0);
      //         openGL.glVertex3i(0, -10, 0);
      //         openGL.glVertex3i(0, 10, 0);
      //         openGL.glVertex3i(0, 0, -10);
      //         openGL.glVertex3i(0, 0, 10);
      
      openGL.glScalef(10 * 0.5, 10 * 0.5, 10 * 0.5);
      openGL.glColor4f(1.0, 0.0, 0.0, 1.0);
      openGL.cylinder(0.25, 0.25, 2, 16, 1);
      openGL.glTranslatef(0, 0, 2);
      openGL.cone(0.5, 1.0, 16, 1);
      openGL.glTranslatef(0, 0, -2);
      openGL.glRotatef(90, 1.0, 0.0, 0.0); // x axis
      openGL.glColor4f(0.0, 1.0, 0.0, 1.0);
      openGL.cylinder(0.25, 0.25, 2, 16, 1);
      openGL.glTranslatef(0, 0, 2);
      openGL.cone(0.5, 1.0, 16, 1);
      openGL.glTranslatef(0, 0, -2);
      openGL.glRotatef(90, 0.0, 1.0, 0.0); // y axis
      openGL.glColor4f(0.0, 0.0, 1.0, 1.0);
      openGL.cylinder(0.25, 0.25, 2, 16, 1);
      openGL.glTranslatef(0, 0, 2);
      openGL.cone(0.5, 1.0, 16, 1);
    }
     
    void makeFloor() noexcept {
      OpenGL::DisplayList displayList(openGL, OBJECT_FLOOR);
    }

    void makeCube() noexcept {
      OpenGL::DisplayList displayList(openGL, OBJECT_CUBE);
      
      openGL.glColor4f(0.0, 1.0, 0.0, 0.75);
      
      {
        OpenGL::Block block(openGL, OpenGL::QUAD_STRIP); // draw the sides of the cube
        static double X[3] = {3, 3, 3};
        openGL.glVertex3dv(X);
       
        // Normal A
        openGL.glNormal3f(0.0, 0.0, -1.0);
        
        openGL.glVertex3i(3, 3, -3); // vertex 1
        openGL.glVertex3i(3, 3, -3); // vertex 1
        openGL.glVertex3i(3, -3, -3); // vertex 2
        openGL.glVertex3i(-3, 3, -3); // vertex 3
        openGL.glVertex3i(-3, -3, -3); // vertex 4
        
        // Normal B
        openGL.glNormal3f(-1.0, 0.0, 0.0);
        openGL.glVertex3i(-3, 3, 3); // vertex 5
        openGL.glVertex3i(-3, -3, 3); // vertex 6
        
        // Normal C
        openGL.glNormal3f(0.0, 0.0, 1.0);
        openGL.glVertex3i(3, 3, 3); // vertex 7
        openGL.glVertex3i(3, -3, 3); // vertex 8
        
        // Normal D
        openGL.glNormal3f(1.0, 0.0, 0.0);
        openGL.glVertex3i(3, 3, -3); // vertex 9
        openGL.glVertex3i(3, -3, -3); // vertex 10
      }
    }
    
    void makeCone() noexcept {
      OpenGL::DisplayList displayList(openGL, OBJECT_CONE);
      openGL.glColor4f(0.25, 0.5, 0.75, 0.5);
      openGL.cone(6.0, 12.0, 8, 8);
    }
    
    void makeTorus() noexcept {
      OpenGL::DisplayList displayList(openGL, OBJECT_TORUS);
      openGL.glColor4f(0.25, 0.5, 0.75, 0.5);
      openGL.torus(4.0, 8.0, 64, 16);
    }
    
    void displayObject(unsigned int object) noexcept {
      openGL.glClearColor(0.0, 0.0, 0.0, 1.0);
      openGL.glClear(OpenGL::COLOR_BUFFER_BIT | OpenGL::DEPTH_BUFFER_BIT);
      
      openGL.glMatrixMode(OpenGL::MODELVIEW);
      openGL.glLoadIdentity();
      Vector3D<double> translation = view.getTranslation();
      openGL.glTranslatef(translation.getX(), translation.getY(), translation.getZ());
      Vector3D<double> rotation = view.getRotation();
      openGL.glRotatef(rotation.getX(), 0.0, 1.0, 0.0);
      openGL.glRotatef(rotation.getY(), 1.0, 0.0, 0.0);
      openGL.glRotatef(rotation.getZ(), 0.0, 0.0, 1.0);
      openGL.glScalef(view.getScale() * 0.005, view.getScale() * 0.005, view.getScale() * 0.005);
      openGL.glCallList(object);
      
      openGL.glFlush();
      swap();
    }
    
    void onDisplay() noexcept {
      switch (mode) {
      case MODE_SYSTEM:
        displayObject(OBJECT_SYSTEM);
        break;
      case MODE_FLOOR:
        displayObject(OBJECT_FLOOR);
        break;
      case MODE_CUBE:
        displayObject(OBJECT_CUBE);
        break;
      case MODE_CONE:
        displayObject(OBJECT_CONE);
        break;
      case MODE_TORUS:
        displayObject(OBJECT_TORUS);
        break;
      }
    }
    
    void onMove(const Position& position) noexcept
    {
      if (verbosity >= Verbosity::ALL_MOUSE_EVENTS) {
        fout << "Window move event: " << position << ENDL;
      }
    }
    
    void onResize(const Dimension& dimension) noexcept
    {
      if (verbosity >= Verbosity::ALL_MOUSE_EVENTS) {
        fout << "Resize event: " << dimension << ENDL;
      }
      
      openGL.glViewport(0, 0, dimension.getWidth(), dimension.getHeight());
      invalidate();
      
//         orthoTop = 1.0;
//         orthoBottom = -1.0;
//         orthoRight = static_cast<double>(dimension.getWidth())/static_cast<double>(dimension.getHeight());
//         orthoLeft = -orthoRight;
        
//         openGL.glMatrixMode(OpenGL::PROJECTION);
//         openGL.glLoadIdentity();
//         openGL.glOrtho(orthoLeft, orthoRight, orthoBottom, orthoTop, orthoNear, orthoFar);
        
//         openGL.glMatrixMode(OpenGL::MODELVIEW);
    }
    
    struct Flag {
      unsigned int mask;
      Literal literal;
    };
    
    void onMouseMove(const Position& position, unsigned int state) noexcept
    {
      const Position difference = position - mouseButtonPosition;
      if (verbosity >= Verbosity::ALL_MOUSE_EVENTS) {
        fout << "Mouse motion event: " << position << ENDL;
      }
      setCursor(OpenGLContext::HAND); // TAG: remove
      
      unsigned int modifiers = getModifiers();
      
      if ((mouseMiddleButtonPressed) || (mouseLeftButtonPressed && mouseRightButtonPressed)) {
        if (modifiers & Key::CONTROL) {
          int viewPort[4];
          openGL.glGetIntegerv(OpenGL::VIEWPORT, viewPort);
          Vector3D<double> position3D = getPosition(position, viewPort);
          view.translation.setX(translationBegin.getX() + position3D.getX() - drag.getX());
          view.translation.setY(translationBegin.getY() + position3D.getY() - drag.getY());
        } else {
          double scale = scaleBegin * Math::exp(static_cast<double>(difference.getY() * 0.01));
          if (scale < 0.00001) {
            scale = 0.0001;
          } else if (scale >= 10.0) {
            scale = 10.0;
          }
          view.setScale(scale);
        }
        invalidate();
      } else if (mouseLeftButtonPressed) {
        if (modifiers & Key::CONTROL) {
          view.rotation.setZ(rotationBegin.getZ() + difference.getX()/(256/*dimension.getWidth()*//180.0));
        } else {
          view.rotation.setX(rotationBegin.getX() + difference.getX()/(256/*dimension.getHeight()*//180.0));
          view.rotation.setY(rotationBegin.getY() + difference.getY()/(256/*dimension.getWidth()*//180.0));
          
          /*double ax = difference.getY();
              double ay = difference.getX();
              double az = 0;
              double angle = 180.0 * sqrt(ax*ax + ay*ay + az*az)/static_cast<double>(dimension.getWidth());
          */
            // use inverse matrix to determine local axis of rotation
            //openGL.glGetDoublev(OpenGL::MODELVIEW_MATRIX, modelViewMatrix);
            //invertMatrix(modelViewMatrix, invertedModelViewMatrix);
            
            //double axisX = invertedModelViewMatrix[0] * ax + invertedModelViewMatrix[4] * ay + invertedModelViewMatrix[7] * az;
            //double axisY = invertedModelViewMatrix[1] * ax + invertedModelViewMatrix[5] * ay + invertedModelViewMatrix[8] * az;
            //double axisZ = invertedModelViewMatrix[2] * ax + invertedModelViewMatrix[6] * ay + invertedModelViewMatrix[9] * az;
            //openGL.glRotatef(angle, axisX, axisY, axisZ);
        }
        invalidate();
      }
//         if (buttons != 0) {
//           fout << "Mouse move: " << position << ' ' << "ACTIVE" << ENDL;
//         } else {
//           fout << "Mouse move: " << position << ' ' << "PASSIVE" << ENDL;
//         }
    }

    void onMouseScope(bool scope) noexcept
    {
      fout << "Event: mouse scope "
           << (scope ? MESSAGE("INSIDE SCOPE") : MESSAGE("OUT OF SCOPE")) << ENDL;
    }
    
    void onMouseButton(const Position& position, Mouse::Button button, Mouse::Event event, unsigned int state) noexcept
    {
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
      
      if (verbosity >= Verbosity::ACTIVE_MOUSE_EVENTS) {
        fout << "Mouse button event: "
             << getMouseButtonName(button) << ' ';
        
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
      }

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
        if (event == Mouse::PRESSED) {
          displayMenu(position, menu);
        }
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
        // command: TRANSLATE_IN_XY_PLANE
        // command: ROTATE_AROUND_X_AND_Y
        // command: SCALE
        // command: MEASURE_DISTANCE
        translationBegin = view.getTranslation();
        rotationBegin = view.getRotation();
        scaleBegin = view.getScale();
      }
      
      int viewPort[4];
      openGL.glGetIntegerv(OpenGL::VIEWPORT, viewPort);
      Vector3D<double> drag = getPosition(position, viewPort);    
      drag.setZ(0);
    }
    
    void onMouseWheel(
      const Position& position, int delta, unsigned int buttons) noexcept {
      if (verbosity >= Verbosity::ACTIVE_MOUSE_EVENTS) {
        fout << "Mouse wheel" << ENDL;
      }
      setTranslation(view.getTranslation() + Vector3D<double>(0, 0, 0.1 * delta/120));
      invalidate();
    }
    
    void onKey(unsigned int key, unsigned int flags, unsigned int modifiers) noexcept
    {
      if (flags & Key::PRESSED) {
        if (flags & Key::DEAD) {
          return;
        }

        if ((flags & Key::ASCII) && ASCIITraits::isGraph(key)) {
          fout << "Key: " << PREFIX << HEX << key << ' '
               << '\'' << static_cast<char>(key) << '\''
               << ' ' << "PRESSED";
        } else {
          fout << "Key: " << PREFIX << HEX << key << ' ' << "PRESSED";
        }
        if (flags & Key::FIRST_TIME) {
          fout << ' ' << "FIRST";
        }
        fout << ENDL;
      } else {
        fout << "Key: " << PREFIX << HEX << key << ' ' << "RELEASED" << ENDL;
      }

      if (flags & Key::PRESSED) {
        if (flags & Key::DEAD) {
          return;
        }
          
        unsigned int command = Command::NOTHING;
        switch (key) {
        case Key::LEFT:
          command = Command::ROTATE_AROUND_X_AXIS_NEG;
          break;
        case Key::RIGHT:
          command = Command::ROTATE_AROUND_X_AXIS_POS;
          break;
        case Key::UP:
          command = Command::ROTATE_AROUND_Y_AXIS_NEG;
          break;
        case Key::DOWN:
          command = Command::ROTATE_AROUND_Y_AXIS_POS;
          break;
        case Key::PRIOR:
          if (modifiers & Key::SHIFT) {
            command = Command::TRANSLATE_ALONG_X_NEG;
          } else if (modifiers & Key::CONTROL) {
            command = Command::TRANSLATE_ALONG_Y_NEG;
          } else if (modifiers & Key::ALT) {
          } else {
            command = Command::TRANSLATE_ALONG_Z_NEG;
          }
          break;
        case Key::NEXT:
          if (modifiers & Key::SHIFT) {
            command = Command::TRANSLATE_ALONG_X_POS;
          } else if (modifiers & Key::CONTROL) {
            command = Command::TRANSLATE_ALONG_Y_POS;
          } else if (modifiers & Key::ALT) {
          } else {
            command = Command::TRANSLATE_ALONG_Z_POS;
          }
          break;
        case Key::HOME:
          command = Command::RESET_VIEW_PARAMETERS;
          break;
        case Key::END:
          break;
        case Key::F1:
          command = Command::SHOW_OPENGL_INFORMATION;
          break;
        case Key::F12:
          setDisplayMode(MODE_FULL_SCREEN);
          break;
        case 'q':
          command = Command::QUIT;
          break;
        case 'a':
          setAutorepeat(!getAutorepeat());
          break;
        case 's':
          command = Command::CYCLE_SHADING_MODEL;
          break;
        case 'b':
          command = blending ? Command::BLENDING_DISABLE : Command::BLENDING_ENABLE;
          break;
        case 'm':
          command = Command::SELECT_SHADING_MODEL_FLAT;
          break;
        case 'L':
          command = lighting ? Command::LIGHTING_DISABLE : Command::LIGHTING_ENABLE;
          break;
        case 'f':
          command = Command::SELECT_POLYGON_MODE_FILL;
          break;
        case 'l':
          command = Command::SELECT_POLYGON_MODE_LINE;
          break;
        case 'p':
          command = Command::SELECT_POLYGON_MODE_POINT;
          break;
        case 'r':
          command = Command::RESET_VIEW_PARAMETERS;
          break;
        }
        onCommand(command);
      }
    }
    
    void onIdle() noexcept
    {
      invalidate();
    }
    
    bool onClose() noexcept
    {
      fout << "Event: close " << ENDL;
      MessageDialog dialog(MESSAGE("Quit"), MESSAGE("Do you really wan't to quit?"), MessageDialog::QUESTION);
      dialog.execute();
      if (dialog.getAnswer() == MessageDialog::YES) {
        Application::getApplication()->terminate();
      }
      return dialog.getAnswer() == MessageDialog::YES;
    }
    
    void onVisibility(Visibility visibility) noexcept
    {
      if (verbosity >= Verbosity::ACTIVE_MOUSE_EVENTS) {
        fout << "Visibility event: "
             << ((visibility == VISIBLE) ? MESSAGE("VISIBLE") : MESSAGE("INVISIBLE"))
             << ENDL;
      }
    }
    
    void onFocus(Focus focus) noexcept
    {
      if (verbosity >= Verbosity::ACTIVE_MOUSE_EVENTS) {
        fout << "Focus event: "
             << ((focus == ACQUIRED_FOCUS) ? MESSAGE("ACQUIRED FOCUS") : MESSAGE("LOST FOCUS"))
             << ENDL;
      }
    }

    void dumpOpenGLInformation() noexcept
    {
      fout << "Vendor: " << openGL.getVendor() << EOL
           << "Renderer: " << openGL.getRenderer() << EOL
           << "Version: " << openGL.getVersion() << EOL
           << "Extensions: " << openGL.getExtensions() << EOL
           << EOL
           << "Rendering context:" << EOL
           << indent(2) << "red bits: " << redBits << EOL
           << indent(2) << "green bits: " << greenBits << EOL
           << indent(2) << "blue bits: " << blueBits << EOL
           << indent(2) << "alpha bits: " << alphaBits << EOL
           << indent(2) << "accumulator red bits: " << accumulatorRedBits << EOL
           << indent(2) << "accumulator green bits: " << accumulatorGreenBits << EOL
           << indent(2) << "accumulator blue bits: " << accumulatorBlueBits << EOL
           << indent(2) << "accumulator alpha bits: " << accumulatorAlphaBits << EOL
           << indent(2) << "depth bits: " << depthBits << EOL
           << indent(2) << "stencil bits: " << stencilBits << EOL
           << indent(2) << "aux buffers: " << auxBuffers << EOL
           << indent(2) << "overlay planes: " << numberOfOverlayPlanes << EOL
           << indent(2) << "underlay planes: " << numberOfUnderlayPlanes << EOL
           << indent(2) << "double buffered: " << isDoubleBuffered() << EOL
           << indent(2) << "stereoscopic: " << isStereoscopic() << EOL
           << indent(2) << "direct: " << isDirect() << EOL
           << indent(2) << "generic: " << isGeneric() << EOL
           << ENDL;
    }
    
    void dumpCommand(const Literal& description) noexcept
    {
      if (verbosity >= Verbosity::COMMANDS) {
        fout << "Command: " << description << ENDL;
      }
    }
     
    void onCommand(unsigned int identifier) noexcept
    {
      switch (identifier) {
      case Command::SELECT_VERBOSITY_NO_INFORMATION:
        dumpCommand(MESSAGE("Set verbosity level to NO_INFORMATION"));
        setVerbosity(Verbosity::NO_INFORMATION);
        break;
      case Command::SELECT_VERBOSITY_WARNINGS:
        dumpCommand(MESSAGE("Set verbosity level to WARNINGS"));
        setVerbosity(Verbosity::WARNINGS);
        break;
      case Command::SELECT_VERBOSITY_NORMAL:
        dumpCommand(MESSAGE("Set verbosity level to NORMAL"));
        setVerbosity(Verbosity::NORMAL);
        break;
      case Command::SELECT_VERBOSITY_COMMANDS:
        dumpCommand(MESSAGE("Set verbosity level to COMMANDS"));
        setVerbosity(Verbosity::COMMANDS);
        break;
      case Command::SELECT_VERBOSITY_ACTIVE_MOUSE_EVENTS:
        dumpCommand(MESSAGE("Set verbosity level to ACTIVE_MOUSE_EVENTS"));
        setVerbosity(Verbosity::ACTIVE_MOUSE_EVENTS);
        break;
      case Command::SELECT_VERBOSITY_ALL_MOUSE_EVENTS:
        dumpCommand(MESSAGE("Set verbosity level to ALL_MOUSE_EVENTS"));
        setVerbosity(Verbosity::ALL_MOUSE_EVENTS);
        break;
      case Command::SELECT_VERBOSITY_EVERYTHING:
        dumpCommand(MESSAGE("Set verbosity level to EVERYTHING"));
        setVerbosity(Verbosity::EVERYTHING);
        break;
      case Command::HELP:
        dumpCommand(MESSAGE("Help"));
        fout << "Help" << EOL << ENDL; // TAG: fixme
        break;
      case Command::SHOW_OPENGL_INFORMATION:
        dumpCommand(MESSAGE("Show OpenGL information"));
        dumpOpenGLInformation();
        break;
      case Command::ABOUT:
        dumpCommand(MESSAGE("About"));
        fout << base::Version().getBanner() << EOL << ENDL;
        break;
      case Command::QUIT:
        dumpCommand(MESSAGE("Quit"));
        if (onClose()) {
          exit();
        }
        break;
      case Command::CYCLE_SHADING_MODEL:
        {
          dumpCommand(MESSAGE("Cycle shading model"));
          unsigned int shadingModel = static_cast<unsigned int>(this->shadingModel) + 1;
          if (shadingModel > static_cast<unsigned int>(ShadingModel::LAST_MODEL)) {
            shadingModel = static_cast<unsigned int>(ShadingModel::FIRST_MODEL);
          }
          setShadingModel(static_cast<ShadingModel::Model>(shadingModel));
        }
        break;
      case Command::SELECT_SHADING_MODEL_FLAT:
        dumpCommand(MESSAGE("Select shading model: flat"));
        setShadingModel(ShadingModel::FLAT);
        break;
      case Command::SELECT_SHADING_MODEL_SMOOTH:
        dumpCommand(MESSAGE("Select shading model: smooth"));
        setShadingModel(ShadingModel::SMOOTH);
        break;
      case Command::CYCLE_POLYGON_MODE:
        {
          dumpCommand(MESSAGE("Cycle polygon mode"));
          unsigned int polygonMode = static_cast<unsigned int>(this->polygonMode) + 1;
          if (polygonMode > static_cast<unsigned int>(PolygonMode::LAST_MODE)) {
            polygonMode = static_cast<unsigned int>(PolygonMode::FIRST_MODE);
          }
          setPolygonMode(static_cast<PolygonMode::Mode>(polygonMode));
        }
        break;
      case Command::SELECT_POLYGON_MODE_FILL:
        dumpCommand(MESSAGE("Select polygon mode: fill"));
        setPolygonMode(PolygonMode::FILL);
        break;
      case Command::SELECT_POLYGON_MODE_LINE:
        dumpCommand(MESSAGE("Select polygon mode: line"));
        setPolygonMode(PolygonMode::LINE);
        break;
      case Command::SELECT_POLYGON_MODE_POINT:
        dumpCommand(MESSAGE("Select polygon mode: point"));
        setPolygonMode(PolygonMode::POINT);
        break;
      case Command::BLENDING_ENABLE:
        dumpCommand(MESSAGE("Enable blending"));
        setBlending(true);
        break;
      case Command::BLENDING_DISABLE:
        dumpCommand(MESSAGE("Disable blending"));
        setBlending(false);
        break;
      case Command::LIGHTING_ENABLE:
        dumpCommand(MESSAGE("Enable lighting"));
        setLighting(true);
        break;
      case Command::LIGHTING_DISABLE:
        dumpCommand(MESSAGE("Disable lighting"));
        setLighting(false);
        break;
      case Command::RESET_VIEW_PARAMETERS:
        dumpCommand(MESSAGE("Reset view parameters"));
        resetViewParameters();
        break;
      case Command::ROTATE_AROUND_X_AXIS_NEG:
        dumpCommand(MESSAGE("Rotate around X axis (neg)"));
        view.setRotation(view.getRotation() + Vector3D<double>(-1.0, 0, 0));
        break;
      case Command::ROTATE_AROUND_X_AXIS_POS:
        dumpCommand(MESSAGE("Rotate around X axis (pos)"));
        view.setRotation(view.getRotation() + Vector3D<double>(1.0, 0, 0));
        break;
      case Command::ROTATE_AROUND_Y_AXIS_NEG:
        dumpCommand(MESSAGE("Rotate around Y axis (neg)"));
        view.setRotation(view.getRotation() + Vector3D<double>(0, -1.0, 0));
        break;
      case Command::ROTATE_AROUND_Y_AXIS_POS:
        dumpCommand(MESSAGE("Rotate around Y axis (pos)"));
        view.setRotation(view.getRotation() + Vector3D<double>(0, 1.0, 0));
        break;
      case Command::ROTATE_AROUND_Z_AXIS_NEG:
        dumpCommand(MESSAGE("Rotate around Z axis (neg)"));
        view.setRotation(view.getRotation() + Vector3D<double>(0, 0, -1.0));
        break;
      case Command::ROTATE_AROUND_Z_AXIS_POS:
        dumpCommand(MESSAGE("Rotate around Z axis (pos)"));
        view.setRotation(view.getRotation() + Vector3D<double>(0, 0, 1.0));
        break;
      case Command::TRANSLATE_ALONG_X_NEG:
        dumpCommand(MESSAGE("Translate along X axis (neg)"));
        view.setTranslation(view.getTranslation() + Vector3D<double>(-0.1, 0, 0));
        break;
      case Command::TRANSLATE_ALONG_X_POS:
        dumpCommand(MESSAGE("Translate along X axis (pos)"));
        view.setTranslation(view.getTranslation() + Vector3D<double>(0.1, 0, 0));
        break;
      case Command::TRANSLATE_ALONG_Y_NEG:
        dumpCommand(MESSAGE("Translate along Y axis (neg)"));
        view.setTranslation(view.getTranslation() + Vector3D<double>(0, -0.1, 0));
        break;
      case Command::TRANSLATE_ALONG_Y_POS:
        dumpCommand(MESSAGE("Translate along Y axis (pos)"));
        view.setTranslation(view.getTranslation() + Vector3D<double>(0, 0.1, 0));
        break;
      case Command::TRANSLATE_ALONG_Z_NEG:
        dumpCommand(MESSAGE("Translate along Z axis (neg)"));
        view.setTranslation(view.getTranslation() + Vector3D<double>(0, 0, -0.1));
        break;
      case Command::TRANSLATE_ALONG_Z_POS:
        dumpCommand(MESSAGE("Translate along Z axis (pos)"));
        setTranslation(view.getTranslation() + Vector3D<double>(0, 0, 0.1));
        break;
      case Command::SELECT_MODE_SYSTEM:
        dumpCommand(MESSAGE("Select view mode: system"));
        setMode(MODE_SYSTEM);
        break;
      case Command::SELECT_MODE_FLOOR:
        dumpCommand(MESSAGE("Select view mode: floor"));
        setMode(MODE_SYSTEM);
        break;
      case Command::SELECT_MODE_CUBE:
        dumpCommand(MESSAGE("Select view mode: cube"));
        setMode(MODE_SYSTEM);
        break;
      case Command::SELECT_MODE_CONE:
        dumpCommand(MESSAGE("Select view mode: cone"));
        setMode(MODE_SYSTEM);
        break;
      case Command::SELECT_MODE_TORUS:
        dumpCommand(MESSAGE("Select view mode: torus"));
        setMode(MODE_TORUS);
        break;
      case Command::SELECT_QUALITY_WORST:
        dumpCommand(MESSAGE("Select quality: worst"));          
        setQuality(QUALITY_WORST);
        break;
      case Command::SELECT_QUALITY_NORMAL:
        dumpCommand(MESSAGE("Select quality: normal"));
        setQuality(QUALITY_NORMAL);
        break;
      case Command::SELECT_QUALITY_BEST:
        dumpCommand(MESSAGE("Select quality: best"));
        setQuality(QUALITY_BEST);
        break;
      default:
        if (verbosity >= Verbosity::WARNINGS) {
          fout << "Command '" << identifier << "' is not supported" << ENDL;
        }
      }
    }
  };
  
  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;

    MyOpenGLContext::Format desiredFormat;
    int formatId = -1;
    
    Array<MyOpenGLContext::Format> formats = MyOpenGLContext::getFormats(
      OpenGLContext::RGB |
      OpenGLContext::DOUBLE_BUFFERED |
      OpenGLContext::DEPTH |
      OpenGLContext::DIRECT
    );
    Array<MyOpenGLContext::Format>::ReadEnumerator enu = formats.getReadEnumerator();

    const bool dumpFormats = false;
    if (dumpFormats) {
      fout << "Available formats:" << ENDL;
    }
    bool gotFormat = false;
    for (unsigned int i = 0; enu.hasNext(); ++i) {
      gotFormat = true;
      const MyOpenGLContext::Format* format = enu.next();
      if (dumpFormats) {
        fout << indent(2) << "Format: " << i << EOL
             << indent(4) << "Color indexed: " << ((format->flags & MyOpenGLContext::COLOR_INDEXED) != 0) << EOL
             << indent(4) << "RGB: " << ((format->flags & MyOpenGLContext::RGB) != 0) << EOL
             << indent(4) << "Double buffered: " << ((format->flags & MyOpenGLContext::DOUBLE_BUFFERED) != 0) << EOL
             << indent(4) << "Stereoscopic: " << ((format->flags & MyOpenGLContext::STEREO) != 0) << EOL
             << indent(4) << "Generic: " << ((format->flags & MyOpenGLContext::GENERIC) != 0) << EOL
             << indent(4) << "Color bits: " << format->colorBits << EOL
             << indent(6) << "Red bits: " << format->redBits << EOL
             << indent(6) << "Green bits: " << format->greenBits << EOL
             << indent(6) << "Blue bits: " << format->blueBits << EOL
             << indent(6) << "Alpha bits: " << format->alphaBits << EOL
             << indent(4) << "Accumulator bits: " << format->accumulatorBits << EOL
             << indent(6) << "Accumulator red bits: " << format->accumulatorRedBits << EOL
             << indent(6) << "Accumulator green bits: " << format->accumulatorGreenBits << EOL
             << indent(6) << "Accumulator blue bits: " << format->accumulatorBlueBits << EOL
             << indent(6) << "Accumulator alpha bits: " << format->accumulatorAlphaBits << EOL
             << indent(4) << "DepthBits: " << format->depthBits << EOL
             << indent(4) << "StencilBits: " << format->stencilBits << EOL
             << indent(4) << "AuxBuffers: " << format->auxBuffers << EOL
             << ENDL;
      }
      if ((formatId == -1) || (format->alphaBits && (!desiredFormat.alphaBits))) {
        formatId = i;
        desiredFormat = *format;
      }
    }
    if (!gotFormat && dumpFormats) {
      fout << indent(2) << "No formats found." << ENDL;
    }

    if (formatId == -1) {
      ferr << "Format not available." << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    MyOpenGLContext myOpenGLContext(
      MESSAGE("OpenGL rules"),
      Position(128, 128),
      Dimension(512, 512),
      desiredFormat
    );

    fout << "OpenGL context information: " << EOL
         << indent(2) << "Client vendor: " << myOpenGLContext.getGLClientVendor() << EOL
         << indent(2) << "Client release: " << myOpenGLContext.getGLClientRelease() << EOL
         << indent(2) << "Client extensions: " << myOpenGLContext.getGLClientExtensions() << EOL
         << indent(2) << "Server vendor: " << myOpenGLContext.getGLServerVendor() << EOL
         << indent(2) << "Server release: " << myOpenGLContext.getGLServerRelease() << EOL
         << indent(2) << "Server extensions: " << myOpenGLContext.getGLServerExtensions() << EOL
         << indent(2) << "Direct context: " << myOpenGLContext.isDirect() << EOL
         << ENDL;

    myOpenGLContext.show();
    myOpenGLContext.raise();
    myOpenGLContext.dispatch();
  }
};

APPLICATION_STUB(OpenGLApplication);
