/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

  /**
    Indicates a menu exception.
    
    @short Menu user interface exception
    @ingroup exceptions ui
    @version 1.0
  */
  
  class _COM_AZURE_DEV__BASE__API MenuException : public UserInterfaceException {
  public:
    
    /**
      Initializes the exception object with no message.
    */
    inline MenuException() noexcept {
    }
    
    /**
      Initializes the exception object.
      
      @param message The message.
    */
    inline MenuException(const char* message) noexcept : UserInterfaceException(message) {
    }
    
    /**
      Initializes the exception object without an associated message.
      
      @param type The identity of the type.
    */
    inline MenuException(const Type& type) noexcept : UserInterfaceException(type) {
    }
  
    /**
      Initializes the exception object.
      
      @param message An NULL-terminated string.
      @param type The identity of the type.
    */
    inline MenuException(const char* message, const Type& type) noexcept : UserInterfaceException(message, type) {
    }
    
    _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
  };

/**
  Menu.
  
  @short Menu.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Menu : public Object {
private:

  class _COM_AZURE_DEV__BASE__API MenuHandle : public ReferenceCountedObject {
  private:

    /** Handle to resource. */
    OperatingSystem::Handle handle;
    
    /* Disable the default copy constructor. */
    MenuHandle(const MenuHandle& copy) noexcept;
    /* Disable the default assignment operator. */
    MenuHandle& operator=(const MenuHandle& assign) noexcept;
  public:

    /**
      Initializes an invalid handle.
    */
    inline MenuHandle() noexcept : handle(OperatingSystem::INVALID_HANDLE) {
    }
    
    /**
      Initializes handle using the specified value.
    */
    explicit inline MenuHandle(OperatingSystem::Handle _handle) noexcept : handle(_handle) {
    }
    
    /**
      Returns the handle.
    */
    inline OperatingSystem::Handle getHandle() const noexcept {
      return handle;
    }
    
    /**
      Returns true if the handle is valid.
    */
    inline bool isValid() const noexcept {
      return handle != OperatingSystem::INVALID_HANDLE;
    }
    
    /**
      Releases the resources used by the menu.
    */
    virtual ~MenuHandle();
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
  Menu();

  /**
    Initializes menu from other menu.
  */
  inline Menu(const Menu& copy) noexcept : handle(copy.handle) {
  }

  /**
    Assignment of menu from other menu.
  */
  inline Menu& operator=(const Menu& assign) noexcept {
    handle = assign.handle;
    return *this;
  }

  /**
    Returns true if the menu is valid.
  */
  inline bool isValid() const noexcept {
    return handle.isValid();
  }

  /**
    Returns the opaque menu handle.
  */
  inline OperatingSystem::Handle getHandle() const noexcept {
//     if (!handle.isValid()) {
//       return OperatingSystem::INVALID_HANDLE;
//     }
    return handle->getHandle();
  }
  
  /**
    Returns the number of menu items.
  */
  unsigned int getNumberOfItems() const;

  /**
    Returns the identifier of the specified menu item.
    
    @param index The index of the menu item.
  */
  unsigned int getIdentifier(unsigned int index) const;

  /**
    Returns the index of the specified menu item.
    
    @param identifier The identifier of the menu item.
  */
  int getIndex(unsigned int identifier) const;

  /**
    Returns the flags describing the menu item.

    @param index The index of the menu item.
  */
  unsigned int getFlags(unsigned int index) const;

  /**
    Returns true if the menu item is a separator.
  */
  inline bool isSeparator(unsigned int index) const {
    return getFlags(index) & SEPARATOR;
  }

  /**
    Returns true if the menu item has a sub menu.

    @param index The index of the menu item.    
  */
  inline bool hasMenu(unsigned int index) const {
    return getFlags(index) & MENU;
  }
  
  /**
    Returns true if the menu item is enabled.

    @param index The index of the menu item.    
  */
  inline bool isEnabled(unsigned int identifier) const {
    return getFlags(identifier) & ENABLED;
  }

  /**
    Returns true if the menu item is checked.

    @param index The index of the menu item.    
  */
  inline bool isChecked(unsigned int identifier) const {
    return getFlags(identifier) & CHECKED;
  }

  /**
    Returns true if the menu item is highlighted.

    @param index The index of the menu item.    
  */
  inline bool isHighlighted(unsigned int identifier) const {
    return getFlags(identifier) & HIGHLIGHTED;
  }

  /**
    Returns the sub menu associated with the menu item.
    
    @param index The index of the menu item.    
  */
  Menu getMenu(unsigned int index);
  
  /**
    Sets the enable flag of the menu item.

    @param index The index of the menu item.
    @param enabled The value of the flag.
  */
  void setEnabled(unsigned int index, bool enabled);
  
  /**
    Sets the checked flag of the menu item.

    @param index The index of the menu item.
    @param enabled The value of the flag.
  */
  void setChecked(unsigned int index, bool checked);
  
  /**
    Sets the highlighted flag of the menu item.

    @param index The index of the menu item.
    @param enabled The value of the flag.
  */
  void setHighlighted(unsigned int index, bool highlighted);

  /**
    Enables the menu item.
    
    @param index The index of the menu item.
  */
  inline void enable(unsigned int index) {
    setEnabled(index, true);
  }

  /**
    Disables the menu item.
    
    @param index The index of the menu item.
  */
  inline void disable(unsigned int index) {
    setEnabled(index, false);
  }
  
  /**
    Checks the menu item.
    
    @param index The index of the menu item.
  */
  inline void check(unsigned int index) {
    setChecked(index, true);
  }
  
  /**
    Unchecks the menu item.
    
    @param index The index of the menu item.
  */
  inline void uncheck(unsigned int index) {
    setChecked(index, false);
  }
  
  /**
    Highlights the menu item.
    
    @param index The index of the menu item.
  */
  inline void highlight(unsigned int index) {
    setHighlighted(index, true);
  }
  
  /**
    Resets the highlight flag of the menu item.
    
    @param index The index of the menu item.
  */
  inline void unhighlight(unsigned int index) {
    setHighlighted(index, false);
  }

  /**
    Sets the string value of the menu item.

    @param index The index of the menu item.
    @param name The new string value of the menu item.
  */
  void setName(unsigned int index, const String& name);

  /**
    Sets the sub menu of the menu item.
    
    @param index The index of the menu item.
    @param menu The new sub menu the menu item.
  */
  void setSubmenu(unsigned int identifier, const Menu& menu);
  
  /**
    Inserts a separator at the specified index.
    
    @param index The index of the menu item.
  */
  void insertSeparator(unsigned int index);
  
  /**
    Appends a separator to the menu.    
  */
  void appendSeparator();

  /**
    Inserts a menu item at the specified index.

    @param index The index of the new menu item.
    @param name The string value of the menu item.
    @param identifier The identifier of the menu item.
    @param flags The flags of the menu item (a conbination of ENABLED, CHECKED, and HIGHLIGHTED).
  */
  void insert(unsigned int index, const String& name, unsigned int identifier, unsigned int flags = ENABLED);
  
  /**
    Appends a menu item to the menu.

    @param name The string value of the menu item.
    @param identifier The identifier of the menu item.
    @param flags The flags of the menu item (a conbination of ENABLED, CHECKED, and HIGHLIGHTED).
  */
  void append(const String& name, unsigned int identifier, unsigned int flags = ENABLED);
  
  /**
    Appends a sub menu to the menu.

    @param name The string value of the menu item.
    @param menu The sub menu.
    @param flags The flags of the menu item (a conbination of ENABLED, CHECKED, and HIGHLIGHTED).
  */
  void appendMenu(const String& name, const Menu& menu, unsigned int flags = ENABLED);

  /**
    Removes the menu item at the specified index from the menu.
     
    @param index The index of the menu item.
  */
  void remove(unsigned int index);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
