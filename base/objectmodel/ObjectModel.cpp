/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/objectmodel/ObjectModel.h>
#include <base/LongInteger.h>
#include <base/mathematics/Math.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace {

  ObjectModel globalObjectModel(false); // can be used across threads so we turn of reuse for lookup
}

ObjectModel::ObjectModel(bool _allowReuse)
  : allowReuse(_allowReuse)
{
  commonVoid = new Void();
  commonBooleanFalse = new Boolean(false);
  commonBooleanTrue = new Boolean(true);
  commonInteger0 = new Integer(0);
  commonInteger1 = new Integer(1);
  commonIntegerMinus1 = new Integer(-1);
  commonFloat0 = new Float(0);
  commonFloat1 = new Float(1);
  commonFloatMinus1 = new Float(-1);
  commonStringEmpty = new String();
  // commonArrayEmpty = new Array();
  // commonObjectEmpty = new Object();
}

// internal format wrapper
inline FormatOutputStream& operator<<(FormatOutputStream& stream, const ObjectModel::Value& value)
{
  return operator<<(stream, Reference<ObjectModel::Value>(const_cast<ObjectModel::Value*>(&value)));
}

base::String ObjectModel::Value::toString(bool niceFormat) const noexcept
{
  if (niceFormat) {
    ObjectModel::NiceFormat s;
    s.setFlags(s.getFlags() | FormatOutputStream::Symbols::POSIX); // for floats
    s << *this << FLUSH;
    return s.getString();
  } else {
    StringOutputStream s;
    s.setFlags(s.getFlags() | FormatOutputStream::Symbols::POSIX); // for floats
    s << *this << FLUSH;
    return s.getString();
  }
}

Reference<ObjectModel::Void> ObjectModel::createVoid()
{
  return commonVoid;
  // return new Void();
}

Reference<ObjectModel::Boolean> ObjectModel::createBoolean(bool value)
{
  return value ? commonBooleanTrue : commonBooleanFalse;
  // return new Boolean(value);
}

Reference<ObjectModel::Integer> ObjectModel::createInteger(int value)
{
  switch (value) {
  case -1:
    return commonIntegerMinus1;
  case 0:
    return commonInteger0;
  case 1:
    return commonInteger1;
  default:
    return new Integer(value);
  }
}

Reference<ObjectModel::Float> ObjectModel::createFloat(double value)
{
  if (value == 0) {
    return commonFloat0;
  } else if (value == 1) {
    return commonFloat1;
  } else if (value == -1) {
    return commonFloatMinus1;
  } else {
    return new Float(value);
  }
}

Reference<ObjectModel::String> ObjectModel::createString(const char* value)
{
  if (!value || !*value) {
    return commonStringEmpty;
  }
  const base::String _value(value);
  auto i = lookup.find(value);
  if (i != lookup.end()) {
    return i->second; // found identical
  }
  auto result = new String(_value); // new string
  lookup[_value] = result;
  return result;
}

Reference<ObjectModel::String> ObjectModel::createString(const base::String& value)
{
  if (!value) {
    return commonStringEmpty;
  }
  const base::String _value(value);
  auto i = lookup.find(value);
  if (i != lookup.end()) {
    return i->second; // found identical
  }
  return new String(_value); // new string
}

Reference<ObjectModel::String> ObjectModel::createStringUnique(const char* value)
{
  if (!value || !*value) {
    return commonStringEmpty;
  }
  return new String(value);
}

Reference<ObjectModel::Array> ObjectModel::createArray()
{
  return new Array();
}

Reference<ObjectModel::Array> ObjectModel::createArray(std::initializer_list<bool> l)
{
  auto a = new Array();
  a->values.setSize(l.size());
  MemorySize i = 0;
  for (const auto v : l) {
    a->setAt(i++, globalObjectModel.createBoolean(v));
  }
  return a;
}

Reference<ObjectModel::Array> ObjectModel::createArray(std::initializer_list<int> l)
{
  auto a = new Array();
  a->values.setSize(l.size());
  MemorySize i = 0;
  for (const auto v : l) {
    a->setAt(i++, globalObjectModel.createInteger(v));
  }
  return a;
}

Reference<ObjectModel::Array> ObjectModel::createArray(std::initializer_list<double> l)
{
  auto a = new Array();
  a->values.setSize(l.size());
  MemorySize i = 0;
  for (const auto v : l) {
    a->setAt(i++, globalObjectModel.createFloat(v));
  }
  return a;
}

Reference<ObjectModel::Array> ObjectModel::createArray(std::initializer_list<const char*> l)
{
  auto a = new Array();
  a->values.setSize(l.size());
  MemorySize i = 0;
  for (const auto v : l) {
    a->setAt(i++, globalObjectModel.createString(v));
  }
  return a;
}

Reference<ObjectModel::Object> ObjectModel::createObject()
{
  return new Object();
}

MemorySize ObjectModel::String::getSize() const noexcept
{
  return value.getLength();
}

MemorySize ObjectModel::Array::getSize() const noexcept
{
  return values.getSize();
}

Reference<ObjectModel::Value> ObjectModel::Array::getAt(MemorySize index) const throw(ObjectModelException)
{
  if (index >= values.getSize()) {
    throw OutOfRange();
  }
  return values[index];
}

void ObjectModel::Array::setAt(MemorySize index, const Reference<Value>& value)
{
  if (index >= values.getSize()) {
    throw OutOfRange();
  }
  values[index] = value;
}

void ObjectModel::Array::append(const Reference<Value>& value)
{
  values.append(value);
}

ObjectModel::Array& ObjectModel::Array::operator=(const std::vector<bool>& _values)
{
  values.setSize(_values.size());
  MemorySize i = 0;
  for (const auto& v : _values) {
    values[i++] = globalObjectModel.createBoolean(v); // always reusing
  }
  return *this;
}

ObjectModel::Array& ObjectModel::Array::operator=(const std::vector<int>& _values)
{
  values.setSize(_values.size());
  MemorySize i = 0;
  Reference<Integer> previous; // catch easy reuse case
  for (const auto& v : _values) {
    if (previous && (v == *previous)) {
      values[i++] = previous;
    } else {
      previous = globalObjectModel.createInteger(v);
      values[i++] = previous;
    }
  }
  return *this;
}

ObjectModel::Array& ObjectModel::Array::operator=(const std::vector<double>& _values)
{
  values.setSize(_values.size());
  MemorySize i = 0;
  Reference<Float> previous; // catch easy reuse case
  for (const auto& v : _values) {
    if (previous && (v == *previous)) {
      values[i++] = previous;
    } else {
      previous = globalObjectModel.createFloat(v);
      values[i++] = previous;
    }
  }
  return *this;
}

ObjectModel::Array& ObjectModel::Array::operator=(const std::vector<base::String>& _values)
{
  values.setSize(_values.size());
  MemorySize i = 0;
  Reference<String> previous; // catch easy reuse case
  for (const auto& v : _values) {
    if (previous && (v == *previous)) {
      values[i++] = previous;
    } else {
      previous = globalObjectModel.createString(v);
      values[i++] = previous;
    }
  }
  return *this;
}

template<>
std::vector<bool> ObjectModel::Array::getAs<bool>() const {
  std::vector<bool> result;
  result.reserve(values.getSize());
  for (const auto& v : values) {
    auto _v = v.cast<Boolean>();
    if (!_v) {
      throw ObjectModelException("Expected boolean array.");
    }
    result.push_back(_v->value);
  }
  return result;
}

template<>
std::vector<int> ObjectModel::Array::getAs<int>() const {
  std::vector<int> result;
  result.reserve(values.getSize());
  for (const auto& v : values) {
    auto _v = v.cast<Boolean>();
    if (!_v) {
      throw ObjectModelException("Expected integer array.");
    }
    result.push_back(_v->value);
  }
  return result;
}

template<>
std::vector<double> ObjectModel::Array::getAs<double>() const {
  std::vector<double> result;
  result.reserve(values.getSize());
  for (const auto& v : values) {
    auto _v = v.cast<Float>();
    if (!_v) {
      throw ObjectModelException("Expected float array.");
    }
    result.push_back(_v->value);
  }
  return result;
}

template<>
std::vector<base::String> ObjectModel::Array::getAs<base::String>() const {
  std::vector<base::String> result;
  result.reserve(values.getSize());
  for (const auto& v : values) {
    auto _v = v.cast<String>();
    if (!_v) {
      throw ObjectModelException("Expected string array.");
    }
    result.push_back(_v->value);
  }
  return result;
}

MemorySize ObjectModel::Object::getSize() const noexcept
{
  return values.getSize();
}

bool ObjectModel::Object::hasKey(const Reference<String>& key) const noexcept
{
  if (!key) {
    return false;
  }
  for (const auto& v : values) {
    if (!v.first) {
      throw NullPointer("Key is null.");
    }
    if (v.first->value == key->value) {
      return true;
    }
  }
  return false;
}

Reference<ObjectModel::Value> ObjectModel::Object::getValue(const Reference<String>& key) const throw(ObjectModelException)
{
  if (!key) {
    return nullptr;
  }
  for (const auto& v : values) {
    if (!v.first) {
      throw NullPointer("Key is null.");
    }
    if (v.first->value == key->value) {
      return v.second;
    }
  }
  return nullptr;
}

void ObjectModel::Object::setValue(const Reference<String>& key, const Reference<Value>& value)
{
  if (!key) {
    throw NullPointer("Key is null.");
  }
  for (auto& v : values) {
    if (!v.first) {
      throw NullPointer("Key is null.");
    }
    if (v.first->value == key->value) {
      v.second = value;
      return;
    }
  }
  values.append(std::pair<Reference<String>, Reference<Value> >(key, value));
}

Reference<ObjectModel::Value> ObjectModel::Object::getPath(const char* path, bool forceNull) throw(ObjectModelException)
{
  if (!path) {
    return nullptr;
  }
#if 0 // best to only add this if required later
  if (*path == '/') { // root is the default
    ++path;
  }
#endif
  Reference<Object> current = this;
  while (*path) {
    const char* begin = path;
    while (*path && (*path != '/') && (*path != '[')) { // find separator
      ++path;
    }

    const base::String key(begin, path - begin);
    if (key.isEmpty()) { // do not allow empty keys for now
      throw ObjectModelException("Invalid path.");
    }

    // handle index for Array array[0]
    bool arrayExpected = false;
    long long arrayIndex = -1;
    if (*path == '[') {
      arrayExpected = true;
      ++path;
      const char* ibegin = path;
      while ((*path >= '0') && (*path <= '9')) {
        ++path;
      }
      arrayIndex = LongInteger::parse(ibegin, path, 0);
      if (arrayIndex < 0) {
        throw ObjectModelException("Invalid array index.");
      }
      if (arrayIndex > PrimitiveTraits<MemorySize>::MAXIMUM) {
        throw ObjectModelException("Invalid array index.");
      }
      if (*path != ']') {
        throw ObjectModelException("Invalid path.");
      }
      ++path; // skip ]
    }

    if (*path) {
      if (*path != '/') {
        throw ObjectModelException("Invalid path.");
      }
    }
    if (*path) { // skip separator
      ++path;
    }

    // find value
    Reference<Value> result;
    for (const auto& v : current->values) {
      if (!v.first) {
        throw NullPointer("Key is null.");
      }
      if (v.first->value == key) {
        result = v.second; // found
        break;
      }
    }
    if (!result) {
      if (forceNull) {
        return nullptr;
      }
      throw ObjectModelException("Path not found.");
    }

    if (arrayExpected) {
      auto a = result.cast<Array>();
      if (!a) {
        throw ObjectModelException("Array expected.");
      }
      if (!((arrayIndex >= 0) && (static_cast<MemorySize>(arrayIndex) < a->values.getSize()))) {
        throw ObjectModelException("Array index out of range.");
      }
      result = a->values[arrayIndex];
    }

    if (!*path) {
      return result;
    }
    current = result.cast<Object>();
  }
  BASSERT(!*path);
  return nullptr;
}

bool ObjectModel::Object::getBoolean(const char* path, bool defaultValue) throw(ObjectModelException)
{
  Reference<Value> v = getPath(path, true);
  if (!v) {
    return defaultValue;
  }
  Reference<Boolean> b = v.cast<Boolean>();
  if (!b) {
    throw ObjectModelException("Not a boolean.");
  }
  return b->value;
}

int ObjectModel::Object::getInteger(const char* path, int defaultValue) throw(ObjectModelException) {
  Reference<Value> v = getPath(path, true);
  if (!v) {
    return defaultValue;
  }
  Reference<Integer> i = v.cast<Integer>();
  if (!i) {
    throw ObjectModelException("Not an integer.");
  }
  return i->value;
}

double ObjectModel::Object::getFloat(const char* path, double defaultValue) throw(ObjectModelException) {
  Reference<Value> v = getPath(path, true);
  if (!v) {
    return defaultValue;
  }
  Reference<Float> d = v.cast<Float>();
  if (!d) {
    throw ObjectModelException("Not a float.");
  }
  return d->value;
}

base::String ObjectModel::Object::getString(const char* path, const base::String& defaultValue) throw(ObjectModelException)
{
  Reference<Value> v = getPath(path, true);
  if (!v) {
    return defaultValue;
  }
  Reference<String> s = v.cast<String>();
  if (!s) {
    throw ObjectModelException("Not a string.");
  }
  return s->value;
}

Reference<ObjectModel::Array> ObjectModel::Object::getArray(const char* path) throw(ObjectModelException)
{
  Reference<Value> v = getPath(path, true);
  if (!v) {
    return nullptr;
  }
  Reference<Array> a = v.cast<Array>();
  if (!a) {
    throw ObjectModelException("Not an array.");
  }
  return a;
}

Reference<ObjectModel::Object> ObjectModel::Object::getObject(const char* path) throw(ObjectModelException)
{
  Reference<Value> v = getPath(path, true);
  if (!v) {
    return nullptr;
  }
  Reference<Object> o = v.cast<Object>();
  if (!o) {
    throw ObjectModelException("Not an object.");
  }
  return o;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Value>& value)
{
  if (!value) {
    return stream << MESSAGE("null");
  }
  switch (value->getType()) {
  case ObjectModel::Value::TYPE_VOID:
    return stream << value.cast<ObjectModel::Void>();
  case ObjectModel::Value::TYPE_BOOLEAN:
    return stream << value.cast<ObjectModel::Boolean>();
  case ObjectModel::Value::TYPE_INTEGER:
    return stream << value.cast<ObjectModel::Integer>();
  case ObjectModel::Value::TYPE_FLOAT:
    return stream << value.cast<ObjectModel::Float>();
  case ObjectModel::Value::TYPE_STRING:
    return stream << value.cast<ObjectModel::String>();
  case ObjectModel::Value::TYPE_BINARY:
    return stream << value.cast<ObjectModel::Binary>();
  case ObjectModel::Value::TYPE_ARRAY:
    return stream << value.cast<ObjectModel::Array>();
  case ObjectModel::Value::TYPE_OBJECT:
    return stream << value.cast<ObjectModel::Object>();
  default:
    throw ObjectModelException("Invalid type.");
  }
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Void>& value)
{
  return stream << MESSAGE("null");
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Boolean>& value)
{
  if (!value) {
    return stream << MESSAGE("null");
  }
  return stream << (value->value ? MESSAGE("true") : MESSAGE("false"));
}

// TAG: how can we automatically map to internal classes from object model?
// TAG: add support for indent for string representation

FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Integer>& value)
{
  if (!value) {
    return stream << MESSAGE("null");
  }
  return stream << value->value;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Float>& value)
{
  if (!value) {
    return stream << MESSAGE("null");
  }

  if (Math::isFinite(value->value)) { // not inf or nan
    stream << value->value; // POSIX from higher level
  } else if (Math::isNaN(value->value)) {
    stream << MESSAGE("\"NaN\""); // should we use null instead of string
  } else if (Math::isInfinity(value->value)) {
    stream << ((value->value < 0) ? MESSAGE("-infinity") : MESSAGE("infinity")); // should we use null instead of string
  } else {
    BASSERT(!"Unsupported float.");
    stream << value->value;
  }

  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::String>& value)
{
  if (!value) {
    return stream << MESSAGE("null");
  }

  stream << '"';
  const MemorySize length = value->value.getLength();
  // for (char ch : value->value) { // TAG: need to read as UTF-8?
  for (MemorySize i = 0; i < length; ++i) {
    char ch = value->value[i];
    // TAG: read ucs4
    BASSERT(ch <= 0x7f); // TAG: add support
    if (ch < ' ') {
      stream << '\\';
      switch (ch) {
      case '\b':
        stream << 'b';
        break;
      case '\f':
        stream << 'f';
        break;
      case '\n':
        stream << 'n';
        break;
      case '\r':
        stream << 'r';
        break;
      case '\t':
        stream << 't';
        break;
      default:
        stream << 'u';
        stream << ASCIITraits::valueToDigit(0);
        stream << ASCIITraits::valueToDigit(0);
        stream << ASCIITraits::valueToDigit((ch >> 4) & 0xf);
        stream << ASCIITraits::valueToDigit((ch >> 0) & 0xf);
      }
    } else if (ch == '\\') {
      stream << '\\';
      stream << '\\';
    } else if (ch == '"') {
      stream << '\\';
      stream << '"';
    } else {
      stream << ch;
    }
  }
  return stream << '"';
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Binary>& value)
{
  if (!value) {
    return stream << MESSAGE("null");
  }
  BASSERT(!"Binary block not supported.");
  // TAG: write as string
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Array>& value)
{
  if (!value) {
    return stream << MESSAGE("null");
  }

  unsigned int level = 0;
  ObjectModel::NiceFormat* niceFormat = nullptr;
  if (!value->isEmpty() && (value->getSize() > 4)) { // TAG: make option for line length? - need to try to convert entire
    niceFormat = dynamic_cast<ObjectModel::NiceFormat*>(&stream);
    if (niceFormat) {
      level = niceFormat->enter();
      // TAG: also add support for compact mode
    }
  }

  stream << '[';
  if (niceFormat) {
    stream << EOL;
  }
  bool first = true;
  for (const auto& v : value->values) {
    if (!first) {
      if (niceFormat) {
        stream << MESSAGE(",") << EOL;
      } else {
        stream << MESSAGE(", ");
      }
    }
    first = false;
    if (niceFormat) {
      stream << indent(level * 2);
    }
    stream << v;
  }
  if (!first && niceFormat) {
    stream << EOL;
  }
  if (niceFormat) {
    niceFormat->exit();
    stream << indent((level - 1) * 2) << ']';
  } else {
    stream << ']';
  }

  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Object>& value)
{
  if (!value) {
    return stream << MESSAGE("null");
  }

  unsigned int level = 0;
  ObjectModel::NiceFormat* niceFormat = nullptr;
  if (!value->isEmpty()) {
    niceFormat = dynamic_cast<ObjectModel::NiceFormat*>(&stream);
    if (niceFormat) {
      level = niceFormat->enter();
      // TAG: also add support for compact mode
    }
  }

  stream << '{';
  if (niceFormat) {
    stream << EOL;
  }
  bool first = true;
  for (const auto& v : value->values) {
    if (!first) {
      if (niceFormat) {
        stream << MESSAGE(",") << EOL;
      } else {
        stream << MESSAGE(", ");
      }
    }
    first = false;
    if (niceFormat) {
      stream << indent(level * 2);
    }
    stream << v.first << MESSAGE(": ") << v.second; // "key": value
  }
  if (!first && niceFormat) {
    stream << EOL;
  }
  if (niceFormat) {
    niceFormat->exit();
    stream << indent((level - 1) * 2) << '}';
  } else {
    stream << '}';
  }

  return stream;
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(ObjectModel) : public UnitTest {
public:

  TEST_PRIORITY(200);
  TEST_PROJECT("base/objectmodel");

  void run() override
  {
    ObjectModel o;
    auto root = o.createObject();
    TEST_ASSERT(root->isEmpty());
    root->setValue(o.createString("name"), o.createString("value"));
    root->setValue(o.createString("state"), o.createBoolean(true));
    root->setValue(o.createString("integer"), o.createInteger(-123));
    root->setValue(o.createString("number"), o.createFloat(123.0));
    TEST_ASSERT(!root->isEmpty());
    TEST_ASSERT(root->getSize() == 4);

    auto a = o.createArray();
    root->setValue(o.createString("list"), a);
    a->append(o.createInteger(12));
    a->append(o.createInteger(54));
    a->append(o.createInteger(54));
    a->append(o.createInteger(66));
    a->append(o.createBoolean(false)); // mixed types
    a->append(o.createFloat(-456));
    a->append(o.createString(L"Hello, World!"));
    TEST_ASSERT(a->getSize() == 7);

    auto sub = o.createObject();
    sub->setValue(o.createString("name"), o.createString("John Doe"));
    sub->setValue(o.createString("description"), o.createString("My description."));
    root->setValue(o.createString("sub"), sub);

    TEST_EQUAL(root->getString("name", ""), "value");
    TEST_EQUAL(root->getString("qwerty", "Default"), "Default");
    TEST_EQUAL(root->getBoolean("state", false), true);
    TEST_EQUAL(root->getBoolean("qwerty", false), false);
    TEST_EQUAL(root->getInteger("integer", -1), -123);
    TEST_EQUAL(root->getInteger("qwerty", -1), -1);
    TEST_EQUAL(root->getFloat("number", 0), 123.0);
    TEST_EQUAL(root->getFloat("qwerty", -1.0), -1.0);
    TEST_ASSERT(root->getObject("sub"));
    TEST_ASSERT(!root->getObject("qwerty"));
    TEST_EQUAL(root->getString("sub/name", ""), "John Doe");
    TEST_EQUAL(root->getString("sub/qwerty", ""), "");
  }
};

TEST_REGISTER(ObjectModel);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
