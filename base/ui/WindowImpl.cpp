/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/WindowImpl.h>
#include <base/dl/DynamicLinker.h>
#include <base/NotImplemented.h>
#include <base/platforms/backend/WindowImpl.h>
 // #include <base/Profiler.h>
#include <base/build.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#if !defined(_WIN32_WINNT)
#  define _WIN32_WINNT _WIN32_WINNT_WINXP
#endif
#  include <windows.h>
#  undef DELETE // yikes
#else // unix (X11)
#  include <base/platforms/os/unix/X11.h>
#if defined(_COM_AZURE_DEV__BASE__USE_X11)
#  include <X11/cursorfont.h>
#  include <X11/keysym.h>
#endif
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

SpinLock WindowImpl::spinLock;
// TAG: should be separate reference counted class
void* WindowImpl::displayHandle = nullptr;
unsigned int WindowImpl::numberOfLocks = 0;

namespace windowImpl {

  DynamicLinker* dynamicLinker = nullptr;

  typedef void (CALL_UI *UIFunction)();
  
  struct FunctionDescriptor {
    const Literal symbol;
    UIFunction* function;
  };

  struct Entry {
    Entry* next;
    WindowImpl* window;
    void* handle;
  };
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // hash table with linked lists
  // TAG: use knowledge about handles to select an optimal prime
  // TAG: remember last window object (better when messages come in bursts)
  Entry* windows[257]; // contains zeros initially

  inline unsigned int getHash(void* handle) throw() {
    const MemorySize offset = Cast::getOffset(handle);
    // TAG: find better shuffle
    return ((offset >> 8) | (offset << ((sizeof(MemorySize) - 1) * 8))) % getArraySize(windows);
  }
#else // unix
  // hash table with linked lists
  // TAG: use knowledge about handles to select an optimal prime
  // TAG: remember last window object (better when messages come in bursts)
  Entry* windows[257]; // contains zeros initially
  
  inline unsigned int getHash(void* handle) throw() {
    const MemorySize offset = Cast::getOffset(handle);
    // TAG: find better shuffle
    return ((offset >> 8) | (offset << ((sizeof(MemorySize) - 1) * 8))) % getArraySize(windows);
  }
#endif // flavor
  
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
    BASSERT((entry->window != 0) && (entry->handle != 0));
    Entry* previous = nullptr;
    while (entry->handle != handle) {
      previous = entry;
      entry = entry->next;
    }
    BASSERT(entry->handle == handle);
    if (previous) {
      previous->next = entry->next;
    } else {
      windows[hash] = entry->next;
    }
    delete entry;
  }

#if defined(_COM_AZURE_DEV__BASE__USE_X11)

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
  //     CROSSHAIR, /**< Cross hair. */
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

  Atom protocolsAtom;
  Atom atoms[3];
#endif // flavor
  
}; // end of windowImpl namespace

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
LRESULT CALLBACK Backend<WindowImpl>::messageHandler(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) noexcept {
  WindowImpl* window = windowImpl::getWindow(handle); // should be atomic
  if (window == 0) {
    return ::DefWindowProc(handle, message, wParam, lParam);
  }
  
  switch (message) {
  case WM_QUIT:
    return 0;
  case WM_CLOSE:
    if (window->onClose()) {
      ::DestroyWindow((HWND)window->drawableHandle);
    }
    return 0;
  case WM_DESTROY:
    window->destroy();
    ::PostQuitMessage(0);
    return 0;
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
      // ::GetUpdateRect((HWND)window->drawableHandle, &rect, FALSE);
      
      PAINTSTRUCT ps;
      /*HDC hdc =*/ ::BeginPaint((HWND)window->drawableHandle, &ps);
      // ps.rcPaint;
      window->onDisplay();
      ::EndPaint((HWND)window->drawableHandle, &ps);
      // ::ValidateRect((HWND)window->drawableHandle, 0); // not required with BeginPaint
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
        window->onMouseButton(
          position,
          WindowImpl::Mouse::LEFT,
          WindowImpl::Mouse::PRESSED,
          0
        );
        return 0;
      case WM_LBUTTONUP:
        window->onMouseButton(
          position,
          WindowImpl::Mouse::LEFT,
          WindowImpl::Mouse::RELEASED,
          0
        );
        return 0;
      case WM_LBUTTONDBLCLK:
        window->onMouseButton(
          position,
          WindowImpl::Mouse::LEFT,
          WindowImpl::Mouse::DOUBLE_CLICKED,
          0
        );
        return 0;
      case WM_MBUTTONDOWN:
        window->onMouseButton(
          position,
          WindowImpl::Mouse::MIDDLE,
          WindowImpl::Mouse::PRESSED,
          0
        );
        return 0;
      case WM_MBUTTONUP:
        window->onMouseButton(
          position,
          WindowImpl::Mouse::MIDDLE,
          WindowImpl::Mouse::RELEASED,
          0
        );
        return 0;
      case WM_MBUTTONDBLCLK:
        window->onMouseButton(
          position,
          WindowImpl::Mouse::MIDDLE,
          WindowImpl::Mouse::DOUBLE_CLICKED,
          0
        );
        return 0;
      case WM_RBUTTONDOWN:
        window->onMouseButton(
          position,
          WindowImpl::Mouse::RIGHT,
          WindowImpl::Mouse::PRESSED,
          0
        );
        return 0;
      case WM_RBUTTONUP:
        window->onMouseButton(
          position,
          WindowImpl::Mouse::RIGHT,
          WindowImpl::Mouse::RELEASED,
          0
        );
        return 0;
      case WM_RBUTTONDBLCLK:
        window->onMouseButton(
          position,
          WindowImpl::Mouse::RIGHT,
          WindowImpl::Mouse::DOUBLE_CLICKED,
          0
        );
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
        mouseEvent.hwndTrack = (HWND)window->drawableHandle;
        mouseEvent.dwHoverTime = 0;
        bassert(
          ::TrackMouseEvent(&mouseEvent),
          UserInterfaceException(Type::getType<WindowImpl>())
        );
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
      window->onMouseMove(
        Position((int16)LOWORD(lParam), (int16)HIWORD(lParam)),
        buttons
      );
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
  case WM_USER + WindowImpl::PING_MESSAGE:
    return true;
  }
  return ::DefWindowProc(
    (HWND)window->drawableHandle,
    message,
    wParam,
    lParam
  );
}
#endif // flavor

void WindowImpl::construct() throw()
{
  windowImpl::addWindow(this, drawableHandle);
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  TRACKMOUSEEVENT mouseEvent;
  mouseEvent.cbSize = sizeof(mouseEvent);
  mouseEvent.dwFlags = TME_LEAVE;
  mouseEvent.hwndTrack = (HWND)drawableHandle;
  mouseEvent.dwHoverTime = 0;
  ::TrackMouseEvent(&mouseEvent);
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  ::XSetWMProtocols(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    windowImpl::atoms,
    getArraySize(windowImpl::atoms)
  );
#endif // flavor
}

void WindowImpl::destroy() throw() {
  graphicsContextHandle = 0;
  if (drawableHandle) {
    windowImpl::removeWindow(drawableHandle);
    drawableHandle = 0;
  }
  screenHandle = 0;
}

bool WindowImpl::loadModule(bool load) throw() {
  bool success = true;
  spinLock.exclusiveLock();
  if (load) {
    if (numberOfLocks++ == 0) { // overflow not possible due to limited resources
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
      // only register class if not already registered
      ATOM windowClass = 0;
      WNDCLASSEX temp = {
        sizeof(temp),
        CS_OWNDC, // style // TAG: other flags: CS_DBLCLKS, CS_NOCLOSE, CS_HREDRAW, CS_VREDRAW
        Backend<WindowImpl>::messageHandler, // message handler
        0, // extra bytes to allocate
        0, // extra window bytes
        (HINSTANCE)::GetModuleHandle(0), // instance
        0, // icon
        (HCURSOR)::LoadImage(0, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED), // cursor
        0, // background
        0, // menu
        _COM_AZURE_DEV__BASE__ID_PREFIX L"/ui/WindowImpl", // class name
        0 // small icon
      };
      windowClass = ::RegisterClassEx(&temp); // zero if fails
      success = windowClass != 0;
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
      // TAG: fix display handle support (DISPLAY variable could have been changed)
      // TAG: need support for connection to any server (e.g. "localhost:0.0")
      Display* display = ::XOpenDisplay(0);
      if (display) {
//         int numberOfVisuals = 0;
//         XVisualInfo temp = 0;
//         temp.depth = 24;
//         XVisualInfo* info = XGetVisualInfo(display, VisualDepthMask, &temp, &numberOfVisuals);
//         fout << numberOfVisuals << ENDL;

//         for (unsigned int i = 0; i < numberOfVisuals; ++i) {
//           fout << "screen:" << info[i].screen << EOL
//                << "depth:" << info[i].depth << EOL
//                << "red:" << info[i].red_mask << EOL
//                << "green:" << info[i].green_mask << EOL
//                << "blue:" << info[i].blue_mask << EOL
//                << "map:" << info[i].colormap_size << EOL
//                << "bits:" << info[i].bits_per_rgb << EOL
//                << ENDL;
//         }
//         XFree(info);
        
        windowImpl::protocolsAtom = ::XInternAtom(
          (Display*)display,
          "WM_PROTOCOLS",
          True
        );
        windowImpl::atoms[DESTROY_MESSAGE] = ::XInternAtom(
          display,
          "WM_DELETE_WINDOW",
          True
        );
        windowImpl::atoms[PING_MESSAGE] =
          ::XInternAtom(
            display,
            String(_COM_AZURE_DEV__BASE__ID_PREFIX L"/ui/WindowImpl/PING_MESSAGE").native(),
            False
          );
        windowImpl::atoms[QUIT_MESSAGE] =
          ::XInternAtom(
            display,
            String(_COM_AZURE_DEV__BASE__ID_PREFIX L"/ui/WindowImpl/QUIT_MESSAGE").native(),
            False
          );
      }
      displayHandle = display;
      success = displayHandle != 0;
#endif // flavor
      if (!success) {
        --numberOfLocks; // undo
      }
    }
  } else { // unload
    if (--numberOfLocks == 0) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
      ::UnregisterClass(
        _COM_AZURE_DEV__BASE__ID_PREFIX L"/ui/WindowImpl",
        ::GetModuleHandle(NULL)
      );
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
      ::XCloseDisplay((Display*)displayHandle);
      displayHandle = 0;
#endif // flavor
    }
  }
  spinLock.releaseLock();
  return success;
}

WindowImpl::WindowImpl() throw(UserInterfaceException)
  : displayMode(MODE_WINDOW),
    lastMousePosition(
      PrimitiveTraits<int>::MINIMUM,
      PrimitiveTraits<int>::MINIMUM
    ),
    modifiers(0),
    autorepeat(true),
    scope(false),
    active(false),
    visibility(WindowImpl::INVISIBLE),
    enabled(false),
    cursor(WindowImpl::ARROW), // keep in sync with window class
    flags(0),
    minimumSize(0, 0),
    maximumSize(0, 0),
    screenHandle(0),
    graphicsContextHandle(0) {
  // TAG: keyboard state only for windows which accept focus
  fill<uint8>(keyboardState, getArraySize(keyboardState), 0);
  bassert(
    loadModule(true),
    UserInterfaceException("Unable to load module", this)
  );
}

WindowImpl::WindowImpl(
  const Position& _position,
  const Dimension& _dimension,
  unsigned int _flags) throw(UserInterfaceException)
  : displayMode(MODE_WINDOW),
    lastMousePosition(
      PrimitiveTraits<int>::MINIMUM,
      PrimitiveTraits<int>::MINIMUM
    ),
    modifiers(0),
    autorepeat(true),
    scope(false),
    active(false),
    visibility(WindowImpl::INVISIBLE),
    enabled(false),
    cursor(WindowImpl::ARROW), // keep in sync with window class
    position(_position),
    dimension(_dimension),
    flags(_flags),
    minimumSize(0, 0),
    maximumSize(0, 0),
    screenHandle(0),
    graphicsContextHandle(0)
{
  fill<uint8>(keyboardState, getArraySize(keyboardState), 0);
  lastMousePosition =
    Position(PrimitiveTraits<int>::MINIMUM, PrimitiveTraits<int>::MINIMUM);
  bassert(
    loadModule(true),
    UserInterfaceException("Unable to load module", this)
  );
}

Position WindowImpl::getBindingOffset(Binding binding) const throw()
{
  const Position position = getPosition();
  const Dimension dimension = getDimension();
  int x = position.getX();
  int y = position.getY();
  switch (binding) {
  case UPPER_LEFT:
    break;
  case UPPER_CENTER:
    x += dimension.getWidth()/2;
    break;
  case UPPER_RIGHT:
    x += dimension.getWidth();
    break;
  case MIDDLE_LEFT:
    y += dimension.getHeight()/2;
    break;
  case MIDDLE_CENTER:
    x += dimension.getWidth()/2;
    y += dimension.getHeight()/2;
    break;
  case MIDDLE_RIGHT:
    x += dimension.getWidth();
    y += dimension.getHeight()/2;
    break;
  case LOWER_LEFT:
    y += dimension.getHeight();
    break;
  case LOWER_CENTER:
    x += dimension.getWidth()/2;
    y += dimension.getHeight();
    break;
  case LOWER_RIGHT:
    x += dimension.getWidth();
    y += dimension.getHeight();
  }
  return Position(x, y);
}

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__UNIX)
void WindowImpl::flush() throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  ::XFlush((Display*)displayHandle);
#endif // flavor
}
#endif // flavor

// TAG: need glx version method, ...
String WindowImpl::getServerVendor() const throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return Literal("UNSPECIFIED");
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  return NativeString(::XServerVendor((Display*)displayHandle));
#else
  return String();
#endif // flavor
}

unsigned int WindowImpl::getServerRelease() const
  throw(UserInterfaceException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return 0;
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  return ::XVendorRelease((Display*)displayHandle);
#else
  return 0;
#endif // flavor
}

void WindowImpl::displayMenu(
  const Position& position, const Menu& menu) throw(UserInterfaceException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::SetForegroundWindow((HWND)drawableHandle);
  POINT point;
  point.x = position.getX();
  point.y = position.getY();
  ::ClientToScreen((HWND)drawableHandle, &point);
  bassert(
    ::TrackPopupMenuEx(
      (HMENU)menu.getHandle(),
      TPM_LEFTALIGN|TPM_TOPALIGN|TPM_LEFTBUTTON,
      point.x,
      point.y,
      (HWND)drawableHandle,
      0
    ),
    UserInterfaceException(this)
  );
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

String WindowImpl::getTitle() const throw(UserInterfaceException)
{
  return title;
}

void WindowImpl::setTitle(const String& title) throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::SetWindowText((HWND)drawableHandle, ToWCharString(title));
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  ::XChangeProperty(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    XA_WM_NAME, // ::XInternAtom(display, "WM_NAME", False)
    XA_STRING,
    8,
    PropModeReplace, // mode
    (unsigned char*)title.getElements(),
    title.getLength()
  );
#endif // flavor
}

String WindowImpl::getIconTitle() const throw(UserInterfaceException)
{
  return iconTitle;
}

void WindowImpl::setIconTitle(
  const String& iconTitle) throw(UserInterfaceException)
{
  this->iconTitle = iconTitle;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // TAG: fixme
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  ::XChangeProperty(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    XA_WM_ICON_NAME, // ::XInternAtom(display, "WM_ICON_NAME", False)
    XA_STRING,
    8,
    PropModeReplace, // mode
    (unsigned char*)title.getElements(),
    title.getLength()
  );
#endif // flavor
}

void WindowImpl::setPosition(
  const Position& position) throw(UserInterfaceException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (position != this->position) {
    // TAG: take into account difference between upper left corner of window and client area
    bassert(
      ::MoveWindow(
        (HWND)drawableHandle,
        position.getX(),
        position.getY(),
        dimension.getWidth(),
        dimension.getHeight(),
        true // repaint
      ),
      UserInterfaceException(this)
    );
  }
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  XWindowChanges changes;
  changes.x = position.getX();
  changes.y = position.getY();
  ::XConfigureWindow(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    CWX|CWY,
    &changes
  );
#endif // flavor
}

void WindowImpl::setDimension(const Dimension& dimension) throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (dimension != this->dimension) {
    bassert(
      ::MoveWindow(
        (HWND)drawableHandle,
        position.getX(),
        position.getY(),
        dimension.getWidth(),
        dimension.getHeight(),
        true // repaint
      ),
      UserInterfaceException(this)
    );
  }
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  XWindowChanges changes;
  changes.width = dimension.getWidth();
  changes.height = dimension.getHeight();
  ::XConfigureWindow(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    CWWidth|CWHeight,
    &changes
  );
#endif // flavor
}

// Region WindowImpl::getRegion() const throw(UserInterfaceException) {
// #if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
// #else // unix
// #endif // flavor
// }

void WindowImpl::setRegion(
  const Position& position,
  const Dimension& dimension) throw(UserInterfaceException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(
    ::MoveWindow(
      (HWND)drawableHandle,
      position.getX(),
      position.getY(),
      dimension.getWidth(),
      dimension.getHeight(),
      true // repaint
    ),
    UserInterfaceException(this)
  );
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  XWindowChanges changes;
  changes.x = position.getX();
  changes.y = position.getY();
  changes.width = dimension.getWidth();
  changes.height = dimension.getHeight();
  ::XConfigureWindow(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    CWX|CWY|CWWidth|CWHeight,
    &changes
  );
#endif // flavor
}

void WindowImpl::setCursor(Cursor cursor) throw(UserInterfaceException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
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
    HCURSOR handle;
    if (cursor == WindowImpl::NONE) {
      static const BYTE andPlane[1] = {0xff};
      static const BYTE xorPlane[1] = {0x00};
      handle = ::CreateCursor( // TAG: must be stored in static cache windowImpl::blankCursor?
        (HINSTANCE)::GetModuleHandle(0), // instance
        0, // x
        0, // y
        1, // width
        1, // height
        andPlane,
        xorPlane
      );
    } else {
      handle = (HCURSOR)::LoadImage( // no need to destroy
        0,
        MAKEINTRESOURCE(NATIVE_CURSORS[cursor]),
        IMAGE_CURSOR,
        0,
        0,
        LR_DEFAULTCOLOR|LR_SHARED
      );
    }
    
    ::SetClassLongPtr((HWND)drawableHandle, GCLP_HCURSOR, (LONG_PTR)handle);
    POINT point;
    ::GetCursorPos(&point);
    ::SetCursorPos(point.x, point.y);
    this->cursor = cursor;
  }
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
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
      Pixmap pixmap = ::XCreateBitmapFromData((Display*)displayHandle, (::Window)drawableHandle, &data, 1, 1);
      XColor color;
      nativeCursor = ::XCreatePixmapCursor((Display*)displayHandle, pixmap, pixmap, &color, &color, 0, 0);
      ::XFreePixmap((Display*)displayHandle, pixmap);
    } else {
      nativeCursor = ::XCreateFontCursor((Display*)displayHandle, NATIVE_GLYPH[cursor]);
    }
    ::XDefineCursor((Display*)displayHandle, (::Window)drawableHandle, nativeCursor);
    ::XFlush((Display*)displayHandle);
    this->cursor = cursor;
  }
#endif // flavor
}

// TAG: display, screen, or other window?
Position WindowImpl::toGlobalPosition(
  const Position& position) const throw(UserInterfaceException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return this->position + position;
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  int x = 0;
  int y = 0;
  ::Window child;
  Bool result = ::XTranslateCoordinates(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    DefaultRootWindow((Display*)displayHandle),
    position.getX(),
    position.getY(),
    &x,
    &y,
    &child
  );
  return Position(x, y);
#else
  return Position();
#endif // flavor
}

// TAG: global or window position
Position WindowImpl::getCursorPosition() const throw(UserInterfaceException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  POINT point;
  ::GetCursorPos(&point);
  return Position(point.x, point.y);
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  int rootX = 0;
  int rootY = 0;
  int x = 0;
  int y = 0;
  unsigned int nativeState = 0;
  ::Window root;
  ::Window child;
  Bool result = ::XQueryPointer(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    &root,
    &child,
    &rootX,
    &rootY,
    &x,
    &y,
    &nativeState
  );
  return Position(x, y);
#else
  return Position();
#endif // flavor
}

// TAG: relative to: display, screen, window, or current position
// TAG: clip region support?
void WindowImpl::setCursorPosition(const Position& position) throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::SetCursorPos(position.getX(), position.getY()); // screen coordinates
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  ::XWarpPointer(
    (Display*)displayHandle,
    None,
    (::Window)drawableHandle,
    0,
    0,
    0,
    0,
    position.getX(),
    position.getY()
  );
#endif // flavor
}

void WindowImpl::releaseCursorConfinement() throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(::ClipCursor(0), UserInterfaceException(this));
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  ::XUngrabPointer((Display*)displayHandle, CurrentTime);
#endif // flavor
}

// void WindowImpl::getCursorConfinement() throw() {
//   RECT rect;
//   ::GetClipCursor(&rect);
//   return Region(Position(rect.left, rect.top), Dimension(rect.right - rect.left + 1, rect.bottom - rect.top + 1));
// }

void WindowImpl::setCursorConfinement() throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  POINT offset;
  offset.x = 0;
  offset.y = 0;
  ::ClientToScreen((HWND)drawableHandle, &offset);
  RECT rect;  
  bassert(
    ::GetClientRect((HWND)drawableHandle, &rect),
    UserInterfaceException(this)
  );
  rect.left += offset.x;
  rect.top += offset.y;
  rect.right += offset.x;
  rect.bottom += offset.y;
  bassert(::ClipCursor(&rect), UserInterfaceException(this));
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  int result = ::XGrabPointer(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    False,
    PointerMotionMask|PointerMotionHintMask|ButtonPressMask|ButtonReleaseMask, // TAG: fixme
    GrabModeAsync, // pointer mode
    GrabModeAsync, // keyboard mode
    (::Window)drawableHandle,
    None, // cursor // TAG: use current cursor
    CurrentTime
  );
#endif // flavor
}

void WindowImpl::setCursorConfinement(
  const Position& position,
  const Dimension& dimension) throw(UserInterfaceException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
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
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(
    ::SelectClipRgn((HDC)graphicsContextHandle, 0) != ERROR,
    UserInterfaceException(this)
  );
#else // unix
#endif // flavor
}

WindowImpl::Region WindowImpl::getClipping() const
  throw(UserInterfaceException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return Region(); // TAG: fixme
#else // unix
  return Region(); // TAG: fixme
#endif // flavor
}

void WindowImpl::setClipping(
  const Position& position,
  const Dimension& dimension) throw(UserInterfaceException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  RECT rect;
  rect.left = position.getX();
  rect.top = position.getY();
  rect.right = rect.left + dimension.getWidth() - 1;
  rect.bottom = rect.top + dimension.getHeight() - 1;
  HRGN region = ::CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
  bassert(::SelectClipRgn((HDC)graphicsContextHandle, region) != ERROR, UserInterfaceException(this));
#else // unix
#endif // flavor
}

void WindowImpl::close() throw(UserInterfaceException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (drawableHandle) {
    bassert(
      ::DestroyWindow((HWND)drawableHandle),
      UserInterfaceException(this)
    );
  }
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  if (drawableHandle) {
    XClientMessageEvent event;
    event.type = ClientMessage;
    event.window = (::Window)drawableHandle;
    event.message_type = windowImpl::protocolsAtom;
    event.format = 32;
    event.data.l[0] = windowImpl::atoms[DESTROY_MESSAGE];
    event.data.l[1] = CurrentTime; // TAG: is this ok
    ::XSendEvent(
      (Display*)displayHandle,
      (::Window)drawableHandle,
      False,
      0,
      (XEvent*)&event
    );
  }
#endif // flavor
}

bool WindowImpl::isChildOf(const WindowImpl& object) throw(UserInterfaceException)
{
  // TAG: fixme
  return false;
}

bool WindowImpl::isParentOf(
  const WindowImpl& object) throw(UserInterfaceException) {
  // TAG: fixme
  return false;
}

bool WindowImpl::isMaximized() throw(UserInterfaceException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return ::IsZoomed((HWND)drawableHandle) == TRUE;
#else // unix
  // TAG: if dimension == screen dimension && position = (0, 0) relative to screen
  return false; // TAG: fixme
#endif // flavor
}

bool WindowImpl::isMinimized() throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return ::IsIconic((HWND)drawableHandle) == TRUE;
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void WindowImpl::maximize() throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::ShowWindow((HWND)drawableHandle, SW_MAXIMIZE);
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  // store current position and dimension
  // what is the decoration frame size?
  int x = 0;
  int y = 0;
  ::Window child;
  Bool result = ::XTranslateCoordinates(
    (Display*)displayHandle,
    (::Window)drawableHandle,
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
    (::Window)drawableHandle,
    CWX|CWY|CWWidth|CWHeight,
    &changes
  );
#endif // flavor
}

void WindowImpl::minimize() throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::ShowWindow((HWND)drawableHandle, SW_MINIMIZE);
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  // WM_CHANGE_STATE ClientMessage event with a format of 32 and a first data element of IconicSlate
  ::XIconifyWindow(
   (Display*)displayHandle,
   (::Window)drawableHandle,
   0 // TAG: fixme screen_number
  );
#endif // flavor
}

void WindowImpl::normalize() throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::ShowWindow((HWND)drawableHandle, SW_RESTORE);
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  Position originalPosition = position; // TAG: fixme - must be attribute
  Dimension originalDimension = dimension; // TAG: fixme - must be attribute
  XWindowChanges changes;
  changes.x = originalPosition.getX();
  changes.y = originalPosition.getY();
  changes.width = originalDimension.getWidth();
  changes.height = originalDimension.getHeight();
  ::XConfigureWindow(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    CWX|CWY|CWWidth|CWHeight,
    &changes
  );
#endif // flavor
}

void WindowImpl::show() throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::ShowWindow((HWND)drawableHandle, SW_SHOW);
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  ::XMapWindow((Display*)displayHandle, (::Window)drawableHandle);
#endif // flavor
}

void WindowImpl::hide() throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::ShowWindow((HWND)drawableHandle, SW_HIDE);
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  ::XUnmapWindow((Display*)displayHandle, (::Window)drawableHandle);
#endif // flavor
}

void WindowImpl::enable() throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(
    ::EnableWindow((HWND)drawableHandle, TRUE),
    UserInterfaceException(this)
  );
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  // TAG: is there a better alternative
  ::XMapWindow((Display*)displayHandle, (::Window)drawableHandle);
#endif // flavor
}

void WindowImpl::disable() throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(
    ::EnableWindow((HWND)drawableHandle, FALSE),
    UserInterfaceException(this)
  );
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  // TAG: is there a better alternative
  ::XUnmapWindow((Display*)displayHandle, (::Window)drawableHandle);
#endif // flavor
}

void WindowImpl::raise() throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(
    ::SetForegroundWindow((HWND)drawableHandle),
    UserInterfaceException(this)
  );
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  ::XMapRaised((Display*)displayHandle, (::Window)drawableHandle);
  // TAG: what about focus
#endif // flavor
}

void WindowImpl::acquireFocus() throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(::SetFocus((HWND)drawableHandle), UserInterfaceException(this));
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  // TAG: fixme
  return;
  ::XSetInputFocus(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    RevertToParent,
    CurrentTime
  );
#endif // flavor
}

// TAG: wrong name; use hasCapture or isCapturing
bool WindowImpl::getCapture() const throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return ::GetCapture() == (HWND)drawableHandle;
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void WindowImpl::setCapture(bool state) throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (state) {
    ::SetCapture((HWND)drawableHandle);
  } else {
    ::ReleaseCapture();
  }
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  if (state) {
    int result = ::XGrabPointer(
      (Display*)displayHandle,
      (::Window)drawableHandle,
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
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
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
          throw UserInterfaceException("Unable to enter full screen mode.", this);
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
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
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

void WindowImpl::onDestruction() throw() {
}

void WindowImpl::onDisplay() throw() {
}

void WindowImpl::onMove(const Position& position) throw() {
}

void WindowImpl::onResize(const Dimension& dimension) throw() {
}

void WindowImpl::onMouseMove(
  const Position& position, unsigned int buttons) throw() {
}

void WindowImpl::onMouseScope(bool scope) throw() {
}

void WindowImpl::onMouseButton(
  const Position& position,
  Mouse::Button button,
  Mouse::Event event,
  unsigned int state) throw() {
}

void WindowImpl::onMouseWheel(
  const Position& position, int delta, unsigned int buttons) throw() {
}

void WindowImpl::onKey(
  unsigned int key, unsigned int flags, unsigned int modifiers) throw() {
}

void WindowImpl::onIdle() throw() {
  wait();
}

bool WindowImpl::onClose() throw() {
  return true; // allow window to be closed
}

void WindowImpl::onVisibility(Visibility visibility) throw() {
}

void WindowImpl::onFocus(Focus focus) throw()
{
}

void WindowImpl::onMenu(unsigned int identifier) throw()
{
  onCommand(identifier);
}

void WindowImpl::onCommand(unsigned int identifier) throw()
{
}

void WindowImpl::invalidate() throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(
    ::InvalidateRect((HWND)drawableHandle, 0, FALSE) != FALSE,
    UserInterfaceException(this)
  );
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  XExposeEvent event;
  event.type = Expose;
  event.x = 0;
  event.y = 0;
  event.width = dimension.getWidth();
  event.height = dimension.getHeight();
  event.count = 0;
  Status status = XSendEvent(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    False, // propagate,
    0, // mask,
    (XEvent*)&event // send
  );
#endif // flavor
}

void WindowImpl::update() throw(UserInterfaceException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // TAG: time this implementation and select faster choice (send WM_PAINT directly)
  bassert(
    ::UpdateWindow((HWND)drawableHandle),
    UserInterfaceException(this)
  );
#else // unix
  // TAG: fixme
  invalidate();
#endif // flavor
}

void WindowImpl::exit() throw()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::PostQuitMessage(0);
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  XClientMessageEvent event;
  event.type = ClientMessage;
  event.window = 0;
  event.message_type = windowImpl::protocolsAtom;
  event.format = 32;
  event.data.l[0] = windowImpl::atoms[QUIT_MESSAGE];
  event.data.l[1] = CurrentTime;  

  ::XPutBackEvent(
    (Display*)displayHandle,
    (XEvent*)&event
  );
#endif // flavor
}

void WindowImpl::wait() throw(UserInterfaceException)
{
  // Profiler::WaitTask profile("WindowImpl::wait()");
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::WaitMessage();
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  XEvent event;
  ::XPeekEvent((Display*)displayHandle, &event);
#endif // flavor
}

bool WindowImpl::wait(unsigned int milliseconds) throw(UserInterfaceException)
{
  // Profiler::WaitTask profile("WindowImpl::wait()");
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (milliseconds > 999999999) { // TAG: fixme
    milliseconds = 999999999;
  }
  return ::MsgWaitForMultipleObjects(0, 0, FALSE, INFINITE, QS_ALLEVENTS) != WAIT_TIMEOUT;
#else // unix
  // TAG: wait with timeout
  return false;
#endif // flavor
}

bool WindowImpl::openDispatch() throw(UserInterfaceException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  MSG msg;
  while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
    if (msg.message == WM_QUIT) {
      return false;
    } else {
      // if (!::TranslateAccelerator(drawableHandle, table, &msg)) {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
      // }
    }
  }
  return true;
#else // unix
  return true;
#endif // flavor
}

void WindowImpl::dispatch() throw(UserInterfaceException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  MSG msg;
  while (true) {
    if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        return;
      } else {
        // if (!::TranslateAccelerator(drawableHandle, table, &msg)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        // }
      }
    }	else {
      // TAG: for each window do onIdle()?
      //onIdle();
      ::WaitMessage(); // TAG: fixme
    }
  }
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  while (true) {
//     while (!::XPending((Display*)displayHandle)) {
//       // TAG: for each window do onIdle()?
//     }
    XEvent event;
    ::XNextEvent((Display*)displayHandle, &event);
    if ((event.type == ClientMessage) &&
        (event.xclient.data.l[0] == windowImpl::atoms[QUIT_MESSAGE])) {
      return;
    }
    WindowImpl* window =
      windowImpl::getWindow((void*)((XAnyEvent*)&event)->window);
    if (!window) {
      continue;
    }
    
    switch (event.type) {
    case ClientMessage:
      if (event.xclient.data.l[0] == windowImpl::atoms[DESTROY_MESSAGE]) {
        window->onDestruction();
        window->destroy();
      }
      break;
    case ConfigureNotify:
      {
        const XConfigureEvent* specificEvent =
          Cast::pointer<const XConfigureEvent*>(&event);
        const Dimension dimension(specificEvent->width, specificEvent->height);
        if (dimension != window->dimension) {
          window->dimension = dimension;
          window->onResize(dimension);
        }
        const Position position(specificEvent->x, specificEvent->y);
        if (position != window->position) {
          window->position = position;
          window->onMove(position);
        }
      }
      break;
    case Expose:
      {
        const XExposeEvent* specificEvent =
          Cast::pointer<const XExposeEvent*>(&event);
        // Region(Position(specificEvent->x, specificEvent->y), Dimension(specificEvent->width, specificEvent->height));
        window->onDisplay();
      }
      break;
    case FocusIn:
      window->onFocus(WindowImpl::ACQUIRED_FOCUS);
      break;
    case FocusOut:
      window->onFocus(WindowImpl::LOST_FOCUS);
      break;
    case ButtonPress:
    case ButtonRelease:
      {
        const XButtonEvent* specificEvent =
          Cast::pointer<const XButtonEvent*>(&event);
        const bool pressed = (event.type == ButtonPress);
        unsigned int state = 0;   
        if ((specificEvent->button == Button1) && pressed ||
            (specificEvent->button != Button1) &&
            (specificEvent->state & Button1Mask)) {
          state |= WindowImpl::Mouse::LEFT;
        }
        if ((specificEvent->button == Button2) && pressed ||
            (specificEvent->button != Button2) &&
            (specificEvent->state & Button2Mask)) {
          state |= WindowImpl::Mouse::MIDDLE;
        }
        if ((specificEvent->button == Button3) && pressed ||
            (specificEvent->button != Button3) &&
            (specificEvent->state & Button3Mask)) {
          state |= WindowImpl::Mouse::RIGHT;
        }
        if ((specificEvent->button == Button4) && pressed ||
            (specificEvent->button != Button4) &&
            (specificEvent->state & Button4Mask)) {
          state |= WindowImpl::Mouse::EXTRA;
        }
        if ((specificEvent->button == Button5) && pressed ||
            (specificEvent->button != Button5) &&
            (specificEvent->state & Button5Mask)) {
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
        window->onMouseButton(
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
        if (position == window->lastMousePosition) {
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
        window->lastMousePosition = position;
        window->onMouseMove(position, state);
      }
      break;
    case KeyPress:
    case KeyRelease:
      {
        unsigned int flags = 0;
        if (event.type == KeyPress) {
          flags |= WindowImpl::Key::PRESSED;
        }
        if (!window->autorepeat) {
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
        window->modifiers = modifiers;
      
        unsigned int code = 0;
        char buffer[2];
        int numberOfChars = ::XLookupString((XKeyEvent*)&event, buffer,  sizeof(buffer), 0, 0);
        switch (numberOfChars) {
        case 2:
          window->onKey((uint8)buffer[0], flags|WindowImpl::Key::DEAD|WindowImpl::Key::ASCII, modifiers);
          window->onKey((uint8)buffer[1], flags|WindowImpl::Key::ASCII, modifiers);
          code = 1; // TAG: clean up
          break;
        case 1:
          window->onKey((uint8)buffer[0], flags|WindowImpl::Key::ASCII, modifiers);
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
          window->onKey(code, flags, modifiers);
        }
      }
      break;
    case EnterNotify:
      window->scope = true;
      window->onMouseScope(true);
      break;
    case LeaveNotify:
      window->scope = false;
      window->onMouseScope(false);
      break;
    case MapNotify:
      if (WindowImpl::VISIBLE != window->visibility) {
        window->visibility = WindowImpl::VISIBLE;
        window->onVisibility(WindowImpl::VISIBLE);
      }
      break;
    case UnmapNotify:
      // TAG: I get no VisibilityNotify after UnmapNotify with X-Win32
      if (WindowImpl::INVISIBLE != window->visibility) {
        window->visibility = WindowImpl::INVISIBLE;
        window->onVisibility(WindowImpl::INVISIBLE);
      }
      break;
    case VisibilityNotify:
      {
        // TAG: bug VisibilityUnobscured is prefixed with VisibilityPartiallyObscured for X-Win32
        const XVisibilityEvent* specificEvent =
          Cast::pointer<const XVisibilityEvent*>(&event);
        Visibility visibility;
        switch (specificEvent->state) {
        case VisibilityFullyObscured:
          visibility = WindowImpl::INVISIBLE;
          break;
        case VisibilityPartiallyObscured:
//           visibility = WindowImpl::OBSCURED;
//           break;
        case VisibilityUnobscured:
        default:
          visibility = WindowImpl::VISIBLE;
        }
        if (visibility != window->visibility) {
          window->visibility = visibility;          
          window->onVisibility(visibility);
        }
        window->onDisplay();
      }
      break;
    case GravityNotify:
      {
        const XGravityEvent* specificEvent =
          Cast::pointer<const XGravityEvent*>(&event);
        Position position(specificEvent->x, specificEvent->y);
        window->position = position;
        window->onMove(position);
      }
    case DestroyNotify:
      break;
    default:
      fout << "Event: unhandled" << ENDL;
    }
  }
#endif // flavor
}

bool WindowImpl::hasMouse() throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return ::GetSystemMetrics(SM_MOUSEPRESENT) == TRUE;
#else // unix
  return true; // TAG: fixme
#endif // flavor
}

unsigned int WindowImpl::getMouseButtons() throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
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
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  return Mouse::LEFT|Mouse::MIDDLE|Mouse::RIGHT|Mouse::EXTRA|Mouse::EXTRA2; // TAG: fixme
#else
  return 0;
#endif // flavor
}

unsigned int WindowImpl::getNumberOfMonitors() throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return ::GetSystemMetrics(SM_CMONITORS);
#else // unix
  return 1; // TAG: fixme
#endif // flavor
}

Dimension WindowImpl::getDisplayDimension() throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  unsigned int width = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
  unsigned int height = ::GetSystemMetrics(SM_CYVIRTUALSCREEN);
  return Dimension(width, height);
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  unsigned int width = ::XWidthOfScreen((Screen*)screenHandle);
  unsigned int height = ::XHeightOfScreen((Screen*)screenHandle);
  return Dimension(width, height);
#else
  return Dimension();
#endif // flavor
}

unsigned int WindowImpl::getMouseButtonIndex(Mouse::Button button) throw() {
  // keep out to date with WindowImpl::Mouse::Button
  switch (button) {
  case Mouse::MIDDLE:
    return 1;
  case Mouse::RIGHT:
    return 2;
  case Mouse::EXTRA:
    return 3;
  case Mouse::EXTRA2:
    return 4;
  case Mouse::WHEEL:
    return 5;
  case Mouse::LEFT:
  default:
    return 0;
  }
}

Literal WindowImpl::getMouseButtonName(Mouse::Button button) throw() {
  // keep out to date with WindowImpl::Mouse::Button
  switch (button) {
  case Mouse::MIDDLE:
    return Literal("MIDDLE");
  case Mouse::RIGHT:
    return Literal("RIGHT");
  case Mouse::EXTRA:
    return Literal("EXTRA");
  case Mouse::EXTRA2:
    return Literal("EXTRA2");
  case Mouse::WHEEL:
    return Literal("WHEEL");
  case Mouse::LEFT:
  default:
    return Literal("LEFT");
  }
}

bool WindowImpl::isResponding(unsigned int milliseconds) throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD_PTR result = 0;
  LRESULT temp = ::SendMessageTimeout(
    (HWND)drawableHandle,
    WM_USER + WindowImpl::PING_MESSAGE,
    0,
    0,
    SMTO_NORMAL,
    minimum(milliseconds, 99999999U),
    &result
  );
  if (temp) {
    return result;
  }
  bassert(::GetLastError() == 0, UserInterfaceException(this));
  return false;
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  // TAG: use timeout period
  XClientMessageEvent event;
  event.type = ClientMessage;
  event.window = (::Window)drawableHandle;
  event.message_type = windowImpl::protocolsAtom;
  event.format = 32;
  event.data.l[0] = windowImpl::atoms[PING_MESSAGE];
  event.data.l[1] = CurrentTime;
  
  ::XSendEvent(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    False,
    0,
    (XEvent*)&event
  );
  return false; // TAG: fixme
#else
  return false;
#endif // flavor
}

WindowImpl::~WindowImpl() throw()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (drawableHandle) {
    ::DestroyWindow((HWND)drawableHandle);
  }
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  if (drawableHandle) {
    XClientMessageEvent event;
    event.type = ClientMessage;
    event.window = (::Window)drawableHandle;
    event.message_type = windowImpl::protocolsAtom;
    event.format = 32;
    event.data.l[0] = windowImpl::atoms[DESTROY_MESSAGE];
    event.data.l[1] = CurrentTime; // TAG: is this ok
    
    ::XSendEvent(
      (Display*)displayHandle,
      (::Window)drawableHandle,
      False,
      0,
      (XEvent*)&event
    );
    // TAG: must block until destruction has completed
  }
#endif // flavor
  // TAG: dispatch until window is destroyed
  loadModule(false); // never fails
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
