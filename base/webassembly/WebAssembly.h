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
#include <base/io/InputStream.h>
#include <base/io/OutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  WebAssembly integration.
*/

class _COM_AZURE_DEV__BASE__API WebAssembly : public Object {
public:

  class Handle;
private:

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
    TYPE_STRING // TAG: remove
  };

  /** Returns type as string. */
  static const char* toString(Type type) noexcept;

  /** Function type. */
  class _COM_AZURE_DEV__BASE__API FunctionType {
  public:

    Array<Type> arguments;
    Array<Type> results;
  };

  /** Function. */
  class _COM_AZURE_DEV__BASE__API Function {
  private:
    
    unsigned int functionReference = 0;
  public:
    
    Function();
    
    // FunctionType getType();
    
#if 0
    /** Calls function. */
    template<typename RESULT, typename... ARGS>
    RESULT call(ARGS... args);
#endif
    
    ~Function();
  };

  enum ExternType {
    EXTERN_FUNCTION,
    EXTERN_GLOBAL,
    EXTERN_TABLE,
    EXTERN_MEMORY
  };
  
  /** Description for symbol. */
  class _COM_AZURE_DEV__BASE__API Symbol {
  public:

    unsigned int index = 0;
    String name;
    String moduleName;
    ExternType externType = EXTERN_FUNCTION;

    // function type
    Function f;
    void* func = nullptr;
    FunctionType functionType;
    
    // memory type
    MemorySize memorySize = 0;
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

  /** Enables logging. */
  // void setUseLog(bool uselog);

  /** Garbage collects any unused memory. */
  void garbageCollect();

  template<class TYPE>
  class MapType {
  public:
  };

  /** Returns the number of arguments. */
  template<typename RESULT, typename... TYPES>
  static constexpr size_t getNumberOfArguments(RESULT(*f)(TYPES ...))
  {
    return sizeof...(TYPES);
  }

  /** Registers global function. */
  void registerFunctionImpl(void* func, Type result, const Type* args, unsigned int argsSize, const String& name, bool nothrow);

  /** Registers global function. */
  template<typename RESULT, typename... ARGS>
  void registerFunction(RESULT (*func)(ARGS...), const String& name = String())
  {
    const Type resultType = MapType<RESULT>::type;
    const Type types[] = { MapType<ARGS>::type..., TYPE_UNSPECIFIED };
    registerFunctionImpl((void*)func, resultType, types, getArraySize(types) - 1, name, noexcept(func));
  }

  /** Loads the given WASM module. */
  bool loadFile(const String& path);

  /** Loads the given WASM module in buffer. */
  bool load(const uint8* wasm, MemorySize size);
  
  /** Loads the given WASM module in buffer. */
  inline bool load(const String& path)
  {
    return load(path.getBytes(), path.getLength());
  }

  /** Returns true if the given WASM module is valid. */
  bool isValid(const uint8* wasm, MemorySize size);

  /** Returns true if the given WASM module is valid. */
  inline bool isValid(const String& bytes)
  {
    return isValid(bytes.getBytes(), bytes.getLength());
  }

#if 0
  // TAG: add override of arguments and environment
  void setArguments(Array<String>& arguments);
  void setEnvironment(Map<String, String>& environment);
#endif
  
  /**
    Makes instance for loaded module.
   
    @param fake If, true dummy imports will be registered automatically.
  */
  bool makeInstance(bool fake = false);

  // TAG: there needs to be resource config - e.g. threads limits
  /**
    Makes a WASI instance.
  */
  bool makeWASIInstance(InputStream* _stdin, OutputStream* _stdout, OutputStream* _stderr);
  
  /** Returns the number of imported symbols. */
  MemorySize getNumberOfImports() const;

  /** Returns the number of exported symbols. */
  MemorySize getNumberOfExports() const;

  /** Returns the imported symbols for the module. */
  Array<Symbol> getImports();

  /** Returns the exported symbols. */
  Array<Symbol> getExports();

  /** Returns information about the given index. */
  Symbol getSymbol(MemorySize index);

  /** Returns information about the given name. */
  Symbol getSymbol(const String& name);

  /** Calls the entry function without arguments. */
  void callEntry();

  /** Calls the function with the given id and arguments. */
  AnyValue call(const String& id, const AnyValue* arguments, MemorySize size);

  /** Calls the function with the given id and arguments. */
  inline AnyValue call(const String& id)
  {
    return call(id, nullptr, 0);
  }

  /** Calls the function with the given id and arguments. */
  AnyValue call(const String& id, const Array<AnyValue>& arguments);

  /** Calls the exported function with the given id and arguments. */
  AnyValue call(unsigned int id, const Array<AnyValue>& arguments);

#if 0
  /** WASM value. */
  class WASMValue {
  private:
    
    Type type = TYPE_i32;
    union {
      int32 i32;
      int64 i64;
      float f;
      double d;
      unsigned int function;
    };
    String s;
  public:

    WASMValue();
    WASMValue(int32);
    WASMValue(int64);
    WASMValue(float);
    WASMValue(double);
    WASMValue(const char* string);
    WASMValue(const wchar* string);
    WASMValue(const String& string);
    WASMValue(const WideString& string);
  };
#endif
  
  typedef AnyValue WASMValue;
  
  // TAG: we could also add call on Function object if it has link to instance
  
  /** Calls function. */
  template<typename RESULT, typename... ARGS>
  RESULT callFunction(const String& id, ARGS... args)
  {
    WASMValue _args[] = { WASMValue(args)..., WASMValue() };
    return call(id, _args, getArraySize(_args) - 1);
  }

  /** Returns function type as string. */
  static String toString(
    const FunctionType& functionType,
    const String& name = String(),
    const String& module = String(), bool colorize = false);

  /** Returns symbol as string. */
  static String toString(const Symbol& s, bool colorize = false);

  /** Returns function type. */
  FunctionType getFunctionType(unsigned int id);

  /** Returns function reference. */
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

template<class TYPE>
class WebAssembly::MapType<const TYPE*> {
public:

  static constexpr Type type = TYPE_i32; // TAG: depends on WASM32/WASM64
  static constexpr MemorySize SIZE = sizeof(TYPE);
};

template<class TYPE>
class WebAssembly::MapType<TYPE*> {
public:

  static constexpr Type type = TYPE_i32;
  static constexpr MemorySize SIZE = sizeof(TYPE);
};

_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const WebAssembly::Symbol& value);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
