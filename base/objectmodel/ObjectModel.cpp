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
}

Reference<ObjectModel::Void> ObjectModel::createVoid() {
  return new Void();
}

Reference<ObjectModel::Boolean> ObjectModel::createBoolean(bool value) {
  return new Boolean(value);
}

Reference<ObjectModel::Integer> ObjectModel::createInteger(int value) {
  return new Integer(value);
}

Reference<ObjectModel::Float> ObjectModel::createFloat(double value) {
  return new Float(value);
}

Reference<ObjectModel::String> ObjectModel::createString(const char* value) {
  return new String(value);
}

Reference<ObjectModel::Array> ObjectModel::createArray() {
  return new Array();
}

Reference<ObjectModel::Object> ObjectModel::createObject() {
  return new Object();
}

// traverse
// getPath() a/b/c

// TAG: add support for default value
Reference<ObjectModel::Value> ObjectModel::Object::getPath(const char* path) throw(ObjectModelException) {
  if (!path) {
    return nullptr;
  }
#if 0
  while (*path) {
    
  }
#endif
  const char* src = path;
  while (*src && (*src != '/')) {
    ++src;
  }
  String name;
  Reference<Value> v = o->getValue(name);
  Reference<Object> o = v.getAs<Object>();
  if (o) {
    if (*src == '/') {
      ++src;
    }
    return o->getPath(src); // TAG: avoid recursion - use loop
  }
}

Reference<ObjectModel::Value> ObjectModel::Object::getPath(const char* path, bool defaultValue) throw(ObjectModelException) {
  Reference<Value> v = getPath(path);
  if (!v) {
    return ObjectModel().createBoolean(defaultValue);
  }
  return v;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
