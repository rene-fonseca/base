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
#include <base/collection/Map.h>
#include <base/AnyValue.h>
#include <base/io/InputStream.h>
#include <base/io/OutputStream.h>
#include <base/mem/Span.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class WebAssemblyFunction;

/**
  WebAssembly integration.
*/

class _COM_AZURE_DEV__BASE__API WebAssembly : public Object {
  friend void setHandle(WebAssembly& wasm, const AnyReference&);
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
    TYPE_f64
  };

  /** Returns type as string. */
  static const char* toString(Type type) noexcept;

  /** Function type. */
  class _COM_AZURE_DEV__BASE__API FunctionType {
  public:

    /** The arguments types. */
    Array<Type> arguments;
    /** The result types. */
    Array<Type> results;
    
    /** Initializes void function() type. */
    FunctionType();
    
    /** Get function type from given function pointer. */
    template<typename RESULT, typename... ARGS>
    FunctionType(RESULT (*)(ARGS... args))
    {
      Type resultType = MapType<RESULT>::type;
      if (resultType != TYPE_UNSPECIFIED) {
        results.append(resultType);
      }
      const Type types[] = { MapType<ARGS>::type..., TYPE_UNSPECIFIED };
      arguments.setSize(sizeof...(ARGS));
      for (MemorySize i = 0; i < sizeof...(ARGS); ++i) {
        arguments.setAt(i, types[i]);
      }
    }

    /** Get function type from given function pointer. */
    template<class TYPE, typename RESULT, typename... ARGS>
    FunctionType(RESULT (TYPE::*)(ARGS... args))
    {
      Type resultType = MapType<RESULT>::type;
      if (resultType != TYPE_UNSPECIFIED) {
        results.append(resultType);
      }
      const Type types[] = { MapType<ARGS>::type..., TYPE_UNSPECIFIED };
      arguments.setSize(sizeof...(ARGS));
      for (MemorySize i = 0; i < sizeof...(ARGS); ++i) {
        arguments.setAt(i, types[i]);
      }
    }

    /** Returns true if equal. */
    bool operator==(const FunctionType& type) const noexcept;
    
    /** Returns true if not equal. */
    inline bool operator!=(const FunctionType& type) const noexcept
    {
      return !operator==(type);
    }
  };

  enum ExternType {
    EXTERN_FUNCTION,
    EXTERN_GLOBAL,
    EXTERN_TABLE,
    EXTERN_MEMORY
  };
  
  /** WASM value. */
  class WASMValue {
  public:
    
    Type type = TYPE_i32;
    union {
      int32 i32;
      int64 i64;
      float f32;
      double f64;
    };

    /** Initialize value. */
    WASMValue();

    /** Initialize value. */
    WASMValue(int32);

    /** Initialize value. */
    WASMValue(int64);

    /** Initialize value. */
    WASMValue(float);

    /** Initialize value. */
    WASMValue(double);

    /** Returns native type. */
    template<typename TYPE>
    TYPE toNative() const;
    
    /*
    template<typename TYPE>
    inline TYPE toNative() const
    {
      _throw WebAssemblyException("Unsupported type.");
    }
    */
  };

  /** Description for symbol. */
  class _COM_AZURE_DEV__BASE__API Symbol {
  public:

    /** Index of extern. */
    unsigned int index = 0;
    /** Name of extern. */
    String name;
    /** Module of import extern. */
    String moduleName;
    /** Extern type. */
    ExternType externType = EXTERN_FUNCTION;

    /** The function address. */
    void* func = nullptr;
    /** The function type. */
    FunctionType functionType;
    
    /** The memory size. */
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

  /** Returns the WASM for the given text (WAT). */
  static String convertWATToWASM(const String& text);

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
  void setUseLog(bool useLog);
    
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

  /** Function context base class. */
  class _COM_AZURE_DEV__BASE__API FunctionContext : public ReferenceCountedObject {
  public:

    ~FunctionContext();
  };

  /** Opaque context. */
  class _COM_AZURE_DEV__BASE__API OpaqueContext : public FunctionContext {
  public:

    void* context = nullptr;
  };

  /** Callback. */
  class WASMCallback : public WebAssembly::FunctionContext {
  public:

    virtual void operator()(
      WebAssembly& source, const WebAssembly::WASMValue* arguments, WebAssembly::WASMValue* results
    ) = 0;
  };

  /** Extern WASM function. */
  typedef void (*WASMFunction)(AnyReference context, WebAssembly& wasm, const WASMValue* arguments, WASMValue* results);
  
  /** Registers global function. */
  void registerFunction(WASMFunction func, AnyReference context,
                        const FunctionType& type, const String& name, const String& module = String());

  /** Registers global function. */
  template<typename RESULT, typename... ARGS>
  void registerFunction(WASMFunction func, AnyReference context,
                        RESULT (*functype)(ARGS...), const String& name, const String& module = String())
  {
    registerFunction(
      func, context,
      getFunctionType(functype), name, module
    );
  }

  /** Registers global callback. */
  void registerCallback(
    R<WASMCallback> callack, const FunctionType& type, const String& name, const String& module = String());

  /** Loads the given WASM module. */
  bool loadFile(const String& path);

  /** Loads the given WASM module in buffer. */
  bool load(const uint8* wasm, MemorySize size);
  
  /** Loads the given WASM module in buffer. */
  inline bool load(const String& path)
  {
    return load(path.getBytes(), path.getLength());
  }
  
  /** Loads the given WAT or WASM module in buffer. */
  bool loadAny(const String& bytes);
  
  enum Format {
    FORMAT_UNSPECIFIED, ///< Unknown format/any format.
    FORMAT_WAT, ///< WAT type.
    FORMAT_WASM ///< WASM type.
  };

  /** Returns the format for the given buffer. Only looks at header. */
  static Format getFormat(const String& bytes);

  /** ABI type. */
  enum WASMABI {
    WASM32, ///< 32-bit WASM.
    WASM64 ///< 64-bit WASM.
  };
  
  /** Returns the WASM ABI. */
  WASMABI getWASMABI() const;

  /** Returns true if the given WASM module is valid. */
  bool isValid(const uint8* wasm, MemorySize size);

  /** Returns true if the given WASM module is valid. */
  inline bool isValid(const String& bytes)
  {
    return isValid(bytes.getBytes(), bytes.getLength());
  }

  /** Sets the command line arguments. */
  void setArguments(const Array<String>& arguments);
  
  /** Sets the initial environment. */
  void setEnvironment(const Map<String, String>& environment);

  /** Sets the initial environment. */
  void setMountedFolders(const Map<String, String>& folders);

  /** Enables profiling. */
  void setUseProfiler(bool useProfiler);

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

  /** Read access memory. */
  ConstSpan<uint8> getMemory() const;
  
  /** Write access memory. */
  Span<uint8> getMemory();

  /** Get memory as string. */
  String getString(MemorySize address, MemorySize size) const;
  
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

  /** Returns the function index for the given name. */
  MemorySize getFunctionIndex(const String& id) const;
  
  /** Calls the entry function without arguments. */
  void callEntry();

  /** Calls the function with the given id and arguments and results. */
  void call(unsigned int id, const WASMValue* arguments, WASMValue* results);

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

  /** Returns function reference. */
  WebAssemblyFunction getFunction(const String& id);
  
  /** Get function type from given function pointer. */
  template<typename RESULT, typename... ARGS>
  static FunctionType getFunctionType(RESULT (*)(ARGS... args))
  {
    FunctionType ft;
    Type resultType = MapType<RESULT>::type;
    if (resultType != TYPE_UNSPECIFIED) {
      ft.results.append(resultType);
    }
    const Type types[] = { MapType<ARGS>::type..., TYPE_UNSPECIFIED };
    ft.arguments.setSize(sizeof...(ARGS));
    for (MemorySize i = 0; i < sizeof...(ARGS); ++i) {
      ft.arguments.setAt(i, types[i]);
    }
    return ft;
  }

  /** Calls function with given arguments. */
  template<typename RESULT, typename... ARGS>
  RESULT invoke(const String& id, ARGS... args)
  {
    AnyValue _args[] = { AnyValue(args)..., AnyValue() };
    return call(id, _args, getArraySize(_args) - 1).template getValue<RESULT>();
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
  WebAssemblyFunction getFunction(unsigned int id);

  class Arguments {
  public:
    
    const WASMValue* src = nullptr;
    
    inline Arguments(const WASMValue* arguments) noexcept
      : src(arguments)
    {
    }
    
    inline const WASMValue* next() noexcept
    {
      return src++;
    }
  };
  
  template<typename... ARGS>
  void forward(void (*function)(ARGS...),
               const WASMValue* _arguments, WASMValue* results)
  {
    // TAG: add support for memory
    Arguments arguments(_arguments);
    (*function)(arguments.next()->toNative<ARGS>()...);
  }
  
  template<typename RESULT, typename... ARGS>
  void forward(RESULT (*function)(ARGS...),
               const WASMValue* _arguments, WASMValue* results)
  {
    // TAG: add support for memory
    Arguments arguments(_arguments);
    RESULT result = (*function)(arguments.next()->toNative<ARGS>()...);
    *results++ = result;
  }

  template<class TYPE, typename... ARGS>
  void forward(void (*function)(ARGS...),
               TYPE* object,
               const WASMValue* _arguments, WASMValue* results)
  {
    // TAG: add support for memory
    Arguments arguments(_arguments);
    (object->*function)(arguments.next()->toNative<ARGS>()...);
  }
  
  template<class TYPE, typename RESULT, typename... ARGS>
  void forward(RESULT (TYPE::*function)(ARGS...),
               TYPE* object,
               const WASMValue* _arguments, WASMValue* results)
  {
    // TAG: add support for memory
    Arguments arguments(_arguments);
    RESULT result = (object->*function)(arguments.next()->toNative<ARGS>()...);
    *results++ = result;
  }

  /** Releases WASM handle. */
  ~WebAssembly();
};

/** Function reference. */
class _COM_AZURE_DEV__BASE__API WebAssemblyFunction {
private:
  
  WebAssembly wa;
  int id = -1;
public:
  
  /** Initializes function. */
  WebAssemblyFunction();

  /** Initializes function. */
  WebAssemblyFunction(WebAssembly wa, const String& id);

  /** Initializes function. */
  WebAssemblyFunction(WebAssembly wa, unsigned int id);

  /** Returns the type of the function. */
  WebAssembly::FunctionType getType();
  
  /** Calls function. */
  inline void call(const WebAssembly::WASMValue* arguments, WebAssembly::WASMValue* results)
  {
    wa.call(id, arguments, results);
  }
  
  /** Calls function. */
  template<typename RESULT, typename... ARGS>
  inline RESULT invoke(ARGS... args)
  {
    return wa.invoke<RESULT>(id, args...);
  }
  
  /** Calls function. */
  template<typename RESULT, typename... ARGS>
  inline RESULT operator()(ARGS... args)
  {
    return wa.invoke<RESULT>(id, args...);
  }
  
  /** Calls function. */
  inline void operator()(const WebAssembly::WASMValue* arguments, WebAssembly::WASMValue* results)
  {
    wa.call(id, arguments, results);
  }
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
class WebAssembly::MapType<unsigned char> {
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

  static constexpr Type type = TYPE_UNSPECIFIED;
};

template<>
class WebAssembly::MapType<const wchar*> {
public:

  static constexpr Type type = TYPE_UNSPECIFIED;
};

template<>
class WebAssembly::MapType<String> {
public:

  static constexpr Type type = TYPE_UNSPECIFIED;
};

template<>
class WebAssembly::MapType<WideString> {
public:

  static constexpr Type type = TYPE_UNSPECIFIED;
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

template<>
inline bool WebAssembly::WASMValue::toNative<bool>() const
{
  if (type != TYPE_i32) {
    _throw WebAssembly::WebAssemblyException("Not compatible with <bool> type.");
  }
  return i32 != 0;
}

template<>
inline int WebAssembly::WASMValue::toNative<int>() const
{
  if (type != TYPE_i32) {
    _throw WebAssembly::WebAssemblyException("Not compatible with <int> type.");
  }
  return static_cast<int>(i32);
}

template<>
inline unsigned int WebAssembly::WASMValue::toNative<unsigned int>() const
{
  if (type != TYPE_i32) {
    _throw WebAssembly::WebAssemblyException("Not compatible with <unsigned int> type.");
  }
  return static_cast<unsigned int>(i32);
}

template<>
inline int64 WebAssembly::WASMValue::toNative<int64>() const
{
  if (type != TYPE_i64) {
    _throw WebAssembly::WebAssemblyException("Not compatible with <int64> type.");
  }
  return static_cast<int64>(i64);
}

template<>
inline uint64 WebAssembly::WASMValue::toNative<uint64>() const
{
  if (type != TYPE_i64) {
    _throw WebAssembly::WebAssemblyException("Not compatible with <uint64> type.");
  }
  return static_cast<uint64>(i64);
}

template<>
inline float WebAssembly::WASMValue::toNative<float>() const
{
  if (type != TYPE_f32) {
    _throw WebAssembly::WebAssemblyException("Not compatible with <float> type.");
  }
  return static_cast<float>(f32);
}

template<>
inline double WebAssembly::WASMValue::toNative<double>() const
{
  if (type != TYPE_f64) {
    _throw WebAssembly::WebAssemblyException("Not compatible with <double> type.");
  }
  return static_cast<double>(f64);
}

template<>
inline long double WebAssembly::WASMValue::toNative<long double>() const
{
  if (type != TYPE_f64) {
    _throw WebAssembly::WebAssemblyException("Not compatible with <long double> type.");
  }
  return static_cast<long double>(f64);
}

_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const WebAssembly::Symbol& value);

_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const WebAssembly::FunctionType& value);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
