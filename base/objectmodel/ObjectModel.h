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

class _COM_AZURE_DEV__BASE__API ObjectModel : public ReferenceCountedObject {
public:

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
    
    virtual Type getType() const noexcept = 0;
  };

  class _COM_AZURE_DEV__BASE__API Void : public Value {
  public:

    inline Void() {
    }

    virtual inline Type getType() const noexcept override {
      return TYPE_VOID;
    }
  };

  class _COM_AZURE_DEV__BASE__API Boolean : public Value {
  public:
    
    bool value = false;

    inline Boolean(bool _value) : value(_value) {
    }

    virtual inline Type getType() const noexcept override {
      return TYPE_BOOLEAN;
    }
  };

  class _COM_AZURE_DEV__BASE__API Integer : public Value {
  public:
    
    int value = 0;

    inline Integer(int _value) : value(_value) {
    }

    virtual inline Type getType() const noexcept override {
      return TYPE_INTEGER;
    }
  };

  class _COM_AZURE_DEV__BASE__API Float : public Value {
  public:
    
    double value = 0;

    inline Float(double _value) : value(_value) {
    }
    
    virtual inline Type getType() const noexcept override {
      return TYPE_FLOAT;
    }
  };

  class _COM_AZURE_DEV__BASE__API String : public Value {
  public:
    
    base::String value;

    inline String(const char* _value) : value(_value) {
    }
    
    virtual inline Type getType() const noexcept override {
      return TYPE_STRING;
    }
  };

  class _COM_AZURE_DEV__BASE__API Array : public Value {
  public:
    
    base::Array<Reference<Value> > values;

    virtual inline Type getType() const noexcept override {
      return TYPE_ARRAY;
    }

    MemorySize getSize() const noexcept;
    
    Reference<Value> getAt(MemorySize index) const throw(ObjectModelException);

    void setAt(MemorySize index, const Reference<Value>& value);
  };

  class _COM_AZURE_DEV__BASE__API Object : public Value {
  public:
    
    base::Array<std::pair<String, Reference<Value> > > values;

    virtual inline Type getType() const noexcept override {
      return TYPE_OBJECT;
    }

    MemorySize getSize() const noexcept;
    
    bool hasKey(const String& key) const noexcept;

    Value hasValue(const String& key) const throw(ObjectModelException);
    
    void setValue(const String& name, Reference<Value> value);

    /** Returns the value at the given path. */
    Reference<Value> getPath(const char* path) throw(ObjectModelException);
  };
private:
  
  Reference<Void> voidx;
  Reference<Boolean> booleanFalse;
  Reference<Boolean> booleanTrue;
  Reference<Integer> integer0;
  Reference<Float> float0;
  Reference<String> emptyString;
public:
  
  /**
    Initializes the exception object with no message.
  */
  ObjectModel();

  Reference<Void> createVoid();

  Reference<Boolean> createBoolean(bool value = false);
  
  Reference<Integer> createInteger(int value = 0);
  
  Reference<Float> createFloat(double value = 0);
  
  Reference<String> createString(const char* value = "");
  
  Reference<Array> createArray();
  
  Reference<Object> createObject();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
