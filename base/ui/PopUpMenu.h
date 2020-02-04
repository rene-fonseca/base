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

#include <base/ui/PopUpWindow.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if 0

class MenuItem;

#if 0
class _COM_AZURE_DEV__BASE__API MenuItem : public ReferenceCountedObject {
public:
  
  static const unsigned int MINIMUM_WIDTH = 1;
  static const unsigned int MINIMUM_HEIGHT = 1;
  static const unsigned int LEFT_MARGIN = 32;
  static const unsigned int RIGHT_MARGIN = 32;
  
  inline MenuItem() noexcept {
  }
  
  virtual Dimension getMinimumSize() const noexcept = 0;
  
  virtual Dimension getPreferredSize() const noexcept = 0;
  
  // need to know to the final dimension of the context
  // need to know the graphics context
  virtual void onDisplay() noexcept;
};

class _COM_AZURE_DEV__BASE__API SeparatorMenuItem : public MenuItem {
public:

  /** The vertical margin. */
  static const unsigned int VERTICAL_MARGIN = 4;
  
  inline SeparatorMenuItem() noexcept {
  }
  
  Dimension getMinimumSize() const noexcept {
    return Dimension(MINIMUM_WIDTH, maximum(MINIMUM_HEIGHT, 2 * VERTICAL_MARGIN + 2));
  }
  
  Dimension getPreferredSize() const noexcept {
    return Dimension(MINIMUM_WIDTH, maximum(MINIMUM_HEIGHT, 2 * VERTICAL_MARGIN + 2));
  }
  
  void onDisplay(GraphicsContext& graphicsContext) noexcept {
    // dark line at middle (-1)
    // light line at middle (+1)
  }
};

class _COM_AZURE_DEV__BASE__API BitmapMenuItem : public MenuItem {
private:
  
  bool grayed = false;
  Bitmap bitmap;
public:
  
  inline BitmapMenuItem() noexcept : grayed(false) {
  }
  
  inline bool isGrayed() const noexcept {
    return grayed;
  }
  
  inline void gray() noexcept {
    grayed = true;
  }
  
  inline void ungray() noexcept {
    grayed = false;
  }
  
  inline const Bitmap& getBitmap() const noexcept {
    return bitmap;
  }
  
  inline void setBitmap(const Bitmap& bitmap) noexcept {
    this->bitmap = bitmap;
  }
  
  Dimension getMinimumSize() const noexcept {
    return Dimension(1, 1);
  }
  
  Dimension getPreferredSize() const noexcept {
    return Dimension(1, 1);
  }
  
  // need to know to the final dimension of the context
  // need to know the graphics context
  void onDisplay(GraphicsContext& graphicsContext) noexcept {
    // if valid bitmap draw it
    Position position(0, 0);
    Dimension dimension(32, 32);
    graphicsContext.drawBitmap(position, dimension, bitmap);
  }
};

// do not add ScrollMenuItem

class _COM_AZURE_DEV__BASE__API CommandMenuItem : public BitmapMenuItem {
private:

  unsigned int command = 0;
public:
  
  inline CommandMenuItem() noexcept {
  }
  
  inline CommandMenuItem(unsigned int _command) noexcept 
    : command(_command) {
  }
  
  inline unsigned int getCommand() const noexcept {
    return command;
  }
  
  inline void setCommand(unsigned int command) noexcept {
    this->command = command;
  }
};

class _COM_AZURE_DEV__BASE__API TextMenuItem : public CommandMenuItem {
public:

  enum Alignment {
    LEFT,
    CENTER,
    RIGHT
  };

  static const unsigned int IN_BETWEEN_MARGIN = 16;
private:

  Alignment alignment;
  String text;
  String shortcut;
public:

  inline TextMenuItem() noexcept : alignment(LEFT) {
  }
  
  inline TextMenuItem(unsigned int command, const String& _text, const String& _shortcut) noexcept
    : alignment(LEFT),
      CommandMenuItem(command),
      text(_text),
      shortcut(_shortcut) {
  }
  
  inline Alignment getAlignment() const noexcept {
    return alignment;
  }

  inline void setAlignment(Alignment alignment) noexcept {
    this->alignment = alignment;
  }
  
  inline const String& getText() const noexcept {
    return text;
  }
  
  inline void setText(const String& text) noexcept {
    this->text = text;
  }

  inline const String& getShortcut() const noexcept {
    return shortcut;
  }

  inline void setShortcut(const String& shortcut) noexcept {
    this->shortcut = shortcut;
  }

  Dimension getMinimumSize() const noexcept {
    return Dimension(0, 0);
  }
  
  Dimension getPreferredSize() const noexcept {
    // if has shortcut
    // width of text + width of shortcut
    // height of (text and shortcut)
    return Dimension(0 + IN_BETWEEN_MARGIN + 0, 0);
  }
  
  void onDisplay() noexcept {
    // if shortcut
  }
};

class _COM_AZURE_DEV__BASE__API CheckableMenuItem : public TextMenuItem {
private:

  bool checked = false;
public:

  inline CheckableMenuItem() noexcept : checked(false) {
  }
  
  inline bool isChecked() const noexcept {
    return checked;
  }

  inline void check() noexcept {
    checked = true;
  }
  
  inline void uncheck() noexcept {
    checked = false;
  }
  
  void onDisplay() noexcept {
    // draw checkmark bitmap
  }
};

class _COM_AZURE_DEV__BASE__API RadioMenuItem : public TextMenuItem {
private:

  bool selected = false;
public:

  inline RadioMenuItem() noexcept : selected(false) {
  }

  inline bool isSelected() const noexcept {
    return selected;
  }

  inline void select() noexcept {
    selected = true;
  }

  inline void deselect() noexcept {
    selected = false;
  }

  void onDisplay() noexcept {
    // draw radio bitmap
  }
};

class _COM_AZURE_DEV__BASE__API SelectionMenuItem : public MenuItem {
public:
  
  // Array<Bitmap>
  // set maximum number of columns
};

class _COM_AZURE_DEV__BASE__API IndependentMenuItem : public MenuItem {
public:

  void onDisplay() noexcept {
  }
};

class _COM_AZURE_DEV__BASE__API EditMenuItem : public IndependentMenuItem {
private:
  
  String text;
public:
  
  EditMenuItem() noexcept {
  }
  
  EditMenuItem(const String& _text) noexcept : text(_text) {
  }
  
  inline const String& getText() const noexcept {
    return text;
  }
  
  inline void setText(const String& text) noexcept {
    this->text = text;
  }
};

// TAG: need MenuItem with its own graphics context -> child window of pop up menu window
#endif

/**
  Pop up window.
  
  @short Pop up window.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API PopUpMenu : public GraphicsContext {
private:
  
  /** The menu items. */
  //Array<MenuItemReference> items;
  
  /**
    Releases the rendering context.
  */
  void destroy() noexcept;

  /**
    Invoked by the menu item.
  */
  void onMenuItem(MenuItem* item) noexcept { // user is responsible for invoking update method
  }
public:
  
  // TAG: need MenuItemCollection class
  class _COM_AZURE_DEV__BASE__API MenuItemCollection : public Array {
  public:
  };
  
  /**
    Returns a reference to the menu item at the specified position.
  */
  MenuItemReference getItem(unsigned int index) const {
    return items[index];
  }

  /**
    Sets the menu item at the specified position.
  */
  void setItem(unsigned int index) noexcept {
    items[index] = item;
  }

  /**
    Adds the menu item to the end of the item container.
  */
  void addItem(const MenuItem& item) noexcept {
    items.add(item);
  }

  /**
    Returns the number of menu items.
  */
  unsigned int getNumberOfItems() const noexcept {
    return items.getSize();
  }
  
  void onChange() noexcept;
  
  /**
    Initializes a pop up window.

    @param flags The flags.
  */
  PopUpMenu(unsigned int flags);
  
  /**
    Initializes a pop up window.

    @param position The initial position.
    @param flags The flags.
  */
  PopUpMenu(const Position& position, unsigned int flags);
  
  void track(const Position& position)
  {
    // translate mouse position to window position
  }
  
  // TAG: the position of the
  void onChange() noexcept
  {
    Array<MenuItemReference>::ReadEnumerator enu = items.getReadEnumerator();
    const Dimension screenSize; // TAG: getScreen...
    Dimension size;
    Dimension preferredSize;
    Dimension minimumSize;
    Dimension desiredSize;
    while (enu.hasNext()) {
      MenuItemReference item = enu.next();
      preferredSize.expand(item.getPreferredSize());
      minimumSize.expand(item.getMinimumSize());
      // vertical
      // horizontal
      // desiredSize 
    }
    // reduce

    finalSize = desiredSize.expand(minimumSize);
  }

  void onScrollUp() noexcept {
    if (firstVisibleItem != firstItem) {
      --firstVisibleItem;
      --lastVisibleItem;
    }
    // recalc();
  }
  
  void onScrollDown() noexcept {
    if (lastVisibleItem != lastItem) {
      ++firstVisibleItem;
      ++lastVisibleItem;
    }
    // recalc();
  }
  
  void onMouseButton() noexcept {
  }
  
  void onDisplay() noexcept {
    clear();
    
    // if scrolling then draw scroll up item - if at top then grayed
    // from first to last item: draw item
    // if item is child then item->onDisplay() otherwise item->onDisplay(...)?
    // if scrolling then draw scroll down item - if at bottom then grayed
  }
};

class _COM_AZURE_DEV__BASE__API UserInterfaceStyle : public Object {
public:
  
  virtual Color getMenuBackground() const noexcept;
  virtual Color getMenuHighlightColor() const noexcept;
  virtual Color getMenuMarginColor() const noexcept;
  virtual Color getMenuTextColor() const noexcept;
  virtual Dimension getMenuBitmapSize() const noexcept;
  virtual Bitmap getCheckedBitmap() const noexcept;
  virtual Bitmap getSelectedBitmap() const noexcept;
};

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
