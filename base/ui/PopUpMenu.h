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

#include <base/ui/PopUpWindow.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if 0
class MenuItem : public ReferenceCountedObject {
public:
  
  static const unsigned int MINIMUM_WIDTH = 1;
  static const unsigned int MINIMUM_HEIGHT = 1;
  static const unsigned int LEFT_MARGIN = 32;
  static const unsigned int RIGHT_MARGIN = 32;
  
  inline MenuItem() throw() {
  }
  
  virtual Dimension getMinimumSize() const throw() = 0;
  
  virtual Dimension getPreferredSize() const throw() = 0;
  
  // need to know to the final dimension of the context
  // need to know the graphics context
  virtual void onDisplay() throw();
};

class SeparatorMenuItem : public MenuItem {
public:

  /** The vertical margin. */
  static const unsigned int VERTICAL_MARGIN = 4;
  
  inline SeparatorMenuItem() throw() {
  }
  
  Dimension getMinimumSize() const throw() {
    return Dimension(MINIMUM_WIDTH, maximum(MINIMUM_HEIGHT, 2 * VERTICAL_MARGIN + 2));
  }
  
  Dimension getPreferredSize() const throw() {
    return Dimension(MINIMUM_WIDTH, maximum(MINIMUM_HEIGHT, 2 * VERTICAL_MARGIN + 2));
  }
  
  void onDisplay(GraphicsContext& graphicsContext) throw() {
    // dark line at middle (-1)
    // light line at middle (+1)
  }
};

class BitmapMenuItem : public MenuItem {
private:
  
  bool grayed;
  Bitmap bitmap;
public:
  
  inline BitmapMenuItem() throw() : grayed(false) {
  }
  
  inline bool isGrayed() const throw() {
    return grayed;
  }
  
  inline void gray() throw() {
    grayed = true;
  }
  
  inline void ungray() throw() {
    grayed = false;
  }
  
  inline Bitmap getBitmap() const throw() {
    return bitmap;
  }
  
  inline void setBitmap(const Bitmap& bitmap) throw() {
    this->bitmap = bitmap;
  }
  
  Dimension getMinimumSize() const throw() {
    return Dimension(1, 1);
  }
  
  Dimension getPreferredSize() const throw() {
    return Dimension(1, 1);
  }
  
  // need to know to the final dimension of the context
  // need to know the graphics context
  void onDisplay(GraphicsContext& graphicsContext) throw() {
    // if valid bitmap draw it
    Position position(0, 0);
    Dimension dimension(32, 32);
    graphicsContext.drawBitmap(position, dimension, bitmap);
  }
};

// do not add ScrollMenuItem

class CommandMenuItem : public BitmapMenuItem {
private:

  unsigned int command;
public:
  
  inline CommandMenuItem() throw() : command(0) {
  }
  
  inline CommandMenuItem(unsigned int _command) throw() 
    : command(_command) {
  }
  
  inline unsigned int getCommand() const throw() {
    return command;
  }
  
  inline void setCommand(unsigned int command) throw() {
    this->command = command;
  }
};

class TextMenuItem : public CommandMenuItem {
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

  inline TextMenuItem() throw() : alignment(LEFT) {
  }
  
  inline TextMenuItem(unsigned int command, const String& _text, const String& _shortcut) throw()
    : alignment(LEFT),
      CommandMenuItem(command),
      text(_text),
      shortcut(_shortcut) {
  }
  
  inline Alignment getAlignment() const throw() {
    return alignment;
  }

  inline void setAlignment(Alignment alignment) throw() {
    this->alignment = alignment;
  }
  
  inline String getText() const throw() {
    return text;
  }
  
  inline void setText(const String& text) throw() {
    this->text = text;
  }

  inline String getShortcut() const throw() {
    return shortcut;
  }

  inline void setShortcut(const String& shortcut) throw() {
    this->shortcut = shortcut;
  }

  Dimension getMinimumSize() const throw() {
    return Dimension(0, 0);
  }
  
  Dimension getPreferredSize() const throw() {
    // if has shortcut
    // width of text + width of shortcut
    // height of (text and shortcut)
    return Dimension(0 + IN_BETWEEN_MARGIN + 0, 0);
  }
  
  void onDisplay() throw() {
    // if shortcut
  }
};

class CheckableMenuItem : public TextMenuItem {
private:

  bool checked;
public:

  inline CheckableMenuItem() throw() : checked(false) {
  }
  
  inline bool isChecked() const throw() {
    return checked;
  }

  inline void check() throw() {
    checked = true;
  }
  
  inline void uncheck() throw() {
    checked = false;
  }
  
  void onDisplay() throw() {
    // draw checkmark bitmap
  }
};

class RadioMenuItem : public TextMenuItem {
private:

  bool selected;
public:

  inline RadioMenuItem() throw() : selected(false) {
  }

  inline bool isSelected() const throw() {
    return selected;
  }

  inline void select() throw() {
    selected = true;
  }

  inline void deselect() throw() {
    selected = false;
  }

  void onDisplay() throw() {
    // draw radio bitmap
  }
};

class SelectionMenuItem : public MenuItem {
public:
  
  // Array<Bitmap>
  // set maximum number of columns
};

class IndependentMenuItem : public MenuItem {
public:

  void onDisplay() throw() {
  }
};

class EditMenuItem : public IndependentMenuItem {
private:
  
  String text;
public:
  
  EditMenuItem() throw() {
  }
  
  EditMenuItem(const String& _text) throw() : text(_text) {
  }
  
  inline String getText() const throw() {
    return text;
  }
  
  inline void setText(const String& text) throw() {
    this->text = text;
  }
};

// TAG: need MenuItem with its own graphics context -> child window of pop up menu window
#endif

/**
  Pop up window.
  
  @short Pop up window.
  @ingroup ui
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class PopUpWindow : public GraphicsContext {
private:
  
  /** The menu items. */
  //Array<MenuItemReference> items;
  
  /**
    Releases the rendering context.
  */
  void destroy() throw();

  /**
    Invoked by the menu item.
  */
  void onMenuItem(MenuItem* item) throw() { // user is responsible for invoking update method
  }
public:
  
  // TAG: need MenuItemCollection class
  class MenuItemCollection : public Array {
  public:
  };
  
  /**
    Returns a reference to the menu item at the specified position.
  */
  MenuItemReference getItem(unsigned int index) const throw(Invalid...) {
    return items[index];
  }

  /**
    Sets the menu item at the specified position.
  */
  void setItem(unsigned int index) throw() {
    items[index] = item;
  }

  /**
    Adds the menu item to the end of the item container.
  */
  void addItem(const MenuItem& item) throw() {
    items.add(item);
  }

  /**
    Returns the number of menu items.
  */
  unsigned int getNumberOfItems() const throw() {
    return items.getSize();
  }
  
  void onChange() throw();
  
  /**
    Initializes a pop up window.

    @param flags The flags.
  */
  PopUpMenu(unsigned int flags) throw(UserInterfaceException);
  
  /**
    Initializes a pop up window.

    @param position The initial position.
    @param flags The flags.
  */
  PopUpMenu(const Position& position, unsigned int flags) throw(UserInterfaceException);
  
  void track(const Position& position) throw(UserInterfaceException) {
    // translate mouse position to window position
  }
  
  // TAG: the position of the
  void onChange() throw() {
    Array<MenuItemReference>::ReadEnumerator enu = items.getReadEnumerator();
    const Dimension screenSize; // TAG: getScreen...
    Dimension xxxSize; // TAG: size
    Dimension preferredSize;
    Dimension minimumSize;
    Dimension desiredSize;
    while (enu.hasNext()) {
      MenuItemReference item = *enu.next();
      preferredSize.expand(item.getPreferredSize());
      minimumSize.expand(item.getMinimumSize());
      // vertical
      // horizontal
      // desiredSize 
    }
    // reduce

    finalSize = desiredSize.expand(minimumSize);
  }

  void onScrollUp() throw() {
    if (firstVisibleItem != firstItem) {
      --firstVisibleItem;
      --lastVisibleItem;
    }
    // recalc();
  }
  
  void onScrollDown() throw() {
    if (lastVisibleItem != lastItem) {
      ++firstVisibleItem;
      ++lastVisibleItem;
    }
    // recalc();
  }
  
  void onMouseButton() throw() {
  }
  
  void onDisplay() throw() {
    clear();
    
    // if scrolling then draw scroll up item - if at top then grayed
    // from first to last item: draw item
    // if item is child then item->onDisplay() otherwise item->onDisplay(...)?
    // if scrolling then draw scroll down item - if at bottom then grayed
  }
};

class UserInterfaceStyle : public Object {
public:
  
  virtual Color getMenuBackground() const throw();
  virtual Color getMenuHighlightColor() const throw();
  virtual Color getMenuMarginColor() const throw();
  virtual Color getMenuTextColor() const throw();
  virtual Dimension getMenuBitmapSize() const throw();
  virtual Bitmap getCheckedBitmap() const throw();
  virtual Bitmap getSelectedBitmap() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
