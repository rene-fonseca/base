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

namespace {

  const String& getB()
  {
    static String id("b");
    return id;
  }

  const String& getI()
  {
    static String id("i");
    return id;
  }

  const String& getStrong()
  {
    static String id("strong");
    return id;
  }
}

const String& HTML::getId()
{
  static String id("id");
  return id;
}

const String& HTML::getStyle()
{
  static String id("style");
  return id;
}

const String& HTML::getTable()
{
  static String id("table");
  return id;
}

const String& HTML::getTR()
{
  static String id("tr");
  return id;
}

const String& HTML::getTH()
{
  static String id("th");
  return id;
}

const String& HTML::getTD()
{
  static String id("td");
  return id;
}

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
  if (!(index < children.getSize())) {
    _throw OutOfRange();
  }
  return children.begin()[index];
}

R<HTMLText> HTMLElement::addText(const String& text)
{
  R<HTMLText> i = new HTMLText(text);
  addChild(i);
  return i;
}

R<HTMLElement> HTMLElement::getFirstChild(const char* name) noexcept
{
  for (auto _child : children) {
    if (R<HTMLElement> child = _child.cast<HTMLElement>()) {
      if (child->name == NativeString(name)) {
        return child;
      }
    }
  }
  return nullptr;
}

R<HTMLElement> HTMLElement::getFirstChild(const String& name) noexcept
{
  for (auto _child : children) {
    if (R<HTMLElement> child = _child.cast<HTMLElement>()) {
      if (child->name == name) {
        return child;
      }
    }
  }
  return nullptr;
}

R<HTMLAttribute> HTMLElement::getAttribute(const String& name) noexcept
{
  for (auto attribute : attributes) {
    if (attribute->name == name) {
      return attribute;
    }
  }
  return nullptr;
}

String HTMLElement::getAttributeValue(const String& name) const noexcept
{
  for (auto attribute : attributes) {
    if (attribute->name == name) {
      return attribute->value;
    }
  }
  return String();
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

namespace html {

  TableElement::TableElement(const char* id)
  {
    e = new HTMLElement(id);
  }

  TableElement::TableElement(const String& id)
  {
    e = new HTMLElement(id);
  }

  const String& TableElement::getName() const noexcept
  {
    return e->name;
  }

  void TableElement::setAttribute(const String& name, const String& value)
  {
    e->setAttribute(name, value);
  }

  void TableElement::setId(const String& id)
  {
    e->setAttribute("id", id);
  }

  void TableElement::setStyle(const String& style)
  {
    e->setAttribute("style", style);
  }

  R<HTMLItem> TableElement::getChild(unsigned int index)
  {
    return e->getChild(index);
  }

  R<HTMLItem> TableElement::getFirstChild(const char* name)
  {
    return e->getFirstChild(name);
  }

  R<HTMLItem> TableElement::getFirstChild(const String& name)
  {
    return e->getFirstChild(name);
  }

  Table::Table()
    : TableElement(HTML::getTable())
  {
  }

  Table::Header::Header()
    : TableElement(HTML::getTH())
  {
  }

  Table::Header& Table::Header::add(R<HTMLItem> _i)
  {
    e->addChild(_i);
    return *this;
  }

  R<HTMLElement> Table::Header::add(const String& name)
  {
    R<HTMLElement> _e = new HTMLElement(name);
    e->addChild(_e);
    return _e;
  }

  R<HTMLElement> Table::Header::bold()
  {
    return add(getB());
  }

  R<HTMLElement> Table::Header::italic()
  {
    return add(getI());
  }

  R<HTMLElement> Table::Header::strong()
  {
    return add(getStrong());
  }

  Table::Header& Table::Header::addText(const String& text)
  {
    e->addChild(new HTMLText(text));
    return *this;
  }

  Table::Header& Table::Header::setText(const String& text)
  {
    e->children.setSize(0);
    addText(text);
    return *this;
  }

  Table::Column::Column()
    : TableElement(HTML::getTD())
  {
  }

  Table::Column& Table::Column::add(R<HTMLItem> _i)
  {
    e->addChild(_i);
    return *this;
  }

  R<HTMLElement> Table::Column::add(const String& name)
  {
    R<HTMLElement> _e = new HTMLElement(name);
    e->addChild(_e);
    return _e;
  }

  R<HTMLElement> Table::Column::bold()
  {
    return add("b");
  }

  R<HTMLElement> Table::Column::italic()
  {
    return add("i");
  }

  R<HTMLElement> Table::Column::strong()
  {
    return add("strong");
  }

  Table::Column& Table::Column::addText(const String& text)
  {
    e->addChild(new HTMLText(text));
    return *this;
  }

  Table::Column& Table::Column::setText(const String& text)
  {
    e->children.setSize(0);
    addText(text);
    return *this;
  }

  Table::Row::Row()
    : TableElement(HTML::getTR())
  {
  }

  Table::Header Table::Row::addHeader()
  {
    Header h;
    e->addChild(h.e);
    return h;
  }

  Table::Header Table::Row::addHeader(const String& text)
  {
    Header h;
    h.addText(text);
    e->addChild(h.e);
    return h;
  }

  Table::Column Table::Row::addColumn()
  {
    Column c;
    e->addChild(c.e);
    return c;
  }

  Table::Column Table::Row::addColumn(const String& text)
  {
    Column c;
    c.addText(text);
    e->addChild(c.e);
    return c;
  }

  Table::Row Table::addRow()
  {
    Row row;
    e->addChild(row.e);
    return row;
  }

  String Table::getHTML()
  {
    if (!e) {
      return String();
    }
    StringOutputStream sos;
    e->toCode(sos);
    return sos;
  }
} // html namespace

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Table) : public UnitTest {
public:

  TEST_PRIORITY(100);
  TEST_PROJECT("base/string");

  void run() override
  {
    using namespace html;
    Table t;
    t.setId("thistable");
    auto r = t.addRow();
    auto title = r.addHeader("Title");
    title.setStyle("text-align: center");
    title.setAttribute("colspan", "2"); // TAG: add support for automatic value to string conversion

    r = t.addRow();
    auto h1 = r.addHeader("Index");
    h1.setStyle("text-align: right");
    auto h2 = r.addHeader("Description");
    h2.setStyle("text-align: left");
    // TAG: add support for parsing style and updating it
    // <span style="color: #ff0000;">
    for (unsigned int i = 0; i < 5; ++i) {
      auto r = t.addRow();
      auto ci = r.addColumn(format() << i);
      ci.setStyle("text-align: right");
      auto cd = r.addColumn();
      cd.setStyle("text-align: left");
      // TAG: add support for simple markdown R<HTMLItem> makeMDItem("*123* __123 `123` ## 123")
      R<HTMLText> t = new HTMLText("descrip<tion>");
      cd.add(t);
      R<HTMLElement> e = new HTMLElement("b");
      e->addText(format() << i);
      cd.add(e);
    }
    String html = t.getHTML();

    if (TEST_IS_DEFINED("writeResults")) {
      fout << html << ENDL;
    }
    TEST_ASSERT(html);
  }
};

TEST_REGISTER(Table);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
