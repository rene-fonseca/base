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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

ObjectModel::ObjectModel()
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
    values[i++] = new Boolean(v); // TAG: add reuse
  }
  return *this;
}

ObjectModel::Array& ObjectModel::Array::operator=(const std::vector<int>& _values)
{
  values.setSize(_values.size());
  MemorySize i = 0;
  for (const auto& v : _values) {
    values[i++] = new Integer(v); // TAG: add reuse
  }
  return *this;
}

ObjectModel::Array& ObjectModel::Array::operator=(const std::vector<double>& _values)
{
  values.setSize(_values.size());
  MemorySize i = 0;
  for (const auto& v : _values) {
    values[i++] = new Float(v); // TAG: add reuse
  }
  return *this;
}

ObjectModel::Array& ObjectModel::Array::operator=(const std::vector<base::String>& _values)
{
  values.setSize(_values.size());
  MemorySize i = 0;
  for (const auto& v : _values) {
    values[i++] = new String(v); // TAG: add reuse
  }
  return *this;
}

template<>
inline std::vector<bool> ObjectModel::Array::getAs<bool>() const {
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
inline std::vector<int> ObjectModel::Array::getAs<int>() const {
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
inline std::vector<double> ObjectModel::Array::getAs<double>() const {
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
inline std::vector<base::String> ObjectModel::Array::getAs<base::String>() const {
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
    bassert(v.first, NullPointer("Key is null."));
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
    bassert(v.first, NullPointer("Key is null."));
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
    bassert(v.first, NullPointer("Key is null."));
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
    while (*path && (*path != '/')) { // find separator
      ++path;
    }
    const base::String key(begin, path - begin);
    if (*path) { // skip separator
      ++path;
    }
    if (key.isEmpty()) { // do not allow empty keys for now
      throw ObjectModelException("Invalid path.");
    }

    // find value
    Reference<Value> result;
    for (const auto& v : current->values) {
      bassert(v.first, NullPointer("Key is null."));
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
    if (!*path) {
      return result;
    }
    current = result.cast<Object>();
  }
  ASSERT(!*path);
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
  return stream << value->value;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::String>& value)
{
  if (!value) {
    return stream << MESSAGE("null");
  }
  return stream << '"' << value->value << '"';
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
      stream << MESSAGE(", ");
    }
    first = false;
    stream << v;
  }
  return stream << ']';
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
      stream << MESSAGE(", ");
    }
    first = false;
    stream << v.first << MESSAGE(": ") << v.second; // "key": value
  }
  return stream << '}';
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
