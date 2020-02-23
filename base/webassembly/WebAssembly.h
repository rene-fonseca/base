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
#include <base/collection/Array.h>
#include <base/AnyValue.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  WebAssembly integration.
*/

class _COM_AZURE_DEV__BASE__API WebAssembly : public Object {
private:

  class Handle;

  AnyReference handle;
public:

  /**
    Returns true if WASM is supported by the runtime.
  */
  static bool isSupported() noexcept;

  /** WASM module. */
  class _COM_AZURE_DEV__BASE__API Module {
  private:
    
    AnyReference handle;
  public:
    
    /** Initializes module. */
    Module();

    /** Destroys module. */
    ~Module();
  };

#if 0
  /** File for virtual file system. */
  class _COM_AZURE_DEV__BASE__API VirtualFile {
  public:

    virtual unsigned int read(uint8* buffer, unsigned int offset, unsigned int size) = 0;

    virtual unsigned int write(const uint8* buffer, unsigned int offset, unsigned int size) = 0;

    virtual void close() = 0;
  };

  /** Virtual file system. */
  class _COM_AZURE_DEV__BASE__API VirtualFileSystem {
  public:
    
    virtual Array<String> getEntries(const String& path) = 0;

    virtual void makeFolder(const String& path) = 0;

    virtual void remove(const String& path) = 0;

    virtual VirtualFile* open(const String& path) = 0;
  };
#endif

  /** Types. */
  enum Type {
    TYPE_UNSPECIFIED,
    TYPE_FUNCTION,
    TYPE_i32,
    TYPE_i64,
    TYPE_f32,
    TYPE_f64,
    TYPE_STRING
  };

  /** Returns type as string. */
  static const char* toString(Type type) noexcept;

  /** Function. */
  class _COM_AZURE_DEV__BASE__API Function {
  public:
    
    Function();
    
    ~Function();
  };

  /** Function type. */
  class _COM_AZURE_DEV__BASE__API FunctionType {
  public:

    Array<Type> arguments;
    Array<Type> results;
  };

  /** Description for symbol. */
  class _COM_AZURE_DEV__BASE__API Symbol {
  public:

    unsigned int index = 0;
    String name;
    Function f;
    
    // type info
    Type type = TYPE_FUNCTION;
    Array<Type> arguments;
    Array<Type> results;
  };

  /** WebAssembly exception. */
  class _COM_AZURE_DEV__BASE__API WebAssemblyException : public Exception {
  public:

    /** WebAssembly exception. */
    inline WebAssemblyException()
    {
    }

    /** WebAssembly exception. */
    inline WebAssemblyException(const char* message)
      : Exception(message)
    {
    }
  };
  
  /** Initializes WebAssembly engine. */
  WebAssembly();

  /** Returns info about the WebAssembly engine. */
  String getEngine();

  /** Returns the processing time. */
  uint64 getProcessingTime() const;

  /** Returns the memory limit. */
  MemorySize getMaximumMemoryUsage() const;

  /** Sets the memory limit. */
  void setMaximumMemoryUsage(MemorySize size);

  /** Returns the current memory usage. */
  MemorySize getMemoryUsage() const;

  /** Garbage collects any unused memory. */
  void garbageCollect();

  template<class TYPE>
  class MapType {
  public:
  };

  /** Returns the number of arguments. */
  template<typename RESULT, typename... TYPES>
  constexpr size_t getNumberOfArguments(RESULT(*f)(TYPES ...))
  {
    return sizeof...(TYPES);
  }

  /** Registers global function. */
  void registerFunctionImpl(void* func, Type result, const Type* args, unsigned int argsSize, const String& name);

  /** Registers global function. */
  template<typename RESULT, typename... ARGS>
  void registerFunction(RESULT (*func)(ARGS...), const String& name = String())
  {
    const Type resultType = MapType<RESULT>::type;
    const Type types[] = { MapType<ARGS>::type..., TYPE_UNSPECIFIED };
    registerFunctionImpl(func, resultType, types, getArraySize(types) - 1, name);
  }

  /** Loads the given WASM module. */
  bool load(const String& path);

  /** Returns true if the given WASM module is valid. */
  bool isValid(const uint8* wasm, MemorySize size);

  /** Loads the given WASM module in buffer. */
  bool load(const uint8* wasm, MemorySize size);

  /** Returns the exported symbols. */
  Array<Symbol> getExports();

  /** Returns information about the given id. */
  Symbol getSymbol(const String& id);

  /** Calls the entry function without arguments. */
  void callEntry();

  /** Calls the function with the given id and arguments. */
  AnyValue call(const String& id, const Array<AnyValue>& arguments);

  /** Calls the exported function with the given id and arguments. */
  AnyValue call(unsigned int id, const Array<AnyValue>& arguments);

#if 0
  /** Calls function. */
  template<typename RESULT, typename... ARGS>
  RESULT callFunction(const String& id, ARGS... args)
  {
    // TAG: avoid AnyValue intermediate so we can validate if arguments match
    Array<AnyValue> args = { AnyValue<ARGS>(args)... };
    AnyValue r = call(id, args);
    return RESULT(); // TAG: cast to result
  }
#endif

  // TAG: FunctionType getFunctionType(unsigned int id);

  Function getFunction(unsigned int id);
  
  /** Calls the exported function with the given id and arguments. */
  AnyValue call(Function func, const Array<AnyValue>& arguments);

  ~WebAssembly();
};

template<>
class WebAssembly::MapType<void> {
public:

  static constexpr Type type = TYPE_UNSPECIFIED;
};

template<>
class WebAssembly::MapType<bool> {
public:

  static constexpr Type type = TYPE_i32;
};

template<>
class WebAssembly::MapType<short> {
public:

  static constexpr Type type = TYPE_i32;
};

template<>
class WebAssembly::MapType<unsigned short> {
public:

  static constexpr Type type = TYPE_i32;
};

template<>
class WebAssembly::MapType<int> {
public:

  static constexpr Type type = TYPE_i32;
};

template<>
class WebAssembly::MapType<unsigned int> {
public:

  static constexpr Type type = TYPE_i32;
};

#if 0
template<>
class WebAssembly::MapType<long> {
public:

  static constexpr Type type = TYPE_i32;
};

template<>
class WebAssembly::MapType<unsigned long> {
public:

  static constexpr Type type = TYPE_i32;
};
#endif

template<>
class WebAssembly::MapType<int64> {
public:

  static constexpr Type type = TYPE_i64;
};

template<>
class WebAssembly::MapType<uint64> {
public:

  static constexpr Type type = TYPE_i64;
};

template<>
class WebAssembly::MapType<float> {
public:

  static constexpr Type type = TYPE_f32;
};

template<>
class WebAssembly::MapType<double> {
public:

  static constexpr Type type = TYPE_f64;
};

template<>
class WebAssembly::MapType<long double> {
public:

  static constexpr Type type = TYPE_f64;
};

template<>
class WebAssembly::MapType<const char*> {
public:

  static constexpr Type type = TYPE_STRING;
};

template<>
class WebAssembly::MapType<const wchar*> {
public:

  static constexpr Type type = TYPE_STRING;
};

template<>
class WebAssembly::MapType<String> {
public:

  static constexpr Type type = TYPE_STRING;
};

template<>
class WebAssembly::MapType<WideString> {
public:

  static constexpr Type type = TYPE_STRING;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
