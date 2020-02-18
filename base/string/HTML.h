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
  R<HTMLItem> copy();

  /** Returns the number of attributes. */
  MemorySize getNumberOfAttributes() const noexcept;
  
  /** Returns the number of children. */
  MemorySize getNumberOfChildren() const noexcept;

  /** Adds child. */
  HTMLElement& addChild(const R<HTMLItem>& e);

  /** Returns the child by index. */
  R<HTMLItem> getChild(unsigned int index);

  /** Returns the first child of the given type. */
  R<HTMLElement> getFirstChild(const char* name);

  /** Returns the attribute by name. Returns nullptr if not set. */
  R<HTMLAttribute> getAttribute(const String& name);

  /** Sets the attribute. */
  void setAttribute(const String& name, const String& value);

  /** Converts element to HTML code. */
  void toCode(StringOutputStream& code) override;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
