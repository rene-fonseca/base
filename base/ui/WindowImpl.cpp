/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/WindowImpl.h>
#include <base/dl/DynamicLinker.h>
#include <base/NotImplemented.h>

// TAG: isChildOf(WindowImpl* object) method

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#  undef DELETE // yikes
#else // unix (X11)
#  include <X11/Xlib.h>
#  include <X11/Xutil.h>
#  include <X11/Xatom.h>
#  include <X11/cursorfont.h>
#  include <X11/keysym.h>
#endif // flavor

#if 1 // TAG: GCC specific
#  define CALL_PASCAL __attribute__((stdcall))
#  define CALL_CPP __attribute__((cdecl))
#endif

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  define CALL_UI CALL_PASCAL
#else
#  define CALL_UI
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// TAG: windows should shared display

SpinLock WindowImpl::spinLock;
// overflow is not possible due to limited resources
unsigned int WindowImpl::numberOfWindows = 0;

namespace windowImpl {

  DynamicLinker* dynamicLinker = 0;

  typedef void (CALL_UI *UIFunction)();
  
  struct FunctionDescriptor {
    const StringLiteral symbol;
    UIFunction* function;
  };
  
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  
  struct Entry {
    Entry* next;
    WindowImpl* window;
    void* handle;
  };
  
  // hash table with linked lists
  // TAG: use knowledge about handles to select an optimal prime
  // TAG: remember last window object (better when messages come in bursts)
  Entry* windows[257]; // contains zeros initially
  
  inline unsigned int getHash(void* handle) throw() {
    const MemorySize offset = Cast::getOffset(handle);
    // TAG: find better shuffle
    return ((offset >> 8) | (offset << ((sizeof(MemorySize) - 1) * 8))) % getArraySize(windows);
  }
  
  WindowImpl* getWindow(void* handle) throw() {
    const unsigned int hash = getHash(handle);
    Entry* entry = windows[hash];
    while (entry) {
      if (entry->handle == handle) {
        return entry->window;
      }
      entry = entry->next;
    }
    return 0;
  }

  void addWindow(WindowImpl* window, void* handle) throw() {
    Entry* temp = new Entry;
    temp->next = 0;
    temp->window = window;
    temp->handle = handle;
    
    const unsigned int hash = getHash(handle);
    Entry* entry = windows[hash];
    if (entry == 0) {
      windows[hash] = temp;
    } else {
      while (entry->next) {
        entry = entry->next;
      }
      entry->next = temp;
    }
  }

  void removeWindow(void* handle) throw() {
    const unsigned int hash = getHash(handle);
    Entry* entry = windows[hash];
    ASSERT((entry->window != 0) && (entry->handle != 0));
    Entry* previous = 0;
    while (entry->handle != handle) {
      previous = entry;
      entry = entry->next;
    }
    ASSERT(entry->handle == handle);
    if (previous) {
      previous->next = entry->next;
    } else {
      windows[hash] = entry->next;
    }
    delete entry;
  }

  class Delegate {
  public:

    static LONG CALL_UI messageHandler(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) throw() {
      WindowImpl* window = getWindow(handle); // should be atomic
      if (window == 0) {
        return ::DefWindowProc(handle, message, wParam, lParam);
      }
      
      switch (message) {
      case WM_QUIT:
        return 0;
      case WM_CLOSE:
        if (window->onClose()) {
          ::DestroyWindow((HWND)window->windowHandle);
        }
        return 0;
      case WM_DESTROY:
        window->onDestruction();
        if (window->graphicsContextHandle) {
          ::DeleteDC((HDC)window->graphicsContextHandle);
          window->graphicsContextHandle = 0;
        }
        ::PostQuitMessage(0);
        return 0;
        // case WM_CAPTURECHANGED:
        //   return 0;
      case WM_MOVE:
        {
          Position position((int16)LOWORD(lParam), (int16)HIWORD(lParam));
          window->position = position;
          window->onMove(position);
          return 0;
        }
      case WM_SIZE:
        {
          Dimension dimension(LOWORD(lParam), HIWORD(lParam));
          window->dimension = dimension;
          window->onResize(dimension);
          return 0;
        }
      case WM_PAINT:
        {          
          // RECT rect;
          // ::GetUpdateRect((HWND)window->windowHandle, &rect, FALSE);
          
          PAINTSTRUCT ps;
          HDC hdc = ::BeginPaint((HWND)window->windowHandle, &ps);
          // ps.rcPaint;
          window->onDisplay();
          ::EndPaint((HWND)window->windowHandle, &ps);
          // ::ValidateRect((HWND)window->windowHandle, 0); // not required with BeginPaint
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
            buttons |= WindowImpl::Key::CONTROL;
          }
          if (wParam & MK_LBUTTON) {
            buttons |= WindowImpl::Mouse::LEFT;
          }
          if (wParam & MK_MBUTTON) {
            buttons |= WindowImpl::Mouse::MIDDLE;
          }
          if (wParam & MK_RBUTTON) {
            buttons |= WindowImpl::Mouse::RIGHT;
          }
          if (wParam & MK_SHIFT) {
            buttons |= WindowImpl::Key::SHIFT;
          }
          switch (message) {
          case WM_LBUTTONDOWN:
            window->onMouseButton(position, WindowImpl::Mouse::LEFT, WindowImpl::Mouse::PRESSED, 0);
            return 0;
          case WM_LBUTTONUP:
            window->onMouseButton(position, WindowImpl::Mouse::LEFT, WindowImpl::Mouse::RELEASED, 0);
            return 0;
          case WM_LBUTTONDBLCLK:
            window->onMouseButton(position, WindowImpl::Mouse::LEFT, WindowImpl::Mouse::DOUBLE_CLICKED, 0);
            return 0;
          case WM_MBUTTONDOWN:
            window->onMouseButton(position, WindowImpl::Mouse::MIDDLE, WindowImpl::Mouse::PRESSED, 0);
            return 0;
          case WM_MBUTTONUP:
            window->onMouseButton(position, WindowImpl::Mouse::MIDDLE, WindowImpl::Mouse::RELEASED, 0);
            return 0;
          case WM_MBUTTONDBLCLK:
            window->onMouseButton(position, WindowImpl::Mouse::MIDDLE, WindowImpl::Mouse::DOUBLE_CLICKED, 0);
            return 0;
          case WM_RBUTTONDOWN:
            window->onMouseButton(position, WindowImpl::Mouse::RIGHT, WindowImpl::Mouse::PRESSED, 0);
            return 0;
          case WM_RBUTTONUP:
            window->onMouseButton(position, WindowImpl::Mouse::RIGHT, WindowImpl::Mouse::RELEASED, 0);
            return 0;
          case WM_RBUTTONDBLCLK:
            window->onMouseButton(position, WindowImpl::Mouse::RIGHT, WindowImpl::Mouse::DOUBLE_CLICKED, 0);
            return 0;
          }
        }
      case WM_MOUSEWHEEL:
        {
          unsigned int keys = LOWORD(wParam); // GET_KEYSTATE_WPARAM
          int delta = (int16)HIWORD(wParam);
          unsigned int buttons = 0;
          if (keys & MK_CONTROL) {
            buttons |= WindowImpl::Key::CONTROL;
          }
          if (keys & MK_LBUTTON) {
            buttons |= WindowImpl::Mouse::LEFT;
          }
          if (keys & MK_MBUTTON) {
            buttons |= WindowImpl::Mouse::MIDDLE;
          }
          if (keys & MK_RBUTTON) {
            buttons |= WindowImpl::Mouse::RIGHT;
          }
          if (keys & MK_SHIFT) {
            buttons |= WindowImpl::Key::SHIFT;
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
            mouseEvent.hwndTrack = (HWND)window->windowHandle;
            mouseEvent.dwHoverTime = 0;
            assert(::TrackMouseEvent(&mouseEvent), UserInterfaceException(Type::getType<WindowImpl>()));
            window->onMouseScope(true);
          }
          unsigned int buttons = 0;
          if (wParam & MK_CONTROL) {
            buttons |= WindowImpl::Key::CONTROL;
          }
          if (wParam & MK_LBUTTON) {
            buttons |= WindowImpl::Mouse::LEFT;
          }
          if (wParam & MK_MBUTTON) {
            buttons |= WindowImpl::Mouse::MIDDLE;
          }
          if (wParam & MK_RBUTTON) {
            buttons |= WindowImpl::Mouse::RIGHT;
          }
          if (wParam & MK_SHIFT) {
            buttons |= WindowImpl::Key::SHIFT;
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
            window->onFocus(WindowImpl::ACQUIRED_FOCUS);
          }
          break;
        case WA_INACTIVE:
        default:
          if (window->active) {
            window->active = false;
            window->onFocus(WindowImpl::LOST_FOCUS);
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
            flags |= WindowImpl::Key::PRESSED;
          }
          if (((lParam >> 30) & 1) == 0) {
            flags |= WindowImpl::Key::FIRST_TIME;
          }
          if (!(flags & WindowImpl::Key::FIRST_TIME) && !window->autorepeat) {
            return 0; // ignore repeats
          }
          if (((lParam >> 24) & 1) != 0) {
            flags |= WindowImpl::Key::EXTENDED;
          }
          
          ::GetKeyboardState(Cast::pointer<BYTE*>(window->keyboardState));
          unsigned int modifiers = 0;
          modifiers |= (window->keyboardState[VK_LSHIFT] & 0x80) ? WindowImpl::Key::LEFT_SHIFT : 0;
          modifiers |= (window->keyboardState[VK_RSHIFT] & 0x80) ? WindowImpl::Key::RIGHT_SHIFT : 0;
          modifiers |= (window->keyboardState[VK_LCONTROL] & 0x80) ? WindowImpl::Key::LEFT_CONTROL : 0;
          modifiers |= (window->keyboardState[VK_RCONTROL] & 0x80) ? WindowImpl::Key::RIGHT_CONTROL : 0;
          modifiers |= (window->keyboardState[VK_LMENU] & 0x80) ? WindowImpl::Key::LEFT_ALT : 0;
          modifiers |= (window->keyboardState[VK_RMENU] & 0x80) ? WindowImpl::Key::RIGHT_ALT : 0;
          modifiers |= (window->keyboardState[VK_CAPITAL] & 0x01) ? WindowImpl::Key::CAPS_LOCK_TOGGLED : 0;
          modifiers |= (window->keyboardState[VK_SCROLL] & 0x01) ? WindowImpl::Key::SCROLL_LOCK_TOGGLED : 0;
          modifiers |= (window->keyboardState[VK_NUMLOCK] & 0x01) ? WindowImpl::Key::NUM_LOCK_TOGGLED : 0;
          window->modifiers = modifiers;
          
          unsigned int code = 0;
          switch (wParam) {
          case VK_RETURN:
            code = WindowImpl::Key::RETURN;
            flags |= WindowImpl::Key::ASCII;
            break;
          case VK_INSERT:
            code = WindowImpl::Key::INSERT;
            break;
          case VK_DELETE:
            code = WindowImpl::Key::DELETE;
            break;
          case VK_HOME:
            code = WindowImpl::Key::HOME;
            break;
          case VK_END:
            code = WindowImpl::Key::END;
            break;
          case VK_PRIOR:
            code = WindowImpl::Key::PRIOR;
            break;
          case VK_NEXT:
            code = WindowImpl::Key::NEXT;
            break;
          case VK_LEFT:
            code = WindowImpl::Key::LEFT;
            break;
          case VK_RIGHT:
            code = WindowImpl::Key::RIGHT;
            break;
          case VK_UP:
            code = WindowImpl::Key::UP;
            break;
          case VK_DOWN:
            code = WindowImpl::Key::DOWN;
            break;
          case VK_SELECT:
            code = WindowImpl::Key::SELECT;
            break;
          case VK_PRINT:
            code = WindowImpl::Key::PRINT;
            break;
          case VK_EXECUTE:
            code = WindowImpl::Key::EXECUTE;
            break;
          case VK_SNAPSHOT:
            code = WindowImpl::Key::SNAPSHOT;
            break;
          case VK_HELP:
            code = WindowImpl::Key::HELP;
            break;            
          case VK_CAPITAL:
            code = WindowImpl::Key::CAPS_LOCK;
            break;
          case VK_SCROLL:
            code = WindowImpl::Key::SCROLL_LOCK;
            break;
          case VK_NUMLOCK:
            code = WindowImpl::Key::NUM_LOCK;
            break;
          case VK_PAUSE:
            code = WindowImpl::Key::PAUSE;
            break;
          case VK_LWIN:
            code = WindowImpl::Key::LEFT_MANAGER;
            break;
          case VK_RWIN:
            code = WindowImpl::Key::RIGHT_MANAGER;
            break;
          case VK_APPS:
            code = WindowImpl::Key::MENU;
            break;
          case VK_F1:
            code = WindowImpl::Key::F1;
            break;
          case VK_F2:
            code = WindowImpl::Key::F2;
            break;
          case VK_F3:
            code = WindowImpl::Key::F3;
            break;
          case VK_F4:
            code = WindowImpl::Key::F4;
            break;
          case VK_F5:
            code = WindowImpl::Key::F5;
            break;
          case VK_F6:
            code = WindowImpl::Key::F6;
            break;
          case VK_F7:
            code = WindowImpl::Key::F7;
            break;
          case VK_F8:
            code = WindowImpl::Key::F8;
            break;
          case VK_F9:
            code = WindowImpl::Key::F9;
            break;
          case VK_F10:
            code = WindowImpl::Key::F10;
            break;
          case VK_F11:
            code = WindowImpl::Key::F11;
            break;
          case VK_F12:
            code = WindowImpl::Key::F12;
            break;
          case VK_F13:
            code = WindowImpl::Key::F13;
            break;
          case VK_F14:
            code = WindowImpl::Key::F14;
            break;
          case VK_F15:
            code = WindowImpl::Key::F15;
            break;
          case VK_F16:
            code = WindowImpl::Key::F16;
            break;
          case VK_F17:
            code = WindowImpl::Key::F17;
            break;
          case VK_F18:
            code = WindowImpl::Key::F18;
            break;
          case VK_F19:
            code = WindowImpl::Key::F19;
            break;
          case VK_F20:
            code = WindowImpl::Key::F20;
            break;
          case VK_F21:
            code = WindowImpl::Key::F21;
            break;
          case VK_F22:
            code = WindowImpl::Key::F22;
            break;
          case VK_F23:
            code = WindowImpl::Key::F23;
            break;
          case VK_F24:
            code = WindowImpl::Key::F24;
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
              window->onKey((uint8)buffer[0], flags|WindowImpl::Key::DEAD|WindowImpl::Key::ASCII, modifiers);
              window->onKey((uint8)buffer[1], flags|WindowImpl::Key::ASCII, modifiers);
              break;
            case 1:
              window->onKey((uint8)buffer[0], flags|WindowImpl::Key::ASCII, modifiers);
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
        //           window->setCursor(window->cursor);
        //           return 0;
        //         }
        //         break;
        //       case WM_SETFOCUS: // keyboard focus
        //         return 0;
        //       case WM_KILLFOCUS: // keyboard focus
        //         return 0;
        //       case WM_TIMER:
        //         return 0;
      case WM_COMMAND:
        if (HIWORD(wParam) == 0) { // from menu
          window->onMenu(LOWORD(wParam));
        } else {
          window->onCommand(LOWORD(wParam));
        }
        return 0;
      case WM_SYSCOMMAND:
        // TAG: fixme (check flags)
        switch (wParam & 0xfff0) {
          //         case SC_SIZE: // prevent
          //           break;
        case SC_SCREENSAVE: // prevent
        case SC_MONITORPOWER: // prevent
          return 0;
        }
        break;
        //       case WM_DISPLAYCHANGE:
        //         return 0;
        //       case WM_ENTERMENULOOP:
        //         return 0;
        //       case WM_EXITMENULOOP:
        //         return 0;
      }
      return ::DefWindowProc((HWND)window->windowHandle, message, wParam, lParam);
    }
  };

#else // unix

  void (*XBlackPixel)();
  void (*XBlackPixelOfScreen)();
  void (*XCloseDisplay)();
  void (*XConfigureWindow)();
  Pixmap (*XCreateBitmapFromData)(Display*, Drawable, char* data, unsigned int width, unsigned int height);
  void (*XCreateFontCursor)();
  void (*XCreateGC)();
  void (*XCreatePixmapCursor)();
  void (*XCreateSimpleWindow)();
  void (*XDefaultScreen)();
  void (*XDefineCursor)();
  void (*XDestroyWindow)();
  void (*XDrawArc)();
  void (*XDrawLine)();
  void (*XDrawPoint)();
  void (*XDrawRectangle)();
  void (*XDrawString)();
  void (*XEventsQueued)();
  void (*XFillArc)();
  void (*XFillRectangle)();
  void (*XFlush)();
  void (*XFreePixmap)();
  void (*XGrabPointer)();
  void (*XHeightOfScreen)();
  void (*XIconifyWindow)();
  void (*XLookupKeysym)();
  void (*XLookupString)();
  void (*XMapWindow)();
  void (*XNextEvent)();
  void (*XOpenDisplay)();
  void (*XPeekEvent)();
  void (*XQueryPointer)();
  void (*XScreenCount)();
  void (*XScreenOfDisplay)();
  void (*XSelectInput)();
  void (*XServerVendor)();
  void (*XSetForeground)();
  void (*XSetInputFocus)();
  void (*XSetTextProperty)();
  void (*XTranslateCoordinates)();
  void (*XUngrabPointer)();
  void (*XVendorRelease)();
  void (*XWarpPointer)();
  void (*XWhitePixel)();
  void (*XWhitePixelOfScreen)();
  void (*XWidthOfScreen)();
  
  static FunctionDescriptor UI_FUNCTIONS[] = {
    {MESSAGE("XBlackPixel"), Cast::pointer<UIFunction*>((void*)&XBlackPixel)},
    {MESSAGE("XBlackPixelOfScreen"), Cast::pointer<UIFunction*>((void*)&XBlackPixelOfScreen)},
    {MESSAGE("XCloseDisplay"), Cast::pointer<UIFunction*>((void*)&XCloseDisplay)},
    {MESSAGE("XConfigureWindow"), Cast::pointer<UIFunction*>((void*)&XConfigureWindow)},
    {MESSAGE("XCreateBitmapFromData"), Cast::pointer<UIFunction*>((void*)&XCreateBitmapFromData)},
    {MESSAGE("XCreateFontCursor"), Cast::pointer<UIFunction*>((void*)&XCreateFontCursor)},
    {MESSAGE("XCreateGC"), Cast::pointer<UIFunction*>((void*)&XCreateGC)},
    {MESSAGE("XCreatePixmapCursor"), Cast::pointer<UIFunction*>((void*)&XCreatePixmapCursor)},
    {MESSAGE("XCreateSimpleWindow"), Cast::pointer<UIFunction*>((void*)&XCreateSimpleWindow)},
    {MESSAGE("XDefaultScreen"), Cast::pointer<UIFunction*>((void*)&XDefaultScreen)},
    {MESSAGE("XDefineCursor"), Cast::pointer<UIFunction*>((void*)&XDefineCursor)},
    {MESSAGE("XDestroyWindow"), Cast::pointer<UIFunction*>((void*)&XDestroyWindow)},
    {MESSAGE("XDrawArc"), Cast::pointer<UIFunction*>((void*)&XDrawArc)},
    {MESSAGE("XDrawLine"), Cast::pointer<UIFunction*>((void*)&XDrawLine)},
    {MESSAGE("XDrawPoint"), Cast::pointer<UIFunction*>((void*)&XDrawPoint)},
    {MESSAGE("XDrawRectangle"), Cast::pointer<UIFunction*>((void*)&XDrawRectangle)},
    {MESSAGE("XDrawString"), Cast::pointer<UIFunction*>((void*)&XDrawString)},
    {MESSAGE("XEventsQueued"), Cast::pointer<UIFunction*>((void*)&XEventsQueued)},
    {MESSAGE("XFillArc"), Cast::pointer<UIFunction*>((void*)&XFillArc)},
    {MESSAGE("XFillRectangle"), Cast::pointer<UIFunction*>((void*)&XFillRectangle)},
    {MESSAGE("XFlush"), Cast::pointer<UIFunction*>((void*)&XFlush)},
    {MESSAGE("XFreePixmap"), Cast::pointer<UIFunction*>((void*)&XFreePixmap)},
    {MESSAGE("XGrabPointer"), Cast::pointer<UIFunction*>((void*)&XGrabPointer)},
    {MESSAGE("XHeightOfScreen"), Cast::pointer<UIFunction*>((void*)&XHeightOfScreen)},
    {MESSAGE("XIconifyWindow"), Cast::pointer<UIFunction*>((void*)&XIconifyWindow)},
    {MESSAGE("XLookupKeysym"), Cast::pointer<UIFunction*>((void*)&XLookupKeysym)},
    {MESSAGE("XLookupString"), Cast::pointer<UIFunction*>((void*)&XLookupString)},
    {MESSAGE("XMapWindow"), Cast::pointer<UIFunction*>((void*)&XMapWindow)},
    {MESSAGE("XNextEvent"), Cast::pointer<UIFunction*>((void*)&XNextEvent)},
    {MESSAGE("XOpenDisplay"), Cast::pointer<UIFunction*>((void*)&XOpenDisplay)},
    {MESSAGE("XPeekEvent"), Cast::pointer<UIFunction*>((void*)&XPeekEvent)},
    {MESSAGE("XQueryPointer"), Cast::pointer<UIFunction*>((void*)&XQueryPointer)},
    {MESSAGE("XScreenCount"), Cast::pointer<UIFunction*>((void*)&XScreenCount)},
    {MESSAGE("XScreenOfDisplay"), Cast::pointer<UIFunction*>((void*)&XScreenOfDisplay)},
    {MESSAGE("XSelectInput"), Cast::pointer<UIFunction*>((void*)&XSelectInput)},
    {MESSAGE("XServerVendor"), Cast::pointer<UIFunction*>((void*)&XServerVendor)},
    {MESSAGE("XSetForeground"), Cast::pointer<UIFunction*>((void*)&XSetForeground)},
    {MESSAGE("XSetInputFocus"), Cast::pointer<UIFunction*>((void*)&XSetInputFocus)},
    {MESSAGE("XSetTextProperty"), Cast::pointer<UIFunction*>((void*)&XSetTextProperty)},
    {MESSAGE("XTranslateCoordinates"), Cast::pointer<UIFunction*>((void*)&XTranslateCoordinates)},
    {MESSAGE("XUngrabPointer"), Cast::pointer<UIFunction*>((void*)&XUngrabPointer)},
    {MESSAGE("XVendorRelease"), Cast::pointer<UIFunction*>((void*)&XVendorRelease)},
    {MESSAGE("XWarpPointer"), Cast::pointer<UIFunction*>((void*)&XWarpPointer)},
    {MESSAGE("XWhitePixel"), Cast::pointer<UIFunction*>((void*)&XWhitePixel)},
    {MESSAGE("XWhitePixelOfScreen"), Cast::pointer<UIFunction*>((void*)&XWhitePixelOfScreen)},
    {MESSAGE("XWidthOfScreen"), Cast::pointer<UIFunction*>((void*)&XWidthOfScreen)}
  };
  
  //   struct Context {
    
  //     // ...
  //     INHERIT, /**< Inherit cursor. */
  //     NONE, /**< No cursor. */
  //     WORKING_IN_BACKGROUND, /**< Cursor indicating working is being the background. */
  //     ARROW, /**< The normal selection cursor. */
  //     CROSSHAIR, /** Cross hair. */
  //     HAND, /**< Hand. */
  //     HELP, /**< Help. */
  //     NO, /**< No. */
  //     MOVING, /**< Moving. */
  //     NORTHEAST_SOUTHWEST, /**< Northeast and southwest pointing arrow. */
  //     NORTH_SOUTH, /**< A birectional arrow. */
  //     NORTHWEST_SOUTHEAST, /**< Northwest and southeast pointing arrow. */
  //     WEST_EAST, /**< A birectional arrow pointer in the west and east direction. */
  //     VERTICAL_ARROW, /**< A vertical arrow. */
  //     WAITING /**< Cursor indicating blocked wait. */
  //   };
  
#endif // flavor
}; // end of WindowImpl namespace

void WindowImpl::onConstruction() throw() {
  // finalize generic initialization
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  windowImpl::addWindow(this, windowHandle);
  
  TRACKMOUSEEVENT mouseEvent;
  mouseEvent.cbSize = sizeof(mouseEvent);
  mouseEvent.dwFlags = TME_LEAVE;
  mouseEvent.hwndTrack = (HWND)windowHandle;
  mouseEvent.dwHoverTime = 0;
  ::TrackMouseEvent(&mouseEvent);
#else // unix
#endif // flavor
}

void WindowImpl::onDestruction() throw() {
}

WindowImpl::WindowImpl() throw(UserInterfaceException)
  : displayHandle(0),
    screenHandle(0),
    windowHandle(0),
    graphicsContextHandle(0),
    displayMode(MODE_WINDOW),
    cursor(WindowImpl::ARROW), // keep in sync with window class
    active(false),
    scope(false),
    autorepeat(true),
    modifiers(0),
    flags(0),
    minimumSize(0, 0),
    maximumSize(0, 0) {
  
  fill<uint8>(keyboardState, getArraySize(keyboardState), 0);
  lastMousePosition = Position(PrimitiveTraits<int>::MINIMUM, PrimitiveTraits<int>::MINIMUM);
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  spinLock.exclusiveLock();
  // only register class if not already registered
  if (WindowImpl::numberOfWindows++ == 0) { // TAG: must be atomic
    WNDCLASSEX temp = {
      sizeof(temp),
      CS_OWNDC, // style // TAG: other flags: CS_DBLCLKS, CS_NOCLOSE, CS_HREDRAW, CS_VREDRAW
      windowImpl::Delegate::messageHandler, // message handler
      0, // extra bytes to allocate
      0, // extra window bytes
      (HINSTANCE)::GetModuleHandle(0), // instance
      0, // icon
      (HCURSOR)::LoadImage(0, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED), // cursor
      0, // background
      0, // menu
      "mip.sdu.dk/~fonseca/base/ui/Window", // class name
      0 // small icon
    };
    ATOM windowClass = ::RegisterClassEx(&temp); // zero if fails
    if (windowClass == 0) {
      --numberOfWindows; // undo increment above
    }
  }
  spinLock.releaseLock();
  assert(
    numberOfWindows != 0,
    UserInterfaceException("Unable to create window context", this)
  );
#else // unix
  Display* display = ::XOpenDisplay(0); // TAG: need support for connection to any server (e.g. "localhost:0.0")
  assert(display, UserInterfaceException("Unable to connect to X server", this));
  
  fout << MESSAGE("Number of available screens: ") << ::XScreenCount(display) << ENDL;
  fout << MESSAGE("Default screen: ") << ::XDefaultScreen(display) << ENDL;
  
  int screenId = ::XDefaultScreen(display);
  Screen* screen = ::XScreenOfDisplay(display, screenId);
  assert(screen, UserInterfaceException("Unable to open screen", this));

  fout << MESSAGE("default depth of screen: ") << DefaultDepthOfScreen(screen) << ENDL;
  
  int blackPixel = ::XBlackPixelOfScreen(screen);
  int whitePixel = ::XWhitePixelOfScreen(screen);
  
  ::Window window = ::XCreateSimpleWindow(
    display,
    DefaultRootWindow(display),
    position.getX(),
    position.getY(),
    dimension.getWidth(),
    dimension.getHeight(),
    0, // border width
    blackPixel, // border color
    blackPixel // background
  );

  ::XChangeProperty(
    display,
    window,
    XA_WM_NAME, // ::XInternAtom(display, "WM_NAME", False)
    XA_STRING,
    8,
    PropModeReplace, // mode
    (unsigned char*)title.getElements(),
    title.getLength()
  );
  
  ::XChangeProperty(
    display,
    window,
    XA_WM_ICON_NAME, // ::XInternAtom(display, "WM_ICON_NAME", False)
    XA_STRING,
    8,
    PropModeReplace, // mode
    (unsigned char*)iconTitle.getElements(),
    iconTitle.getLength()
  );

  ::XSelectInput(
    display,
    window,
    StructureNotifyMask |
    FocusChangeMask |
    VisibilityChangeMask |
    KeyPressMask |
    KeyReleaseMask |
    KeymapStateMask |
    ButtonPressMask |
    ButtonReleaseMask |
    EnterWindowMask |
    LeaveWindowMask |
    PointerMotionMask
  );
  ::XMapWindow(display, window);
  GC graphicsContext = ::XCreateGC(display, window, 0, 0);
  ::XSetForeground(display, graphicsContext, whitePixel);
  
  // wait for the MapNotify event
  XEvent event;
  do {
    ::XNextEvent(display, &event);
  } while (event.type != MapNotify);
  
  displayHandle = display;
  screenHandle = screen;
  windowHandle = (void*)window; // int to void*
  graphicsContextHandle = (void*)graphicsContext; // GC to void*
#endif // flavor
}

WindowImpl::WindowImpl(const String& _title, const Position& _position, const Dimension& _dimension, unsigned int _flags) throw(UserInterfaceException)
  : displayHandle(0),
    screenHandle(0),
    windowHandle(0),
    graphicsContextHandle(0),
    displayMode(MODE_WINDOW),
    cursor(WindowImpl::ARROW), // keep in sync with window class
    active(false),
    scope(false),
    autorepeat(true),
    modifiers(0),
    title(_title),
    iconTitle(_title),
    position(_position),
    dimension(_dimension),
    flags(_flags),
    minimumSize(0, 0),
    maximumSize(0, 0) {
  
  fill<uint8>(keyboardState, getArraySize(keyboardState), 0);
  lastMousePosition = Position(PrimitiveTraits<int>::MINIMUM, PrimitiveTraits<int>::MINIMUM);
  
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  spinLock.exclusiveLock();
  // only register class if not already registered
  if (WindowImpl::numberOfWindows++ == 0) { // TAG: must be atomic
    WNDCLASSEX temp = {
      sizeof(temp),
      CS_OWNDC, // style // TAG: other flags: CS_DBLCLKS, CS_NOCLOSE, CS_HREDRAW, CS_VREDRAW
      windowImpl::Delegate::messageHandler, // message handler
      0, // extra bytes to allocate
      0, // extra window bytes
      (HINSTANCE)::GetModuleHandle(0), // instance
      0, // icon
      (HCURSOR)::LoadImage(0, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED), // cursor
      0, // background
      0, // menu
      "mip.sdu.dk/~fonseca/base/ui/Window", // class name
      0 // small icon
    };
    ATOM windowClass = ::RegisterClassEx(&temp); // zero if fails
    if (windowClass == 0) {
      --numberOfWindows; // undo increment above
    }
  }
  spinLock.releaseLock();
  assert(
    numberOfWindows != 0,
    UserInterfaceException("Unable to create window context", this)
  );
#else // unix
  Display* display = ::XOpenDisplay(0); // TAG: need support for connection to any server (e.g. "localhost:0.0")
  assert(display, UserInterfaceException("Unable to connect to X server", this));
  
  fout << MESSAGE("Number of available screens: ") << ::XScreenCount(display) << ENDL;
  fout << MESSAGE("Default screen: ") << ::XDefaultScreen(display) << ENDL;
  
  int screenId = ::XDefaultScreen(display);
  Screen* screen = ::XScreenOfDisplay(display, screenId);
  assert(screen, UserInterfaceException("Unable to open screen", this));

  fout << MESSAGE("default depth of screen: ") << DefaultDepthOfScreen(screen) << ENDL;
  
  int blackPixel = ::XBlackPixelOfScreen(screen);
  int whitePixel = ::XWhitePixelOfScreen(screen);
  
  ::Window window = ::XCreateSimpleWindow(
    display,
    DefaultRootWindow(display),
    position.getX(),
    position.getY(),
    dimension.getWidth(),
    dimension.getHeight(),
    0, // border width
    blackPixel, // border color
    blackPixel // background
  );

  ::XChangeProperty(
    display,
    window,
    XA_WM_NAME, // ::XInternAtom(display, "WM_NAME", False)
    XA_STRING,
    8,
    PropModeReplace, // mode
    (unsigned char*)title.getElements(),
    title.getLength()
  );
  
  ::XChangeProperty(
    display,
    window,
    XA_WM_ICON_NAME, // ::XInternAtom(display, "WM_ICON_NAME", False)
    XA_STRING,
    8,
    PropModeReplace, // mode
    (unsigned char*)iconTitle.getElements(),
    iconTitle.getLength()
  );

  ::XSelectInput(
    display,
    window,
    StructureNotifyMask |
    FocusChangeMask |
    VisibilityChangeMask |
    KeyPressMask |
    KeyReleaseMask |
    KeymapStateMask |
    ButtonPressMask |
    ButtonReleaseMask |
    EnterWindowMask |
    LeaveWindowMask |
    PointerMotionMask
  );
  ::XMapWindow(display, window);
  GC graphicsContext = ::XCreateGC(display, window, 0, 0);
  ::XSetForeground(display, graphicsContext, whitePixel);
  
  // wait for the MapNotify event
  XEvent event;
  do {
    ::XNextEvent(display, &event);
  } while (event.type != MapNotify);
  
  displayHandle = display;
  screenHandle = screen;
  windowHandle = (void*)window; // int to void*
  graphicsContextHandle = (void*)graphicsContext; // GC to void*
#endif // flavor
}

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__UNIX)
void WindowImpl::flush() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  ::XFlush((Display*)displayHandle);
#endif // flavor
}
#endif // flavor

// TAG: need glx version method, ...
String WindowImpl::getServerVendor() const throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return MESSAGE("UNSPECIFIED");
#else // unix
  return ::XServerVendor((Display*)displayHandle);
#endif // flavor
}

unsigned int WindowImpl::getServerRelease() const throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return 0;
#else // unix
  return ::XVendorRelease((Display*)displayHandle);
#endif // flavor
}

void WindowImpl::displayMenu(const Position& position, const Menu& menu) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::SetForegroundWindow((HWND)windowHandle);
  POINT point;
  point.x = position.getX();
  point.y = position.getY();
  ::ClientToScreen((HWND)windowHandle, &point);
        WRITE_SOURCE_LOCATION();
  assert(
    ::TrackPopupMenuEx(
      (HMENU)menu.getHandle(),
      TPM_LEFTALIGN|TPM_TOPALIGN|TPM_LEFTBUTTON,
      point.x,
      point.y,
      (HWND)windowHandle,
      0
    ),
    UserInterfaceException(this)
  );
        WRITE_SOURCE_LOCATION();
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

String WindowImpl::getTitle() const throw(UserInterfaceException) {
  return title;
}

void WindowImpl::setTitle(const String& title) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::SetWindowText((HWND)windowHandle, title.getElements());
#else // unix
  ::XChangeProperty(
    (Display*)displayHandle,
    (::Window)windowHandle,
    XA_WM_NAME, // ::XInternAtom(display, "WM_NAME", False)
    XA_STRING,
    8,
    PropModeReplace, // mode
    (unsigned char*)title.getElements(),
    title.getLength()
  );
#endif // flavor
}

String WindowImpl::getIconTitle() const throw(UserInterfaceException) {
  return iconTitle;
}

void WindowImpl::setIconTitle(const String& iconTitle) throw(UserInterfaceException) {
  this->iconTitle = iconTitle;
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  // TAG: fixme
#else // unix
  ::XChangeProperty(
    (Display*)displayHandle,
    (::Window)windowHandle,
    XA_WM_ICON_NAME, // ::XInternAtom(display, "WM_ICON_NAME", False)
    XA_STRING,
    8,
    PropModeReplace, // mode
    (unsigned char*)title.getElements(),
    title.getLength()
  );
#endif // flavor
}

void WindowImpl::setPosition(const Position& position) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  RECT rect;
  ::GetWindowRect((HWND)windowHandle, &rect);
  assert(
    ::MoveWindow(
      (HWND)windowHandle,
      position.getX(),
      position.getY(),
      rect.right - rect.left + 1,
      rect.bottom - rect.top + 1,
      true // repaint
    ),
    UserInterfaceException(this)
  );
#else // unix
  XWindowChanges changes;
  changes.x = position.getX();
  changes.y = position.getY();
  ::XConfigureWindow(
    (Display*)displayHandle,
    (::Window)windowHandle,
    CWX|CWY,
    &changes
  );
#endif // flavor
}

void WindowImpl::setDimension(const Dimension& dimension) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  RECT rect;
  ::GetWindowRect((HWND)windowHandle, &rect);
  assert(
    ::MoveWindow(
      (HWND)windowHandle,
      rect.left,
      rect.top,
      dimension.getWidth(),
      dimension.getHeight(),
      true // repaint
    ),
    UserInterfaceException(this)
  );
#else // unix
  XWindowChanges changes;
  changes.width = dimension.getWidth();
  changes.height = dimension.getHeight();
  ::XConfigureWindow(
    (Display*)displayHandle,
    (::Window)windowHandle,
    CWWidth|CWHeight,
    &changes
  );
#endif // flavor
}

void WindowImpl::setRegion(const Position& position, const Dimension& dimension) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(
    ::MoveWindow(
      (HWND)windowHandle,
      position.getX(),
      position.getY(),
      dimension.getWidth(),
      dimension.getHeight(),
      true // repaint
    ),
    UserInterfaceException(this)
  );
#else // unix
  XWindowChanges changes;
  changes.x = position.getX();
  changes.y = position.getY();
  changes.width = dimension.getWidth();
  changes.height = dimension.getHeight();
  ::XConfigureWindow(
    (Display*)displayHandle,
    (::Window)windowHandle,
    CWX|CWY|CWWidth|CWHeight,
    &changes
  );
#endif // flavor
}

void WindowImpl::setCursor(Cursor cursor) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
// LEFT_ARROW, RIGHT_ARROW, INFO, DESTROY, HELP,
// CYCLE, SPRAY, TEXT, UP_DOWN, LEFT_RIGHT,
// TOP_SIDE, BOTTOM_SIDE, LEFT_SIDE,
// RIGHT_SIDE, TOP_LEFT_CORNER,
// TOP_RIGHT_CORNER, BOTTOM_RIGHT_CORNER,
// BOTTOM_LEFT_CORNER, FULL_CROSSHAIR, INHERIT
  static const unsigned int NATIVE_CURSORS[] = {
    0, // INHERIT // TAG: not what
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
    if (cursor == WindowImpl::NONE) {
      assert(::ShowCursor(FALSE), UserInterfaceException(this));
    } else {
      HCURSOR handle = (HCURSOR)::LoadImage(
        0,
        MAKEINTRESOURCE(NATIVE_CURSORS[cursor]),
        IMAGE_CURSOR,
        0,
        0,
        LR_DEFAULTCOLOR|LR_SHARED
      );
      ::SetClassLong((HWND)windowHandle, GCL_HCURSOR, (LONG)handle);
      POINT point;
      ::GetCursorPos(&point);
      ::SetCursorPos(point.x, point.y);
      if (this->cursor == WindowImpl::NONE) {
        assert(::ShowCursor(TRUE), UserInterfaceException(this));
      }
    }
    this->cursor = cursor;
  }
#else // unix
  if (cursor != this->cursor) {
    static const int NATIVE_GLYPH[] = {
      None, // INHERIT
      None, // NONE (not used)
      XC_arrow, // WORKING_IN_BACKGROUND
      XC_arrow, // ARROW
      XC_crosshair, // CROSSHAIR
      XC_hand1, // HAND // TAG: only w2k or later (OCR_HAND)
      XC_question_arrow, // HELP
      XC_arrow, // NO
      None, // MOVING
      None, // NORTHEAST_SOUTHWEST
      None, // NORTH_SOUTH
      None, // NORTHWEST_SOUTHEAST
      None, // WEST_EAST
      None, // VERTICAL_ARROW
      XC_watch // WAITING
      // XC_xterm // TEXT
      // XC_spraycan // SPRAY_CAN
      // XC_exchange // EXCHANGE
      // XC_pirate // DESTROY
    };

    // TAG: use context to cache cursor
    // TAG: temporary resource leak
    ::Cursor nativeCursor;
    if (cursor == NONE) {
      char data = 0;
      Pixmap pixmap = ::XCreateBitmapFromData((Display*)displayHandle, (::Window)windowHandle, &data, 1, 1);
      XColor color;
      nativeCursor = ::XCreatePixmapCursor((Display*)displayHandle, pixmap, pixmap, &color, &color, 0, 0);
      ::XFreePixmap((Display*)displayHandle, pixmap);
    } else {
      nativeCursor = ::XCreateFontCursor((Display*)displayHandle, NATIVE_GLYPH[cursor]);
    }
    ::XDefineCursor((Display*)displayHandle, (::Window)windowHandle, nativeCursor);
    ::XFlush((Display*)displayHandle);
    this->cursor = cursor;
  }
#endif // flavor
}

// TAG: display, screen, or other window?
Position WindowImpl::toGlobalPosition(const Position& position) const throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return position; // TAG: fixme
#else // unix
  int x;
  int y;
  ::Window child;
  Bool result = ::XTranslateCoordinates(
    (Display*)displayHandle,
    (::Window)windowHandle,
    DefaultRootWindow((Display*)displayHandle),
    position.getX(),
    position.getY(),
    &x,
    &y,
    &child
  );
  return Position(x, y);
#endif // flavor
}

// TAG: global or window position
Position WindowImpl::getCursorPosition() const throw(UserInterfaceException) {  
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  POINT point;
  ::GetCursorPos(&point);
  return Position(point.x, point.y);
#else // unix
  int rootX;
  int rootY;
  int x;
  int y;
  unsigned int nativeState;
  ::Window root;
  ::Window child;
  Bool result = ::XQueryPointer(
    (Display*)displayHandle,
    (::Window)windowHandle,
    &root,
    &child,
    &rootX,
    &rootY,
    &x,
    &y,
    &nativeState
  );
  return Position(x, y);
#endif // flavor
}

// TAG: relative to: display, screen, window, or current position
// TAG: clip region support?
void WindowImpl::setCursorPosition(const Position& position) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::SetCursorPos(position.getX(), position.getY()); // screen coordinates
#else // unix
  ::XWarpPointer(
    (Display*)displayHandle,
    None,
    (::Window)windowHandle,
    0,
    0,
    0,
    0,
    position.getX(),
    position.getY()
  );
#endif // flavor
}

void WindowImpl::releaseCursorConfinement() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(::ClipCursor(0), UserInterfaceException(this));
#else // unix
  ::XUngrabPointer((Display*)displayHandle, CurrentTime);
#endif // flavor
}

// void WindowImpl::getCursorConfinement() throw() {
//   RECT rect;
//   ::GetClipCursor(&rect);
//   return Region(Position(rect.left, rect.top), Dimension(rect.right - rect.left + 1, rect.bottom - rect.top + 1));
// }

void WindowImpl::setCursorConfinement() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  POINT offset;
  offset.x = 0;
  offset.y = 0;
  ::ClientToScreen((HWND)windowHandle, &offset);
  RECT rect;  
  assert(::GetClientRect((HWND)windowHandle, &rect), UserInterfaceException(this));
  rect.left += offset.x;
  rect.top += offset.y;
  rect.right += offset.x;
  rect.bottom += offset.y;
  assert(::ClipCursor(&rect), UserInterfaceException(this));
#else // unix
  int result = ::XGrabPointer(
    (Display*)displayHandle,
    (::Window)windowHandle,
    False,
    PointerMotionMask|PointerMotionHintMask|ButtonPressMask|ButtonReleaseMask, // TAG: fixme
    GrabModeAsync, // pointer mode
    GrabModeAsync, // keyboard mode
    (::Window)windowHandle,
    None, // cursor // TAG: use current cursor
    CurrentTime
  );
#endif // flavor
}

void WindowImpl::setCursorConfinement(const Position& position, const Dimension& dimension) throw(UserInterfaceException) {
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

void WindowImpl::disableClipping() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(::SelectClipRgn((HDC)graphicsContextHandle, 0) != ERROR, UserInterfaceException(this));
#else // unix
#endif // flavor
}

WindowImpl::Region WindowImpl::getClipping() const throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
#endif // flavor
}

void WindowImpl::setClipping(const Position& position, const Dimension& dimension) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  RECT rect;
  rect.left = position.getX();
  rect.top = position.getY();
  rect.right = rect.left + dimension.getWidth() - 1;
  rect.bottom = rect.top + dimension.getHeight() - 1;
  HRGN region = ::CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
  assert(::SelectClipRgn((HDC)graphicsContextHandle, region) != ERROR, UserInterfaceException(this));
#else // unix
#endif // flavor
}

void WindowImpl::close() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (windowHandle) {
    assert(::DestroyWindow((HWND)windowHandle), UserInterfaceException(this));
  }
#else // unix
  if (windowHandle) {
    ::XDestroyWindow((Display*)displayHandle, (::Window)windowHandle);
    windowHandle = 0;
  }
#endif // flavor
}

bool WindowImpl::isMaximized() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return ::IsZoomed((HWND)windowHandle) == TRUE;
#else // unix
  // TAG: if dimension == screen dimension && position = (0, 0) relative to screen
  return false; // TAG: fixme
#endif // flavor
}

bool WindowImpl::isMinimized() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return ::IsIconic((HWND)windowHandle) == TRUE;
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void WindowImpl::maximize() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::ShowWindow((HWND)windowHandle, SW_MAXIMIZE);
#else // unix
  // store current position and dimension
  // what is the decoration frame size?
  int x;
  int y;
  ::Window child;
  Bool result = ::XTranslateCoordinates(
    (Display*)displayHandle,
    (::Window)windowHandle,
    DefaultRootWindow((Display*)displayHandle),
    position.getX(),
    position.getY(),
    &x,
    &y,
    &child
  );

  // TAG: border size should must be considered
  XWindowChanges changes;
  changes.x = x;
  changes.y = y;
  changes.width = XWidthOfScreen((Screen*)screenHandle);
  changes.height = XHeightOfScreen((Screen*)screenHandle);
  ::XConfigureWindow(
    (Display*)displayHandle,
    (::Window)windowHandle,
    CWX|CWY|CWWidth|CWHeight,
    &changes
  );
#endif // flavor
}

void WindowImpl::minimize() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::ShowWindow((HWND)windowHandle, SW_MINIMIZE);
#else // unix
  // WM_CHANGE_STATE ClientMessage event with a format of 32 and a first data element of IconicSlate
  ::XIconifyWindow(
   (Display*)displayHandle,
   (::Window)windowHandle,
   0 // TAG: fixme screen_number
  );
#endif // flavor
}

void WindowImpl::normalize() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::ShowWindow((HWND)windowHandle, SW_RESTORE);
#else // unix
  Position originalPosition = position; // TAG: fixme - must be attribute
  Dimension originalDimension = dimension; // TAG: fixme - must be attribute
  XWindowChanges changes;
  changes.x = originalPosition.getX();
  changes.y = originalPosition.getY();
  changes.width = originalDimension.getWidth();
  changes.height = originalDimension.getHeight();
  ::XConfigureWindow(
    (Display*)displayHandle,
    (::Window)windowHandle,
    CWX|CWY|CWWidth|CWHeight,
    &changes
  );
#endif // flavor
}

void WindowImpl::show() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::ShowWindow((HWND)windowHandle, SW_SHOW);
#else // unix
  // TAG: raise?
  throw NotImplemented(this);
#endif // flavor
}

void WindowImpl::hide() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::ShowWindow((HWND)windowHandle, SW_HIDE);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void WindowImpl::enable() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(
    ::EnableWindow((HWND)windowHandle, TRUE),
    UserInterfaceException(this)
  );
#else // unix
#endif // flavor
}

void WindowImpl::disable() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(
    ::EnableWindow((HWND)windowHandle, FALSE),
    UserInterfaceException(this)
  );
#else // unix
#endif // flavor
}


void WindowImpl::acquireFocus() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(::SetFocus((HWND)windowHandle), UserInterfaceException(this));
#else // unix
  ::XSetInputFocus(
    (Display*)displayHandle,
    (::Window)windowHandle,
    RevertToParent,
    CurrentTime
  );
#endif // flavor
}

bool WindowImpl::getCapture() const throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return ::GetCapture() == (HWND)windowHandle;
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void WindowImpl::setCapture(bool state) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (state) {
    ::SetCapture((HWND)windowHandle);
  } else {
    ::ReleaseCapture();
  }
#else // unix
  if (state) {
    int result = ::XGrabPointer(
      (Display*)displayHandle,
      (::Window)windowHandle,
      False,
      PointerMotionMask|PointerMotionHintMask|ButtonPressMask|ButtonReleaseMask, // TAG: fixme
      GrabModeAsync, // pointer mode
      GrabModeAsync, // keyboard
      None, // confinement
      None, // cursor
      CurrentTime
    );
    ::XAllowEvents((Display*)displayHandle, AsyncPointer, CurrentTime);
  } else {
    ::XUngrabPointer((Display*)displayHandle, CurrentTime);
  }
#endif // flavor
}

void WindowImpl::setDisplayMode(DisplayMode displayMode) throw(UserInterfaceException) {
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
        fill<uint8>(Cast::getAddress(mode), sizeof(mode), 0);
        mode.dmSize = sizeof(mode);
        mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
        mode.dmBitsPerPel = 0;
        mode.dmPelsWidth = 0;
        mode.dmPelsHeight = 0;
        if (::ChangeDisplaySettings(&mode, CDS_FULLSCREEN)) {
          throw UserInterfaceException("Unable to enter full screen mode", this);
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

unsigned int WindowImpl::getKeyState(unsigned int code) const throw(UserInterfaceException) {
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
  
void WindowImpl::onDisplay() throw() {
}

void WindowImpl::onMove(const Position& position) throw() {
}

void WindowImpl::onResize(const Dimension& dimension) throw() {
}

void WindowImpl::onMouseMove(const Position& position, unsigned int buttons) throw() {
}

void WindowImpl::onMouseScope(bool scope) throw() {
}

void WindowImpl::onMouseButton(const Position& position, Mouse::Button button, Mouse::Event event, unsigned int state) throw() {
}

void WindowImpl::onMouseWheel(const Position& position, int delta, unsigned int buttons) throw() {
}

void WindowImpl::onKey(unsigned int key, unsigned int flags, unsigned int modifiers) throw() {
}

void WindowImpl::onIdle() throw() {
  wait();
}

bool WindowImpl::onClose() throw() {
  return true; // allow to close window
}

void WindowImpl::onVisibility(Visibility visibility) throw() {
}

void WindowImpl::onFocus(Focus focus) throw() {
}

void WindowImpl::onMenu(unsigned int identifier) throw() {
  onCommand(identifier);
}

void WindowImpl::onCommand(unsigned int identifier) throw() {
}

// void WindowImpl::invalidate() throw(UserInterfaceException) {
//   assert(
//     ::InvalidateRect((HWND)windowHandle, 0, FALSE) != FALSE,
//     UserInterfaceException(this)
//   );
// }

void WindowImpl::update() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  // TAG: time this implementation and select faster choice
  assert(
    ::UpdateWindow((HWND)windowHandle),
    UserInterfaceException(this)
  );
//   assert(
//     ::PostMessage((HWND)windowHandle, WM_PAINT, 0, 0),
//     UserInterfaceException(this)
//   );
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void WindowImpl::exit() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::PostQuitMessage(0);
#else // unix
  throw NotImplemented(Type::getType<WindowImpl>());
#endif // flavor
}

void WindowImpl::wait() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::WaitMessage();
#else // unix
  throw NotImplemented(Type::getType<Menu>());
#endif // flavor
}

bool WindowImpl::wait(unsigned int milliseconds) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (milliseconds > 999999999) { // TAG: fixme
    milliseconds = 999999999;
  }
  return ::MsgWaitForMultipleObjects(0, 0, FALSE, INFINITE, QS_ALLEVENTS) != WAIT_TIMEOUT;
#else // unix
  throw NotImplemented(Type::getType<Menu>());
#endif // flavor
}

void WindowImpl::dispatch() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
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
      onIdle();
    }
  }
#else // unix
  for (;;) {
    XEvent event;
    ::XNextEvent((Display*)displayHandle, &event);
    // TAG: check if src and dest window are ok
    switch (event.type) {
    case MapNotify:
      fout << MESSAGE("Event: MapNotify") << ENDL;
      break;
    case MappingNotify:
      fout << MESSAGE("Event: MappingNotify") << ENDL;
      break;
    case ConfigureNotify:
      {
        const XConfigureEvent* specificEvent = Cast::pointer<const XConfigureEvent*>(&event);
        const Dimension dimension(specificEvent->width, specificEvent->height);
        if (dimension != this->dimension) {
          this->dimension = dimension;
          onResize(dimension);
        }
        const Position position(specificEvent->x, specificEvent->y);
        if (position != this->position) {
          this->position = position;
          onMove(position);
        }
      }
      break;
    case Expose:
      {
        const XExposeEvent* specificEvent = Cast::pointer<const XExposeEvent*>(&event);
        // Region(Position(specificEvent->x, specificEvent->y), Dimension(specificEvent->width, specificEvent->height));
        onDisplay();
      }
      break;
    case ButtonPress:
    case ButtonRelease:
      {
        const XButtonEvent* specificEvent = Cast::pointer<const XButtonEvent*>(&event);
        const bool pressed = (event.type == ButtonPress);
        unsigned int state = 0;   
        if ((specificEvent->button == Button1) && pressed || (specificEvent->button != Button1) && (specificEvent->state & Button1Mask)) {
          state |= WindowImpl::Mouse::LEFT;
        }
        if ((specificEvent->button == Button2) && pressed || (specificEvent->button != Button2) && (specificEvent->state & Button2Mask)) {
          state |= WindowImpl::Mouse::MIDDLE;
        }
        if ((specificEvent->button == Button3) && pressed || (specificEvent->button != Button3) && (specificEvent->state & Button3Mask)) {
          state |= WindowImpl::Mouse::RIGHT;
        }
        if ((specificEvent->button == Button4) && pressed || (specificEvent->button != Button4) && (specificEvent->state & Button4Mask)) {
          state |= WindowImpl::Mouse::EXTRA;
        }
        if ((specificEvent->button == Button5) && pressed || (specificEvent->button != Button5) && (specificEvent->state & Button5Mask)) {
          state |= WindowImpl::Mouse::EXTRA2;
        }
        if (specificEvent->state & ControlMask) {
          state |= WindowImpl::Key::CONTROL;
        }
        if (specificEvent->state & ShiftMask) {
          state |= WindowImpl::Key::SHIFT;
        }
        if (specificEvent->state & LockMask) { // TAG: what about this one
        }
        if (specificEvent->state & Mod1Mask) { // TAG: what about this one
        }
        if (specificEvent->state & Mod2Mask) { // TAG: what about this one
        }
        if (specificEvent->state & Mod3Mask) { // TAG: what about this one
        }
        if (specificEvent->state & Mod4Mask) { // TAG: what about this one
        }
        if (specificEvent->state & Mod5Mask) { // TAG: what about this one
        }

        WindowImpl::Mouse::Button button;
        switch (specificEvent->button) {
        case Button1:
          button = WindowImpl::Mouse::LEFT;
          break;
        case Button2:
          button = WindowImpl::Mouse::MIDDLE;
          break;
        case Button3:
          button = WindowImpl::Mouse::RIGHT;
          break;
        case Button4:
          button = WindowImpl::Mouse::EXTRA;
          break;
        case Button5:
          button = WindowImpl::Mouse::EXTRA2;
          break;
        }
        onMouseButton(
          Position(specificEvent->x, specificEvent->y),
          button,
          pressed ? WindowImpl::Mouse::PRESSED : WindowImpl::Mouse::RELEASED,
          state
        );
      }
      break;
    case MotionNotify:
      {
        const XMotionEvent* specificEvent = Cast::pointer<const XMotionEvent*>(&event);
        const Position position(specificEvent->x, specificEvent->y);
        // TAG: only if scope has not been changed and...?
        if (position == lastMousePosition) {
          break;
        }
        unsigned int state = 0;
        if (specificEvent->state & Button1Mask) {
          state |= WindowImpl::Mouse::LEFT;
        }
        if (specificEvent->state & Button2Mask) {
          state |= WindowImpl::Mouse::MIDDLE;
        }
        if (specificEvent->state & Button3Mask) {
          state |= WindowImpl::Mouse::RIGHT;
        }
        if (specificEvent->state & Button4Mask) {
          state |= WindowImpl::Mouse::EXTRA;
        }
        if (specificEvent->state & Button5Mask) {
          state |= WindowImpl::Mouse::EXTRA2;
        }
        if (specificEvent->state & ControlMask) {
          state |= WindowImpl::Key::CONTROL;
        }
        if (specificEvent->state & ShiftMask) {
          state |= WindowImpl::Key::SHIFT;
        }
        if (specificEvent->state & LockMask) { // TAG: what about this one
        }
        if (specificEvent->state & Mod1Mask) { // TAG: what about this one
          state |= WindowImpl::Key::ALT;
        }
        if (specificEvent->state & Mod2Mask) { // TAG: what about this one
        }
        if (specificEvent->state & Mod3Mask) { // TAG: what about this one
        }
        if (specificEvent->state & Mod4Mask) { // TAG: what about this one
        }
        if (specificEvent->state & Mod5Mask) { // TAG: what about this one
        }
        lastMousePosition = position;
        onMouseMove(position, state);
      }
      break;
    case KeyPress:
    case KeyRelease:
      {
        unsigned int flags = 0;
        if (event.type == KeyPress) {
          flags |= WindowImpl::Key::PRESSED;
        }
        if (!autorepeat) {
          XEvent nextEvent;
          if (::XEventsQueued((Display*)displayHandle, QueuedAfterReading)) {
            ::XPeekEvent((Display*)displayHandle, &nextEvent);
            if ((nextEvent.type == KeyPress) &&
                (nextEvent.xkey.window == event.xkey.window) &&
                (nextEvent.xkey.keycode == event.xkey.keycode) &&
                (nextEvent.xkey.time == event.xkey.time)) {
              ::XNextEvent((Display*)displayHandle, &nextEvent); // remove key event
              break; // ignore repeats
            }
          }
        }
        
        // flags |= WindowImpl::Key::FIRST_TIME; // TAG: fixme
        
        //           ::GetKeyboardState(Cast::pointer<BYTE*>(window->keyboardState));
        unsigned int modifiers = 0;
        //       modifiers |= (window->keyboardState[VK_LSHIFT] & 0x80) ? WindowImpl::Key::LEFT_SHIFT : 0;
        //       modifiers |= (window->keyboardState[VK_RSHIFT] & 0x80) ? WindowImpl::Key::RIGHT_SHIFT : 0;
        //       modifiers |= (window->keyboardState[VK_LCONTROL] & 0x80) ? WindowImpl::Key::LEFT_CONTROL : 0;
        //       modifiers |= (window->keyboardState[VK_RCONTROL] & 0x80) ? WindowImpl::Key::RIGHT_CONTROL : 0;
        //       modifiers |= (window->keyboardState[VK_LMENU] & 0x80) ? WindowImpl::Key::LEFT_ALT : 0;
        //       modifiers |= (window->keyboardState[VK_RMENU] & 0x80) ? WindowImpl::Key::RIGHT_ALT : 0;
        //       modifiers |= (window->keyboardState[VK_CAPITAL] & 0x01) ? WindowImpl::Key::CAPS_LOCK_TOGGLED : 0;
        //       modifiers |= (window->keyboardState[VK_SCROLL] & 0x01) ? WindowImpl::Key::SCROLL_LOCK_TOGGLED : 0;
        //       modifiers |= (window->keyboardState[VK_NUMLOCK] & 0x01) ? WindowImpl::Key::NUM_LOCK_TOGGLED : 0;
        this->modifiers = modifiers;
      
        unsigned int code = 0;
        char buffer[2];
        int numberOfChars = ::XLookupString((XKeyEvent*)&event, buffer,  sizeof(buffer), 0, 0);
        switch (numberOfChars) {
        case 2:
          onKey((uint8)buffer[0], flags|WindowImpl::Key::DEAD|WindowImpl::Key::ASCII, modifiers);
          onKey((uint8)buffer[1], flags|WindowImpl::Key::ASCII, modifiers);
          code = 1; // TAG: clean up
          break;
        case 1:
          onKey((uint8)buffer[0], flags|WindowImpl::Key::ASCII, modifiers);
          code = 1; // TAG: clean up
          break;
        case 0:
          break; // ignore key
        }

        if (code != 0) {
          continue;
        }
      
        flags |= WindowImpl::Key::EXTENDED;
      
        KeySym nativeCode = ::XLookupKeysym((XKeyEvent*)&event, 0);
        switch (nativeCode) {
          // ASCII: BackSpace, Tab, Linefeed, Clear, Return, Sys_req, Escape, Delete
        case XK_Return:
          code = WindowImpl::Key::RETURN;
          flags |= WindowImpl::Key::ASCII;
          break;
        case XK_Insert:
          code = WindowImpl::Key::INSERT;
          break;
        case XK_Delete:
          code = WindowImpl::Key::DELETE;
          break;
        case XK_Home:
          code = WindowImpl::Key::HOME;
          break;
        case XK_End:
          code = WindowImpl::Key::END;
          break;
        case XK_Prior:
          code = WindowImpl::Key::PRIOR;
          break;
        case XK_Next:
          code = WindowImpl::Key::NEXT;
          break;
        case XK_Left:
          code = WindowImpl::Key::LEFT;
          break;
        case XK_Right:
          code = WindowImpl::Key::RIGHT;
          break;
        case XK_Up:
          code = WindowImpl::Key::UP;
          break;
        case XK_Down:
          code = WindowImpl::Key::DOWN;
          break;
        case XK_Select:
          code = WindowImpl::Key::SELECT;
          break;
        case XK_Print:
          code = WindowImpl::Key::PRINT;
          break;
        case XK_Execute:
          code = WindowImpl::Key::EXECUTE;
          break;
          //      case VK_SNAPSHOT:
          //             code = WindowImpl::Key::SNAPSHOT;
          //             break;
        case XK_Undo:
          break; // TAG: fixme
        case XK_Redo:
          break; // TAG: fixme
        case XK_Menu:
          break; // TAG: fixme
        case XK_Find:
          break; // TAG: fixme
        case XK_Cancel:
          break; // TAG: fixme
        case XK_Help:
          code = WindowImpl::Key::HELP;
          break;
        case XK_Break:
          break; // TAG: fixme
        case XK_KP_Space:
          break;
        case XK_KP_Tab:
          break;
        case XK_KP_Enter:
          break;
        case XK_KP_F1:
          break;
        case XK_KP_F2:
          break;
        case XK_KP_F3:
          break;
        case XK_KP_F4:
          break;
        case XK_KP_Home:
          break;
        case XK_KP_Left:
          break;
        case XK_KP_Up:
          break;
        case XK_KP_Right:
          break;
        case XK_KP_Down:
          break;
        case XK_KP_Prior:
          break;
        case XK_KP_Next:
          break;
        case XK_KP_End:
          break;
        case XK_KP_Begin:
          break;
        case XK_KP_Insert:
          break;
        case XK_KP_Delete:
          break;
        case XK_KP_Equal:
          break;
        case XK_KP_Multiply:
          break;
        case XK_KP_Add:
          break;
        case XK_KP_Separator:
          break;
        case XK_KP_Subtract:
          break;
        case XK_KP_Decimal:
          break;
        case XK_KP_Divide:
          break;
        case XK_KP_0:
          break;
        case XK_KP_1:
          break;
        case XK_KP_2:
          break;
        case XK_KP_3:
          break;
        case XK_KP_4:
          break;
        case XK_KP_5:
          break;
        case XK_KP_6:
          break;
        case XK_KP_7:
          break;
        case XK_KP_8:
          break;
        case XK_KP_9:
          break;
          // TAG: fix for win32
        case XK_Shift_L:
          code = WindowImpl::Key::SHIFT_LEFT;
          break;
        case XK_Shift_R:  
          code = WindowImpl::Key::SHIFT_RIGHT;
          break;
        case XK_Control_L:
          code = WindowImpl::Key::CONTROL_LEFT;
          break;
        case XK_Control_R:
          code = WindowImpl::Key::CONTROL_RIGHT;
          break;
        case XK_Meta_L:
          code = WindowImpl::Key::META_LEFT;
          break;
        case XK_Meta_R:
          code = WindowImpl::Key::META_RIGHT;
          break;
        case XK_Alt_L:
          code = WindowImpl::Key::ALT_LEFT;
          break;
        case XK_Alt_R:
          code = WindowImpl::Key::ALT_RIGHT;
          break;
        case XK_Super_L:
          code = WindowImpl::Key::SUPER_LEFT;
          break;
        case XK_Super_R:
          code = WindowImpl::Key::SUPER_RIGHT;
          break;
        case XK_Hyper_L:
          code = WindowImpl::Key::HYPER_LEFT;
          break;
        case XK_Hyper_R:
          code = WindowImpl::Key::HYPER_RIGHT;
          break;
        case XK_Caps_Lock:
          // case XK_Shift_Lock:
          code = WindowImpl::Key::CAPS_LOCK;
          break;
        case XK_Scroll_Lock:
          code = WindowImpl::Key::SCROLL_LOCK;
          break;
        case XK_Num_Lock:
          code = WindowImpl::Key::NUM_LOCK;
          break;
        case XK_Pause:
          code = WindowImpl::Key::PAUSE;
          break;
          //       case XK_Sys_req:
          //         code = WindowImpl::Key::SYS_REQUEST;
          //         break;
          //       case VK_LWIN:
          //         code = WindowImpl::Key::LEFT_MANAGER;
          //         break;
          //       case VK_RWIN:
          //         code = WindowImpl::Key::RIGHT_MANAGER;
          //         break;
          //       case VK_APPS:
          //         code = WindowImpl::Key::MENU;
          //         break;
        case XK_F1:
          code = WindowImpl::Key::F1;
          break;
        case XK_F2:
          code = WindowImpl::Key::F2;
          break;
        case XK_F3:
          code = WindowImpl::Key::F3;
          break;
        case XK_F4:
          code = WindowImpl::Key::F4;
          break;
        case XK_F5:
          code = WindowImpl::Key::F5;
          break;
        case XK_F6:
          code = WindowImpl::Key::F6;
          break;
        case XK_F7:
          code = WindowImpl::Key::F7;
          break;
        case XK_F8:
          code = WindowImpl::Key::F8;
          break;
        case XK_F9:
          code = WindowImpl::Key::F9;
          break;
        case XK_F10:
          code = WindowImpl::Key::F10;
          break;
        case XK_F11:
          code = WindowImpl::Key::F11;
          break;
        case XK_F12:
          code = WindowImpl::Key::F12;
          break;
        case XK_F13:
          code = WindowImpl::Key::F13;
          break;
        case XK_F14:
          code = WindowImpl::Key::F14;
          break;
        case XK_F15:
          code = WindowImpl::Key::F15;
          break;
        case XK_F16:
          code = WindowImpl::Key::F16;
          break;
        case XK_F17:
          code = WindowImpl::Key::F17;
          break;
        case XK_F18:
          code = WindowImpl::Key::F18;
          break;
        case XK_F19:
          code = WindowImpl::Key::F19;
          break;
        case XK_F20:
          code = WindowImpl::Key::F20;
          break;
        case XK_F21:
          code = WindowImpl::Key::F21;
          break;
        case XK_F22:
          code = WindowImpl::Key::F22;
          break;
        case XK_F23:
          code = WindowImpl::Key::F23;
          break;
        case XK_F24:
          code = WindowImpl::Key::F24;
          break;
        }
        if (code) {
          onKey(code, flags, modifiers);
        }
      }
      break;
    case EnterNotify:
      scope = true;
      onMouseScope(true);
      break;
    case LeaveNotify:
      scope = false;
      onMouseScope(false);
      break;
    case UnmapNotify:
      fout << MESSAGE("Event: UnmapNotify") << ENDL;
      break;
    case VisibilityNotify:
      {
        const XVisibilityEvent* specificEvent = Cast::pointer<const XVisibilityEvent*>(&event);
        onVisibility(
          (specificEvent->state == VisibilityFullyObscured) ? WindowImpl::INVISIBLE : WindowImpl::VISIBLE
        );
      }
      break;
      //     case ClientMessage:
      //       break;
    case DestroyNotify:
      fout << MESSAGE("Event: DestroyNotify") << ENDL;
      break;
      //     case CirculateNotify:
      //       break;
      //     case CreateNotify:
      //       break;
      //     case GravityNotify:
      //       break;
      //     case ReparentNotify:
      //       break;
    default:
      fout << MESSAGE("Event: unhandled") << ENDL;
    }
  }
#endif // flavor
}

bool WindowImpl::hasMouse() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return ::GetSystemMetrics(SM_MOUSEPRESENT) == TRUE;
#else // unix
  return true; // TAG: fixme
#endif // flavor
}

unsigned int WindowImpl::getMouseButtons() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  unsigned int result = 0;
  unsigned int temp = ::GetSystemMetrics(SM_CMOUSEBUTTONS);
  result |= (temp >= 1) ? Mouse::LEFT : 0;
  result |= (temp >= 2) ? Mouse::RIGHT : 0;
  result |= (temp >= 3) ? Mouse::MIDDLE : 0;
  result |= (temp >= 4) ? Mouse::EXTRA : 0;
  result |= (temp >= 5) ? Mouse::EXTRA2 : 0;
  if (::GetSystemMetrics(SM_MOUSEWHEELPRESENT)) {
    result |= Mouse::WHEEL;
  }
  return result;
#else // unix
  return Mouse::LEFT|Mouse::MIDDLE|Mouse::RIGHT|Mouse::EXTRA|Mouse::EXTRA2; // TAG: fixme
#endif // flavor
}

unsigned int WindowImpl::getNumberOfMonitors() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return ::GetSystemMetrics(SM_CMONITORS);
#else // unix
  return 1; // TAG: fixme
#endif // flavor
}

Dimension WindowImpl::getDisplayDimension() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  unsigned int width = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
  unsigned int height = ::GetSystemMetrics(SM_CYVIRTUALSCREEN);
  return Dimension(width, height);
#else // unix
  unsigned int width = ::XWidthOfScreen((Screen*)screenHandle);
  unsigned int height = ::XHeightOfScreen((Screen*)screenHandle);
  return Dimension(width, height);
#endif // flavor
}

unsigned int WindowImpl::getMouseButtonIndex(Mouse::Button button) throw() {
  // keep out to date with WindowImpl::Mouse::Button
  switch (button) {
  case Mouse::LEFT:
    return 0;
  case Mouse::MIDDLE:
    return 1;
  case Mouse::RIGHT:
    return 2;
  case Mouse::EXTRA:
    return 3;
  case Mouse::EXTRA2:
    return 4;
  }
}

StringLiteral WindowImpl::getMouseButtonName(Mouse::Button button) throw() {
  // keep out to date with WindowImpl::Mouse::Button
  switch (button) {
  case Mouse::LEFT:
    return MESSAGE("LEFT");
  case Mouse::MIDDLE:
    return MESSAGE("MIDDLE");
  case Mouse::RIGHT:
    return MESSAGE("RIGHT");
  case Mouse::EXTRA:
    return MESSAGE("EXTRA");
  case Mouse::EXTRA2:
    return MESSAGE("EXTRA2");
  }
}

WindowImpl::~WindowImpl() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  // TAG: do we need reference counting for context?
  if (windowHandle) {
    ::DestroyWindow((HWND)windowHandle);
  }
  windowImpl::removeWindow(windowHandle); // should be atomic
  spinLock.exclusiveLock();
  if (--numberOfWindows == 0) {
    ::UnregisterClass("mip.sdu.dk/~fonseca/base/ui/Window", ::GetModuleHandle(0));
  }
  spinLock.releaseLock();
#else // unix
  if (windowHandle) {
    ::XDestroyWindow((Display*)displayHandle, (::Window)windowHandle);
  }
  ::XCloseDisplay((Display*)displayHandle);
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
