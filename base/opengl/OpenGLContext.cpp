/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/opengl/OpenGLContext.h>
#include <base/Application.h>
#include <base/dl/DynamicLinker.h>
#include <base/NotImplemented.h>

// TAG: need any cast pointer
//   template<class TYPE, class RESULT>
//   RESULT aspointer(TYPE value) throw() {
//     return x(value); // check if size of type == sizeof(void*)
//   }

// TAG: minimized title

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#  undef DELETE // yikes
#endif // flavor

#if 1
#  define CALL_PASCAL __attribute__((stdcall))
#  define CALL_CPP __attribute__((cdecl))
#endif

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  define CALL_UI CALL_PASCAL
#else
#  define CALL_UI
#endif

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  define CALL_OPENGL CALL_PASCAL
#else
#  define CALL_OPENGL
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace OpenGLContextImpl {

  DynamicLinker* dynamicLinker = 0;

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  
  HGLRC (CALL_OPENGL *wglCreateContext)(HDC hdc);
  HGLRC (CALL_OPENGL *wglCreateLayerContext)(HDC hdc, int plane);
  BOOL (CALL_OPENGL *wglDeleteContext)(HGLRC hglrc);
  HGLRC (CALL_OPENGL *wglGetCurrentContext)();
  HDC (CALL_OPENGL *wglGetCurrentDC)();
  PROC (CALL_OPENGL *wglGetProcAddress)(LPCSTR lpszProc);
  BOOL (CALL_OPENGL *wglMakeCurrent)(HDC hdc, HGLRC hglrc);
  BOOL (CALL_OPENGL *wglSwapLayerBuffers)(HDC hdc, UINT planes);
  BOOL (CALL_OPENGL *wglDescribeLayerPlane)(HDC hdc, int pixelFormat, int layerPlane, UINT bytes, LPLAYERPLANEDESCRIPTOR lpd);
  
  typedef void (CALL_OPENGL *OpenGLFunction)();
  
  struct Descriptor {
    const StringLiteral symbol;
    OpenGLFunction* function;
  };
  
  static Descriptor OPEN_GL_FUNCTIONS[] = {
    {MESSAGE("wglCreateContext"), Cast::pointer<OpenGLFunction*>((void*)&wglCreateContext)},
    {MESSAGE("wglCreateLayerContext"), Cast::pointer<OpenGLFunction*>((void*)&wglCreateLayerContext)},
    {MESSAGE("wglDeleteContext"), Cast::pointer<OpenGLFunction*>((void*)&wglDeleteContext)},
    {MESSAGE("wglGetCurrentContext"), Cast::pointer<OpenGLFunction*>((void*)&wglGetCurrentContext)},
    {MESSAGE("wglGetCurrentDC"), Cast::pointer<OpenGLFunction*>((void*)&wglGetCurrentDC)},
    {MESSAGE("wglGetProcAddress"), Cast::pointer<OpenGLFunction*>((void*)&wglGetProcAddress)},
    {MESSAGE("wglMakeCurrent"), Cast::pointer<OpenGLFunction*>((void*)&wglMakeCurrent)},
    {MESSAGE("wglSwapLayerBuffers"), Cast::pointer<OpenGLFunction*>((void*)&wglSwapLayerBuffers)},
    {MESSAGE("wglDescribeLayerPlane"), Cast::pointer<OpenGLFunction*>((void*)&wglDescribeLayerPlane)}
  };

  struct Context {
    HWND window;
    HDC deviceContext;
    HGLRC renderingContext;
  };
  
  struct Entry {
    Entry* next;
    OpenGLContext* openGLContext;
    void* handle;
  };
  
  // hash table with linked lists
  Entry* windows[256]; // initialized with zeros initially
  
  static OpenGLContext* getOpenGLContext(void* handle) throw() {
    // TAG: fix hash - handle % sizeof(void) == 0 ?
    unsigned int hash = Cast::getOffset(handle) % getArraySize(OpenGLContextImpl::windows);
    Entry* entry = OpenGLContextImpl::windows[hash];
    while (entry) {
      if (entry->handle == handle) {
        return entry->openGLContext;
      }
      entry = entry->next;
    }
    return 0;
  }

  static void addOpenGLContext(OpenGLContext* openGLContext, void* handle) throw() {
    Entry* temp = new Entry;
    temp->next = 0;
    temp->openGLContext = openGLContext;
    temp->handle = handle;
    
    unsigned int hash = Cast::getOffset(handle) % getArraySize(OpenGLContextImpl::windows);
    Entry* entry = OpenGLContextImpl::windows[hash];
    if (entry == 0) {
      OpenGLContextImpl::windows[hash] = temp;
    } else {
      while (entry->next) {
        entry = entry->next;
      }
      entry->next = temp;
    }
  }

  static void removeOpenGLContext(void* handle) throw() {
    unsigned int hash = Cast::getOffset(handle) % getArraySize(OpenGLContextImpl::windows);
    Entry* entry = OpenGLContextImpl::windows[hash];
    ASSERT((entry->openGLContext != 0) && (entry->handle != 0));
    Entry* previous = 0;
    while (entry->handle != handle) {
      previous = entry;
      entry = entry->next;
    }
    ASSERT(entry->handle == handle);
    if (previous) {
      previous->next = entry->next;
    } else {
      OpenGLContextImpl::windows[hash] = entry->next;
    }
    delete entry;
  }
  
  class Delegate {
  public:

    static LONG CALL_UI messageHandler(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) throw() {
      OpenGLContext* window = getOpenGLContext(handle); // should be atomic
      if (window == 0) {
        return ::DefWindowProc(handle, message, wParam, lParam);
      }

//       // TAG: is this required
//       if (message == WM_CREATE) { // special 'cause context not initialized yet
//         return 0;
//       }
      
      OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(window->handle);
      
      switch (message) {
      case WM_QUIT:
        return 0;
      case WM_CLOSE:
        if (window->onClose()) {
          ::DestroyWindow(context->window);
        }
        return 0;
      case WM_DESTROY:
        OpenGLContextImpl::wglMakeCurrent(0, 0); // make not current
        if (context->renderingContext) {
          OpenGLContextImpl::wglDeleteContext(context->renderingContext);
          context->renderingContext = 0;
        }
        if (context->deviceContext) {
          ::DeleteDC(context->deviceContext);
          context->deviceContext = 0;
        }
        ::PostQuitMessage(0);
        return 0;
//       case WM_CAPTURECHANGED:
//         return 0;
      case WM_MOVE:
        {
          Position position((int16)LOWORD(lParam), (int16)HIWORD(lParam));
          window->setCurrentPosition(position);
          window->onMove(position);
          return 0;
        }
      case WM_SIZE:
        {
          Dimension dimension(LOWORD(lParam), HIWORD(lParam));
          window->setCurrentDimension(dimension);
          window->onResize(dimension);
          return 0;
        }
      case WM_PAINT:
        {          
          // RECT rect;
          // ::GetUpdateRect(context->window, &rect, FALSE);
          
          PAINTSTRUCT ps;
          HDC hdc = ::BeginPaint(context->window, &ps);
          // ps.rcPaint;
          window->onDisplay();
          ::EndPaint(context->window, &ps);
          // ::ValidateRect(context->window, 0); // not required with BeginPaint
          return 0;
        }
      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
      case WM_LBUTTONDBLCLK:
      case WM_MBUTTONDOWN:
      case WM_MBUTTONUP:
      case WM_MBUTTONDBLCLK:
      case WM_RBUTTONDOWN:
      case WM_RBUTTONUP:
      case WM_RBUTTONDBLCLK:
        {
          Position position(LOWORD(lParam), HIWORD(lParam));
          unsigned int buttons = 0;
          if (wParam & MK_CONTROL) {
            buttons |= OpenGLContext::Key::CONTROL;
          }
          if (wParam & MK_LBUTTON) {
            buttons |= OpenGLContext::Mouse::LEFT;
          }
          if (wParam & MK_MBUTTON) {
            buttons |= OpenGLContext::Mouse::MIDDLE;
          }
          if (wParam & MK_RBUTTON) {
            buttons |= OpenGLContext::Mouse::RIGHT;
          }
          if (wParam & MK_SHIFT) {
            buttons |= OpenGLContext::Key::SHIFT;
          }
          switch (message) {
          case WM_LBUTTONDOWN:
            window->onMouseButton(position, OpenGLContext::Mouse::LEFT, OpenGLContext::Mouse::PRESSED);
            return 0;
          case WM_LBUTTONUP:
            window->onMouseButton(position, OpenGLContext::Mouse::LEFT, OpenGLContext::Mouse::RELEASED);
            return 0;
          case WM_LBUTTONDBLCLK:
            window->onMouseButton(position, OpenGLContext::Mouse::LEFT, OpenGLContext::Mouse::DOUBLE_CLICKED);
            return 0;
          case WM_MBUTTONDOWN:
            window->onMouseButton(position, OpenGLContext::Mouse::MIDDLE, OpenGLContext::Mouse::PRESSED);
            return 0;
          case WM_MBUTTONUP:
            window->onMouseButton(position, OpenGLContext::Mouse::MIDDLE, OpenGLContext::Mouse::RELEASED);
            return 0;
          case WM_MBUTTONDBLCLK:
            window->onMouseButton(position, OpenGLContext::Mouse::MIDDLE, OpenGLContext::Mouse::DOUBLE_CLICKED);
            return 0;
          case WM_RBUTTONDOWN:
            window->onMouseButton(position, OpenGLContext::Mouse::RIGHT, OpenGLContext::Mouse::PRESSED);
            return 0;
          case WM_RBUTTONUP:
            window->onMouseButton(position, OpenGLContext::Mouse::RIGHT, OpenGLContext::Mouse::RELEASED);
            return 0;
          case WM_RBUTTONDBLCLK:
            window->onMouseButton(position, OpenGLContext::Mouse::RIGHT, OpenGLContext::Mouse::DOUBLE_CLICKED);
            return 0;
          }
        }
      case WM_MOUSEWHEEL:
        {
          unsigned int keys = LOWORD(wParam); // GET_KEYSTATE_WPARAM
          int delta = (int16)HIWORD(wParam);
          unsigned int buttons = 0;
          if (keys & MK_CONTROL) {
            buttons |= OpenGLContext::Key::CONTROL;
          }
          if (keys & MK_LBUTTON) {
            buttons |= OpenGLContext::Mouse::LEFT;
          }
          if (keys & MK_MBUTTON) {
            buttons |= OpenGLContext::Mouse::MIDDLE;
          }
          if (keys & MK_RBUTTON) {
            buttons |= OpenGLContext::Mouse::RIGHT;
          }
          if (keys & MK_SHIFT) {
            buttons |= OpenGLContext::Key::SHIFT;
          }
          window->onMouseWheel(Position((int16)LOWORD(lParam), (int16)HIWORD(lParam)), delta, buttons);
          return 0;
        }
      case WM_MOUSELEAVE:
        window->scope = false;
        window->onMouseScope(false);
        return 0;
      case WM_MOUSEMOVE:
        {
          if (!window->scope) {
            window->scope = true;
            TRACKMOUSEEVENT mouseEvent;
            mouseEvent.cbSize = sizeof(mouseEvent);
            mouseEvent.dwFlags = TME_LEAVE;
            mouseEvent.hwndTrack = context->window;
            mouseEvent.dwHoverTime = 0;
            assert(::TrackMouseEvent(&mouseEvent), OpenGLException(Type::getType<OpenGLContext>()));
            window->onMouseScope(true);
          }
          unsigned int buttons = 0;
          if (wParam & MK_CONTROL) {
            buttons |= OpenGLContext::Key::CONTROL;
          }
          if (wParam & MK_LBUTTON) {
            buttons |= OpenGLContext::Mouse::LEFT;
          }
          if (wParam & MK_MBUTTON) {
            buttons |= OpenGLContext::Mouse::MIDDLE;
          }
          if (wParam & MK_RBUTTON) {
          buttons |= OpenGLContext::Mouse::RIGHT;
          }
          if (wParam & MK_SHIFT) {
          buttons |= OpenGLContext::Key::SHIFT;
          }
          window->onMouseMove(Position((int16)LOWORD(lParam), (int16)HIWORD(lParam)), buttons);
          return 0;
        }
      case WM_ACTIVATE:
        switch (LOWORD(wParam)) {
        case WA_ACTIVE:
        case WA_CLICKACTIVE:
          if (!window->active) {
            window->active = true;
            window->onFocus(true);
          }
          break;
        case WA_INACTIVE:
        default:
          if (window->active) {
            window->active = false;
            window->onFocus(false);
          }
        }
        return 0;
        // TAG: window->onVisibility();
      case WM_KEYDOWN:
      case WM_KEYUP:
      case WM_SYSKEYDOWN:
      case WM_SYSKEYUP:
        {
          unsigned int flags = 0;
          if ((message == WM_KEYDOWN) || (message == WM_SYSKEYDOWN)) {
            flags |= OpenGLContext::Key::PRESSED;
          }
          if (((lParam >> 30) & 1) == 0) {
            flags |= OpenGLContext::Key::FIRST_TIME;
          }
          if (!(flags & OpenGLContext::Key::FIRST_TIME) && !window->autorepeat) {
            return 0; // ignore repeats
          }
          if (((lParam >> 24) & 1) != 0) {
            flags |= OpenGLContext::Key::EXTENDED;
          }
          
          ::GetKeyboardState(Cast::pointer<BYTE*>(window->keyboardState));
          unsigned int modifiers = 0;
          modifiers |= (window->keyboardState[VK_LSHIFT] & 0x80) ? OpenGLContext::Key::LEFT_SHIFT : 0;
          modifiers |= (window->keyboardState[VK_RSHIFT] & 0x80) ? OpenGLContext::Key::RIGHT_SHIFT : 0;
          modifiers |= (window->keyboardState[VK_LCONTROL] & 0x80) ? OpenGLContext::Key::LEFT_CONTROL : 0;
          modifiers |= (window->keyboardState[VK_RCONTROL] & 0x80) ? OpenGLContext::Key::RIGHT_CONTROL : 0;
          modifiers |= (window->keyboardState[VK_LMENU] & 0x80) ? OpenGLContext::Key::LEFT_ALT : 0;
          modifiers |= (window->keyboardState[VK_RMENU] & 0x80) ? OpenGLContext::Key::RIGHT_ALT : 0;
          modifiers |= (window->keyboardState[VK_CAPITAL] & 0x01) ? OpenGLContext::Key::CAPS_LOCK_TOGGLED : 0;
          modifiers |= (window->keyboardState[VK_SCROLL] & 0x01) ? OpenGLContext::Key::SCROLL_LOCK_TOGGLED : 0;
          modifiers |= (window->keyboardState[VK_NUMLOCK] & 0x01) ? OpenGLContext::Key::NUM_LOCK_TOGGLED : 0;
          window->modifiers = modifiers;
          
          unsigned int code = 0;
          switch (wParam) {
          case VK_RETURN:
            code = OpenGLContext::Key::RETURN;
            flags |= OpenGLContext::Key::ASCII;
            break;
          case VK_INSERT:
            code = OpenGLContext::Key::INSERT;
            break;
          case VK_DELETE:
            code = OpenGLContext::Key::DELETE;
            break;
          case VK_HOME:
            code = OpenGLContext::Key::HOME;
            break;
          case VK_END:
            code = OpenGLContext::Key::END;
            break;
          case VK_PRIOR:
            code = OpenGLContext::Key::PRIOR;
            break;
          case VK_NEXT:
            code = OpenGLContext::Key::NEXT;
            break;
          case VK_LEFT:
            code = OpenGLContext::Key::LEFT;
            break;
          case VK_RIGHT:
            code = OpenGLContext::Key::RIGHT;
            break;
          case VK_UP:
            code = OpenGLContext::Key::UP;
            break;
          case VK_DOWN:
            code = OpenGLContext::Key::DOWN;
            break;
          case VK_SELECT:
            code = OpenGLContext::Key::SELECT;
            break;
          case VK_PRINT:
            code = OpenGLContext::Key::PRINT;
            break;
          case VK_EXECUTE:
            code = OpenGLContext::Key::EXECUTE;
            break;
          case VK_SNAPSHOT:
            code = OpenGLContext::Key::SNAPSHOT;
            break;
          case VK_HELP:
            code = OpenGLContext::Key::HELP;
            break;            
          case VK_CAPITAL:
            code = OpenGLContext::Key::CAPS_LOCK;
            break;
          case VK_SCROLL:
            code = OpenGLContext::Key::SCROLL_LOCK;
            break;
          case VK_NUMLOCK:
            code = OpenGLContext::Key::NUM_LOCK;
            break;
          case VK_PAUSE:
            code = OpenGLContext::Key::PAUSE;
            break;
          case VK_LWIN:
            code = OpenGLContext::Key::LEFT_MANAGER;
            break;
          case VK_RWIN:
            code = OpenGLContext::Key::RIGHT_MANAGER;
            break;
          case VK_APPS:
            code = OpenGLContext::Key::MENU;
            break;
          case VK_F1:
            code = OpenGLContext::Key::F1;
            break;
          case VK_F2:
            code = OpenGLContext::Key::F2;
            break;
          case VK_F3:
            code = OpenGLContext::Key::F3;
            break;
          case VK_F4:
            code = OpenGLContext::Key::F4;
            break;
          case VK_F5:
            code = OpenGLContext::Key::F5;
            break;
          case VK_F6:
            code = OpenGLContext::Key::F6;
            break;
          case VK_F7:
            code = OpenGLContext::Key::F7;
            break;
          case VK_F8:
            code = OpenGLContext::Key::F8;
            break;
          case VK_F9:
            code = OpenGLContext::Key::F9;
            break;
          case VK_F10:
            code = OpenGLContext::Key::F10;
            break;
          case VK_F11:
            code = OpenGLContext::Key::F11;
            break;
          case VK_F12:
            code = OpenGLContext::Key::F12;
            break;
          case VK_F13:
            code = OpenGLContext::Key::F13;
            break;
          case VK_F14:
            code = OpenGLContext::Key::F14;
            break;
          case VK_F15:
            code = OpenGLContext::Key::F15;
            break;
          case VK_F16:
            code = OpenGLContext::Key::F16;
            break;
          case VK_F17:
            code = OpenGLContext::Key::F17;
            break;
          case VK_F18:
            code = OpenGLContext::Key::F18;
            break;
          case VK_F19:
            code = OpenGLContext::Key::F19;
            break;
          case VK_F20:
            code = OpenGLContext::Key::F20;
            break;
          case VK_F21:
            code = OpenGLContext::Key::F21;
            break;
          case VK_F22:
            code = OpenGLContext::Key::F22;
            break;
          case VK_F23:
            code = OpenGLContext::Key::F23;
            break;
          case VK_F24:
            code = OpenGLContext::Key::F24;
            break;
#if 0
          case VK_BROWSER_BACK:
          case VK_BROWSER_FORWARD:
          case VK_BROWSER_REFRESH:
          case VK_BROWSER_STOP:
          case VK_BROWSER_SEARCH:
          case VK_BROWSER_FAVORITES:
          case VK_BROWSER_HOME:
          case VK_VOLUME_MUTE:
          case VK_VOLUME_DOWN:
          case VK_VOLUME_UP:
          case VK_MEDIA_NEXT_TRACK:
          case VK_MEDIA_PREV_TRACK:
          case VK_MEDIA_STOP:
          case VK_MEDIA_PLAY_PAUSE:
          case VK_LAUNCH_MAIL:
          case VK_LAUNCH_MEDIA_SELECT:
          case VK_LAUNCH_APP1:
          case VK_LAUNCH_APP2:
#endif
          }
          if (code) {
            window->onKey(code, flags, modifiers);
          } else {
            WORD buffer[2];
            // ::ToUnicode(wParam, 0, Cast::pointer<BYTE*>(window->keyboardState), (wchar*)&buffer, sizeof(buffer), 0);
            switch (::ToAscii(wParam, 0, Cast::pointer<BYTE*>(window->keyboardState), buffer, 0)) {
            case 2:
              window->onKey((uint8)buffer[0], flags|OpenGLContext::Key::DEAD|OpenGLContext::Key::ASCII, modifiers);
              window->onKey((uint8)buffer[1], flags|OpenGLContext::Key::ASCII, modifiers);
              break;
            case 1:
              window->onKey((uint8)buffer[0], flags|OpenGLContext::Key::ASCII, modifiers);
              break;
            case 0:
              break; // ignore key
            }
          }
          return  0;
        }
      case WM_GETMINMAXINFO:
        if (window->minimumSize.getSize() || window->maximumSize.getSize()) {
          MINMAXINFO* temp = (MINMAXINFO*)lParam;
          if (window->minimumSize.getSize()) {
            temp->ptMinTrackSize.x = window->minimumSize.getWidth();
            temp->ptMinTrackSize.y = window->minimumSize.getHeight();
          }
          if (window->maximumSize.getSize()) {
            temp->ptMaxTrackSize.x = window->maximumSize.getWidth();
            temp->ptMaxTrackSize.y = window->maximumSize.getHeight();
          }
          return 0;
        }
        break;
//       case WM_SETCURSOR:
//         if (LOWORD(lParam) == HTCLIENT) {
//           fout << MESSAGE("WM_SETCURSOR") << ENDL;
//           window->setCursor(window->cursor);
//           return 0;
//         }
//         break;
      case WM_SETFOCUS:
        fout << MESSAGE("WM_SETFOCUS") << ENDL;
        return 0;
      case WM_KILLFOCUS:
        fout << MESSAGE("WM_KILLFOCUS") << ENDL;
        return 0;
      case WM_TIMER:
        return 0;
      case WM_COMMAND:
        if (HIWORD(wParam) == 0) { // from menu
          window->onMenu(LOWORD(wParam));
        } else {
          window->onCommand(LOWORD(wParam));
        }
        return 0;
      case WM_SYSCOMMAND:
        switch (wParam & 0xfff0) {
//         case SC_SIZE: // prevent
//           break;
        case SC_SCREENSAVE: // prevent
        case SC_MONITORPOWER: // prevent
          return 0;
        }
        break;
      case WM_DISPLAYCHANGE:
        fout << MESSAGE("MESSAGE: DISPLAY CHANGE") << ENDL;
        return 0;
//       case WM_ENTERMENULOOP:
//         return 0;
//       case WM_EXITMENULOOP:
//         return 0;
      }
      return ::DefWindowProc(context->window, message, wParam, lParam);
    }
  };

  ATOM windowClass = 0;
#endif // win32
  
  /*
    GCC: referencing a member of a forward declared class results in "does not have member" but should have been something like "class not defined"
  */
  inline void* createContext(const String& title, const Position& position, const Dimension& dimension, unsigned int flags) throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
    static SpinLock spinLock;

    // TAG: need new operator that does not throw an exception
    OpenGLContextImpl::Context* context = new OpenGLContextImpl::Context;
    // TAG: raise OpenGLException("Unable to create OpenGL context", Type::getType<OpenGLException>())
    context->window = 0;
    context->deviceContext = 0;
    context->renderingContext = 0;
    
    spinLock.exclusiveLock();
    if (!OpenGLContextImpl::dynamicLinker) {
      // never release (should support explicit release)
      OpenGLContextImpl::dynamicLinker = new DynamicLinker(MESSAGE("opengl32.dll"));
      assert(OpenGLContextImpl::dynamicLinker, OpenGLException("Unable to load OpenGL module", Type::getType<OpenGLException>()));
      for (unsigned int i = 0; i < getArraySize(OpenGLContextImpl::OPEN_GL_FUNCTIONS); ++i) {
        *OpenGLContextImpl::OPEN_GL_FUNCTIONS[i].function =
          (OpenGLContextImpl::OpenGLFunction)OpenGLContextImpl::dynamicLinker->getSymbol(
            OpenGLContextImpl::OPEN_GL_FUNCTIONS[i].symbol
          ); // TAG: fix cast
      }
    }

    // only register class if not already registered
    if (OpenGLContext::numberOfContexts++ == 0) { // TAG: must be atomic
      WNDCLASSEX temp = {
        sizeof(temp),
        CS_OWNDC, // style // TAG: other flags: CS_DBLCLKS, CS_NOCLOSE, CS_HREDRAW, CS_VREDRAW
        Delegate::messageHandler, // message handler
        0, // extra bytes to allocate
        0, // extra window bytes
        (HINSTANCE)::GetModuleHandle(0), // instance
        0, // icon
        (HCURSOR)::LoadImage(0, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED), // cursor
        0, // background
        0, // menu
        "mip.sdu.dk/~fonseca/base/opengl/OpenGLContext", // class name
        0 // small icon
      };
      OpenGLContextImpl::windowClass = ::RegisterClassEx(&temp); // zero if fails
    }
    spinLock.releaseLock();

    RECT rect;
    rect.left = position.getX();
    rect.right = position.getX() + dimension.getWidth();
    rect.top = position.getY();
    rect.bottom = position.getY() + dimension.getHeight();
    DWORD extendedStyle = 0;
    DWORD style = 0;
    
    if (false) { // displayMode == MODE_FULL_SCREEN) {
      extendedStyle = WS_EX_APPWINDOW;
      style = WS_POPUP;
      // TAG: not here ::ShowCursor(FALSE);
    } else {
      extendedStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
      style = WS_OVERLAPPEDWINDOW;
    }
    ::AdjustWindowRectEx(&rect, style, FALSE, extendedStyle);
    
    context->window = ::CreateWindowEx(
      extendedStyle, // extended style
      (const char*)(MemorySize)OpenGLContextImpl::windowClass, // class name
      title.getElements(), // window title
      style | // window style
      WS_CLIPCHILDREN | WS_CLIPSIBLINGS, // required for OpenGL
      0, // x position,
      0, // y position,
      rect.right - rect.left,
      rect.bottom - rect.top,
      0, // no parent window
      0, // use the window class menu
      (HINSTANCE)::GetModuleHandle(0), // the instance that owns the window (ignored on NT)
      0 // application window data structure
    );
    assert(context->window, OpenGLException("Unable to create window", Type::getType<OpenGLException>()));
    
    PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
      1, // version number
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL, // support window and OpenGL (rest set below)
      0, // type set below
      24, // 24-bit color depth
      0, 0, 0, 0, 0, 0, // color bits ignored
      0, // no alpha buffer
      0, // shift bit ignored
      0, // accumulation buffer
      0, 0, 0, 0, // accum bits ignored
      0, // z-buffer
      0, // stencil buffer
      0, // auxiliary buffer
      PFD_MAIN_PLANE, // main layer (ignored in new implementations)
      0, // reserved
      0, 0, 0 // layer masks ignored
    };
    
    // pfd.dwFlags |= PFD_DRAW_TO_BITMAP
    // pfd.dwFlags |= PFD_SWAP_LAYER_BUFFERS
    if (flags & OpenGLContext::DOUBLE_BUFFERED) {
      pfd.dwFlags |= PFD_DOUBLEBUFFER;
    }
    if (flags & OpenGLContext::STEREO) {
      pfd.dwFlags |= PFD_STEREO;
    }
    if (flags & OpenGLContext::ACCUMULATOR) {
      // TAG: fixme
    }
    if (flags & OpenGLContext::ALPHA) {
      // TAG: fixme
    }
    if (flags & OpenGLContext::DEPTH) {
      // TAG: fixme
      pfd.cDepthBits = 32;
    }
    if (flags & OpenGLContext::STENCIL) {
      // TAG: fixme
      pfd.cStencilBits = 32;
    }
    if (flags & OpenGLContext::MULTI_SAMPLE) {
      // TAG: fixme
    }
    if (flags & OpenGLContext::LUMINANCE) {
      // TAG: fixme
    }
    pfd.iPixelType = (flags & OpenGLContext::COLOR_INDEX) ? PFD_TYPE_COLORINDEX : PFD_TYPE_RGBA;  
    
    if (!(context->deviceContext = ::GetDC(context->window))) {
      ::DestroyWindow(context->window);
      throw OpenGLException("Unable to connect to device context", Type::getType<OpenGLException>());
    }

    int index;
    if (!(index = ::ChoosePixelFormat(context->deviceContext, &pfd))) {
      throw OpenGLException("Format not supported", Type::getType<OpenGLException>());
    }
    if (!::SetPixelFormat(context->deviceContext, index, &pfd)) {
      throw OpenGLException("Unable to set format", Type::getType<OpenGLException>());
    }
    if (!(context->renderingContext = OpenGLContextImpl::wglCreateContext(context->deviceContext))) {
      ::DestroyWindow(context->window);
      throw OpenGLException("Unable to create rendering context", Type::getType<OpenGLException>());
    }
    if (!OpenGLContextImpl::wglMakeCurrent(context->deviceContext, context->renderingContext)) {
      ::DestroyWindow(context->window);
      throw OpenGLException("Invalid rendering context", Type::getType<OpenGLException>());
    }
    return context;
#else // unix
    throw NotImplemented(Type::getType<OpenGLContext>());
#endif // flavor
  }
};

// overflow is not possible due to limited resources
unsigned int OpenGLContext::numberOfContexts = 0;

OpenGLContext::OpenGLContext(const String& title, const Position& position, const Dimension& dimension, unsigned int flags) throw(OpenGLException)
  : handle(OpenGLContextImpl::createContext(title, position, dimension, flags)), openGL() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  OpenGLContextImpl::addOpenGLContext(this, context->window);

  // make window visible and update client area
  ::ShowWindow(context->window, SW_SHOWNORMAL); // TAG: or use SW_SHOW
  ::SetForegroundWindow(context->window);
	::SetFocus(context->window);
  ::InvalidateRect(context->window, 0, FALSE);
  ::UpdateWindow(context->window); // send WM_PAINT message

  autorepeat = true;
  scope = false;
  active = true;
  cursor = OpenGLContext::ARROW; // keep in sync with window class above
  minimumSize = Dimension(0, 0);
  maximumSize = Dimension(0, 0);
  
  TRACKMOUSEEVENT mouseEvent;
  mouseEvent.cbSize = sizeof(mouseEvent);
  mouseEvent.dwFlags = TME_LEAVE;
  mouseEvent.hwndTrack = context->window;
  mouseEvent.dwHoverTime = 0;
  assert(::TrackMouseEvent(&mouseEvent), OpenGLException(this));
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::displayMenu(const Position& position, const Menu& menu) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  ::SetForegroundWindow(context->window);
  POINT point;
  point.x = position.getX();
  point.y = position.getY();
  ::ClientToScreen(context->window, &point);
  // returns with error is menu already active
  // assert(
  ::SetLastError(1234);
    ::TrackPopupMenuEx(
      (HMENU)menu.getHandle(),
      TPM_LEFTALIGN|TPM_TOPALIGN|TPM_LEFTBUTTON,
      point.x,
      point.y,
      context->window,
      0
    );
  fout << "GetLastError: " << ::GetLastError() << ENDL;
        //    UserInterfaceException(this)
        //  );
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

unsigned int OpenGLContext::getNumberOfOverlayPlanes() const throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  PIXELFORMATDESCRIPTOR pfd;
  if (context->deviceContext) {
    int index = ::GetPixelFormat(context->deviceContext);
    if (index > 0) {
      if (::DescribePixelFormat(context->deviceContext, index, sizeof(PIXELFORMATDESCRIPTOR), &pfd)) {
        return (pfd.bReserved >> 0) & 0xf;
      }
    }
  }
  throw OpenGLException(this);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

unsigned int OpenGLContext::getNumberOfUnderlayPlanes() const throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  PIXELFORMATDESCRIPTOR pfd;
  if (context->deviceContext) {
    int index = ::GetPixelFormat(context->deviceContext);
    if (index > 0) {
      if (::DescribePixelFormat(context->deviceContext, index, sizeof(PIXELFORMATDESCRIPTOR), &pfd)) {
        return (pfd.bReserved >> 4) & 0xf;
      }
    }
  }
  throw OpenGLException(this);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::exit() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  // OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  ::PostQuitMessage(0);
#else // unix
  throw NotImplemented(Type::getType<OpenGLContext>());
#endif // flavor
}

void OpenGLContext::dispatch() throw(OpenGLException /*, SingletonException*/) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  // static SpinLock spinLock;
  // assert(spinLock.tryExclusiveLock(), SingletonException(Type::getType<OpenGLContext>()));
  MSG msg;
  while (true) {
    if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        return;
      } else {
        // if (!::TranslateAccelerator(windowHandle, table, &msg)) {
          ::TranslateMessage(&msg);
          ::DispatchMessage(&msg);
        // }
      }
    }	else {
      if (active) {
        onIdle();
      }
    }
  }
  // spinLock.releaseLock();
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

// PixelFormat OpenGLContext::getPixelFormat() throw() {
//   int index = ::GetPixelFormat(context->deviceContext);
//   PIXELFORMATDESCRIPTOR pfd;
//   ::DescribePixelFormat(context->deviceContext, index, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
//   PixelFormat pixelFormat;
//   pixelFormat.redBits = pfd.cRedBits;
//   return pixelFormat;
// }

void OpenGLContext::makeCurrent() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  OpenGLContextImpl::wglMakeCurrent(context->deviceContext, context->renderingContext);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::deselect() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::wglMakeCurrent(0, 0);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::swap() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  if (!::SwapBuffers(context->deviceContext)) {
    fout << "Unable to swap buffers: " << ::GetLastError() << " " << (void*)context->deviceContext << " " << (void*)context->window << ENDL;
  }
//   BUG: ERROR_PROC_NOT_FOUND or ERROR_INVALID_WINDOW_HANDLE occur approx. 5% of the times
//   assert(
//     ::SwapBuffers(context->deviceContext),
//     OpenGLException("Unable to swap buffer", this)
//   );
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

#if 0
void OpenGLContext::swap(int plane) throw(OutOfRange, OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  unsigned int numberOfUnderlays = -15; // TAG: must be attribute
  unsigned int numberOfOverlays = 15; // TAG: must be attribute
  assert(
    (plane >= -numberOfUnderlays) && (plane <= numberOfOverlays),
    OutOfRange(this)
  );
  static const UINT PLANES[15 - (-15) + 1] = {
    WGL_SWAP_UNDERLAY15,
    WGL_SWAP_UNDERLAY14,
    WGL_SWAP_UNDERLAY13,
    WGL_SWAP_UNDERLAY12,
    WGL_SWAP_UNDERLAY11,
    WGL_SWAP_UNDERLAY10,
    WGL_SWAP_UNDERLAY9,
    WGL_SWAP_UNDERLAY8,
    WGL_SWAP_UNDERLAY7,
    WGL_SWAP_UNDERLAY6,
    WGL_SWAP_UNDERLAY5,
    WGL_SWAP_UNDERLAY4,
    WGL_SWAP_UNDERLAY3,
    WGL_SWAP_UNDERLAY2,
    WGL_SWAP_UNDERLAY1,
    WGL_SWAP_MAIN_PLANE,
    WGL_SWAP_OVERLAY1,
    WGL_SWAP_OVERLAY2,
    WGL_SWAP_OVERLAY3,
    WGL_SWAP_OVERLAY4,
    WGL_SWAP_OVERLAY5,
    WGL_SWAP_OVERLAY6,
    WGL_SWAP_OVERLAY7,
    WGL_SWAP_OVERLAY8,
    WGL_SWAP_OVERLAY9,
    WGL_SWAP_OVERLAY10,
    WGL_SWAP_OVERLAY11,
    WGL_SWAP_OVERLAY12,
    WGL_SWAP_OVERLAY13,
    WGL_SWAP_OVERLAY14,
    WGL_SWAP_OVERLAY15
  };
  BOOL result = OpenGLContextImpl::wglSwapLayerBuffers(context->hdc, PLANES[plane + 15]);
  // TAG: fixme
#else // unix
  throw NotImplemented(this);
#endif // flavor
}
#endif

// void OpenGLContext::invalidate() throw(OpenGLException) {
//   OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
//   assert(
//     ::InvalidateRect(context->window, 0, FALSE) != FALSE,
//     OpenGLException(this)
//   );
// }

void OpenGLContext::update() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  // TAG: time this implementation and select faster choice
  assert(
    ::UpdateWindow(context->window),
    OpenGLException(this)
  );
//   assert(
//     ::PostMessage(context->window, WM_PAINT, 0, 0),
//     OpenGLException(this)
//   );
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::close() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  if (context->window) {
    assert(::DestroyWindow(context->window), OpenGLException(this));
  }
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

OpenGLContext::~OpenGLContext() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  // TAG: do we need reference counting for context?
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  if (context->window) {
    ::DestroyWindow(context->window);
  }
  OpenGLContextImpl::removeOpenGLContext(context->window); // should be atomic
  delete context;
  if (--numberOfContexts == 0) { // TAG: must be atomic
    ::UnregisterClass("mip.sdu.dk/~fonseca/base/opengl/OpenGLContext", ::GetModuleHandle(0));
  }
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

String OpenGLContext::getTitle() const throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  // TAG: more simple to use title attribute
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  String result;
  while (true) {
    int length = ::GetWindowTextLength(context->window);
    result.forceToLength(length + 1); // 1 extra to detect if title has been extended
    int newLength = ::GetWindowText(context->window, result.getElements(), result.getLength() + 1);
    if ((newLength > length) || (newLength == 0) && (::GetLastError() != ERROR_SUCCESS)) {
      continue;
    }
    return result;
  }
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::setTitle(const String& title) throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  ::SetWindowText(context->window, title.getElements());
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

// void OpenGLContext::getPosition() throw(OpenGLException) {
//   OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
//   RECT rect;
//   ::GetWindowRect(context->window, &rect);
//   return Position(right - left + 1, bottom - top + 1);
// }

void OpenGLContext::setPosition(const Position& position) throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  RECT rect;
  ::GetWindowRect(context->window, &rect);
  assert(
    ::MoveWindow(
      context->window,
      position.getX(),
      position.getY(),
      rect.right - rect.left + 1,
      rect.bottom - rect.top + 1,
      false // repaint
    ),
    OpenGLException(this)
  );
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

#if 0

unsigned int OpenGLContext::getNumberOfMonitors() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  return ::GetSystemMetrics(SM_CMONITORS);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

bool OpenGLContext::hasMouse() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  return ::GetSystemMetrics(SM_MOUSEPRESENT);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

unsigned int OpenGLContext::getMouseButtons() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  unsigned int result = 0;
  unsigned int temp = ::GetSystemMetrics(SM_CMOUSEBUTTONS);
  // SM_SWAPBUTTON
  if (::GetSystemMetrics(SM_MOUSEWHEELPRESENT)) {
    result |= Mouse::WHEEL;
  }
  return result;
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

enum Screen {
  WINDOW,
  SCREEN,
  PRIMARY,
  VIRTUAL
};

// GetDeviceCaps(hdcPrimaryMonitor, HORZRES/VERTRES)

Dimension OpenGLContext::getScreenDimension(Screen screen) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  unsigned int width;
  unsigned int height;
  switch (screen) {
  case VIRTUAL:
    width = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
    height = ::GetSystemMetrics(SM_CYVIRTUALSCREEN);
    break;
  case PRIMARY:
    width = ::GetSystemMetrics(SM_CXSCREEN);
    height = ::GetSystemMetrics(SM_CYSCREEN);
    break;
  }
  return Dimension(width, height);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}
#endif

// Dimension OpenGLContext::getSize() throw(OpenGLException) {
//   OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
//   RECT rect;
//   assert(::GetClientRect(context->window, &rect), OpenGLException(this));
//   assert(::GetWindowRect(context->window, &rect), OpenGLException(this));
//   return Dimension(left, top);
// }

void OpenGLContext::setSize(const Dimension& dimension) throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  RECT rect;
  ::GetWindowRect(context->window, &rect);
  assert(
    ::MoveWindow(
      context->window,
      rect.left,
      rect.top,
      dimension.getWidth(),
      dimension.getHeight(),
      false // repaint
    ),
    OpenGLException(this)
  );
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::setCursor(Cursor cursor) throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  static const unsigned int CURSORS[] = {
    0, // NONE (not used)
    OCR_APPSTARTING, // WORKING_IN_BACKGROUND
    OCR_NORMAL, // ARROW
    OCR_CROSS, // CROSSHAIR
    32649, // HAND // TAG: only w2k or later (OCR_HAND)
    32651, // HELP
    OCR_NO, // NO
    OCR_SIZEALL, // MOVING
    OCR_SIZENESW, // NORTHEAST_SOUTHWEST
    OCR_SIZENS, // NORTH_SOUTH
    OCR_SIZENWSE, // NORTHWEST_SOUTHEAST
    OCR_SIZEWE, // WEST_EAST
    OCR_UP, // VERTICAL_ARROW
    OCR_WAIT // WAITING
  };
  // TAG: what about OCR_IBEAM

  if (cursor != this->cursor) {
    OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
    HCURSOR handle = (HCURSOR)::LoadImage(0, MAKEINTRESOURCE(CURSORS[cursor]), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
    ::SetClassLong(context->window, GCL_HCURSOR, (LONG)handle);
    POINT point;
    ::GetCursorPos(&point);
    ::SetCursorPos(point.x, point.y);
    this->cursor = cursor;
  }
  
// //   if (cursor != this->cursor) {
//     if (cursor == OpenGLContext::NONE) {
//       ::SetCursor(0);
//     } else {
//       HCURSOR handle = (HCURSOR)::LoadImage(0, MAKEINTRESOURCE(CURSORS[cursor]), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
//       ::SetCursor(handle);
//     }
// //     this->cursor = cursor;
// //   }
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::hideCursor() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  assert(::ShowCursor(FALSE), OpenGLException(this));
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::showCursor() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  assert(::ShowCursor(TRUE), OpenGLException(this));
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

Position OpenGLContext::getCursorPosition() const throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  POINT point;
  ::GetCursorPos(&point);
  return Position(point.x, point.y);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::setCursorPosition(const Position& position) throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  ::SetCursorPos(position.getX(), position.getY());
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

// void OpenGLContext::getCursorConfinement() throw() {
//   RECT rect;
//   ::GetClipCursor(&rect);
//   return Region(Position(rect.left, rect.top), Dimension(rect.right - rect.left + 1, rect.bottom - rect.top + 1));
// }

void OpenGLContext::releaseCursorConfinement() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  assert(::ClipCursor(0), OpenGLException(this));
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::setCursorConfinement() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  POINT offset;
  offset.x = 0;
  offset.y = 0;
  ::ClientToScreen(context->window, &offset);
  RECT rect;  
  assert(::GetClientRect(context->window, &rect), OpenGLException(this));
  rect.left += offset.x;
  rect.top += offset.y;
  rect.right += offset.x;
  rect.bottom += offset.y;
  assert(::ClipCursor(&rect), OpenGLException(this));
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::setCursorConfinement(const Position& position, const Dimension& dimension) throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  RECT rect;
  rect.left = position.getX();
  rect.top = position.getY();
  rect.right = rect.left + dimension.getWidth() - 1;
  rect.bottom = rect.top + dimension.getHeight() - 1;
  ::ClipCursor(&rect);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

#if 0
// Dimension OpenGLContext::getSize() throw(OpenGLException) {
//   OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
//   RECT rect;
//   assert(::GetClientRect(context->window, &rect), OpenGLException(this));
//   assert(::GetWindowRect(context->window, &rect), OpenGLException(this));
//   return Dimension(left, top);
// }
::GetWindowRect(context->window, &rect);
#endif

bool OpenGLContext::isMaximized() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  return ::IsZoomed(context->window) == TRUE;
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

bool OpenGLContext::isMinimized() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  return ::IsIconic(context->window) == TRUE;
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::maximize() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  ::ShowWindow(context->window, SW_MAXIMIZE);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::minimize() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  ::ShowWindow(context->window, SW_MINIMIZE);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::show() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  ::ShowWindow(context->window, SW_SHOW);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::hide() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  ::ShowWindow(context->window, SW_HIDE);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::acquireFocus() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  assert(::SetFocus(context->window), OpenGLException(this));
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::setDisplayMode(DisplayMode displayMode) throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  if (displayMode != this->displayMode) {

    switch (displayMode) {
    case MODE_WINDOW:
      ::ChangeDisplaySettings(0, 0);
      // ::ShowCursor(TRUE);
      break;
    case MODE_FULL_SCREEN:
      {
        DEVMODE mode;
        clear(mode);
        mode.dmSize = sizeof(mode);
        mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
        mode.dmBitsPerPel = 0;
        mode.dmPelsWidth = 0;
        mode.dmPelsHeight = 0;
        if (::ChangeDisplaySettings(&mode, CDS_FULLSCREEN)) {
          throw OpenGLException("Unable to enter full screen mode", this);
        }
        // ::EnumDisplaySettings();
      }
      break;
    }
    
    this->displayMode = displayMode;
  }
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

bool OpenGLContext::getCapture() const throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
  return ::GetCapture() == context->window;
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::setCapture(bool state) throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  if (state) {
    OpenGLContextImpl::Context* context = Cast::pointer<OpenGLContextImpl::Context*>(handle);
    ::SetCapture(context->window);
  } else {
    ::ReleaseCapture();
  }
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

unsigned int OpenGLContext::getKeyState(unsigned int code) const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  static const uint8 KEYS[] = {
    VK_INSERT,
    VK_DELETE,
    VK_HOME,
    VK_END,
    VK_PRIOR,
    VK_NEXT,
    VK_LEFT,
    VK_RIGHT,
    VK_UP,
    VK_DOWN,
    VK_SELECT,
    VK_PRINT,
    VK_EXECUTE,
    VK_SNAPSHOT,
    VK_HELP,
    VK_CAPITAL, // CAPS_LOCK
    VK_SCROLL, // SCROLL_LOCK
    VK_NUMLOCK, // NUM_LOCK
    VK_PAUSE,
    VK_LWIN, // LEFT_MANAGER
    VK_RWIN, // RIGHT_MANAGER
    VK_APPS, // MENU
    VK_F1,
    VK_F2,
    VK_F3,
    VK_F4,
    VK_F5,
    VK_F6,
    VK_F7,
    VK_F8,
    VK_F9,
    VK_F10,
    VK_F11,
    VK_F12,
    VK_F13,
    VK_F14,
    VK_F15,
    VK_F16,
    VK_F17,
    VK_F18,
    VK_F19,
    VK_F20,
    VK_F21,
    VK_F22,
    VK_F23,
    VK_F24
  };
  // TAG: need support for ANSI codes (e.g. 'a')
  if (!((code >= Key::INSERT) && (code <= Key::F24))) {
    return Key::UNKNOWN;
  }
  uint8 nativeCode = KEYS[code - Key::INSERT];
  uint8 nativeState = keyboardState[nativeCode];
  unsigned int state = 0;
  state |= (nativeState & 0x80) ? Key::PRESSED : 0;
  state |= (nativeState & 0x01) ? Key::TOGGLED : 0;
  return state;
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void OpenGLContext::onDisplay() throw() {
  openGL.glClearColor(0.0, 0.0, 0.0, 1.0);
  openGL.glClear(OpenGL::COLOR_BUFFER_BIT | OpenGL::DEPTH_BUFFER_BIT);
  openGL.glFlush();
  swap();
}

void OpenGLContext::onMove(const Position& position) throw() {
}

void OpenGLContext::onResize(const Dimension& dimension) throw() {
  openGL.glViewport(0, 0, dimension.getWidth(), dimension.getHeight());
  onDisplay();
}

void OpenGLContext::onMouseMove(const Position& position, unsigned int buttons) throw() {
}

void OpenGLContext::onMouseScope(bool scope) throw() {
}

void OpenGLContext::onMouseButton(const Position& position, Mouse::Button button, Mouse::Subevent event) throw() {  
}

void OpenGLContext::onMouseWheel(const Position& position, int delta, unsigned int buttons) throw() {
}

void OpenGLContext::onVisibility(bool visible) throw() {
}

void OpenGLContext::onFocus(bool state) throw() {
}

void OpenGLContext::onKey(unsigned int key, unsigned int flags, unsigned int modifier) throw() {
}

void OpenGLContext::onIdle() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::WaitMessage();
  // ::MsgWaitForMultipleObjects(0, 0, FALSE, INFINITE, QS_ALLEVENTS);
#else // unix
  throw NotImplemented(Type::getType<Menu>());
#endif // flavor
}

bool OpenGLContext::onClose() throw() {
  Application::getApplication()->terminate(); // TAG: remove this
  return true;
}

void OpenGLContext::onMenu(unsigned int identifier) throw() {
  onCommand(identifier);
}

void OpenGLContext::onCommand(unsigned int identifier) throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
