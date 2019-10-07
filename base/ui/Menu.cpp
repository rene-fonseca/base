/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/ui/Menu.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#else // unix
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// TAG: check out menu flag MNS_MODELESS
Menu::MenuHandle::~MenuHandle() {
  if (isValid()) { // dont try to close if handle is invalidated
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    // TAG: only if unassigned to window
    if (!::DestroyMenu((HMENU)getHandle())) {
      throw MenuException("Unable to destroy menu", this);
    }
#else // unix
    // TAG: fixme
#endif // flavor
  }
}

Menu::Menu() throw(MenuException)
  : handle(new MenuHandle()) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HMENU handle = ::CreatePopupMenu();
  bassert(handle != 0, MenuException("Unable to create menu", this));
  this->handle = new MenuHandle((OperatingSystem::Handle)handle); // TAG: could raise exception
  bassert(this->handle.isValid(), MenuException("Unable to create menu", this));
#else // unix
  // TAG: fixme
#endif // flavor
}

unsigned int Menu::getNumberOfItems() const throw(MenuException) {
  if (!handle.isValid()) {
    return 0;
  }
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HMENU menu = (HMENU)handle->getHandle();
  int result = ::GetMenuItemCount(menu);
  bassert(result >= 0, MenuException(this));
  return result;
#else // unix
  // TAG: fixme
  return 0;
#endif // flavor
}

unsigned int Menu::getIdentifier(unsigned int index) const throw(MenuException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  bassert(index <= PrimitiveTraits<int>::MAXIMUM, MenuException(this));
  HMENU menu = (HMENU)handle->getHandle();
  MENUITEMINFO info;
  info.cbSize = sizeof(info);
  info.fMask = MIIM_ID;
  bassert(::GetMenuItemInfo(menu, index, TRUE, &info), MenuException(this));
  return info.wID;
#else // unix
  // TAG: fixme
  return 0;
#endif // flavor
}

int Menu::getIndex(unsigned int identifier) const throw(MenuException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HMENU menu = (HMENU)handle->getHandle();
//   MENUITEMINFO info;
//   info.cbSize = sizeof(info);
//   info.fMask = MIIM_ID;
//   bassert(::GetMenuItemInfo(menu, index, TRUE, &info), MenuException(this));
  int index = -1; // TAG: fixme
  return index;
#else // unix
  // TAG: fixme
  return 0;
#endif // flavor
}

unsigned int Menu::getFlags(unsigned int index) const throw(MenuException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HMENU menu = (HMENU)handle->getHandle();
  MENUITEMINFO info;
  info.cbSize = sizeof(info);
  info.fMask = MIIM_FTYPE | MIIM_STATE | MIIM_TYPE | MIIM_SUBMENU;
  info.dwTypeData = 0;
  bassert(::GetMenuItemInfo(menu, index, TRUE, &info), MenuException(this));
  unsigned int flags = 0;
  flags |= (info.fType & MFT_SEPARATOR) ? Menu::SEPARATOR : 0;
  flags |= (info.fType & MFT_STRING) ? Menu::STRING : 0;
  flags |= (info.fState & MFS_ENABLED) ? Menu::ENABLED : 0;
  flags |= (info.fState & MFS_CHECKED) ? Menu::CHECKED : 0;
  flags |= (info.fState & MFS_HILITE) ? Menu::HIGHLIGHTED : 0;
  flags |= info.hSubMenu ? Menu::MENU : 0;
  return flags;
#else // unix
  return 0;
#endif // flavor
}

Menu Menu::getMenu(unsigned int index) throw(MenuException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HMENU menu = (HMENU)handle->getHandle();
  MENUITEMINFO info;
  info.cbSize = sizeof(info);
  info.fMask = MIIM_SUBMENU;
  bassert(::GetMenuItemInfo(menu, index, TRUE, &info), MenuException(this));
  bassert(info.hSubMenu, MenuException(this));
  Menu result;
  result.handle = new MenuHandle((OperatingSystem::Handle)info.hSubMenu);
  return result;
#else // unix
  // TAG: fixme
  return Menu();
#endif // flavor
}

void Menu::setEnabled(unsigned int index, bool enabled) throw(MenuException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HMENU menu = (HMENU)handle->getHandle();
  MENUITEMINFO info;
  info.cbSize = sizeof(info);
  info.fMask = MIIM_STATE;
  info.fState = enabled ? MFS_ENABLED : MFS_DISABLED;
  bassert(::SetMenuItemInfo(menu, index, TRUE, &info), MenuException(this));
#else // unix
  // TAG: fixme
#endif // flavor
}

void Menu::setChecked(unsigned int index, bool checked) throw(MenuException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HMENU menu = (HMENU)handle->getHandle();
  MENUITEMINFO info;
  info.cbSize = sizeof(info);
  info.fMask = MIIM_STATE;
  info.fState = checked ? MFS_CHECKED : MFS_UNCHECKED;
  bassert(::SetMenuItemInfo(menu, index, TRUE, &info), MenuException(this));
#else // unix
  // TAG: fixme
#endif // flavor
}

void Menu::setHighlighted(unsigned int index, bool highlighted) throw(MenuException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HMENU menu = (HMENU)handle->getHandle();
  MENUITEMINFO info;
  info.cbSize = sizeof(info);
  info.fMask = MIIM_STATE;
  info.fState = highlighted ? MFS_HILITE : MFS_UNHILITE;
  bassert(::SetMenuItemInfo(menu, index, TRUE, &info), MenuException(this));
#else // unix
  // TAG: fixme
#endif // flavor
}

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)

// TAG: put in windows mapping header

class SystemString {
private:

  std::wstring buffer;
public:

  inline SystemString() {
  }

  inline SystemString(const String& s) : buffer(toWide(s)) {
  }

  inline SystemString(const std::string& s) : buffer(toWide(s)) {
  }

  inline operator wchar*() noexcept {
    return const_cast<wchar*>(buffer.c_str());
  }

  inline operator const wchar*() const noexcept {
    return buffer.c_str();
  }
};
#endif

void Menu::setName(unsigned int index, const String& name) throw(MenuException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HMENU menu = (HMENU)handle->getHandle();
  MENUITEMINFO info;
  info.cbSize = sizeof(info);
  info.fMask = MIIM_FTYPE | MIIM_STRING;
  info.fType = MFT_STRING;
  SystemString _name(name);
  info.dwTypeData = _name;
  bassert(::SetMenuItemInfo(menu, index, TRUE, &info), MenuException(this));
#else // unix
   // TAG: fixme 
#endif // flavor
}

void Menu::setSubmenu(unsigned int identifier, const Menu& menu) throw(MenuException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HMENU nativeMenu = (HMENU)handle->getHandle();
  MENUITEMINFO info;
  info.cbSize = sizeof(info);
  info.fMask = MIIM_SUBMENU;
  info.hSubMenu = (HMENU)menu.getHandle(); // TAG: need  to get lock on menu
  bassert(::SetMenuItemInfo(nativeMenu, identifier, FALSE, &info), MenuException(this));
#else // unix
  // TAG: fixme
#endif // flavor
}

void Menu::insertSeparator(unsigned int index) throw(MenuException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HMENU menu = (HMENU)handle->getHandle();
  MENUITEMINFO info;
  info.cbSize = sizeof(info);
  info.fMask = MIIM_FTYPE;
  info.fType = MFT_SEPARATOR;
  bassert(::InsertMenuItem(menu, index, TRUE, &info), MenuException(this));
#else // unix
  // TAG: fixme
#endif // flavor
}

void Menu::appendSeparator() throw(MenuException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HMENU menu = (HMENU)handle->getHandle();
  int index = ::GetMenuItemCount(menu);
  bassert(index >= 0, MenuException(this));
  MENUITEMINFO info;
  info.cbSize = sizeof(info);
  info.fMask = MIIM_FTYPE;
  info.fType = MFT_SEPARATOR;
  bassert(::InsertMenuItem(menu, index, TRUE, &info), MenuException(this));
#else // unix
  // TAG: fixme
#endif // flavor
}

void Menu::insert(unsigned int index, const String& name, unsigned int identifier, unsigned int flags) throw(MenuException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  bassert(identifier <= PrimitiveTraits<uint16>::MAXIMUM, MenuException(this));
  HMENU menu = (HMENU)handle->getHandle();
  MENUITEMINFO info;
  info.cbSize = sizeof(info);
  info.fMask = MIIM_FTYPE | MIIM_STATE | MIIM_ID | MIIM_STRING;
  info.fType = MFT_STRING;
  info.fState = 0;
  info.fState |= (flags & Menu::ENABLED) ? MFS_ENABLED : MFS_DISABLED;
  info.fState |= (flags & Menu::CHECKED) ? MFS_CHECKED : MFS_UNCHECKED;
  info.fState |= (flags & Menu::HIGHLIGHTED) ? MFS_HILITE : MFS_UNHILITE;
  info.wID = identifier;
  SystemString _name(name);
  info.dwTypeData = _name;
  bassert(::InsertMenuItem(menu, index, TRUE, &info), MenuException(this));
#else // unix
  // TAG: fixme
#endif // flavor
}

void Menu::append(const String& name, unsigned int identifier, unsigned int flags) throw(MenuException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  bassert(identifier <= PrimitiveTraits<uint16>::MAXIMUM, MenuException(this));
  HMENU menu = (HMENU)handle->getHandle();
  int index = ::GetMenuItemCount(menu);
  bassert(index >= 0, MenuException(this));
  MENUITEMINFO info;
  info.cbSize = sizeof(info);
  info.fMask = MIIM_FTYPE | MIIM_STATE | MIIM_ID | MIIM_STRING;
  info.fType = MFT_STRING;
  info.fState = 0;
  info.fState |= (flags & Menu::ENABLED) ? MFS_ENABLED : MFS_DISABLED;
  info.fState |= (flags & Menu::CHECKED) ? MFS_CHECKED : MFS_UNCHECKED;
  info.fState |= (flags & Menu::HIGHLIGHTED) ? MFS_HILITE : MFS_UNHILITE;
  info.wID = identifier;
  SystemString _name(name);
  info.dwTypeData = _name;
  bassert(::InsertMenuItem(menu, index, TRUE, &info), MenuException(this));
#else // unix
  // TAG: fixme
#endif // flavor
}

void Menu::appendMenu(const String& name, const Menu& menu, unsigned int flags) throw(MenuException) {
  // TAG: get lock on menu
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HMENU nativeMenu = (HMENU)handle->getHandle();
  int index = ::GetMenuItemCount(nativeMenu);
  bassert(index >= 0, MenuException(this));
  MENUITEMINFO info;
  info.cbSize = sizeof(info);
  info.fMask = MIIM_FTYPE | MIIM_STATE | MIIM_STRING | MIIM_SUBMENU;
  info.fType = MFT_STRING;
  info.fState = 0;
  info.fState |= (flags & Menu::ENABLED) ? MFS_ENABLED : MFS_DISABLED;
  info.fState |= (flags & Menu::CHECKED) ? MFS_CHECKED : MFS_UNCHECKED;
  info.fState |= (flags & Menu::HIGHLIGHTED) ? MFS_HILITE : MFS_UNHILITE;
  info.hSubMenu = (HMENU)menu.getHandle(); // TAG: need to get lock on menu
  const char* temp = name.getElements();
  info.dwTypeData = (char*)temp;
  bassert(::InsertMenuItem(nativeMenu, index, TRUE, &info), MenuException(this));
#else // unix
  // TAG: fixme
#endif // flavor
}

void Menu::remove(unsigned int index) throw(MenuException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HMENU menu = (HMENU)handle->getHandle();
  bassert(::DeleteMenu(menu, index, MF_BYPOSITION), MenuException(this));
#else // unix
  // TAG: fixme
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
