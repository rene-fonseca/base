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
#include <base/math/Math.h>
#include <base/string/ANSIEscapeSequence.h>
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
  commonFloatM0 = new Float(-0.);
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

// internal format wrapper
inline ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const ObjectModel::Value& value)
{
  return operator<<(stream, Reference<ObjectModel::Value>(const_cast<ObjectModel::Value*>(&value)));
}

base::String ObjectModel::Value::toString(unsigned int flags) const noexcept
{
  if (flags != 0) {
    ObjectModel::NiceFormat s;
    s.setNiceFlags(flags);
    s.setIndent((flags & FLAG_USE_TAB) ? 1 : 2);
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

Reference<ObjectModel::Integer> ObjectModel::createInteger(int64 value)
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
    return (1/value < 0) ? commonFloatM0 : commonFloat0;
  } else if (value == 1) {
    return commonFloat1;
  } else if (value == -1) {
    return commonFloatMinus1;
  } else {
    return new Float(value);
  }
}

Reference<ObjectModel::Comment> ObjectModel::createComment(const base::String& value)
{
  return new Comment(value); // new comment
}

Reference<ObjectModel::String> ObjectModel::createString(const char* value)
{
  if (!value || !*value) {
    return commonStringEmpty;
  }
  const base::String _value(value);
  auto i = lookup.find(value);
  if (i) {
    return *i; // found identical
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
  if (i) {
    return *i; // found identical
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

MemorySize ObjectModel::Comment::getSize() const noexcept
{
  return value.getLength();
}

MemorySize ObjectModel::String::getSize() const noexcept
{
  return value.getLength();
}

MemorySize ObjectModel::Array::getSize() const noexcept
{
  return values.getSize();
}

Reference<ObjectModel::Value> ObjectModel::Array::getAt(MemorySize index) const
{
  if (index >= values.getSize()) {
    _throw OutOfRange();
  }
  return values[index];
}

void ObjectModel::Array::setAt(MemorySize index, const Reference<Value>& value)
{
  if (index >= values.getSize()) {
    _throw OutOfRange();
  }
  values[index] = value;
}

void ObjectModel::Array::append(const Reference<Value>& value)
{
  values.append(value);
}

ObjectModel::Array& ObjectModel::Array::operator=(const base::Array<bool>& _values)
{
  values.setSize(_values.getSize());
  MemorySize i = 0;
  for (const auto& v : _values) {
    values[i++] = globalObjectModel.createBoolean(v); // always reusing
  }
  return *this;
}

ObjectModel::Array& ObjectModel::Array::operator=(const base::Array<int>& _values)
{
  values.setSize(_values.getSize());
  MemorySize i = 0;
  Reference<Integer> previous; // catch easy reuse case
  for (auto v : _values) {
    if (previous && (v == static_cast<int>(*previous))) {
      values[i++] = previous;
    } else {
      previous = globalObjectModel.createInteger(v);
      values[i++] = previous;
    }
  }
  return *this;
}

ObjectModel::Array& ObjectModel::Array::operator=(const base::Array<double>& _values)
{
  values.setSize(_values.getSize());
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

ObjectModel::Array& ObjectModel::Array::operator=(const base::Array<base::String>& _values)
{
  values.setSize(_values.getSize());
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
base::Array<bool> ObjectModel::Array::getAs<bool>() const
{
  base::Array<bool> result;
  result.setSize(values.getSize());
  auto dest = result.getBeginIterator();
  for (const auto& v : values) {
    auto _v = v.cast<Boolean>();
    if (!_v) {
      _throw ObjectModelException("Expected boolean array.");
    }
    *dest++ = _v->value;
  }
  return result;
}

template<>
base::Array<int> ObjectModel::Array::getAs<int>() const
{
  base::Array<int> result;
  result.setSize(values.getSize());
  auto dest = result.getBeginIterator();
  for (const auto& v : values) {
    auto _v = v.cast<Boolean>();
    if (!_v) {
      _throw ObjectModelException("Expected integer array.");
    }
    *dest++ = _v->value;
  }
  return result;
}

template<>
base::Array<double> ObjectModel::Array::getAs<double>() const
{
  base::Array<double> result;
  result.setSize(values.getSize());
  auto dest = result.getBeginIterator();
  for (const auto& v : values) {
    auto _v = v.cast<Float>();
    if (!_v) {
      _throw ObjectModelException("Expected float array.");
    }
    *dest++ = _v->value;
  }
  return result;
}

template<>
base::Array<base::String> ObjectModel::Array::getAs<base::String>() const
{
  base::Array<base::String> result;
  result.setSize(values.getSize());
  auto dest = result.getBeginIterator();
  for (const auto& v : values) {
    auto _v = v.cast<String>();
    if (!_v) {
      _throw ObjectModelException("Expected string array.");
    }
    *dest++ = _v->value;
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
    if (!INLINE_ASSERT(v.getFirst())) {
      continue;
    }
    if (v.getFirst()->value == key->value) {
      return true;
    }
  }
  return false;
}

bool ObjectModel::Object::hasKey(const base::String& key) const noexcept
{
  for (const auto& v : values) {
    if (!INLINE_ASSERT(v.getFirst())) {
      continue;
    }
    if (v.getFirst()->value == key) {
      return true;
    }
  }
  return false;
}

bool ObjectModel::Object::hasKey(const char* _key) const noexcept
{
  if (!_key) {
    return false;
  }
  NativeString key(_key);
  for (const auto& v : values) {
    if (!INLINE_ASSERT(v.getFirst())) {
      continue;
    }
    if (v.getFirst()->value == key) {
      return true;
    }
  }
  return false;
}

bool ObjectModel::Object::removeKey(const Reference<String>& key) noexcept
{
  if (!key) {
    return false;
  }
  for (auto i = values.begin(); i != values.end(); ++i) {
    auto j = *i;
    if (j.getFirst()->value == key->value) {
      values.remove(i);
      return true;
    }
  }
  return false;
}

bool ObjectModel::Object::removeKey(const base::String& key) noexcept
{
  for (auto i = values.begin(); i != values.end(); ++i) {
    auto j = *i;
    if (j.getFirst()->value == key) {
      values.remove(i);
      return true;
    }
  }
  return false;
}

bool ObjectModel::Object::removeKey(const char* _key) noexcept
{
  if (!_key) {
    return false;
  }
  NativeString key(_key);
  for (auto i = values.begin(); i != values.end(); ++i) {
    auto j = *i;
    if (j.getFirst()->value == key) {
      values.remove(i);
      return true;
    }
  }
  return false;
}

Reference<ObjectModel::Value> ObjectModel::Object::getValue(const Reference<String>& key) const noexcept
{
  if (!key) {
    return nullptr;
  }
  for (const auto& v : values) {
    if (!INLINE_ASSERT(v.getFirst())) {
      continue;
    }
    if (v.getFirst()->value == key->value) {
      return v.getSecond();
    }
  }
  return nullptr;
}

Reference<ObjectModel::Value> ObjectModel::Object::getValue(const base::String& key) const noexcept
{
  for (const auto& v : values) {
    if (!INLINE_ASSERT(v.getFirst())) {
      continue;
    }
    if (v.getFirst()->value == key) {
      return v.getSecond();
    }
  }
  return nullptr;
}

Reference<ObjectModel::Value> ObjectModel::Object::getValue(const char* _key) const noexcept
{
  if (!_key) {
    return nullptr;
  }
  NativeString key(_key);
  for (const auto& v : values) {
    if (!INLINE_ASSERT(v.getFirst())) {
      continue;
    }
    if (v.getFirst()->value == key) {
      return v.getSecond();
    }
  }
  return nullptr;
}

void ObjectModel::Object::setValue(const Reference<String>& key, const Reference<Value>& value)
{
  if (!key) {
    _throw NullPointer("Key is null.");
  }
  for (auto& v : values) {
    if (!INLINE_ASSERT(v.getFirst())) {
      continue;
    }
    if (v.getFirst()->value == key->value) {
      v.setSecond(value);
      return;
    }
  }
  values.append(Association(key, value));
}

void ObjectModel::Object::setValue(const Reference<String>& key, const base::String& value)
{
  if (!key) {
    _throw NullPointer("Key is null.");
  }
  auto _value = globalObjectModel.createString(value);
  for (auto& v : values) {
    if (!INLINE_ASSERT(v.getFirst())) {
      continue;
    }
    if (v.getFirst()->value == key->value) {
      v.setSecond(_value);
      return;
    }
  }
  values.append(Association(key, _value));
}

void ObjectModel::Object::setValue(const base::String& key, const Reference<Value>& value)
{
  for (auto& v : values) {
    if (!INLINE_ASSERT(v.getFirst())) {
      continue;
    }
    if (v.getFirst()->value == key) {
      v.setSecond(value);
      return;
    }
  }
  values.append(Association(globalObjectModel.createString(key), value));
}

void ObjectModel::Object::setValue(const base::String& key, NullPtr)
{
  for (auto& v : values) {
    if (!INLINE_ASSERT(v.getFirst())) {
      continue;
    }
    if (v.getFirst()->value == key) {
      v.setSecond(nullptr);
      return;
    }
  }
  values.append(Association(globalObjectModel.createString(key), nullptr));
}

void ObjectModel::Object::setValue(const base::String& key, const bool value)
{
  for (auto& v : values) {
    if (!INLINE_ASSERT(v.getFirst())) {
      continue;
    }
    if (v.getFirst()->value == key) {
      v.setSecond(globalObjectModel.createBoolean(value));
      return;
    }
  }
  values.append(Association(globalObjectModel.createString(key), globalObjectModel.createBoolean(value)));
}

void ObjectModel::Object::setValue(const base::String& key, const int64 value)
{
  for (auto& v : values) {
    if (!INLINE_ASSERT(v.getFirst())) {
      continue;
    }
    if (v.getFirst()->value == key) {
      v.setSecond(globalObjectModel.createInteger(value));
      return;
    }
  }
  values.append(Association(globalObjectModel.createString(key), globalObjectModel.createInteger(value)));
}

void ObjectModel::Object::setValue(const base::String& key, const double value)
{
  for (auto& v : values) {
    if (!INLINE_ASSERT(v.getFirst())) {
      continue;
    }
    if (v.getFirst()->value == key) {
      v.setSecond(globalObjectModel.createFloat(value));
      return;
    }
  }
  values.append(Association(globalObjectModel.createString(key), globalObjectModel.createFloat(value)));
}

void ObjectModel::Object::setValue(const base::String& key, const base::String& value)
{
  for (auto& v : values) {
    if (!INLINE_ASSERT(v.getFirst())) {
      continue;
    }
    if (v.getFirst()->value == key) {
      v.setSecond(globalObjectModel.createString(value));
      return;
    }
  }
  values.append(Association(globalObjectModel.createString(key), globalObjectModel.createString(value)));
}

void ObjectModel::Object::setValue(const char* key, const char* value)
{
  if (!key) {
    _throw NullPointer("Key is null.");
  }
  if (!value) {
    setValue(base::String(key), nullptr);
  } else {
    setValue(base::String(key), base::String(value));
  }
}

// TAG: add support for setting value by path - "-" can be used to append to array
// TAG: add support for parsed pointer - to avoid allocations

Reference<ObjectModel::Value> ObjectModel::Object::getPath(const char* path, bool forceNull)
{
  if (!path) {
    return nullptr;
  }
  if (!*path) {
    return this; // entire document
  }
  
  Reference<Value> current = this;
  while (*path) {
    if (*path != '/') {
      _throw ObjectModelException("Invalid path.");
    }
    ++path;
    
    if (auto o = current.cast<Object>()) {
      const char* begin = path;
      while (*path && (*path != '/')) { // find separator
        ++path;
      }
      base::String key(begin, path - begin);
      key.replaceAll("~1", "/"); // must be first
      key.replaceAll("~0", "~");
      
      Reference<Value> result = o->getValue(key);
      if (!result) {
        if (forceNull) {
          return nullptr;
        }
        _throw ObjectModelException("Path not found.");
      }
      current = result;

    } else if (auto a = current.cast<Array>()) {
      const char* ibegin = path;
      if (*path == '0') {
        ++path;
      } else if ((*path >= '1') && (*path <= '9')) {
        ++path;
        while ((*path >= '0') && (*path <= '9')) {
          ++path;
        }
      }
      if (path == ibegin) {
        _throw ObjectModelException("Invalid array index.");
      }
      long long arrayIndex = LongInteger::parse(ibegin, path, 0);
      if (arrayIndex < 0) {
        _throw ObjectModelException("Invalid array index.");
      }
      if (arrayIndex > PrimitiveTraits<MemorySize>::MAXIMUM) {
        _throw ObjectModelException("Invalid array index.");
      }
      
      if (!((arrayIndex >= 0) && (static_cast<MemorySize>(arrayIndex) < a->values.getSize()))) {
        if (forceNull) {
          return nullptr;
        }
        _throw ObjectModelException("Array index out of range.");
      }
      current = a->values[arrayIndex];
    } else {
      if (forceNull) {
        return nullptr;
      }
      _throw ObjectModelException("Path not found.");
    }
  }
  BASSERT(!*path);
  return current;
}

bool ObjectModel::Object::getBoolean(const char* path, bool defaultValue)
{
  Reference<Value> v = getPath(path, true);
  if (!v) {
    return defaultValue;
  }
  Reference<Boolean> b = v.cast<Boolean>();
  if (!b) {
    _throw ObjectModelException("Not a boolean.");
  }
  return b->value;
}

int64 ObjectModel::Object::getInteger(const char* path, int64 defaultValue)
{
  Reference<Value> v = getPath(path, true);
  if (!v) {
    return defaultValue;
  }
  Reference<Integer> i = v.cast<Integer>();
  if (!i) {
    _throw ObjectModelException("Not an integer.");
  }
  return i->value;
}

double ObjectModel::Object::getFloat(const char* path, double defaultValue) {
  Reference<Value> v = getPath(path, true);
  if (!v) {
    return defaultValue;
  }
  Reference<Float> d = v.cast<Float>();
  if (!d) {
    _throw ObjectModelException("Not a float.");
  }
  return d->value;
}

base::String ObjectModel::Object::getString(const char* path, const base::String& defaultValue)
{
  Reference<Value> v = getPath(path, true);
  if (!v) {
    return defaultValue;
  }
  Reference<String> s = v.cast<String>();
  if (!s) {
    _throw ObjectModelException("Not a string.");
  }
  return s->value;
}

Reference<ObjectModel::Array> ObjectModel::Object::getArray(const char* path)
{
  Reference<Value> v = getPath(path, true);
  if (!v) {
    return nullptr;
  }
  Reference<Array> a = v.cast<Array>();
  if (!a) {
    _throw ObjectModelException("Not an array.");
  }
  return a;
}

Reference<ObjectModel::Object> ObjectModel::Object::getObject(const char* path)
{
  Reference<Value> v = getPath(path, true);
  if (!v) {
    return nullptr;
  }
  Reference<Object> o = v.cast<Object>();
  if (!o) {
    _throw ObjectModelException("Not an object.");
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
  case ObjectModel::Value::TYPE_COMMENT:
    return stream << value.cast<ObjectModel::Comment>();
  case ObjectModel::Value::TYPE_STRING:
    return stream << value.cast<ObjectModel::String>();
  case ObjectModel::Value::TYPE_BINARY:
    return stream << value.cast<ObjectModel::Binary>();
  case ObjectModel::Value::TYPE_ARRAY:
    return stream << value.cast<ObjectModel::Array>();
  case ObjectModel::Value::TYPE_OBJECT:
    return stream << value.cast<ObjectModel::Object>();
  default:
    _throw ObjectModelException("Invalid type.");
  }
}

ObjectModel::NiceFormat& ObjectModel::NiceFormat::writeKeyword(const Literal& literal)
{
  if (flags & FLAG_COLOR) {
    if (flags & FLAG_COLOR_RGB) {
      *this << setForeground(ANSIEscapeSequence::MAGENTA) << bold() << literal << normal();
    } else {
      *this << setForeground(ANSIEscapeSequence::MAGENTA) << bold() << literal << normal();
    }
  } else {
    *this << literal;
  }
  return *this;
}

ObjectModel::NiceFormat& ObjectModel::NiceFormat::writeTextUnquoted(const Literal& literal)
{
  if (flags & FLAG_COLOR) {
    if (flags & FLAG_COLOR_RGB) {
      *this << setForeground(ANSIEscapeSequence::MAGENTA) << bold() << '"' << literal << '"'<< normal();
    } else {
      *this << setForeground(ANSIEscapeSequence::MAGENTA) << bold() << '"' << literal << '"'<< normal();
    }
  } else {
    *this << '"' << literal << '"';
  }
  return *this;
}

ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Value>& value)
{
  if (!value) {
    return stream.writeKeyword(MESSAGE("null"));
  }

  switch (value->getType()) {
  case ObjectModel::Value::TYPE_BOOLEAN:
    return stream << value.cast<ObjectModel::Boolean>();
  case ObjectModel::Value::TYPE_INTEGER:
    return stream << value.cast<ObjectModel::Integer>();
  case ObjectModel::Value::TYPE_FLOAT:
    return stream << value.cast<ObjectModel::Float>();
  case ObjectModel::Value::TYPE_STRING:
    return stream << value.cast<ObjectModel::String>();
  case ObjectModel::Value::TYPE_ARRAY:
    return stream << value.cast<ObjectModel::Array>();
  case ObjectModel::Value::TYPE_OBJECT:
    return stream << value.cast<ObjectModel::Object>();
  case ObjectModel::Value::TYPE_VOID:
    return stream << value.cast<ObjectModel::Void>();
  case ObjectModel::Value::TYPE_COMMENT:
    return stream << value.cast<ObjectModel::Comment>();
  case ObjectModel::Value::TYPE_BINARY:
    return stream << value.cast<ObjectModel::Binary>();
  default:
    _throw ObjectModelException("Invalid type.");
  }
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Void>& value)
{
  stream << MESSAGE("null");
  return stream;
}

ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Void>& value)
{
  return stream.writeKeyword(MESSAGE("null"));
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Boolean>& value)
{
  if (!value) {
    return stream << MESSAGE("null");
  }
  return stream << (value->value ? MESSAGE("true") : MESSAGE("false"));
}

ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Boolean>& value)
{
  if (!value) {
    return stream.writeKeyword(MESSAGE("null"));
  }
  return stream.writeKeyword(value->value ? MESSAGE("true") : MESSAGE("false"));
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

ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Integer>& value)
{
  if (!value) {
    return stream.writeKeyword(MESSAGE("null"));
  }

  const bool useANSI = stream.getNiceFlags() & ObjectModel::FLAG_COLOR;
  if (useANSI) {
    if (stream.getNiceFlags() & ObjectModel::FLAG_COLOR_RGB) {
      stream << setForeground(ANSIEscapeSequence::BLUE);
    } else {
      stream << setForeground(ANSIEscapeSequence::BLUE);
    }
  }
  stream << value->value;
  if (useANSI) {
    stream << normal();
  }
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Float>& value)
{
  if (!value) {
    return stream << MESSAGE("null");
  }
  
  if (Math::isFinite(value->value)) { // not inf or nan
    stream << ENSUREFLOAT << value->value; // POSIX from higher level
  } else if (Math::isNaN(value->value)) {
    stream << MESSAGE("\"nan\""); // should we use null instead of string
  } else if (Math::isInfinity(value->value)) {
    stream << ((value->value < 0) ? MESSAGE("-infinity") : MESSAGE("infinity")); // should we use null instead of string
  } else {
    BASSERT(!"Unsupported float.");
    stream << ENSUREFLOAT << value->value;
  }

  return stream;
}

ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Float>& value)
{
  if (!value) {
    return stream.writeKeyword(MESSAGE("null"));
  }

  if (!Math::isFinite(value->value)) { // not inf or nan
    if (stream.getNiceFlags() & ObjectModel::FLAG_USE_NULL_FOR_SPECIAL) {
      return stream.writeKeyword(MESSAGE("null"));
    } else if (Math::isNaN(value->value)) {
      return stream.writeTextUnquoted(MESSAGE("NaN"));
    } else if (Math::isInfinity(value->value)) {
      return stream.writeTextUnquoted((value->value < 0) ? MESSAGE("-infinity") : MESSAGE("infinity")); // should we use null instead of string
    } else {
      BASSERT(!"Unsupported float.");
    }
  }

  const bool useANSI = stream.getNiceFlags() & ObjectModel::FLAG_COLOR;
  if (useANSI) {
    if (stream.getNiceFlags() & ObjectModel::FLAG_COLOR_RGB) {
      stream << setForeground(ANSIEscapeSequence::BLUE);
    } else {
      stream << setForeground(ANSIEscapeSequence::BLUE);
    }
  }
  stream << ENSUREFLOAT << value->value; // POSIX from higher level
  if (useANSI) {
    stream << normal();
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
  stream << '"';
  return stream;
}

namespace {

#if 0
  void parseUrl(Parser& parser)
  {
    if (parser.peek() : "a-zA-Z");
    parser.skip("https"); // TAG: consisting of a sequence of characters beginning with a letter and followed by any combination of letters, digits, plus (+), period (.), or hyphen (-)
    parser.skip(':');
    parser.skip("//");
    skip user
    skip password
    parser.skip('@');
    skip host
    parser.skip(':');
    // skip port parser.skip();
    // parser skip path
    if (parser.peek() == '?') {
      parser.skip('?');
      parser skip query
    }
    if (parser.peek() == '#') {
      parser.skip('#');
      parser skip fragment
    }
  }

  void parseUrl(const String& text)
  {
    Parser paser(url);
    parseUrl(parser);
  }
#endif
}

ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::String>& value)
{
  if (!value) {
    return stream.writeKeyword(MESSAGE("null"));
  }
  
  const bool useANSI = stream.getNiceFlags() & ObjectModel::FLAG_COLOR;
  bool showUrl = false;
  if (useANSI) {
    // TAG: check if valid url and highlight
    showUrl = value->value.startsWith("https://") || value->value.startsWith("http://") || value->value.startsWith("wss://");
    if (stream.getNiceFlags() & ObjectModel::FLAG_COLOR_RGB) {
      stream << ANSIEscapeSequence::color(255, 0, 0);
    } else {
      if (showUrl) {
        stream << underscore();
        stream << setForeground(ANSIEscapeSequence::GREEN); // TAG: find nice color
      } else {
        stream << setForeground(ANSIEscapeSequence::RED);
      }
    }
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
  stream << '"';
  if (useANSI) {
    if (showUrl) {
      stream << nounderscore();
    }
    stream << normal();
  }
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Comment>& value)
{
  if (!value) {
    return stream; // skip
  }
  
  stream << "// ";
  const MemorySize length = value->value.getLength();
  for (MemorySize i = 0; i < length; ++i) {
    char ch = value->value[i];
    // TAG: read ucs4
    BASSERT(ch <= 0x7f); // TAG: add support
    switch (ch) {
    case '\n':
      stream << " ";
      break;
    case '\r':
      stream << " ";
      break;
    default:
      stream << ch;
    }
  }
  return stream;
}

ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Comment>& value)
{
  if (!value) {
    return stream; // skip
  }
  
  const bool useANSI = stream.getNiceFlags() & ObjectModel::FLAG_COLOR;
  if (useANSI) {
    if (stream.getNiceFlags() & ObjectModel::FLAG_COLOR_RGB) {
      stream << setForeground(ANSIEscapeSequence::GREEN);
    } else {
      stream << setForeground(ANSIEscapeSequence::GREEN);
    }
  }

  stream << "// ";
  const MemorySize length = value->value.getLength();
  for (MemorySize i = 0; i < length; ++i) {
    char ch = value->value[i];
    // TAG: read ucs4
    BASSERT(ch <= 0x7f); // TAG: add support
    switch (ch) {
    case '\n':
      stream << " ";
      break;
    case '\r':
      stream << " ";
      break;
    default:
      stream << ch;
    }
  }
  if (useANSI) {
    stream << normal();
  }
  return stream;
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

ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Binary>& value)
{
  if (!value) {
    return stream.writeKeyword(MESSAGE("null"));
  }

  BASSERT(!"Binary block not supported.");
  // TAG: write as string Base64?
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Array>& value)
{
  if (!value) {
    return stream << MESSAGE("null");
  }

  stream << '[';
  bool first = true;
  for (const auto& v : value->values) {
    if (!first) {
      stream << ','; // no space for compact
    }
    first = false;
    stream << v;
  }
  stream << ']';

  return stream;
}

ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Array>& value)
{
  if (!value) {
    return stream.writeKeyword(MESSAGE("null"));
  }

  unsigned int level = 0;
  if (!value->isEmpty() && (value->getSize() > 4)) { // TAG: make option for line length? - need to try to convert entire
    if (stream.getNiceFlags() & ObjectModel::FLAG_INDENT) {
      level = stream.enter();
    }
  }

  stream << '[';
  if (level > 0) {
    stream << EOL;
  }
  bool first = true;
  for (const auto& v : value->values) {
    if (!first) {
      if (level > 0) {
        stream << MESSAGE(",") << EOL;
      } else {
        stream << MESSAGE(", ");
      }
    }
    first = false;
    if (level > 0) {
      stream.writeIndent();
    }
    stream << v;
  }
  if (!first && (level > 0)) {
    stream << EOL;
  }
  if (level > 0) {
    stream.exit();
    stream.writeIndent();
  }
  stream << ']';

  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Object>& value)
{
  if (!value) {
    return stream << MESSAGE("null");
  }

  stream << '{';
  bool first = true;
  for (const auto& v : value->values) {
    if (!first) {
      stream << ','; // no space for compact
    }
    first = false;
    stream << v.getFirst() << ':' << v.getSecond(); // "key": value // no space for compact
  }
  stream << '}';

  return stream;
}

namespace {
  
#if 0
  void getStats(ObjectModel::Stats& stats, const Reference<ObjectModel::Void>& value);
  void getStats(ObjectModel::Stats& stats, const Reference<ObjectModel::Boolean>& value);
  void getStats(ObjectModel::Stats& stats, const Reference<ObjectModel::Integer>& value);
  void getStats(ObjectModel::Stats& stats, const Reference<ObjectModel::Float>& value);
  void getStats(ObjectModel::Stats& stats, const Reference<ObjectModel::String>& value);
  void getStats(ObjectModel::Stats& stats, const Reference<ObjectModel::Binary>& value);
  void getStats(ObjectModel::Stats& stats, const Reference<ObjectModel::Comment>& value)
#endif

  void getStatsImpl(ObjectModel::Stats& stats, const Reference<ObjectModel::Value>& value);

  void getStatsImpl(ObjectModel::Stats& stats, const Reference<ObjectModel::Array>& value)
  {
    if (!value) {
      return;
    }
    for (const auto& v : value->values) {
      getStatsImpl(stats, v);
    }
  }

  void getStatsImpl(ObjectModel::Stats& stats, const Reference<ObjectModel::Object>& value)
  {
    if (!value) {
      return;
    }
    for (const auto& v : value->values) {
      stats.numberOfStrings++; // should we count fields also
      getStatsImpl(stats, v.getSecond());
    }
  }
  
void getStatsImpl(ObjectModel::Stats& stats, const Reference<ObjectModel::Value>& value)
{
  if (!value) {
    stats.numberOfNulls++;
    return;
  }

  switch (value->getType()) {
  case ObjectModel::Value::TYPE_BOOLEAN:
    // getStats(stats, value.cast<ObjectModel::Boolean>());
    stats.numberOfBools++;
    break;
  case ObjectModel::Value::TYPE_INTEGER:
    // getStats(stats, value.cast<ObjectModel::Integer>());
    stats.numberOfInts++;
    break;
  case ObjectModel::Value::TYPE_FLOAT:
    // getStats(stats, value.cast<ObjectModel::Float>());
    stats.numberOfFloats++;
    break;
  case ObjectModel::Value::TYPE_STRING:
    // getStats(stats, value.cast<ObjectModel::String>());
    stats.numberOfStrings++;
    break;
  case ObjectModel::Value::TYPE_ARRAY:
    getStatsImpl(stats, value.cast<ObjectModel::Array>());
    break;
  case ObjectModel::Value::TYPE_OBJECT:
    getStatsImpl(stats, value.cast<ObjectModel::Object>());
    break;
  case ObjectModel::Value::TYPE_VOID:
    // getStats(stats, value.cast<ObjectModel::Void>());
    stats.numberOfNulls++;
    break;
  case ObjectModel::Value::TYPE_COMMENT:
    // getStats(stats, value.cast<ObjectModel::Comment>());
    stats.numberOfComments++;
    break;
  case ObjectModel::Value::TYPE_BINARY:
    // getStats(stats, value.cast<ObjectModel::Binary>());
    stats.numberOfBinaries++;
    break;
  default:
    _throw ObjectModelException("Invalid type.");
  }
}

}

ObjectModel::Stats ObjectModel::getStats(const Reference<ObjectModel::Value>& value)
{
  ObjectModel::Stats stats;
  getStatsImpl(stats, value);
  return stats;
}

namespace {

  inline ObjectModel::NiceFormat& operator<<(FormatOutputStream& _stream, ObjectModel::NiceFormat& stream)
  {
    static_assert(std::is_base_of<FormatOutputStream, ObjectModel::NiceFormat>(), "Stream must derive from FormatOutputStream.");
    return stream;
  }
}

namespace internal {

  /** Cast stream to type. */
  template<class TYPE>
  inline TYPE& operator<<(FormatOutputStream& _stream, TYPE& stream)
  {
    static_assert(std::is_base_of<FormatOutputStream, TYPE>(), "Stream must derived from FormatOutputStream.");
    BASSERT(static_cast<FormatOutputStream*>(&stream) == &_stream);
    return stream;
  }
}

ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Object>& value)
{
  if (!value) {
    return stream.writeKeyword(MESSAGE("null"));
  }

  const bool useANSI = stream.getNiceFlags() & ObjectModel::FLAG_COLOR;

  unsigned int level = 0;
  if (!value->isEmpty()) {
    if (stream.getNiceFlags() & ObjectModel::FLAG_INDENT) {
      level = stream.enter();
    }
  }

  stream << '{';
  if (level > 0) {
    stream << EOL;
  }
  bool first = true;
  for (const auto& v : value->values) {
    if (!first) {
      if (level > 0) {
        stream << MESSAGE(",") << EOL;
      } else {
        stream << MESSAGE(", ");
      }
    }
    first = false;
    if (level > 0) {
      stream.writeIndent();
    }
    // << casts to FormatOutputStream!
    if (useANSI) {
      stream << bold();
    }
    stream << v.getFirst();
    if (useANSI) {
      stream << normal();
    }
    stream << MESSAGE(": ") << stream << v.getSecond(); // "key": value
  }
  if (!first && (level > 0)) {
    stream << EOL;
  }
  if (level > 0) {
    stream.exit();
    stream.writeIndent();
  }
  stream << '}';

  return stream;
}

namespace {

  class XMLContext {
  private:

    Document& d;
    String arrayName = "item";
  public:

    XMLContext(Document& _d, const String& _arrayName)
      : d(_d), arrayName(_arrayName)
    {
    }

    Node getXMLNode(Node& n, const Reference<ObjectModel::Value>& value);
  };

Node XMLContext::getXMLNode(Node& n, const Reference<ObjectModel::Value>& value)
{
  switch (value->getType()) {
  case ObjectModel::Value::TYPE_BOOLEAN:
    return d.createText((value.cast<ObjectModel::Boolean>()) ? "true" : "false");
  case ObjectModel::Value::TYPE_INTEGER:
    return d.createText(StringOutputStream() << (value.cast<ObjectModel::Integer>())->value);
  case ObjectModel::Value::TYPE_FLOAT:
    return d.createText(StringOutputStream() << (value.cast<ObjectModel::Float>())->value);
  case ObjectModel::Value::TYPE_STRING:
    return d.createText((value.cast<ObjectModel::String>())->value);
  case ObjectModel::Value::TYPE_ARRAY:
    {
      auto a = value.cast<ObjectModel::Array>();
      for (auto v : a->values) {
        Element e = d.createElement(arrayName);
        e.appendChild(getXMLNode(e, v));
        n.appendChild(e);
      }
      return Node();
    }
  case ObjectModel::Value::TYPE_OBJECT:
    {
      auto o = value.cast<ObjectModel::Object>();
      for (auto v : o->values) {
        Element e = d.createElement(*(v.getFirst()));
        if (Node sn = getXMLNode(e, v.getSecond())) {
          e.appendChild(sn);
        }
        n.appendChild(e);
      }
      return Node();
    }
  case ObjectModel::Value::TYPE_VOID:
    return d.createText("null");
  case ObjectModel::Value::TYPE_COMMENT:
    return d.createComment(*(value.cast<ObjectModel::Comment>()));
  case ObjectModel::Value::TYPE_BINARY:
    return d.createText((value.cast<ObjectModel::Binary>())->toString()); // TAG: encode as base64?
  default:
    ;
  }
  _throw ObjectModelException("Invalid type.");
}

} // namespace

Document ObjectModel::getXML(const Reference<ObjectModel::Value>& value, const String& name, const String& ns)
{
  if (!value) {
    return Document();
  }

  DOMImplementation dom;
  Document d = dom.createDocument();
  Element r = d.createElementNS(ns, name);
  d.appendChild(r);
  XMLContext context(d, "item");
  if (Node n = context.getXMLNode(r, value)) {
    r.appendChild(n);
  }
  return d;
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

    TEST_EQUAL(root->getString("/name", ""), "value");
    TEST_EQUAL(root->getString("/qwerty", "Default"), "Default");
    TEST_EQUAL(root->getBoolean("/state", false), true);
    TEST_EQUAL(root->getBoolean("/qwerty", false), false);
    TEST_EQUAL(root->getInteger("/integer", -1), -123);
    TEST_EQUAL(root->getInteger("/qwerty", -1), -1);
    TEST_EQUAL(root->getFloat("/number", 0), 123.0);
    TEST_EQUAL(root->getFloat("/qwerty", -1.0), -1.0);
    TEST_ASSERT(root->getObject("/sub"));
    TEST_ASSERT(!root->getObject("/qwerty"));
    TEST_EQUAL(root->getString("/sub/name", ""), "John Doe");
    TEST_EQUAL(root->getString("/sub/qwerty", ""), "");

    if (DOMImplementation::isSupported()) {
      Document d = ObjectModel::getXML(root, "root", "ns:json"); // TAG: fix namespace
      if (d) {
        base::String xml = DOMImplementation().saveDocumentToMemory(d, true);
        TEST_ASSERT(xml);
        if (TEST_IS_DEFINED("writeResults")) {
          fout << xml << ENDL;
        }
      }
    }
  }
};

TEST_REGISTER(ObjectModel);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
