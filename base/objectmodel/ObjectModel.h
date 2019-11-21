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
#include <base/string/StringOutputStream.h>
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
  inline ObjectModelException() noexcept {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline ObjectModelException(const char* message) noexcept : Exception(message) {
  }
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

/** Generic object model. */
class _COM_AZURE_DEV__BASE__API ObjectModel : public ReferenceCountedObject {
public:

  enum FormattingFlags {
    FLAG_COMPACT = 1 << 0,
    FLAG_INDENT = 1 << 1, // split into lines and indent
    FLAG_COLOR = 1 << 2, // use ANSI codes
    FLAG_COLOR_RGB = 1 << 4, // use RGB escape codes - FLAG_COLOR must still be set
    FLAG_USE_TAB = 1 << 5, // use TAB for indentation
    FLAG_USE_NULL_FOR_SPECIAL = 1 << 6, // use null instead of string with ids
    FLAG_INCLUDE_COMMENTS = 1 << 7, // include comment with // style
    DEFAULT_FORMATTING = FLAG_INDENT
  };

  /** Use this like StringOutputStream to get nice formatted output. */
  class _COM_AZURE_DEV__BASE__API NiceFormat : public StringOutputStream {
  private:

    unsigned int level = 0;
    unsigned int maximumLineLength = 80;
    unsigned int flags = 0; // see FormattingFlags
    unsigned int indent = 2; // for nice formatting
  public:

    inline NiceFormat() : StringOutputStream() {
    }

    inline unsigned int getNiceFlags() const noexcept {
      return flags;
    }

    inline void setNiceFlags(unsigned int flags) noexcept {
      this->flags = flags;
    }

    inline unsigned int getIndent() const noexcept {
      return indent;
    }

    inline void setIndent(unsigned int indent) noexcept {
      this->indent = indent;
    }

    inline unsigned int getMaximumLineLength() const noexcept {
      return maximumLineLength;
    }

    inline void setMaximumLineLength(unsigned int maximumLineLength) noexcept {
      this->maximumLineLength = maximumLineLength;
    }

    inline unsigned int enter() noexcept {
      return ++level;
    }

    inline void exit() noexcept {
      --level;
    }

    inline NiceFormat& writeIndent()
    {
      if (flags & FLAG_USE_TAB) {
        *this << tabindent(level * indent);
      } else {
        *this << base::indent(level * indent);
      }
      return *this;
    }

    NiceFormat& writeKeyword(const Literal& literal);

    NiceFormat& writeTextUnquoted(const Literal& literal);
  };

  /** Value. */
  class _COM_AZURE_DEV__BASE__API Value : public ReferenceCountedObject {
  public:
    
    enum Type {
      TYPE_VOID,
      TYPE_BOOLEAN,
      TYPE_INTEGER,
      TYPE_FLOAT,
      TYPE_STRING,
      TYPE_BINARY,
      TYPE_ARRAY,
      TYPE_OBJECT,
      TYPE_COMMENT
    };

    /** Returns the type. */
    virtual Type getType() const noexcept = 0;

    /** Returns string representation. */
    base::String toString(unsigned int flags = DEFAULT_FORMATTING) const noexcept;

    /** Returns string representation. */
    inline base::String toStringNoFormatting() const noexcept {
      return toString(0);
    }
  };

  /** Void. */
  class _COM_AZURE_DEV__BASE__API Void : public Value {
  public:

    inline Void() {
    }

    /** Returns the type. */
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

    /** Returns the type. */
    virtual inline Type getType() const noexcept override {
      return TYPE_BOOLEAN;
    }

    inline operator bool() const noexcept {
      return value;
    }
  };

  /** Integer. */
  class _COM_AZURE_DEV__BASE__API Integer : public Value {
  public:
    
    int64 value = 0;

    inline Integer(int _value) : value(_value) {
    }

    inline Integer(int64 _value) : value(_value) {
    }

    /** Returns the type. */
    virtual inline Type getType() const noexcept override {
      return TYPE_INTEGER;
    }

    inline bool operator==(int compare) const noexcept {
      return value == compare;
    }

    inline bool operator!=(int compare) const noexcept {
      return value != compare;
    }

    inline bool operator==(int64 compare) const noexcept {
      return value == compare;
    }

    inline bool operator!=(int64 compare) const noexcept {
      return value != compare;
    }

    inline operator int() const noexcept {
      return value;
    }

    inline operator int64() const noexcept {
      return value;
    }
  };

  /** Float. */
  class _COM_AZURE_DEV__BASE__API Float : public Value {
  public:
    
    double value = 0;

    inline Float(double _value) : value(_value) {
    }
    
    /** Returns the type. */
    virtual inline Type getType() const noexcept override {
      return TYPE_FLOAT;
    }

    inline operator double() const noexcept {
      return value;
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
    
    /** Returns the type. */
    virtual inline Type getType() const noexcept override {
      return TYPE_STRING;
    }

    inline operator const base::String&() const noexcept {
      return value;
    }
  };

  /** Comment. */
  class _COM_AZURE_DEV__BASE__API Comment : public Value {
  public:
    
    base::String value;

    inline Comment() {
    }

    inline Comment(const char* _value) : value(_value) {
    }

    inline Comment(const base::String& _value) : value(_value) {
    }

    /** Returns the size of the string. */
    MemorySize getSize() const noexcept;

    /** Returns true if string is empty. */
    inline bool isEmpty() const noexcept {
      return getSize() == 0;
    }
    
    /** Returns the type. */
    virtual inline Type getType() const noexcept override {
      return TYPE_STRING;
    }

    inline operator const base::String&() const noexcept {
      return value;
    }
  };

  /** Binary blob. */
  class _COM_AZURE_DEV__BASE__API Binary : public Value {
  public:
    
    std::vector<uint8> value;

    inline Binary() {
    }

    inline Binary(const std::vector<uint8>& _value) : value(_value) {
    }

    /** Returns the size. */
    MemorySize getSize() const noexcept {
      return value.size();
    }

    /** Returns true if string is empty. */
    inline bool isEmpty() const noexcept {
      return getSize() == 0;
    }
    
    /** Returns the type. */
    virtual inline Type getType() const noexcept override {
      return TYPE_BINARY;
    }

    inline operator const std::vector<uint8>&() const noexcept {
      return value;
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

      Element(const Element& copy) noexcept;
      Element& operator=(const Element& assign) noexcept;
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

    /** Returns the type. */
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
    Array& operator=(const std::vector<int64>& values);
    Array& operator=(const std::vector<double>& values);
    Array& operator=(const std::vector<base::String>& values);

    template<class TYPE>
    std::vector<TYPE> getAs() const;      
  };

  /** Object. */
  class _COM_AZURE_DEV__BASE__API Object : public Value {
  public:
    
    typedef std::pair<Reference<String>, Reference<Value> > Association;
    base::Array<std::pair<Reference<String>, Reference<Value> > > values;

    /** Returns the type. */
    virtual inline Type getType() const noexcept override {
      return TYPE_OBJECT;
    }

    /** Returns the number of keys in the object. */
    MemorySize getSize() const noexcept;

    /** Returns the size of the array. */
    inline bool isEmpty() const noexcept
    {
      return getSize() == 0;
    }

    /** Returns true if the key exists. */
    bool hasKey(const Reference<String>& key) const noexcept;
    bool hasKey(const base::String& key) const noexcept;
    bool hasKey(const char* key) const noexcept;

    /** Removes key if it exists. */
    bool removeKey(const Reference<String>& key) noexcept;
    bool removeKey(const base::String& key) noexcept;
    bool removeKey(const char* key) noexcept;

    /** Returns the value for the given key. Returns nullptr is key doesn't exist. */
    Reference<Value> getValue(const Reference<String>& key) const noexcept;
    Reference<Value> getValue(const base::String& key) const noexcept;
    Reference<Value> getValue(const char* key) const noexcept;

    /** Sets the value for the given key. key must not be nullptr. */
    void setValue(const Reference<String>& key, const Reference<Value>& value);
    void setValue(const base::String& key, const Reference<Value>& value);
    void setValue(const base::String& key, std::nullptr_t);
    void setValue(const base::String& key, const bool value);
    void setValue(const base::String& key, const int64 value);
    void setValue(const base::String& key, const double value);
    void setValue(const base::String& key, const base::String& value);
    void setValue(const char* key, const char* value);

    class Element {
      friend class Object;
    private:

      Object& object;
      Reference<String> key;

      Element(const Element& copy) noexcept;
      Element& operator=(const Element& assign) noexcept;
    public:

      inline Element(Object& _object, const Reference<String>& _key) : object(_object), key(_key)
      {
      }

      inline Element& operator=(const Reference<Value>& value)
      {
        object.setValue(key, value);
        return *this;
      }

      inline Element& operator=(const Reference<Void>& value)
      {
        return operator=(Reference<Value>(value));
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
  Reference<Float> commonFloat0;
  Reference<Float> commonFloat1;
  Reference<Float> commonFloatMinus1;
  Reference<String> commonStringEmpty;
  // Reference<Array> commonArrayEmpty;
  // Reference<Object> commonObjectEmpty;

  bool allowReuse = false; // only applies to dynamic strings since we could get race conditions otherwise
  /** Used to avoid reallocated of the same strings. */
  std::map<base::String, Reference<String> > lookup;
public:
  
  /**
    Initializes the exception object with no message.
  */
  ObjectModel(bool _allowReuse = true);

  /** Creates a void. May be reused. */
  Reference<Void> createVoid();

  /** Creates a new boolean. May be reused. */
  Reference<Boolean> createBoolean(bool value = false);
  
  /** Creates an integer. May be reused. */
  Reference<Integer> createInteger(int value = 0);

  /** Creates an integer. May be reused. */
  Reference<Integer> createInteger64(int64 value = 0);

  /** Creates a float. May be reused. */
  Reference<Float> createFloat(double value = 0);

  /** Creates an empty string. */
  inline const Reference<String>& createEmptyString()
  {
    return commonStringEmpty;
  }

  /** Creates a comment. Similar strings may be reused. */
  Reference<Comment> createComment(const base::String& value);

  /** Creates a string. Similar strings may be reused. */
  Reference<String> createString(const char* value);

  /** Creates a string. Similar strings may be reused. */
  Reference<String> createString(const base::String& value);

  /** Creates a string without any reuse. */
  Reference<String> createStringUnique(const char* value);

  /** Creates an array. */
  Reference<Array> createArray();

  /** Creates an array. */
  Reference<Array> createArray(std::initializer_list<bool> l);

  /** Creates an array. */
  Reference<Array> createArray(std::initializer_list<int> l);

  /** Creates an array. */
  Reference<Array> createArray(std::initializer_list<double> l);

  /** Creates an array. */
  Reference<Array> createArray(std::initializer_list<const char*> l);

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
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Binary>& value);
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Reference<ObjectModel::Comment>& value);

_COM_AZURE_DEV__BASE__API ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Value>& value);
_COM_AZURE_DEV__BASE__API ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Void>& value);
_COM_AZURE_DEV__BASE__API ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Boolean>& value);
_COM_AZURE_DEV__BASE__API ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Integer>& value);
_COM_AZURE_DEV__BASE__API ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Float>& value);
_COM_AZURE_DEV__BASE__API ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::String>& value);
_COM_AZURE_DEV__BASE__API ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Array>& value);
_COM_AZURE_DEV__BASE__API ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Object>& value);
_COM_AZURE_DEV__BASE__API ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Binary>& value);
_COM_AZURE_DEV__BASE__API ObjectModel::NiceFormat& operator<<(ObjectModel::NiceFormat& stream, const Reference<ObjectModel::Comment>& value);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
