/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/collection/Array.h>
#include <base/io/Handle.h>
#include <base/mem/Reference.h>
#include <base/string/String.h>
#include <base/ui/UserInterfaceException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

  /**
    Indicates a menu exception.
    
    @short Menu user interface exception
    @ingroup exceptions ui
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  
  class MenuException : public UserInterfaceException {
  public:
    
    /**
      Initializes the exception object with no message.
    */
    inline MenuException() throw() {
    }
    
    /**
      Initializes the exception object.
      
      @param message The message.
    */
    inline MenuException(const char* message) throw() : UserInterfaceException(message) {
    }
    
    /**
      Initializes the exception object without an associated message.
      
      @param type The identity of the type.
    */
    inline MenuException(Type type) throw() : UserInterfaceException(type) {
    }
  
    /**
      Initializes the exception object.
      
      @param message An NULL-terminated string.
      @param type The identity of the type.
    */
    inline MenuException(const char* message, Type type) throw() : UserInterfaceException(message, type) {
    }
  };

/**
  Menu.
  
  @short Menu.
  @ingroup ui
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Menu : public Object {
private:

  class MenuHandle : public ReferenceCountedObject {
  private:

    /** Handle to resource. */
    OperatingSystem::Handle handle;
    
    /* Disable the default copy constructor. */
    MenuHandle(const MenuHandle& copy) throw();
    /* Disable the default assignment operator. */
    MenuHandle& operator=(const MenuHandle& eq) throw();
  public:

    /**
      Initializes an invalid handle.
    */
    inline MenuHandle() throw() : handle(OperatingSystem::INVALID_HANDLE) {
    }
    
    /**
      Initializes handle using the specified value.
    */
    explicit inline MenuHandle(OperatingSystem::Handle _handle) throw() : handle(_handle) {
    }
    
    /**
      Returns the handle.
    */
    inline OperatingSystem::Handle getHandle() const throw() {
      return handle;
    }
    
    /**
      Returns true if the handle is valid.
    */
    inline bool isValid() const throw() {
      return handle != OperatingSystem::INVALID_HANDLE;
    }
    
    /**
      Releases the resources used by the menu.
    */
    virtual ~MenuHandle() throw(MenuException);
  };

  /** The opaque handle to the menu. */
  Reference<MenuHandle> handle;
public:

  enum Flag {
    ENABLED = 1, /**< Enabled menu item. */
    CHECKED = 2, /**< Checked menu item. */
    HIGHLIGHTED = 4, /**< Highlighted menu item. */
    SEPARATOR = 8, /**< A separator. */
    STRING = 16, /**< A string menu item. */
    MENU = 32 /**< A menu item with a sub menu. */
  };
  
  /**
    Creates an empty menu.
  */
  Menu() throw(MenuException);

  /**
    Initializes menu from other menu.
  */
  inline Menu(const Menu& copy) throw() : handle(copy.handle) {
  }

  /**
    Assignment of menu from other menu.
  */
  inline Menu& operator=(const Menu& eq) throw() {
    handle = eq.handle;
    return *this;
  }

  /**
    Returns true if the menu is valid.
  */
  inline bool isValid() const throw() {
    return handle.isValid();
  }

  /**
    Returns the opaque menu handle.
  */
  inline OperatingSystem::Handle getHandle() const throw() {
//     if (!handle.isValid()) {
//       return OperatingSystem::INVALID_HANDLE;
//     }
    return handle->getHandle();
  }
  
  /**
    Returns the number of menu items.
  */
  unsigned int getNumberOfItems() const throw(MenuException);

  /**
    Returns the identifier of the specified menu item.
    
    @param index The index of the menu item.
  */
  unsigned int getIdentifier(unsigned int index) const throw(MenuException);

  /**
    Returns the index of the specified menu item.
    
    @param identifier The identifier of the menu item.
  */
  int getIndex(unsigned int identifier) const throw(MenuException);

  /**
    Returns the flags describing the menu item.

    @param index The index of the menu item.
  */
  unsigned int getFlags(unsigned int index) const throw(MenuException);

  /**
    Returns true if the menu item is a separator.
  */
  inline bool isSeparator(unsigned int index) const throw(MenuException) {
    return getFlags(index) & SEPARATOR;
  }

  /**
    Returns true if the menu item has a sub menu.

    @param index The index of the menu item.    
  */
  inline bool hasMenu(unsigned int index) const throw(MenuException) {
    return getFlags(index) & MENU;
  }
  
  /**
    Returns true if the menu item is enabled.

    @param index The index of the menu item.    
  */
  inline bool isEnabled(unsigned int identifier) const throw(MenuException) {
    return getFlags(identifier) & ENABLED;
  }

  /**
    Returns true if the menu item is checked.

    @param index The index of the menu item.    
  */
  inline bool isChecked(unsigned int identifier) const throw(MenuException) {
    return getFlags(identifier) & CHECKED;
  }

  /**
    Returns true if the menu item is highlighted.

    @param index The index of the menu item.    
  */
  inline bool isHighlighted(unsigned int identifier) const throw(MenuException) {
    return getFlags(identifier) & HIGHLIGHTED;
  }

  /**
    Returns the sub menu associated with the menu item.
    
    @param index The index of the menu item.    
  */
  Menu getMenu(unsigned int index) throw(MenuException);
  
  /**
    Sets the enable flag of the menu item.

    @param index The index of the menu item.
    @param enabled The value of the flag.
  */
  void setEnabled(unsigned int index, bool enabled) throw(MenuException);
  
  /**
    Sets the checked flag of the menu item.

    @param index The index of the menu item.
    @param enabled The value of the flag.
  */
  void setChecked(unsigned int index, bool checked) throw(MenuException);
  
  /**
    Sets the highlighted flag of the menu item.

    @param index The index of the menu item.
    @param enabled The value of the flag.
  */
  void setHighlighted(unsigned int index, bool highlighted) throw(MenuException);

  /**
    Enables the menu item.
    
    @param index The index of the menu item.
  */
  inline void enable(unsigned int index) throw(MenuException) {
    setEnabled(index, true);
  }

  /**
    Disables the menu item.
    
    @param index The index of the menu item.
  */
  inline void disable(unsigned int index) throw(MenuException) {
    setEnabled(index, false);
  }
  
  /**
    Checks the menu item.
    
    @param index The index of the menu item.
  */
  inline void check(unsigned int index) throw(MenuException) {
    setChecked(index, true);
  }
  
  /**
    Unchecks the menu item.
    
    @param index The index of the menu item.
  */
  inline void uncheck(unsigned int index) throw(MenuException) {
    setChecked(index, false);
  }
  
  /**
    Highlights the menu item.
    
    @param index The index of the menu item.
  */
  inline void highlight(unsigned int index) throw(MenuException) {
    setHighlighted(index, true);
  }
  
  /**
    Resets the highlight flag of the menu item.
    
    @param index The index of the menu item.
  */
  inline void unhighlight(unsigned int index) throw(MenuException) {
    setHighlighted(index, false);
  }

  /**
    Sets the string value of the menu item.

    @param index The index of the menu item.
    @param name The new string value of the menu item.
  */
  void setName(unsigned int index, const String& name) throw(MenuException);

  /**
    Sets the sub menu of the menu item.
    
    @param index The index of the menu item.
    @param menu The new sub menu the menu item.
  */
  void setSubmenu(unsigned int identifier, const Menu& menu) throw(MenuException);
  
  /**
    Inserts a separator at the specified index.
    
    @param index The index of the menu item.
  */
  void insertSeparator(unsigned int index) throw(MenuException);
  
  /**
    Appends a separator to the menu.    
  */
  void appendSeparator() throw(MenuException);

  /**
    Inserts a menu item at the specified index.

    @param index The index of the new menu item.
    @param name The string value of the menu item.
    @param identifier The identifier of the menu item.
    @param flags The flags of the menu item (a conbination of ENABLED, CHECKED, and HIGHLIGHTED).
  */
  void insert(unsigned int index, const String& name, unsigned int identifier, unsigned int flags = ENABLED) throw(MenuException);
  
  /**
    Appends a menu item to the menu.

    @param name The string value of the menu item.
    @param identifier The identifier of the menu item.
    @param flags The flags of the menu item (a conbination of ENABLED, CHECKED, and HIGHLIGHTED).
  */
  void append(const String& name, unsigned int identifier, unsigned int flags = ENABLED) throw(MenuException);
  
  /**
    Appends a sub menu to the menu.

    @param name The string value of the menu item.
    @param menu The sub menu.
    @param flags The flags of the menu item (a conbination of ENABLED, CHECKED, and HIGHLIGHTED).
  */
  void appendMenu(const String& name, const Menu& menu, unsigned int flags = ENABLED) throw(MenuException);

  /**
    Removes the menu item at the specified index from the menu.
     
    @param index The index of the menu item.
  */
  void remove(unsigned int index) throw(MenuException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
