/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/HTML.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

String HTML::encode(const String& text)
{
  String result;
  result.ensureCapacity(text.getLength() * 2);
  const uint8* src = text.getBytes();
  const uint8* end = src + text.getLength();
  while (src != end) {
    const char ch = *src++;
    switch (ch) {
    case '&':
      result += MESSAGE("&amp;");
      break;
    case '>':
      result += MESSAGE("&gt;");
      break;
    case '<':
      result += MESSAGE("&lt;");
      break;
    case '"':
      result += MESSAGE("&quot;");
      break;
    case '\'':
      result += MESSAGE("&apos;");
      break;
    default:
      result += ch;
    }
  }
  return result;
}

HTMLAttribute::HTMLAttribute() noexcept
{
}

HTMLAttribute::HTMLAttribute(const String& _name, const String& _value) noexcept
  : name(_name), value(_value)
{
}

R<HTMLAttribute> HTMLAttribute::copy()
{
  return new HTMLAttribute(name, value);
}

void HTMLAttribute::toCode(StringOutputStream& code)
{
  code << name;
  code << "=\"";
  code << HTML::encode(value);
  code << "\"";
}

HTMLText::HTMLText() noexcept
{
}

HTMLText:: HTMLText(const String& _text) noexcept
  : text(_text)
{
}

R<HTMLItem> HTMLText::copy()
{
  return new HTMLText(text);
}

void HTMLText::toCode(StringOutputStream& code)
{
  code << HTML::encode(text);
}

HTMLElement::HTMLElement() noexcept
{
}

HTMLElement::HTMLElement(const String& _name) noexcept
  : name(_name)
{
}

MemorySize HTMLElement::getNumberOfAttributes() const noexcept
{
  return attributes.getSize();
}

MemorySize HTMLElement::getNumberOfChildren() const noexcept
{
  return children.getSize();
}

R<HTMLItem> HTMLElement::copy()
{
  R<HTMLElement> r = new HTMLElement();
  r->attributes.setSize(attributes.getSize());
  r->children.setSize(children.getSize());
  r->name = name;
  auto adest = r->attributes.begin();
  for (auto attribute : attributes) {
    *adest++ = attribute->copy();
  }
  auto cdest = r->children.begin();
  for (auto child : children) {
    *cdest++ = child->copy();
  }
  return r;
}

HTMLElement& HTMLElement::addChild(const R<HTMLItem>& e)
{
  children.setSize(children.getSize() + 1, e);
  return *this;
}

R<HTMLItem> HTMLElement::getChild(unsigned int index)
{
  return children.begin()[index];
}

R<HTMLElement> HTMLElement::getFirstChild(const char* name)
{
  for (auto _child : children) {
    if (R<HTMLElement> child = _child.cast<HTMLElement>()) {
      if (child->name) {
        return child;
      }
    }
  }
  return nullptr;
}

R<HTMLAttribute> HTMLElement::getAttribute(const String& name)
{
  for (auto attribute : attributes) {
    if (attribute->name) {
      return attribute;
    }
  }
  return nullptr;
}

void HTMLElement::setAttribute(const String& name, const String& value)
{
  if (auto attribute = getAttribute(name)) {
    attribute->value = value;
    return;
  }
  attributes.setSize(attributes.getSize() + 1, new HTMLAttribute(name, value));
}

void HTMLElement::toCode(StringOutputStream& code)
{
  code << "<" << name;
  for (auto attribute : attributes) {
    code << SP;
    attribute->toCode(code);
  }
  code << ">";
  for (auto child : children) {
    child->toCode(code);
  }
  code << "</" << name << ">";
}




/** Table element. */
class _COM_AZURE_DEV__BASE__API TableElement {
public:

  R<HTMLElement> e;

  TableElement(const char* id)
  {
    e = new HTMLElement(id);
  }

  inline const String& getName() const noexcept
  {
    return e->name;
  }

  void setId(const String& id)
  {
    e->setAttribute("id", id);
  }

  void setStyle(const String& style)
  {
    e->setAttribute("style", style);
  }

  R<HTMLItem> getChild(unsigned int index)
  {
    return e->getChild(index);
  }

  R<HTMLItem> getFirstChild(const char* name)
  {
    return e->getFirstChild(name);
  }
};

/** Make an HTML table. */
class _COM_AZURE_DEV__BASE__API Table : public TableElement {
private:

  R<HTMLElement> table;
public:

  // TAG: reuse element keywords

  Table()
    : TableElement("table")
  {
  }

  /** TH element. */
  class _COM_AZURE_DEV__BASE__API Header : public TableElement {
  public:

    Header()
      : TableElement("tr")
    {
    }

    Header& addText(const String& text)
    {
      e->addChild(new HTMLText(text));
      return *this;
    }

    Header& setText(const String& text)
    {
      e->children.setSize(0);
      addText(text);
      return *this;
    }
  };

  /** TD element. */
  class _COM_AZURE_DEV__BASE__API Column : public TableElement {
  public:

    Column()
      : TableElement("td")
    {
    }

    Column& add(R<HTMLElement> _e)
    {
      e->addChild(_e);
      return *this;
    }

    R<HTMLElement> add(const char* name)
    {
      R<HTMLElement> _e = new HTMLElement(name);
      e->addChild(_e);
      return _e;
    }

    R<HTMLElement> bold()
    {
      return add("b");
    }

    R<HTMLElement> italic()
    {
      return add("i");
    }

    R<HTMLElement> strong()
    {
      return add("strong");
    }

    Column& addText(const String& text)
    {
      e->addChild(new HTMLText(text));
      return *this;
    }

    Column& setText(const String& text)
    {
      e->children.setSize(0);
      addText(text);
      return *this;
    }
  };

  /** TR element. */
  class _COM_AZURE_DEV__BASE__API Row : public TableElement {
  public:

    Row()
      : TableElement("tr")
    {
    }

    Row& addHeader(const String& text)
    {
      Header r;
      e->addChild(r.e);
      return *this;
    }

    Row& addColumn(const String& text)
    {
      Column r;
      e->addChild(r.e);
      return *this;
    }
  };

  Row addRow()
  {
    return Row();
  }

  String getHTML();
};

String Table::getHTML()
{
  StringOutputStream sos;
  table->toCode(sos);
  return sos;
}


#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Table) : public UnitTest {
public:

  TEST_PRIORITY(100);
  TEST_PROJECT("base/string");

  void run() override
  {
    Table t;
    auto r = t.addRow().addHeader("Index").addHeader("Description");

    for (unsigned int i = 0; i < 5; ++i) {
      auto r = t.addRow();
      auto ci = r.addColumn(format() << i);
      auto cd = r.addColumn(format() << "descrip<tion>" << i);
    }
    String html = t.getHTML();

    if (1 || TEST_IS_DEFINED("writeResults")) {
      fout << html << ENDL;
    }
    TEST_ASSERT(html);
  }
};

TEST_REGISTER(Table);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
