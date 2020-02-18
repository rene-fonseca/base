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

#include <base/string/String.h>
#include <base/mem/Allocator.h> // avoid cylic dependency

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class StringOutputStream;

/** HTML. */
class _COM_AZURE_DEV__BASE__API HTML {
public:

  /** Returns 'id' string. */
  static const String& getId();
  /** Returns 'style' string. */
  static const String& getStyle();
  /** Returns 'table' string. */
  static const String& getTable();
  /** Returns 'tr' string. */
  static const String& getTR();
  /** Returns 'th' string. */
  static const String& getTH();
  /** Returns 'td' string. */
  static const String& getTD();

  /** Encode string for HTML. */
  static String encode(const String& text);
};

/** HTML encodes by type. */
template<class TYPE>
class HTMLEncode {
public:

  static inline const TYPE& map(const TYPE& value) noexcept
  {
    return value;
  }
};

/** HTML encodes by type. */
template<>
class HTMLEncode<String> {
public:

  static inline String map(const String& value) noexcept
  {
    return HTML::encode(value);
  }
};

/** HTML encodes by type. */
template<>
class HTMLEncode<WideString> {
public:

  static inline String map(const WideString& value) noexcept
  {
    return HTML::encode(value);
  }
};

/** HTML text. */
class _COM_AZURE_DEV__BASE__API HTMLItem : public ReferenceCountedObject {
public:

  /** Copies item. */
  virtual R<HTMLItem> copy() = 0;

  /** Converts item to code. */
  virtual void toCode(StringOutputStream& code) = 0;
};

/** HTML attribute. */
class _COM_AZURE_DEV__BASE__API HTMLAttribute : public ReferenceCountedObject {
public:

  /** The name of the attribute. */
  String name;
  /** The value of the attribute. */
  String value;

  /** Initializes attribute as unnamed. */
  HTMLAttribute() noexcept;

  /** Initializes attribute. */
  HTMLAttribute(const String& _name, const String& _value) noexcept;

  /** Copies attribute. */
  R<HTMLAttribute> copy();

  /** Converts attribute to HTML code. */
  void toCode(StringOutputStream& code);
};

/** HTML text. */
class _COM_AZURE_DEV__BASE__API HTMLText : public HTMLItem {
public:

  /** The text. */
  String text;

  /** Initialize the item as empty. */
  HTMLText() noexcept;
  
  /** Initializes the text item. */
  HTMLText(const String& _text) noexcept;

  /** Returns the text. */
  inline const String& getText() const noexcept
  {
    return text;
  }

  /** Sets the text. */
  inline void setText(const String& text) noexcept
  {
    this->text = text;
  }

  /** Copies the text item. */
  R<HTMLItem> copy() override;

  /** Converts text to HTML code. */
  void toCode(StringOutputStream& code) override;
};

/** HTML element. */
class _COM_AZURE_DEV__BASE__API HTMLElement : public HTMLItem {
public:

  /** The element name. */
  String name;
  /** Attributes. */
  Allocator<R<HTMLAttribute> > attributes;
  /** Children. */
  Allocator<R<HTMLItem> > children;

  /** Initialize unnamed element. */
  HTMLElement() noexcept;

  /** Initialize element. */
  HTMLElement(const String& _name) noexcept;

  /** Copies element. */
  R<HTMLItem> copy() override;

  /** Returns the number of attributes. */
  MemorySize getNumberOfAttributes() const noexcept;
  
  /** Returns the number of children. */
  MemorySize getNumberOfChildren() const noexcept;

  /** Adds child. */
  HTMLElement& addChild(const R<HTMLItem>& e);

  /** Returns the child by index. */
  R<HTMLItem> getChild(unsigned int index);

  /** Adds text item. */
  R<HTMLText> addText(const String& text);

  /** Returns the first child of the given type. */
  R<HTMLElement> getFirstChild(const char* name) noexcept;

  /** Returns the first child of the given type. */
  R<HTMLElement> getFirstChild(const String& name) noexcept;

  /** Returns the attribute by name. Returns nullptr if not set. */
  R<HTMLAttribute> getAttribute(const String& name) noexcept;

  /** Returns the value of the attribute with the given name. Returns empty string if not set. */
  String getAttributeValue(const String& name) const noexcept;

  /** Sets the attribute. */
  void setAttribute(const String& name, const String& value);

  /** Converts element to HTML code. */
  void toCode(StringOutputStream& code) override;
};

namespace html {

  /** Table element. */
  class _COM_AZURE_DEV__BASE__API TableElement {
  public:

    R<HTMLElement> e;

    /** Constructs new element. */
    TableElement(const char* id);

    /** Constructs new element. */
    TableElement(const String& id);

    /** Returns the name of the element. */
    const String& getName() const noexcept;

    /** Sets the attribute. */
    void setAttribute(const String& name, const String& value);

    /** Sets the id. */
    void setId(const String& id);

    /** Sets the style. */
    void setStyle(const String& style);

    /** Returns the child by index. */
    R<HTMLItem> getChild(unsigned int index);

    /** Returns the first child of the given type. */
    R<HTMLItem> getFirstChild(const char* name);

    /** Returns the first child of the given type. */
    R<HTMLItem> getFirstChild(const String& name);
  };

  /** Make an HTML table. */
  class _COM_AZURE_DEV__BASE__API Table : public TableElement {
  public:

    /** Initializes table. */
    Table();

    /** TH element. */
    class _COM_AZURE_DEV__BASE__API Header : public TableElement {
    public:

      /** Initializes header. */
      Header();

      /** Adds item. */
      Header& add(R<HTMLItem> item);

      /** Adds element. */
      R<HTMLElement> add(const String& name);

      /** Adds element. */
      R<HTMLElement> bold();

      /** Adds element. */
      R<HTMLElement> italic();

      /** Adds element. */
      R<HTMLElement> strong();

      /** Adds text. */
      Header& addText(const String& text);

      /** Sets element to text. */
      Header& setText(const String& text);
    };

    /** TD element. */
    class _COM_AZURE_DEV__BASE__API Column : public TableElement {
    public:

      /** Initializes column. */
      Column();

      /** Adds item. */
      Column& add(R<HTMLItem> item);

      /** Adds element. */
      R<HTMLElement> add(const String& name);

      /** Adds element. */
      R<HTMLElement> bold();

      /** Adds element. */
      R<HTMLElement> italic();

      /** Adds element. */
      R<HTMLElement> strong();

      /** Adds text. */
      Column& addText(const String& text);

      /** Sets element to text. */
      Column& setText(const String& text);
    };

    /** TR element. */
    class _COM_AZURE_DEV__BASE__API Row : public TableElement {
    public:

      /** Initializes row. */
      Row();

      /** Adds a header column. */
      Header addHeader();

      /** Adds a header column. */
      Header addHeader(const String& text);

      /** Adds a normal column. */
      Column addColumn();

      /** Adds a normal column. */
      Column addColumn(const String& text);
    };

    /** Adds a new row. */
    Row addRow();

    /** Returns HTML code. */
    String getHTML();
  };

} // html namespace

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
