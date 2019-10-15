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

#include <base/Exception.h>
#include <base/mem/Reference.h>
#include <base/collection/Array.h>
#include <base/string/FormatOutputStream.h>
#include <map>
#include <vector>
#include <utility>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This exception is raised when ObjectModel problem occurs.
  
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API ObjectModelException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  inline ObjectModelException() throw() {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline ObjectModelException(const char* message) throw() : Exception(message) {
  }
};

/** Generic object model. */
class _COM_AZURE_DEV__BASE__API ObjectModel : public ReferenceCountedObject {
public:

  /** Value. */
  class _COM_AZURE_DEV__BASE__API Value : public ReferenceCountedObject {
  public:
    
    enum Type {
      TYPE_VOID,
      TYPE_BOOLEAN,
      TYPE_INTEGER,
      TYPE_FLOAT,
      TYPE_STRING,
      TYPE_ARRAY,
      TYPE_OBJECT
    };
    
    /** Retuns the type. */
    virtual Type getType() const noexcept = 0;

    /** Retuns string representation. */
    // virtual base::String toString() const noexcept = 0;
  };

  /** Void. */
  class _COM_AZURE_DEV__BASE__API Void : public Value {
  public:

    inline Void() {
    }

    /** Retuns the type. */
    virtual inline Type getType() const noexcept override {
      return TYPE_VOID;
    }
  };

  /** Boolean. */
  class _COM_AZURE_DEV__BASE__API Boolean : public Value {
  public:
    
    bool value = false;

    inline Boolean(bool _value) : value(_value) {
    }

    /** Retuns the type. */
    virtual inline Type getType() const noexcept override {
      return TYPE_BOOLEAN;
    }
  };

  /** Integer. */
  class _COM_AZURE_DEV__BASE__API Integer : public Value {
  public:
    
    int value = 0;

    inline Integer(int _value) : value(_value) {
    }

    /** Retuns the type. */
    virtual inline Type getType() const noexcept override {
      return TYPE_INTEGER;
    }
  };

  /** Float. */
  class _COM_AZURE_DEV__BASE__API Float : public Value {
  public:
    
    double value = 0;

    inline Float(double _value) : value(_value) {
    }
    
    /** Retuns the type. */
    virtual inline Type getType() const noexcept override {
      return TYPE_FLOAT;
    }
  };

  /** String. */
  class _COM_AZURE_DEV__BASE__API String : public Value {
  public:
    
    base::String value;

    inline String() {
    }

    inline String(const char* _value) : value(_value) {
    }

    inline String(const base::String& _value) : value(_value) {
    }

    /** Returns the size of the string. */
    MemorySize getSize() const noexcept;

    /** Returns true if string is empty. */
    inline bool isEmpty() const noexcept {
      return getSize() == 0;
    }
    
    /** Retuns the type. */
    virtual inline Type getType() const noexcept override {
      return TYPE_STRING;
    }
  };

  /** Array. */
  class _COM_AZURE_DEV__BASE__API Array : public Value {
  public:
    
    base::Array<Reference<Value> > values;

    class Element {
      friend class Array;
    private:

      Array& array;
      MemorySize index = 0;

      Element(const Element& copy) throw();
      Element& operator=(const Element& eq) throw();
    public:

      inline Element(Array& _array, MemorySize _index) : array(_array), index(_index)
      {
      }

      inline Element& operator=(const Reference<Value>& value) {
        array.setAt(index, value);
        return *this;
      }

#if 1 // TAG: allow reuse of new values
      inline Element& operator=(void*) {
        array.setAt(index, new Void());
        return *this;
      }

      inline Element& operator=(const bool value) {
        array.setAt(index, new Boolean(value));
        return *this;
      }

      inline Element& operator=(const int value) {
        array.setAt(index, new Integer(value));
        return *this;
      }

      inline Element& operator=(const float value) {
        array.setAt(index, new Float(value));
        return *this;
      }

      inline Element& operator=(const String& value) {
        array.setAt(index, new String(value));
        return *this;
      }
#endif

      inline operator Reference<Value>() const {
        return array.getAt(index);
      }
    };

    /** Retuns the type. */
    virtual inline Type getType() const noexcept override {
      return TYPE_ARRAY;
    }

    /** Returns the size of the array. */
    MemorySize getSize() const noexcept;

    /** Returns the size of the array. */
    inline bool isEmpty() const noexcept {
      return getSize() == 0;
    }

    /** Returns the value at the given index. */
    Reference<Value> getAt(MemorySize index) const throw(ObjectModelException);

    /** Sets the value at the given index. */
    void setAt(MemorySize index, const Reference<Value>& value);

    /** Sets the value at the given index. */
    void append(const Reference<Value>& value);

    /** Returns the value at the given index. */
    inline Element operator[](MemorySize index)
    {
      return Element(*this, index);
    }

    Array& operator=(const std::vector<bool>& values);
    Array& operator=(const std::vector<int>& values);
    Array& operator=(const std::vector<double>& values);
    Array& operator=(const std::vector<base::String>& values);

    template<class TYPE>
    std::vector<TYPE> getAs() const;      
  };

  /** Object. */
  class _COM_AZURE_DEV__BASE__API Object : public Value {
  public:
    
    base::Array<std::pair<Reference<String>, Reference<Value> > > values;

    /** Retuns the type. */
    virtual inline Type getType() const noexcept override {
      return TYPE_OBJECT;
    }

    /** Retuns the number of keys in the object. */
    MemorySize getSize() const noexcept;

    /** Returns the size of the array. */
    inline bool isEmpty() const noexcept {
      return getSize() == 0;
    }

    /** Returns true if the key exists. */
    bool hasKey(const Reference<String>& key) const noexcept;

    /** Returns the value for the given key. */
    Reference<Value> getValue(const Reference<String>& key) const throw(ObjectModelException);
    
    /** Sets the value for the given key. */
    void setValue(const Reference<String>& key, const Reference<Value>& value);

    class Element {
      friend class Object;
    private:

      Object& object;
      Reference<String> key;

      Element(const Element& copy) throw();
      Element& operator=(const Element& eq) throw();
    public:

      inline Element(Object& _object, const Reference<String>& _key) : object(_object), key(_key)
      {
      }

      inline Element& operator=(const Reference<Value>& value)
      {
        object.setValue(key, value);
        return *this;
      }

      inline Element& operator=(const Reference<Boolean>& value)
      {
        return operator=(Reference<Value>(value));
      }

      inline Element& operator=(const Reference<Integer>& value)
      {
        return operator=(Reference<Value>(value));
      }

      inline Element& operator=(const Reference<Float>& value)
      {
        return operator=(Reference<Value>(value));
      }

      inline Element& operator=(const Reference<String>& value)
      {
        return operator=(Reference<Value>(value));
      }

      inline Element& operator=(const Reference<Array>& value)
      {
        return operator=(Reference<Value>(value));
      }

      inline Element& operator=(const Reference<Object>& value)
      {
        return operator=(Reference<Value>(value));
      }

      inline Element& operator=(const bool value)
      {
        object.setValue(key, new Boolean(value)); // TAG: reuse
        return *this;
      }

      inline Element& operator=(const int value)
      {
        object.setValue(key, new Integer(value)); // TAG: reuse
        return *this;
      }

      inline Element& operator=(const double value)
      {
        object.setValue(key, new Float(value)); // TAG: reuse
        return *this;
      }

      inline Element& operator=(const char* value)
      {
        object.setValue(key, new String(value)); // TAG: reuse
        return *this;
      }

      inline operator Reference<Value>() const
      {
        return object.getValue(key);
      }
    };

#if 0
    class ReadElement {
    private:

      const Object& object;
      MemorySize index = 0;
    public:

      inline ReadElement(const Object& _object, MemorySize _index) : object(_object), index(_index)
      {
      }

      inline operator Reference<Value>() const {
        return object.getValue(i, v);
      }
    };
#endif

    inline Element operator[](const Reference<String>& key)
    {
      return Element(*this, key);
    }

    inline Element operator[](const base::String& key)
    {
      return operator[](new String(key)); // TAG: reuse
    }

    inline Element operator[](const Literal& key)
    {
      return operator[](new String(key)); // TAG: reuse
    }

    inline Element operator[](const char* key)
    {
      return operator[](new String(key)); // TAG: reuse
    }

#if 0
    inline ReadElement operator[](MemorySize index) const
    {
      return ReadElement(*this, index);
    }
#endif

    /**
      Returns the value at the given path.

      @param path The path of the desired value.
      @param forceNull Avoid exception if value doesn't exist. Returns nullptr otherwise.
    */
    Reference<Value> getPath(const char* path, bool forceNull = false) throw(ObjectModelException);

    /** Returns the boolean for the given path. Returns the default value if doesn't exist. Raises exception if value isn't a boolean. */
    bool getBoolean(const char* path, bool defaultValue) throw(ObjectModelException);

    /** Returns the integer for the given path. Returns the default value if doesn't exist. Raises exception if value isn't an integer. */
    int getInteger(const char* path, int defaultValue) throw(ObjectModelException);

    /** Returns the float for the given path. Returns the default value if doesn't exist. Raises exception if value isn't a float. */
    double getFloat(const char* path, double defaultValue) throw(ObjectModelException);

    /** Returns the string for the given path. Returns the default value if doesn't exist. Raises exception if value isn't a string. */
    base::String getString(const char* path, const base::String& defaultValue) throw(ObjectModelException);

    /** Returns the array for the given path. Returns the default value if doesn't exist. Raises exception if value isn't an array. */
    Reference<Array> getArray(const char* path) throw(ObjectModelException);

    /** Returns the object for the given path. Returns the default value if doesn't exist. Raises exception if value isn't an object. */
    Reference<Object> getObject(const char* path) throw(ObjectModelException);
  };
private:
  
  // Avoid allocation of common values.
  Reference<Void> commonVoid;
  Reference<Boolean> commonBooleanFalse;
  Reference<Boolean> commonBooleanTrue;
  Reference<Integer> commonInteger0;
  Reference<Integer> commonInteger1;
  Reference<Integer> commonIntegerMinus1;
  // std::vector<Reference<Float> > commonIntegers;
  // std::vector<Reference<Float> > commonFloats;
  Reference<Float> commonFloat0;
  Reference<Float> commonFloat1;
  Reference<Float> commonFloatMinus1;
  Reference<String> commonStringEmpty;

  /** Used to avoid reallocated of the same strings. */
  std::map<base::String, Reference<String> > lookup;
  // TAG: add support for reusing strings
public:
  
  /**
    Initializes the exception object with no message.
  */
  ObjectModel();

  /** Creates a void. May be reused. */
  Reference<Void> createVoid();

  /** Creates a new boolean. May be reused. */
  Reference<Boolean> createBoolean(bool value = false);
  
  /** Creates an integer. May be reused. */
  Reference<Integer> createInteger(int value = 0);
  
  /** Creates a float. May be reused. */
  Reference<Float> createFloat(double value = 0);

  /** Creates an empty string. */
  inline const Reference<String>& createEmptyString()
  {
    return commonStringEmpty;
  }

  /** Creates a string. Similar strings may be reused. */
  Reference<String> createString(const char* value);

  /** Creates a string. Similar strings may be reused. */
  Reference<String> createString(const base::String& value);

  /** Creates a string without any reuse. */
  Reference<String> createStringUnique(const char* value);

  /** Creates an array. */
  Reference<Array> createArray();
  
  /** Creates an object. */
  Reference<Object> createObject();
};

/** Returns true if the given values are identical. */
inline bool operator==(const Reference<ObjectModel::Value>& a, bool b) {
  Reference<ObjectModel::Boolean> _a = a.cast<ObjectModel::Boolean>();
  return _a && (_a->value == b);
}

/** Returns true if the given values are identical. */
inline bool operator==(const Reference<ObjectModel::Value>& a, int b) {
  Reference<ObjectModel::Integer> _a = a.cast<ObjectModel::Integer>();
  return _a && (_a->value == b);
}

/** Returns true if the given values are identical. */
inline bool operator==(const Reference<ObjectModel::Value>& a, float b) {
  Reference<ObjectModel::Float> _a = a.cast<ObjectModel::Float>();
  return _a && (_a->value == b);
}

/** Returns true if the given values are identical. */
inline bool operator==(const Reference<ObjectModel::Value>& a, const String& b) {
  Reference<ObjectModel::String> _a = a.cast<ObjectModel::String>();
  return _a && (_a->value == b);
}

inline Reference<ObjectModel::Value> set(const Reference<ObjectModel::Value>& v, bool b) throw(ObjectModelException)
{
  auto _v = v.cast<ObjectModel::Boolean>();
  if (!_v) {
    throw ObjectModelException("Value is not a boolean.");
  }
  _v->value = b;
  return v;
}

template<>
_COM_AZURE_DEV__BASE__API std::vector<bool> ObjectModel::Array::getAs<bool>() const;

template<>
_COM_AZURE_DEV__BASE__API std::vector<int> ObjectModel::Array::getAs<int>() const;

template<>
_COM_AZURE_DEV__BASE__API std::vector<double> ObjectModel::Array::getAs<double>() const;

template<>
_COM_AZURE_DEV__BASE__API std::vector<base::String> ObjectModel::Array::getAs<base::String>() const;

_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Value>& value);
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Void>& value);
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Boolean>& value);
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Integer>& value);
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Float>& value);
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::String>& value);
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Array>& value);
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Object>& value);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE