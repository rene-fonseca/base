/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/webassembly/WebAssembly.h>
#include <base/io/File.h>
#include <base/UnsignedInteger.h>
#include <base/Module.h>
#include <base/string/Format.h>
#include <base/string/ANSIEscapeSequence.h>
#include <base/Functor.h>
#include <base/mem/MemoryDump.h>
#include <base/Profiler.h>
#include <base/UnitTest.h>
#include <base/build.h>

// #undef _COM_AZURE_DEV__BASE__USE_WASMTIME

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
// #  include <base/platforms/backend/WASI.cpp>
// #  define _COM_AZURE_DEV__BASE__USE_WASMTIME_WASI
#  define _COM_AZURE_DEV__BASE__USE_WASMTIME_VERSION "0.12" // wasmtime: get version from header
#  include <wasmtime/wasm.h>
#  include <wasmtime/wasi.h>
#  define own
#  include <wasmtime/wasmtime.h>
#  define own
#endif

// TAG: use runtime linking like opengl
// TAG: add support for multiple engines - but adding Engine class

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class WASIContext;

#if 0
// https://github.com/WebAssembly/WASI/blob/master/phases/snapshot/witx/typenames.witx
// https://github.com/bytecodealliance/wasmtime/blob/master/docs/WASI-api.md
// https://github.com/CraneStation/wasi-libc/blob/master/libc-bottom-half/headers/public/wasi/api.h
// TAG: add security manifest
// TAG: lookup each API - from imports - add auto generate manifest - allow to run with disapproved security slots
#endif

WebAssembly::WASMValue::WASMValue()
  : type(TYPE_i32), i32(0)
{
}

WebAssembly::WASMValue::WASMValue(int32 value)
  : type(TYPE_i32), i32(value)
{
}

WebAssembly::WASMValue::WASMValue(int64 value)
  : type(TYPE_i64), i64(value)
{
}

WebAssembly::WASMValue::WASMValue(float value)
  : type(TYPE_f32), f32(value)
{
}

WebAssembly::WASMValue::WASMValue(double value)
  : type(TYPE_f64), f64(value)
{
}

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
String getValuesAsString(const wasm_val_t args[], MemorySize size)
{
  StringOutputStream sos;
  sos << "(";
  for (MemorySize i = 0; i < size; ++i) {
    const wasm_val_t& arg = args[i];
    if (i > 0) {
      sos << ", ";
    }
    switch (arg.kind) {
    case WASM_I32:
      sos << "i32: " << arg.of.i32 << "=" << HEX << arg.of.i32;
      break;
    case WASM_I64:
      sos << "i64: " << arg.of.i64 << "=" << HEX << arg.of.i64;
      break;
    case WASM_F32:
      sos << "f32: " << arg.of.f32 << "=" << FHEX << arg.of.f32;
      break;
    case WASM_F64:
      sos << "f64: " << arg.of.f64 << "=" << FHEX << arg.of.f64;
      break;
    case WASM_ANYREF:
      sos << "<REF>"; // TAG: what can we show
      break;
    case WASM_FUNCREF:
      sos << "<FUNCTION> = " << arg.of.i32; // TAG: make sure we get function id
      break;
    default:
      sos << "<BAD>";
    }
  }
  sos << ")";
  return sos;
}

own wasm_trap_t* bindToImplementation(void* env, const wasm_val_t args[], wasm_val_t results[]) noexcept;
own wasm_trap_t* forwardCallback(void* env, const wasm_val_t args[], wasm_val_t _results[]) noexcept;
own wasm_trap_t* fakeHook(void* env, const wasm_val_t args[], wasm_val_t results[]) noexcept;
own wasm_trap_t* hook(void* env, const wasm_val_t args[], wasm_val_t results[]) noexcept;
#endif

bool WebAssembly::isSupported() noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
  return true;
#else
  return false;
#endif
}

const char* WebAssembly::toString(Type type) noexcept
{
  switch (type) {
  case TYPE_UNSPECIFIED:
    return "UNSPECIFIED";
  case TYPE_FUNCTION:
    return "FUNCTION";
  case TYPE_i32:
    return "i32";
  case TYPE_i64:
    return "i64";
  case TYPE_f32:
    return "f32";
  case TYPE_f64:
    return "f64";
  default:
    BASSERT(!"Not supported.");
    return "UNSPECIFIED";
  }
}

WebAssemblyFunction::WebAssemblyFunction()
{
}

WebAssemblyFunction::WebAssemblyFunction(WebAssembly _wa, const String& _id)
  : wa(_wa), id(wa.getFunctionIndex(_id))
{
}

WebAssemblyFunction::WebAssemblyFunction(WebAssembly _wa, unsigned int _id)
  : wa(_wa), id(_id)
{
}

WebAssembly::FunctionType WebAssemblyFunction::getType()
{
  return wa.getFunctionType(id);
}

class WebAssembly::Handle : public ReferenceCountedObject {
public:

  class BindNativeType {
  public:

    enum {
      TYPE_BOOL,
      TYPE_INT16,
      TYPE_UINT16,
      TYPE_INT32,
      TYPE_UINT32,
      TYPE_INT64,
      TYPE_UINT64,
      TYPE_POINTER, // match size of object
      TYPE_CHAR,
      TYPE_WCHAR
    };
  };

  /** Call context. */
  class FunctionContext {
  public:
    
    WebAssembly::Handle* handle = nullptr;
    MemorySize argSize = 0;
    MemorySize resultSize = 0;
    String module;
    String name;
    
    WASMFunction func;
    void* context = nullptr;
    
    PreferredAtomicCounter invocations;
    
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    const wasm_functype_t* functype = nullptr;
    
    FunctionContext(WebAssembly::Handle* _handle, const wasm_functype_t* _functype) noexcept
      : handle(_handle), functype(_functype)
    {
      BASSERT(functype);
      const wasm_valtype_vec_t* ps = wasm_functype_params(functype);
      argSize = ps->size;
      const wasm_valtype_vec_t* rs = wasm_functype_results(functype);
      resultSize = rs->size;
    }
#endif
    
    FunctionContext(WebAssembly::Handle* _handle, MemorySize _argSize, MemorySize _resultSize) noexcept
      : handle(_handle), argSize(_argSize), resultSize(_resultSize)
    {
    }
        
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    inline Pair<uint8*, MemorySize> getMemory() noexcept
    {
      wasm_memory_t* memory = handle->memory;
      byte_t* bytes = wasm_memory_data(memory);
      size_t size = wasm_memory_data_size(memory);
      return {reinterpret_cast<uint8*>(bytes), size};
    }

    inline Pair<const uint8*, MemorySize> getMemory() const noexcept
    {
      wasm_memory_t* memory = handle->memory;
      byte_t* bytes = wasm_memory_data(memory);
      size_t size = wasm_memory_data_size(memory);
      return {reinterpret_cast<const uint8*>(bytes), size};
    }

    const uint8* read(MemorySize slot, MemorySize size) const
    {
      auto memory = getMemory();
      MemorySize end = slot + size;
      if ((slot >= memory.getSecond()) ||
          (end < slot) ||
          (end >= memory.getSecond())) {
        _throw WebAssemblyException("Accessing out of memory scope.");
      }
      return memory.getFirst() + slot;
    }

    inline const uint8* read(const wasm_val_t& v, MemorySize size) const
    {
      if (v.kind == WASM_I32) {
        return read(v.of.i32, size);
      }
      _throw WebAssemblyException("Unable to access memory.");
    }

    uint8* write(MemorySize slot, MemorySize size)
    {
      auto memory = getMemory();
      MemorySize end = slot + size;
      if ((slot >= memory.getSecond()) ||
          (end < slot) ||
          (end >= memory.getSecond())) {
        _throw WebAssemblyException("Accessing out of memory scope.");
      }
      return memory.getFirst() + slot;
    }

    inline uint8* write(const wasm_val_t& v, MemorySize size)
    {
      if (v.kind == WASM_I32) {
        return write(v.of.i32, size);
      }
      _throw WebAssemblyException("Unable to access memory.");
    }

    /** Returns trap for given message. */
    own wasm_trap_t* getTrap(const char* _message)
    {
      wasm_message_t message;
      wasm_store_t* store = handle->store;
      const MemorySize length = getNullTerminatedLength(_message);
      message.size = length + 1; // null-terminator is required to be included
      message.data = reinterpret_cast<wasm_byte_t*>(const_cast<char*>(_message));
      own wasm_trap_t* trap = wasm_trap_new(store, &message);
      return trap;
    }

    /** Returns trap for given exception. */
    own wasm_trap_t* getTrap(const Exception& e)
    {
      StringOutputStream sos;
      sos << e;
      return getTrap(sos.getString().native());
    }
#endif
  }; // TAG: allows forward to function
private:

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
  class Engine {
  public:
    
    wasm_engine_t* engine = nullptr;
    
    Engine()
    {
      engine = wasm_engine_new(); // 1 per process
    }
    
    ~Engine()
    {
      wasm_engine_delete(engine);
    }
  };
  
  wasm_engine_t* engine = nullptr;
  wasm_store_t* store = nullptr;
  own wasm_instance_t* instance = nullptr;
  own wasi_instance_t* wasiInstance = nullptr;
  own wasm_module_t* module = nullptr;
  own wasm_extern_vec_t exports = {0};
  wasm_memory_t* memory = nullptr;
#endif
  Array<String> exportNames;
  MemorySize maximumMemoryUsage = 0;
  bool useLog = false;
  Array<String> arguments;
  Map<String, String> environment;
  Map<String, String> folders;

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
  /** Returns string for given name. */
  static String toString(const wasm_name_t& _text)
  {
    if (_text.data) {
      return String(_text.data, _text.size);
    }
    return String();
  }
#endif

  /** Registered import function. */
  class ImportFunction {
  public:
  
    WASMFunction func = nullptr;
    void* context = nullptr;
    FunctionType type;
    String module;
    String name;
  };

  /** Registered import functions. */
  Array<ImportFunction> importFunctions;
  /** Function contexts. */
  Array<FunctionContext*> functionContexts;
public:
  
  inline bool getUseLog() const noexcept
  {
    return useLog;
  }

  inline void setUseLog(bool _useLog) noexcept
  {
    useLog = _useLog;
  }
  
  void registerFunctionImpl(WASMFunction func, void* context,
                            const FunctionType& type, const String& name, const String& module)
  {
    ImportFunction f;
    f.type = type;
    f.name = name;
    f.module = module;
    f.func = func;
    f.context = context;
    importFunctions.append(f);
  }
  
  void writeLog(const String& text)
  {
    // TAG: need output
    ferr << text << ENDL;
  }
    
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
  void writeLog(const String& id, bool in,
                const wasm_val_t* args, MemorySize argSize, const wasm_val_t* results, MemorySize resultSize)
  {
    StringOutputStream sos;
    sos << Date::getNow().getISO8601_US() << SP
        << (in ? "CALL " : "CALLBACK ")
        << getValuesAsString(results, resultSize) << SP
        << id << getValuesAsString(args, argSize);
    writeLog(sos);
  }

  void writeLog(const String& id, bool in,
                const wasm_val_t* args, MemorySize argSize, const wasm_trap_t* trap)
  {
    String msg = getTrapMessage(trap);
    StringOutputStream sos;
    sos << Date::getNow().getISO8601_US() << SP
        << (in ? "CALL " : "CALLBACK ")
        << "TRAP " << escape(msg) << SP
        << id << getValuesAsString(args, argSize);
    writeLog(sos);
  }

  static wasm_engine_t* getGlobalEngine()
  {
    static Engine* _engine = new Engine();
    return _engine->engine;
  }
#endif

  Handle()
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    engine = getGlobalEngine();
    if (!engine) {
      _throw ResourceException("Failed to initialize WASM.");
    }
    store = wasm_store_new(engine);
    if (!store) {
      _throw ResourceException("Failed to initialize WASM.");
    }
#endif
  }
  
  static String convertWATToWASM(const String& text)
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    wasm_byte_vec_t wat;
    clear(wat);
    wat.size = text.getLength();
    wat.data = (byte_t*)const_cast<uint8*>(text.getBytes());

    wasm_byte_vec_t wasm;
    wasm_engine_t* engine = getGlobalEngine();
    wasm_message_t errorMessage;
    if (!wasmtime_wat2wasm(
           engine,
           &wat,
           &wasm,
           &errorMessage)) {
      // TAG: add support for line and column info
      String msg = getMessage(errorMessage);
      _throw WebAssemblyException("Invalid WebAssembly text.");
    }
    String result(wasm.data, wasm.size);
    wasm_byte_vec_delete(&wasm);
    return result;
#else
    return String();
#endif
  }
  
  String getEngine()
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    return String("Wasmtime " _COM_AZURE_DEV__BASE__USE_WASMTIME_VERSION);
#else
    return String("WASM 1.0");
#endif
  }

  MemorySize getMaximumMemoryUsage() const
  {
    return maximumMemoryUsage;
  }

  void setMaximumMemoryUsage(MemorySize size)
  {
    maximumMemoryUsage = size;
  }

  MemorySize getMemoryUsage() const
  {
    return 0;
  }

  uint64 getProcessingTime() const
  {
    return 0;
  }
  
  void garbageCollect()
  {
  }
  
  bool isValid(const uint8* wasm, MemorySize size)
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    wasm_byte_vec_t binary;
    clear(binary);
    binary.size = size;
    binary.data = (byte_t*)const_cast<uint8*>(wasm);

    // wasm_byte_vec_new_uninitialized(&binary, size);
    bool status = wasm_module_validate(store, &binary);
    // wasm_byte_vec_delete(&binary);
    // return status;
    return status;
#else
    return false;
#endif
  }

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
  void dumpExtern(MemorySize i, /*const*/ wasm_extern_t* _extern, const String& name = String())
  {
    const wasm_externkind_t kind = wasm_extern_kind(_extern);
    switch (kind) {
    case WASM_EXTERN_FUNC:
      {
        wasm_func_t* func = wasm_extern_as_func(_extern);
        fout << "  " << i << " " << "FUNC" << " " << func << " NAME=" << name << ENDL;
      }
      break;
    case WASM_EXTERN_GLOBAL:
      {
        wasm_global_t* global = wasm_extern_as_global(_extern);
        fout << "  " << i << " " << "GLOBAL" << " " << global << " NAME=" << name  << ENDL;
      }
      break;
    case WASM_EXTERN_TABLE:
      {
        wasm_table_t* table = wasm_extern_as_table(_extern);
        fout << "  " << i << " " << "TABLE" << " " << table << " NAME=" << name  << ENDL;
      }
      break;
    case WASM_EXTERN_MEMORY:
      {
        wasm_memory_t* memory = wasm_extern_as_memory(_extern);
        fout << "  " << i << " " << "MEMORY" << " " << memory << " NAME=" << name  << ENDL;
      }
      break;
    default:
      ; // ignore
    }
  }
#endif

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
  
  static String getMessage(const wasm_message_t& message)
  {
    MemorySize size = message.size;
    if (size) {
      // fout << MemoryDump((const uint8*)message.data, message.size) << ENDL;
      // TAG: wasmtime: why do we get an extra null-terminator added - https://github.com/bytecodealliance/wasmtime/issues/994
      while ((size > 0) && !message.data[size - 1]) {
        --size;
      }
      return String(reinterpret_cast<const char*>(message.data), size);
    }
    return String();
  }
  
  String getTrapMessage(const wasm_trap_t* trap)
  {
    if (trap) {
      own wasm_message_t message;
      wasm_trap_message(trap, &message);
      return getMessage(message);
    }
    return String();
  }
  
  [[noreturn]] void onTrap(own wasm_trap_t* trap)
  {
    if (trap) {
      String msg = getTrapMessage(trap);
      _throw WebAssemblyException(msg.native());
    } else {
      _throw WebAssemblyException("Trap was raised.");
    }
  }
  
  static inline WebAssembly::Type toType(wasm_valkind_t kind)
  {
    switch (kind) {
    case WASM_I32:
      return WebAssembly::TYPE_i32;
    case WASM_I64:
      return WebAssembly::TYPE_i64;
    case WASM_F32:
      return WebAssembly::TYPE_f32;
    case WASM_F64:
      return WebAssembly::TYPE_f64;
    case WASM_ANYREF:
      BASSERT(!"Unsupported type.");
      return WebAssembly::TYPE_UNSPECIFIED;
    case WASM_FUNCREF:
      return WebAssembly::TYPE_FUNCTION;
    default:
      BASSERT(!"Unsupported type.");
    }
    return TYPE_UNSPECIFIED;
  }
  
  FunctionType getFunctionType(const wasm_functype_t* functype)
  {
    FunctionType r;
    if (!functype) {
      return r;
    }

    const wasm_valtype_vec_t* ps = wasm_functype_params(functype);
    const wasm_valtype_vec_t* rs = wasm_functype_results(functype);

    r.arguments.setSize(ps->size);
    for (MemorySize i = 0; i < ps->size; ++i) {
      wasm_valtype_t* v = ps->data[i];
      wasm_valkind_t kind = wasm_valtype_kind(v);
      r.arguments[i] = toType(kind);
    }

    r.results.setSize(rs->size);
    for (MemorySize i = 0; i < rs->size; ++i) {
      wasm_valtype_t* v = rs->data[i];
      wasm_valkind_t kind = wasm_valtype_kind(v);
      r.results[i] = toType(kind);
    }
    
    return r;
  }
#endif
  
  WebAssembly::FunctionType getFunctionType(unsigned int id)
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    own wasm_importtype_vec_t imports;
    if (id < imports.size) {
      const wasm_importtype_t* import = imports.data[id];
      if (import) {
        const wasm_externtype_t* externType = wasm_importtype_type(import);
        wasm_externkind_t kind = wasm_externtype_kind(externType);
        if (kind == WASM_EXTERN_FUNC) {
          const wasm_functype_t* functype = wasm_externtype_as_functype_const(externType);
          return getFunctionType(functype);
        }
      }
    }
#endif
    return FunctionType();
  }

  bool load(const uint8* wasm, MemorySize size)
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    
#if 0
    wasm_byte_vec_t binaryZeros;
    wasm_byte_vec_new_uninitialized(&binaryZeros, 1024);
    bool status = wasm_module_validate(store, &binaryZeros);
    module = wasm_module_new(store, &binaryZeros);
#endif
    
    wasm_byte_vec_t binary;
    // wasm_byte_vec_new_uninitialized(&binary, size);
    // copy(reinterpret_cast<uint8*>(binary.data), wasm, size);
    clear(binary);
    binary.size = size;
    binary.data = (byte_t*)const_cast<uint8*>(wasm);
    module = wasm_module_new(store, &binary);
    // wasm_byte_vec_delete(&binary);
 
    if (!wasm_module_validate(store, &binary)) {
      return false;
    }

#if 0
    own wasm_importtype_vec_t _imports;
    wasm_module_imports(module, &_imports);
    // fout << "IMPORTS:" << ENDL;
    for (MemorySize i = 0; i < _imports.size; ++i) {
      const wasm_importtype_t* import = _imports.data[i];
      // const wasm_name_t* moduleName = wasm_importtype_module(import);
      const wasm_name_t* name = wasm_importtype_name(import);
      const wasm_externtype_t* type = wasm_importtype_type(import);
      wasm_externkind_t kind = wasm_externtype_kind(type);
      // dumpExtern(i, (wasm_extern_t*)import, name ? toString(*name) : String());
    }
#endif
    
    own wasm_exporttype_vec_t _exports;
    wasm_module_exports(module, &_exports);
    exportNames.setSize(_exports.size);
    // fout << "EXPORTS:" << ENDL;
    for (MemorySize i = 0; i < _exports.size; ++i) {
      const wasm_exporttype_t* e = _exports.data[i];
      const wasm_name_t* _name = wasm_exporttype_name(e);
      String name(_name ? toString(*_name) : String());
      exportNames[i] = name;
      // const wasm_externtype_t* type = wasm_exporttype_type(e);
      // wasm_externkind_t kind = wasm_externtype_kind(type);
      // dumpExtern(i, (wasm_extern_t*)e, name);
    }
        
    return true;
#else
    return false;
#endif
  }
  
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
  /** Registers WASI import. */
  void registerWASIImport(const wasm_extern_t** dest,
                          wasm_importtype_vec_t src,
                          wasm_func_callback_with_env_t hook,
                          const String& _moduleName,
                          const String& _name)
  {
    for (MemorySize i = 0; i < src.size; ++i) {
      const wasm_importtype_t* import = src.data[i];
      if (!import) {
        continue;
      }
      // TAG: check __moduleName against previous value
      const wasm_name_t* __moduleName = wasm_importtype_module(import);
      const String moduleName = __moduleName ? toString(*__moduleName) : String();
      if (moduleName != _moduleName) {
        continue;
      }
      const wasm_name_t* __name = wasm_importtype_name(import);
      const String name = __name ? toString(*__name) : String();
      if (name != _name) {
        continue;
      }
      const wasm_externtype_t* externType = wasm_importtype_type(import);
      wasm_externkind_t kind = wasm_externtype_kind(externType);
      switch (kind) {
      case WASM_EXTERN_FUNC:
        {
          const wasm_functype_t* functype = wasm_externtype_as_functype_const(externType);
          FunctionContext* functionContext = new FunctionContext(this, functype);
          functionContext->module = moduleName;
          functionContext->name = name;
          functionContexts.append(functionContext);
          own wasm_func_t* hook_func = wasm_func_new_with_env(store, functype, hook, functionContext, nullptr);
          dest[i] = wasm_func_as_extern(hook_func);
        }
        break;
      default:
        BASSERT(!"Unsupported import.");
      }
      break;
    }
  }

  /** Adds imports. Does NOT overwrite imports. */
  void registerImports(const wasm_extern_t** dest, wasm_importtype_vec_t src, const Array<ImportFunction>& lookup)
  {
    for (MemorySize i = 0; i < src.size; ++i) {
      const wasm_importtype_t* import = src.data[i];
      if (!import) {
        continue;
      }
      if (dest[i]) { // already registered
        continue;
      }
      const wasm_name_t* _moduleName = wasm_importtype_module(import);
      // wasmtime: should this be a URN that is registered
      const String moduleName = _moduleName ? toString(*_moduleName) : String();
      const wasm_name_t* _name = wasm_importtype_name(import);
      const String name = _name ? toString(*_name) : String();
      const wasm_externtype_t* externType = wasm_importtype_type(import);
      wasm_externkind_t kind = wasm_externtype_kind(externType);
      if (kind == WASM_EXTERN_FUNC) {
        const wasm_functype_t* functype = wasm_externtype_as_functype_const(externType);
        for (const auto& importFunction : lookup) {
          if ((importFunction.name == name) && (importFunction.module == moduleName)) {
            FunctionContext* functionContext = new FunctionContext(this, functype);
            functionContext->func = importFunction.func;
            functionContext->context = importFunction.context;
            functionContext->module = moduleName;
            functionContext->name = name;
            functionContexts.append(functionContext);

            own wasm_func_t* hook = wasm_func_new_with_env(store, functype, forwardCallback, functionContext, nullptr);
            dest[i] = wasm_func_as_extern(hook);
            break;
          }
        }
      } else {
        BASSERT(!"Unsupported import.");
      }
    }
  }

  /** Adds dummy imports. Does NOT overwrite imports. */
  void registerFakeImports(const wasm_extern_t** dest, wasm_importtype_vec_t src)
  {
    for (MemorySize i = 0; i < src.size; ++i) {
      const wasm_importtype_t* import = src.data[i];
      if (!import) {
        continue;
      }
      if (dest[i]) { // already registered
        continue;
      }
      const wasm_name_t* _moduleName = wasm_importtype_module(import);
      // wasmtime: should this be a URN that is registered
      const String moduleName = _moduleName ? toString(*_moduleName) : String();
      const wasm_name_t* _name = wasm_importtype_name(import);
      const String name = _name ? toString(*_name) : String();
      const wasm_externtype_t* externType = wasm_importtype_type(import);
      wasm_externkind_t kind = wasm_externtype_kind(externType);
      switch (kind) {
      case WASM_EXTERN_FUNC:
        {
          const wasm_functype_t* functype = wasm_externtype_as_functype_const(externType);
          FunctionContext* functionContext = new FunctionContext(this, functype);
          functionContext->module = moduleName;
          functionContext->name = name;
          functionContexts.append(functionContext);
          own wasm_func_t* hook_func = wasm_func_new_with_env(store, functype, fakeHook, functionContext, nullptr);
          dest[i] = wasm_func_as_extern(hook_func);
        }
        break;
      default:
        BASSERT(!"Unsupported import.");
      }
    }
  }
#endif
  
  void setArguments(const Array<String>& arguments)
  {
    this->arguments = arguments;
  }

  void setEnvironment(const Map<String, String>& environment)
  {
    this->environment = environment;
  }

  void setMountedFolders(const Map<String, String>& folders)
  {
    this->folders = folders;
  }

  void setUseProfiler(bool useProfiler)
  {
  }

  bool makeInstance(bool fake, bool wasi = false)
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    if (!module) {
      _throw WebAssemblyException("Module not loaded.");
    }
    
    own wasm_importtype_vec_t _imports;
    wasm_module_imports(module, &_imports);
    PrimitiveArray<const wasm_extern_t*> imports(_imports.size);
    for (MemorySize i = 0; i < imports.size(); ++i) {
      imports[i] = nullptr;
    }
    
    if ((_imports.size > 0) && importFunctions) {
      registerImports(imports.begin(), _imports, importFunctions);
    }
    
    if (fake) {
      registerFakeImports(imports.begin(), _imports);
    }
    
#if 0
      own wasm_functype_t* hello_type = wasm_functype_new_0_0();
      FunctionContext* functionContext = new FunctionContext(this, 0, 0);
      functionContexts.append(functionContext);
      own wasm_func_t* hook_func = wasm_func_new_with_env(store, hello_type, hook, functionContext, nullptr);
      // own wasm_func_t* hello_func = wasm_func_new(store, hello_type, hello);
      wasm_functype_delete(hello_type);
#endif
    
    if (wasi) {
      // registerWASIImports(imports.begin(), _imports);
      
      own wasi_config_t* config = wasi_config_new();
      if (!config) {
        _throw WebAssemblyException("Failed to initialize WASI config.");
      }

      Array<const char*> argv;
      for (auto argument : arguments) {
        argv.append(argument.native());
      }
      wasi_config_set_argv(config, arguments.getSize(), argv.getFirstReference());

      Array<const char*> enames;
      Array<const char*> evalues;
      for (auto nv : environment) {
        enames.append(nv.getKey().native());
        evalues.append(nv.getValue().native());
      }
      wasi_config_set_env(
        config,
        environment.getSize(),
        enames.getFirstReference(),
        evalues.getFirstReference()
      );
      
      // TAG: wasi - need to bind to Input/Output streams
      wasi_config_inherit_stdin(config);
      wasi_config_inherit_stdout(config);
      wasi_config_inherit_stderr(config);

      for (auto nv : folders) {
        if (!wasi_config_preopen_dir(config, nv.getKey().native(), nv.getValue().native())) {
          _throw WebAssemblyException("Failed to add folder.");
        }
      }
      
      own wasm_trap_t* trap = nullptr;
      wasiInstance = wasi_instance_new(
        store,
        config,
        &trap
      );
      // do NOT delete wasi_config_delete(config);
      if (trap) {
        onTrap(trap);
      }
      
      // register WASI imports
      const wasm_extern_t** dest = imports.begin();
      for (MemorySize i = 0; i < _imports.size; ++i) {
        const wasm_importtype_t* import = _imports.data[i];
        if (import) {
          const wasm_name_t* _moduleName = wasm_importtype_module(import);
          const String moduleName = _moduleName ? toString(*_moduleName) : String();
          const wasm_name_t* _name = wasm_importtype_name(import);
          const String name = _name ? toString(*_name) : String();
          // fout << (moduleName + "!" + name) << ":" << ENDL;
          const wasm_extern_t* e = wasi_instance_bind_import( // wasmtime: has hardcoded module
            wasiInstance,
            import
          );
          if (e) {
            // fout << "  " << e << ENDL;
            *dest = e;
          }
        }
        ++dest;
      }

    } else {
      own wasm_trap_t* trap = nullptr;
      instance = wasm_instance_new(store, module, imports, &trap);
      if (trap) {
        onTrap(trap);
      }
    }

    // wasm_func_delete(hello_func);
    if (!wasiInstance && !instance) {
      return false;
    }
    // TAG: need a away to get instance from wasiInstance
    if (instance) {
      wasm_instance_exports(instance, &exports);
    }
    
    // get memory
    for (size_t i = 0; i < exports.size; ++i) {
      if (wasm_extern_t* e = exports.data[i]) {
        wasm_externkind_t kind = wasm_extern_kind(e);
        if (kind == WASM_EXTERN_MEMORY) {
          this->memory = wasm_extern_as_memory(e);
          break;
        }
      }
    }

#if 0
    // fout << "EXPORTS:" << ENDL;
    for (MemorySize i = 0; i < exports.size; ++i) {
      wasm_extern_t* e = exports.data[i];
      own const wasm_externtype_t* et = wasm_extern_type(e);
      // const wasm_externtype_t* wasm_exporttype_type(const wasm_exporttype_t*);
      dumpExtern(i, e);
    }
#endif
    return true;
#endif
    return false;
  }
  
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
  /** Registers WASI function. */
  template<typename RESULT, typename... ARGS>
  void registerWASIFunction(RESULT (WASIContext::*function)(ARGS...),
                            const wasm_extern_t** dest,
                            wasm_importtype_vec_t src,
                            wasm_func_callback_with_env_t hook,
                            const String& module,
                            const String& name)
  {
    const Type resultType = MapType<RESULT>::type;
    const Type types[] = { MapType<ARGS>::type..., TYPE_UNSPECIFIED };
    // TAG: need to compare function type
    registerWASIImport(dest, src, hook, module, name);
  }

  void registerWASIImports(const wasm_extern_t** dest, wasm_importtype_vec_t src);
#endif
  
  bool makeWASIInstance(InputStream* _stdin, OutputStream* _stdout, OutputStream* _stderr)
  {
    return makeInstance(true, true);
  }
  
  ConstSpan<uint8> getMemory() const
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    if (!memory) {
      _throw WebAssemblyException("Memory not available.");
    }
    const byte_t* bytes = wasm_memory_data(memory);
    const size_t _size = wasm_memory_data_size(memory);
    return ConstSpan<uint8>(reinterpret_cast<const uint8*>(bytes), _size);
#else
    return ConstSpan<uint8>();
#endif
  }

  Span<uint8> getMemory()
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    if (!memory) {
      _throw WebAssemblyException("Memory not available.");
    }
    byte_t* bytes = wasm_memory_data(memory);
    const size_t _size = wasm_memory_data_size(memory);
    return Span<uint8>(reinterpret_cast<uint8*>(bytes), _size);
#else
    return Span<uint8>();
#endif
  }

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
  Symbol getSymbol(MemorySize i, const wasm_functype_t* ft, const String& name, const String& moduleName)
  {
    Symbol s;
    s.index = i;
    s.name = name;
    s.moduleName = moduleName;
    s.externType = EXTERN_FUNCTION;
    s.functionType = getFunctionType(ft);
    return s;
  }

  Symbol getSymbol(MemorySize i, const wasm_func_t* f, const String& name, const String& moduleName)
  {
    Symbol s;
    s.index = i;
    s.name = name;
    s.moduleName = moduleName;
    s.externType = EXTERN_FUNCTION;
    if (f) {
      s.func = (void*)f;
      own wasm_functype_t* functype = wasm_func_type(f);
      if (functype) {
        s.functionType = getFunctionType(functype);
        wasm_functype_delete(functype);
      }
    }
    return s;
  }
#endif

  MemorySize getNumberOfImports() const
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    own wasm_importtype_vec_t _imports;
    wasm_module_imports(module, &_imports);
    return _imports.size;
#else
    return 0;
#endif
  }

  MemorySize getNumberOfExports() const
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    return exports.size;
#else
    return 0;
#endif
  }

  Array<Symbol> getImports()
  {
    Array<Symbol> result;
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)

    own wasm_importtype_vec_t _imports;
    wasm_module_imports(module, &_imports);

    result.setSize(_imports.size);
    
    for (MemorySize i = 0; i < _imports.size; ++i) {
      const wasm_importtype_t* import = _imports.data[i];
      if (!import) {
        continue;
      }
      
      const wasm_name_t* _moduleName = wasm_importtype_module(import); // wasmtime: should this be a URN that is registered
      const String moduleName = _moduleName ? toString(*_moduleName) : String();
      const wasm_name_t* _name = wasm_importtype_name(import);
      const String name = _name ? toString(*_name) : String();
      const wasm_externtype_t* externType = wasm_importtype_type(import);
      wasm_externkind_t kind = wasm_externtype_kind(externType);

      switch (kind) {
      case WASM_EXTERN_FUNC:
        {
          const wasm_functype_t* ft = wasm_externtype_as_functype_const(externType);
          result[i] = getSymbol(i, ft, name, moduleName);
        }
        break;
      case WASM_EXTERN_GLOBAL:
        {
          result[i].externType = EXTERN_GLOBAL;
        }
        break;
      case WASM_EXTERN_TABLE:
        {
          result[i].externType = EXTERN_TABLE;
        }
        break;
      case WASM_EXTERN_MEMORY:
        {
          result[i].externType = EXTERN_MEMORY;
          // TAG: const wasm_memory_t* ft = wasm_externtype_as_memory_const(externType);
        }
        break;
      default:
        BASSERT(!"Unsupported extern.");
      }
    }
#endif
    return result;
  }
  
  Array<Symbol> getExports()
  {
    Array<Symbol> result;
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    result.setSize(exports.size);
    for (size_t i = 0; i < exports.size; ++i) {
      wasm_extern_t* e = exports.data[i];
      if (!e) {
        continue;
      }
      result[i].index = i;
      wasm_externkind_t kind = wasm_extern_kind(e);
      switch (kind) {
      case WASM_EXTERN_FUNC:
        {
          result[i].externType = EXTERN_FUNCTION;
          const wasm_func_t* func = wasm_extern_as_func(e);
          if (func) {
            String name((i < exportNames.getSize()) ? exportNames[i] : String());
            result[i] = getSymbol(i, func, name, String());
          }
        }
        break;
      case WASM_EXTERN_GLOBAL:
        {
          result[i].externType = EXTERN_GLOBAL;
          const wasm_global_t* global = wasm_extern_as_global(e);
          if (global) {
          }
        }
        break;
      case WASM_EXTERN_TABLE:
        {
          result[i].externType = EXTERN_TABLE;
          const wasm_table_t* table = wasm_extern_as_table(e);
          if (table) {
          }
        }
        break;
      case WASM_EXTERN_MEMORY:
        {
          Symbol& symbol = result[i];
          symbol.externType = EXTERN_MEMORY;
          const wasm_memory_t* memory = wasm_extern_as_memory(e);
          if (memory) {
#if 0
            // TAG: wasm_memory_type not available
            own wasm_memorytype_t* type = wasm_memory_type(memory);
            const wasm_limits_t* limits = wasm_memorytype_limits(type);
            fout << "LIMITS [%1; %2]" % Subst(limits->min, limits->max) << ENDL;
#endif
            size_t size = wasm_memory_data_size(memory);
            symbol.memorySize = size;
            // byte_t* bytes = wasm_memory_data(memory);
            // fout << "SIZE %1" % Subst(size) << ENDL;
            // wasm_memory_pages_t pages = wasm_memory_size(memory);
            // fout << "PAGES %1" % Subst(pages) << ENDL;
            // bool wasm_memory_grow(wasm_memory_t*, wasm_memory_pages_t delta);
          }
        }
        break;
      default:
        BASSERT(!"Unsupported extern.");
      }
    }
#endif
    return result;
  }

  Symbol getSymbol(MemorySize index)
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    if (index < exports.size) {
      wasm_externkind_t kind = wasm_extern_kind(exports.data[index]);
      if (kind != WASM_EXTERN_FUNC) {
        return Symbol();
      }
      const wasm_func_t* func = wasm_extern_as_func(exports.data[index]);
      // TAG: should we add isIndex() ? exportnames.isIndex(index), maybe getAt(index, default)
      String name((index < exportNames.getSize()) ? exportNames[index] : String());
      return getSymbol(index, func, name, String());
    }
#endif
    return Symbol();
  }
  
  Symbol getSymbol(const String& id)
  {
    MemoryDiff index = exportNames.indexOf(id);
    if (index < 0) {
      return Symbol();
    }
    
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    wasm_extern_t* e = exports.data[index];
    wasm_externkind_t kind = wasm_extern_kind(e);
    if (kind != WASM_EXTERN_FUNC) {
      return Symbol();
    }
    const wasm_func_t* func = wasm_extern_as_func(e);
    // TAG: should we add isIndex()? exportnames.isIndex(index), maybe getAt(index, default)
    String name((index < exportNames.getSize()) ? exportNames[index] : String());
    return getSymbol(index, func, name, String());
#endif
    return Symbol();
  }
    
  FormatOutputStream& writeFunction(
    FormatOutputStream& stream, const String& id, const AnyValue* arguments, MemorySize size)
  {
    stream << "WebAssemble: calling: " << id << "(";
    bool first = true;
    for (MemorySize i = 0; i < size; ++i) {
      const AnyValue& a = arguments[i];
      if (!first) {
        stream << ", ";
      }
      first = false;
      stream << a;
    }
    stream << ")" << ENDL;
    return stream;
  }

  void call(unsigned int id, const WASMValue* _arguments, WASMValue* _results)
  {
    String name = exportNames[id];
    if (!name) {
      name = String(format() << id);
    }

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    if (id < exports.size) {
      wasm_extern_t* e = exports.data[id];
      wasm_externkind_t kind = wasm_extern_kind(e);
      if (kind != WASM_EXTERN_FUNC) {
        _throw WebAssemblyException("Not a function.");
      }
      const wasm_func_t* func = wasm_extern_as_func(e);
      if (!func) {
        _throw WebAssemblyException("Not a function.");
      }

      const size_t size = wasm_func_param_arity(func);
      const size_t resultSize = wasm_func_result_arity(func);
      PrimitiveStackArray<wasm_val_t> args(size);
      PrimitiveStackArray<wasm_val_t> results(resultSize);
      if (size > 0) {
        own wasm_functype_t* functype = wasm_func_type(func);
        // const wasm_valtype_vec_t* rs = wasm_functype_results(functype);
        const wasm_valtype_vec_t* ps = wasm_functype_params(functype);
        if (INLINE_ASSERT(ps && (ps->size == size))) {
          for (MemorySize i = 0; i < size; ++i) {
            convert(args[i], _arguments[i]);
          }
        }
        wasm_functype_delete(functype);
      }

      if (own wasm_trap_t* trap = wasm_func_call(func, args, results)) {
        if (getUseLog()) {
          writeLog(name, true, args, size, trap);
        }
        onTrap(trap);
      }
      if (getUseLog()) {
        writeLog(name, true, args, size, results, resultSize);
      }
      for (MemorySize i = 0; i < resultSize; ++i) {
        convert(_results[i], results[i]);
      }
      return;
    }
#endif
    _throw WebAssemblyException("No such function.");
  }

  AnyValue call(MemorySize id, const AnyValue* arguments, MemorySize argumentsSize)
  {
    String name = exportNames[id];
    if (!name) {
      name = String(format() << id);
    }

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    if (id < exports.size) {
      wasm_extern_t* e = exports.data[id];
      wasm_externkind_t kind = wasm_extern_kind(e);
      if (kind != WASM_EXTERN_FUNC) {
        _throw WebAssemblyException("Not a function.");
      }
      const wasm_func_t* func = wasm_extern_as_func(e);
      if (!func) {
        _throw WebAssemblyException("Not a function.");
      }

      const size_t size = wasm_func_param_arity(func);
      const size_t resultSize = wasm_func_result_arity(func);
      PrimitiveStackArray<wasm_val_t> args(size);
      PrimitiveStackArray<wasm_val_t> results(resultSize);
      if (size > 0) {
        own wasm_functype_t* functype = wasm_func_type(func);
        // const wasm_valtype_vec_t* rs = wasm_functype_results(functype);
        const wasm_valtype_vec_t* ps = wasm_functype_params(functype);
        if (INLINE_ASSERT(ps && (ps->size == size))) {
          convert(args, args + size, *ps, arguments, argumentsSize);
        }
        wasm_functype_delete(functype);
      }

      if (own wasm_trap_t* trap = wasm_func_call(func, args, results)) {
        if (getUseLog()) {
          writeLog(name, true, args, size, trap);
        }
        onTrap(trap);
      }
      if (getUseLog()) {
        writeLog(name, true, args, size, results, resultSize);
      }
      return convertToAnyValue(results, resultSize);
    }
#endif
    _throw WebAssemblyException("No such function.");
  }
  
  void callEntry()
  {
    call(0, nullptr, nullptr);
  }

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
  void convert(wasm_val_t& dest, const WebAssembly::WASMValue& src)
  {
    switch (src.type) {
    case WebAssembly::TYPE_i32:
      dest.kind = WASM_I32;
      dest.of.i32 = src.i32;
      break;
    case WebAssembly::TYPE_i64:
      dest.kind = WASM_I64;
      dest.of.i64 = src.i64;
      break;
    case WebAssembly::TYPE_f32:
      dest.kind = WASM_F32;
      dest.of.f32 = src.f32;
      break;
    case WebAssembly::TYPE_f64:
      dest.kind = WASM_F64;
      dest.of.f64 = src.f64;
      break;
    default:
      _throw WebAssemblyException("Unsupported type.");
    }
  }
  
  void convert(WebAssembly::WASMValue& dest, const wasm_val_t& src)
  {
    switch (src.kind) {
    case WASM_I32:
      dest.type = WebAssembly::TYPE_i32;
      dest.i32 = src.of.i32;
      break;
    case WASM_I64:
      dest.type = WebAssembly::TYPE_i64;
      dest.i64 = src.of.i64;
      break;
    case WASM_F32:
      dest.type = WebAssembly::TYPE_f32;
      dest.f32 = src.of.f32;
      break;
    case WASM_F64:
      dest.type = WebAssembly::TYPE_f64;
      dest.f64 = src.of.f64;
      break;
    default:
      _throw WebAssemblyException("Unsupported type.");
    }
  }

  wasm_val_t* convert(wasm_val_t* dest, const AnyValue& value)
  {
    // TAG: add support for string
    // we do not know if signed or unsigned
    switch (dest->kind) {
    case WASM_I32:
      if (!value.isInteger()) {
        _throw WebAssemblyException("Expected i32.");
      }
      if (value.isUnsigned()) {
        const uint64 v = value.getUnsignedLongLongInteger();
        if (v > static_cast<uint64>(PrimitiveTraits<uint32>::MAXIMUM)) {
          _throw WebAssemblyException("Expected i32.");
        }
        dest->of.i32 = static_cast<uint32>(v);
      } else { // signed
        const int64 v = value.getLongLongInteger();
        if (v > static_cast<int64>(PrimitiveTraits<int32>::MAXIMUM)) {
          _throw WebAssemblyException("Expected i32.");
        } else if (v < static_cast<int64>(PrimitiveTraits<int32>::MINIMUM)) {
          _throw WebAssemblyException("Expected i32.");
        }
        dest->of.i32 = static_cast<int32>(v);
      }
      ++dest;
      break;
    case WASM_I64:
      if (!value.isInteger()) {
        _throw WebAssemblyException("Expected i64.");
      }
      if (value.isUnsigned()) {
        dest->of.i64 = value.getUnsignedLongLongInteger();
      } else { // signed
        dest->of.i64 = value.getLongLongInteger();
      }
      ++dest;
      break;
    case WASM_F32:
      if (!value.isFloatingPoint()) {
        _throw WebAssemblyException("Expected f32.");
      }
      dest->of.f32 = value.getFloat();
      ++dest;
      break;
    case WASM_F64:
      if (!value.isFloatingPoint()) {
        _throw WebAssemblyException("Expected f64.");
      }
      dest->of.f64 = value.getDouble();
      ++dest;
      break;
    default:
      _throw WebAssemblyException("Unsupported type.");
    }
    return dest;

#if 0
    switch (value.getRepresentation()) {
    case AnyValue::BOOLEAN:
      dest->kind = WASM_I32;
      dest++->of.i32 = value.getBoolean() ? 1 : 0;
      break;
    case AnyValue::SHORT_INTEGER:
    case AnyValue::INTEGER:
      dest->kind = WASM_I32;
      dest++->of.i32 = value.getInteger();
      break;
    case AnyValue::UNSIGNED_SHORT_INTEGER:
    case AnyValue::UNSIGNED_INTEGER:
      dest->kind = WASM_I32;
      dest++->of.i32 = value.getUnsignedInteger();
      break;
    case AnyValue::LONG_INTEGER:
    case AnyValue::LONG_LONG_INTEGER:
      dest->kind = WASM_I64;
      dest++->of.i64 = value.getLongLongInteger();
      break;
    case AnyValue::UNSIGNED_LONG_INTEGER:
    case AnyValue::UNSIGNED_LONG_LONG_INTEGER:
      dest->kind = WASM_I64;
      dest++->of.i64 = value.getUnsignedLongLongInteger();
      break;
    case AnyValue::FLOAT:
      // API is doing float mapping for now (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION)
      // detect if float32_t is compatible with float
      dest->kind = WASM_F32;
      dest++->of.f32 = value.getFloat();
      break;
    case AnyValue::DOUBLE:
      dest->kind = WASM_F64;
      dest++->of.f64 = value.getDouble();
      break;
    case AnyValue::LONG_DOUBLE:
      dest->kind = WASM_F64;
      dest++->of.f64 = value.getDouble();
      break;
    case AnyValue::CHARACTER:
    case AnyValue::WIDE_CHARACTER:
    case AnyValue::STRING:
    case AnyValue::WIDE_STRING:
      {
        // TAG: how can we pass on string
        _throw NotSupported("String is not supported.");

        String s(value.getString());
        dest->kind = WASM_I32;
        dest++->of.i32 = s.getLength();
        dest->kind = WASM_I32;
        dest++->of.i32 = s.getLength();
      }
      break;
    default:
      _throw NotSupported("Unsupported argument.");
    }
    return dest;
#endif
  }
  
  void convert(wasm_val_t* dest, const wasm_val_t* end,
               const wasm_valtype_vec_t& types,
               const AnyValue* arguments, MemorySize argumentsSize)
  {
    for (MemorySize i = 0; i < argumentsSize; ++i) {
      if (dest == end) {
        _throw WebAssemblyException("Arguments mismatch.");
      }
      const AnyValue& a = arguments[i];
      dest->kind = wasm_valtype_kind(types.data[i]);
      dest = convert(dest, a);
    }
    if (dest != end) {
      _throw WebAssemblyException("Arguments mismatch.");
    }
  }
  
  AnyValue convertToAnyValue(const wasm_val_t* src, MemorySize size)
  {
    // TAG: add support for cast to string type
    AnyValue r;
    if (size == 0) {
      return r;
    }
    switch (src->kind) {
    case WASM_I32:
      r = src->of.i32;
      break;
    case WASM_I64:
      r = src->of.i64;
      break;
    case WASM_F32:
      r = src->of.f32;
      break;
    case WASM_F64:
      r = src->of.f64;
      break;
    case WASM_ANYREF:
      _throw WebAssemblyException("Unsupported type.");
      break;
    case WASM_FUNCREF:
      _throw WebAssemblyException("Unsupported type.");
      break;
    default:
      _throw WebAssemblyException("Unsupported type.");
    }
    return r;
  }
#endif
  
  // TAG: use wasm_name_new_from_string

  MemorySize getFunctionIndex(const String& id) const
  {
    MemorySize index = 0;
    if (id) {
      Validified<unsigned int> _index = UnsignedInteger::parseNoThrow(id, UnsignedInteger::DEC);
      if (_index.isValid()) {
        index = _index;
      } else {
        MemoryDiff i = exportNames.indexOf(id);
        if (i < 0) {
          _throw WebAssemblyException("No such function.");
        }
        index = i;
      }
    }
    return index;
  }

  ~Handle()
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    
    for (auto c : functionContexts) {
      delete c;
    }

    wasm_extern_vec_delete(&exports);
    
    if (module) {
      wasm_module_delete(module);
    }
    if (instance) {
      wasm_instance_delete(instance);
    }
#endif
  }
};

WebAssembly::WebAssembly()
{
  handle = new Handle();
}

String WebAssembly::getEngine()
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getEngine();
}

String WebAssembly::convertWATToWASM(const String& text)
{
  return WebAssembly::Handle::convertWATToWASM(text);
}

uint64 WebAssembly::getProcessingTime() const
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getProcessingTime();
}

MemorySize WebAssembly::getMaximumMemoryUsage() const
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getMaximumMemoryUsage();
}

void WebAssembly::setMaximumMemoryUsage(MemorySize size)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->setMaximumMemoryUsage(size);
}

MemorySize WebAssembly::getMemoryUsage() const
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getMemoryUsage();
}

void WebAssembly::garbageCollect()
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->garbageCollect();
}

String WebAssembly::toString(const FunctionType& functionType, const String& name, const String& module, bool colorize)
{
  StringOutputStream sos;
  bool first = true;
  if (colorize) {
    sos << setForeground(ANSIEscapeSequence::RED) << italic();
  }
  sos << "[";
  for (const auto type : functionType.results) {
    if (!first) {
      sos << ", ";
    }
    first = false;
    sos << toString(type);
  }
  sos << "] ";
  if (colorize) {
    sos << normal();
    sos << setForeground(ANSIEscapeSequence::GREEN);
  }
  if (module) {
    sos << module << "!";
  }
  if (colorize) {
    sos << normal() << bold();
    sos << setForeground(ANSIEscapeSequence::BLUE);
  }
  if (name) {
    sos << name;
  } else {
    sos << "(*)";
  }
  if (colorize) {
    sos << nobold() << italic();
  }
  sos << "(";
  first = true;
  for (const auto type : functionType.arguments) {
    if (!first) {
      sos << ", ";
    }
    first = false;
    sos << toString(type);
  }
  sos << ")";
  if (colorize) {
    sos << normal();
  }
  return sos;
}

String WebAssembly::toString(const Symbol& s, bool colorize)
{
  switch (s.externType) {
  case EXTERN_FUNCTION:
    if (s.name) {
      return toString(s.functionType, s.name, s.moduleName, colorize);
    } else {
      return toString(s.functionType, format() << s.index, s.moduleName, colorize);
    }
  case EXTERN_GLOBAL:
    return "GLOBAL";
  case EXTERN_TABLE:
    return "TABLE";
  case EXTERN_MEMORY:
    return format() << "MEMORY SIZE=" << s.memorySize;
  }
  return String();
}

WebAssembly::FunctionType WebAssembly::getFunctionType(unsigned int id)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getFunctionType(id);
}

void WebAssembly::registerFunction(WASMFunction func, void* context,
                                   const FunctionType& type, const String& name, const String& module)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->registerFunctionImpl(func, context, type, name, module);
}

bool WebAssembly::loadFile(const String& path)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  
  Allocator<uint8> buffer;
  {
    // fout << "WebAssembly::load(): " << path << ENDL;
    File file(path, File::READ, 0);
    if (file.getSize() > (128 * 1024 * 1024)) {
      return false;
    }
    buffer.setSize(file.getSize());
    file.read(buffer.getElements(), static_cast<unsigned int>(file.getSize()));
  }
  return load(buffer.getElements(), buffer.getSize());
}

bool WebAssembly::isValid(const uint8* wasm, MemorySize size)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->isValid(wasm, size);
}

bool WebAssembly::load(const uint8* wasm, MemorySize size)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->load(wasm, size);
}

WebAssembly::Format WebAssembly::getFormat(const String& bytes)
{
  if (bytes.getLength() >= 4) {
    static const char MAGIC[4] = {0x00, 0x61, 0x73, 0x6d};
    bool gotMagic = true;
    for (MemorySize i = 0; i < getArraySize(MAGIC); ++i) {
      if (bytes[i] != MAGIC[i]) {
        gotMagic = false;
        break;
      }
    }
    if (gotMagic) {
      return FORMAT_WASM;
    }
  }
 
  if (bytes.getLength() > 1) {
    if (bytes[0] == '(') { // TAG: can we use startsWith "(module"
      return FORMAT_WAT;
    }
  }

  return FORMAT_UNSPECIFIED;
}

bool WebAssembly::loadAny(const String& bytes)
{
  Format format = getFormat(bytes);
  switch (format) {
  case FORMAT_WASM:
    return load(bytes);
  case FORMAT_WAT:
    return load(convertWATToWASM(bytes));
  default:
    return false;
  }
}

void WebAssembly::setArguments(const Array<String>& arguments)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->setArguments(arguments);
}

void WebAssembly::setEnvironment(const Map<String, String>& environment)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->setEnvironment(environment);
}

void WebAssembly::setMountedFolders(const Map<String, String>& folders)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->setMountedFolders(folders);
}

void WebAssembly::setUseProfiler(bool useProfiler)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->setUseProfiler(useProfiler);
}

WebAssembly::WASMABI WebAssembly::getWASMABI() const
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return WASM32; // TAG: add support
}

bool WebAssembly::makeInstance(bool fake)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->makeInstance(fake);
}

bool WebAssembly::makeWASIInstance(InputStream* _stdin, OutputStream* _stdout, OutputStream* _stderr)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->makeWASIInstance(_stdin, _stdout, _stderr);
}

ConstSpan<uint8> WebAssembly::getMemory() const
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getMemory();
}

Span<uint8> WebAssembly::getMemory()
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getMemory();
}

String WebAssembly::getString(MemorySize slot, MemorySize size) const
{
  ConstSpan<uint8> span = getMemory();
  const MemorySize end = slot + size;
  if ((slot >= span.getSize()) ||
      (end < slot) || // overflow case
      (end >= span.getSize())) {
    _throw WebAssemblyException("Accessing out of memory scope.");
  }
  return String(reinterpret_cast<const char*>(span.begin()) + slot, size);
}

Array<WebAssembly::Symbol> WebAssembly::getImports()
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getImports();
}

Array<WebAssembly::Symbol> WebAssembly::getExports()
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getExports();
}

MemorySize WebAssembly::getNumberOfImports() const
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getNumberOfImports();
}

MemorySize WebAssembly::getNumberOfExports() const
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getNumberOfExports();
}

WebAssembly::Symbol WebAssembly::getSymbol(MemorySize index)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getSymbol(index);
}

WebAssembly::Symbol WebAssembly::getSymbol(const String& id)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getSymbol(id);
}

void WebAssembly::setUseLog(bool useLog)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  handle->setUseLog(useLog);
}

MemorySize WebAssembly::getFunctionIndex(const String& id) const
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getFunctionIndex(id);
}

void WebAssembly::callEntry()
{
  Profiler::Task profile("WebAssembly::call()", "WASM");
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->callEntry();
}

void WebAssembly::call(unsigned int id, const WASMValue* arguments, WASMValue* results)
{
  Profiler::Task profile("WebAssembly::call()", "WASM");
  auto handle = this->handle.cast<WebAssembly::Handle>();
  handle->call(id, arguments, results);
}

AnyValue WebAssembly::call(const String& id, const AnyValue* arguments, MemorySize argumentsSize)
{
  Profiler::Task profile("WebAssembly::call()", "WASM");
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->call(handle->getFunctionIndex(id), arguments, argumentsSize);
}

AnyValue WebAssembly::call(const String& id, const Array<AnyValue>& arguments)
{
  Profiler::Task profile("WebAssembly::call()", "WASM");
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->call(handle->getFunctionIndex(id), arguments.getFirstReference(), arguments.getSize());
}

AnyValue WebAssembly::call(unsigned int id, const Array<AnyValue>& arguments)
{
  Profiler::Task profile("WebAssembly::call()", "WASM");
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->call(id, arguments.getFirstReference(), arguments.getSize());
}

WebAssembly::~WebAssembly()
{
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const WebAssembly::Symbol& value)
{
  FormatOutputStream::PushContext push(stream);
  return stream << WebAssembly::toString(value);
}

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)

typedef uint32 WASM_MemorySize;

inline MemorySize getMemorySlot(const wasm_val_t& v)
{
  if (v.kind != WASM_I32) {
    _throw WebAssembly::WebAssemblyException("Not a memory pointer.");
  }
  return v.of.i32;
}

// TAG: need template to handle automatic mapping
own wasm_trap_t* bindToImplementation(void* env, const wasm_val_t args[], wasm_val_t results[]) noexcept
{
  Profiler::Task profile("WebAssembly::callback()", "WASM");
  WebAssembly::Handle::FunctionContext* context = reinterpret_cast<WebAssembly::Handle::FunctionContext*>(env);
  if (!context) {
    return context->getTrap("Missing context.");
  }
  context->invocations++;
  auto& stream = fout;
  try {
    if (context->module) {
      stream << context->module << "!";
    }
    stream << context->name << getValuesAsString(args, context->argSize);
    // TAG: is types correct for input or do we need to set explicitly
    stream << " -> " << getValuesAsString(results, context->resultSize);
    stream << " INVOKES=" << context->invocations << ENDL;
    // TAG: map WASM types to native types for function
    for (MemorySize i = 0; i < context->resultSize; ++i) {
      wasm_val_t& v = results[i];
      BASSERT(v.kind == WASM_I32); // TAG: temp test
      v.kind = WASM_I32;
      v.of.i32 = 0;
    }
  } catch (Exception& e) {
    return context->getTrap(e);
  } catch (...) {
    return context->getTrap("Unknown exception throw.");
  }
  return nullptr;
}

template<typename TYPE>
inline void toWASM(wasm_val_t& dest, TYPE v)
{
  _throw WebAssembly::WebAssemblyException("Unsupported type.");
}

#if 0
template<>
inline void toWASM<void>(wasm_val_t& dest, void v)
{
  dest.kind = WASM_I32;
  dest.of.i32 = 0;
}
#endif

template<>
inline void toWASM<bool>(wasm_val_t& dest, bool v)
{
  dest.kind = WASM_I32;
  dest.of.i32 = v;
}

template<>
inline void toWASM<short>(wasm_val_t& dest, short v)
{
  dest.kind = WASM_I32;
  dest.of.i32 = v;
}

template<>
inline void toWASM<unsigned short>(wasm_val_t& dest, unsigned short v)
{
  dest.kind = WASM_I32;
  dest.of.i32 = v;
}

template<>
inline void toWASM<int>(wasm_val_t& dest, int v)
{
  dest.kind = WASM_I32;
  dest.of.i32 = v;
}

template<>
inline void toWASM<unsigned int>(wasm_val_t& dest, unsigned int v)
{
  dest.kind = WASM_I32;
  dest.of.i32 = v;
}

template<>
inline void toWASM<int64>(wasm_val_t& dest, int64 v)
{
  dest.kind = WASM_I64;
  dest.of.i64 = v;
}

template<>
inline void toWASM<uint64>(wasm_val_t& dest, uint64 v)
{
  dest.kind = WASM_I64;
  dest.of.i64 = v;
}

  template<>
  inline void toWASM<float>(wasm_val_t& dest, float v)
  {
    dest.kind = WASM_I64;
    dest.of.i64 = v;
  }

  template<>
  inline void toWASM<double>(wasm_val_t& dest, double v)
  {
    dest.kind = WASM_F64;
    dest.of.f64 = v;
  }
  template<>
  inline void toWASM<long double>(wasm_val_t& dest, long double v)
  {
    dest.kind = WASM_F64;
    dest.of.f64 = static_cast<double>(v);
  }

template<typename TYPE>
inline TYPE toNative(const wasm_val_t& v)
{
  _throw WebAssembly::WebAssemblyException("Unsupported type.");
}

template<>
inline bool toNative<bool>(const wasm_val_t& v)
{
  if (v.kind != WASM_I32) {
    _throw WebAssembly::WebAssemblyException("Unsupported type.");
  }
  return v.of.i32 != 0;
}

template<>
inline short toNative<short>(const wasm_val_t& v)
{
  if (v.kind != WASM_I32) {
    _throw WebAssembly::WebAssemblyException("Unsupported type.");
  }
  if (v.of.i32 < PrimitiveTraits<short>::MINIMUM) {
    _throw WebAssembly::WebAssemblyException("Out of range value.");
  }
  if (v.of.i32 > PrimitiveTraits<short>::MAXIMUM) {
    _throw WebAssembly::WebAssemblyException("Out of range value.");
  }
  return static_cast<short>(v.of.i32);
}

template<>
inline unsigned short toNative<unsigned short>(const wasm_val_t& v)
{
  if (v.kind != WASM_I32) {
    _throw WebAssembly::WebAssemblyException("Unsupported type.");
  }
  if (v.of.i32 < PrimitiveTraits<unsigned short>::MINIMUM) {
    _throw WebAssembly::WebAssemblyException("Out of range value.");
  }
  if (v.of.i32 > PrimitiveTraits<unsigned short>::MAXIMUM) {
    _throw WebAssembly::WebAssemblyException("Out of range value.");
  }
  return static_cast<unsigned short>(v.of.i32);
}

template<>
inline int toNative<int>(const wasm_val_t& v)
{
  if (v.kind != WASM_I32) {
    _throw WebAssembly::WebAssemblyException("Unsupported type.");
  }
  return v.of.i32;
}

template<>
inline unsigned int toNative<unsigned int>(const wasm_val_t& v)
{
  if (v.kind != WASM_I32) {
    _throw WebAssembly::WebAssemblyException("Unsupported type.");
  }
  return static_cast<unsigned int>(v.of.i32);
}

template<>
inline int64 toNative<int64>(const wasm_val_t& v)
{
  if (v.kind != WASM_I64) {
    _throw WebAssembly::WebAssemblyException("Unsupported type.");
  }
  return v.of.i64;
}

template<>
inline uint64 toNative<uint64>(const wasm_val_t& v)
{
  if (v.kind != WASM_I64) {
    _throw WebAssembly::WebAssemblyException("Unsupported type.");
  }
  return static_cast<uint64>(v.of.i64);
}

template<typename TYPE>
inline TYPE toNativePointer(const wasm_val_t& v, WebAssembly::Handle::FunctionContext* context)
{
  if (v.kind != WASM_I32) {
    _throw WebAssembly::WebAssemblyException("Unsupported type.");
  }
  WASM_MemorySize slot = getMemorySlot(v);
  return reinterpret_cast<TYPE>((void*)context->read(slot, sizeof(typename std::remove_pointer<TYPE>::type)));
}

/** Converts to native value. */
template<typename TYPE>
class NativeValue {
private:
  
  const wasm_val_t& v;
  WebAssembly::Handle::FunctionContext* context = nullptr;
public:
  
  inline NativeValue(const wasm_val_t& _v, WebAssembly::Handle::FunctionContext* _context)
    : v(_v), context(_context)
  {
  }
  
  inline operator TYPE() const
  {
    return toNative<TYPE>(v);
  }
};

/** Converts to native value. */
template<typename TYPE>
class NativeValue<TYPE*> {
private:
  
  const wasm_val_t& v;
  WebAssembly::Handle::FunctionContext* context = nullptr;
public:
  
  inline NativeValue(const wasm_val_t& _v, WebAssembly::Handle::FunctionContext* _context)
    : v(_v), context(_context)
  {
  }
  
  inline operator TYPE*() const
  {
    return toNativePointer<TYPE*>(v, context);
  }
};

class WASIContext {
private:

  WebAssembly::Handle::FunctionContext* context = nullptr;
  const wasm_val_t* args = nullptr;
  MemorySize i = 0;
  
  class FD {
  public:
  };
  
  Array<FD> fds;
  
  inline void store(uint8* dest, const uint8* destEnd, const uint8* src, const uint8* srcEnd)
  {
    if ((destEnd - dest) <= (srcEnd - src)) {
      while (dest != destEnd) {
        *dest++ = *src++;
      }
    } else {
      while (src != srcEnd) {
        *dest++ = *src++;
      }
    }
  }
public:
  
  // TAG: toNative must handle both WASM32 and WASM64
  
  inline WASIContext(WebAssembly::Handle::FunctionContext* _context, const wasm_val_t* _args) noexcept
    : context(_context), args(_args)
  {
  }

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME_WASI)
  __wasi_errno_t args_get(uint8_t** argv, uint8_t* argv_buf)
  {
    if (!argv || !argv_buf) {
      return __WASI_ERRNO_INVAL;
    }
    return __WASI_ERRNO_SUCCESS;
  }

  __wasi_errno_t args_sizes_get(__wasi_size_t *argc, __wasi_size_t *argv_buf_size)
  {
    if (!argc || !argv_buf_size) {
      return __WASI_ERRNO_INVAL;
    }
    *argc = 0;
    return __WASI_ERRNO_SUCCESS;
  }

  __wasi_errno_t environ_sizes_get(__wasi_size_t *argc, __wasi_size_t *argv_buf_size)
  {
    if (!argc || !argv_buf_size) {
      return __WASI_ERRNO_INVAL;
    }
    *argc = 0;
    return __WASI_ERRNO_SUCCESS;
  }
  
  __wasi_errno_t clock_res_get(__wasi_clockid_t id, __wasi_timestamp_t *resolution)
  {
    if (!resolution) {
      return __WASI_ERRNO_INVAL;
    }
    switch (id) {
    case __WASI_CLOCKID_REALTIME:
      *resolution = 1000;
      return __WASI_ERRNO_SUCCESS;
    case __WASI_CLOCKID_MONOTONIC:
      *resolution = 1000;
      return __WASI_ERRNO_SUCCESS;
    // case __WASI_CLOCKID_THREAD_CPUTIME_ID:
    }
    return __WASI_ERRNO_INVAL;
  }
  
  __wasi_errno_t clock_time_get(__wasi_clockid_t id, __wasi_timestamp_t precision, __wasi_timestamp_t *time)
  {
    if (!time) {
      return __WASI_ERRNO_INVAL;
    }
    switch (id) {
    case __WASI_CLOCKID_REALTIME:
      *time = Timer::getRealNow() * 1000;
      return __WASI_ERRNO_SUCCESS;
    case __WASI_CLOCKID_MONOTONIC:
      *time = Timer::getNow() * 1000;
      return __WASI_ERRNO_SUCCESS;
    // case __WASI_CLOCKID_THREAD_CPUTIME_ID:
    }
    return __WASI_ERRNO_INVAL;
  }
  
  __wasi_errno_t fd_prestat_get(__wasi_fd_t fd, __wasi_prestat_t* prestat) noexcept
  {
    if (false) {
      return __WASI_ERRNO_BADF;
    }
    if (!prestat) {
      return __WASI_ERRNO_INVAL;
    }
    prestat->tag = __WASI_PREOPENTYPE_DIR;
    prestat->u.dir.pr_name_len = 1;
    if (fd > 10) {
      return __WASI_ERRNO_NOENT;
    }
    return __WASI_ERRNO_SUCCESS; // __WASI_ERRNO_NOENT; // __WASI_ERRNO_NOTDIR;
  }
  
  __wasi_errno_t fd_prestat_dir_name(__wasi_fd_t fd, uint8_t * _path, __wasi_size_t path_len)
  {
    if (false) {
      return __WASI_ERRNO_BADF;
    }
    if (!_path) {
      return __WASI_ERRNO_INVAL;
    }
    const char* path = ".";
    store(_path, _path + path_len, (const uint8*)path, (const uint8*)path + 2);
    return __WASI_ERRNO_SUCCESS;
  }
  
  __wasi_errno_t fd_sync(__wasi_fd_t fd)
  {
    if (false) {
      return __WASI_ERRNO_BADF;
    }
    return __WASI_ERRNO_SUCCESS;
  }
  
  __wasi_errno_t fd_advise(__wasi_fd_t fd, __wasi_filesize_t offset, __wasi_filesize_t len, __wasi_advice_t advice)
  {
    if (false) {
      return __WASI_ERRNO_BADF;
    }
    return __WASI_ERRNO_SUCCESS;
  }
  
  __wasi_errno_t fd_allocate(__wasi_fd_t fd, __wasi_filesize_t offset, __wasi_filesize_t len)
  {
    if (false) {
      return __WASI_ERRNO_BADF;
    }
    return __WASI_ERRNO_SUCCESS;
  }
  
  __wasi_errno_t fd_fdstat_set_rights(__wasi_fd_t fd, __wasi_rights_t fs_rights_base, __wasi_rights_t fs_rights_inheriting)
  {
    if (false) {
      return __WASI_ERRNO_BADF;
    }
    return __WASI_ERRNO_SUCCESS;
  }

  __wasi_errno_t fd_tell(__wasi_fd_t fd, __wasi_filesize_t *offset)
  {
    if (false) {
      return __WASI_ERRNO_BADF;
    }
    if (!offset) {
      return __WASI_ERRNO_INVAL;
    }
    return __WASI_ERRNO_SUCCESS;
  }

  __wasi_errno_t fd_seek(__wasi_fd_t fd, __wasi_filedelta_t offset, __wasi_whence_t whence, __wasi_filesize_t *newoffset) noexcept
  {
    if (false) {
      return __WASI_ERRNO_BADF;
    }
    if (!newoffset) {
      return __WASI_ERRNO_INVAL;
    }
    return __WASI_ERRNO_SUCCESS;
  }
  
  __wasi_errno_t fd_write(__wasi_fd_t fd, const __wasi_ciovec_t *iovs, __wasi_size_t iovs_len, __wasi_size_t *nwritten) noexcept
  {
    if (false) {
      return __WASI_ERRNO_BADF;
    }
    if (!iovs || !nwritten) {
      return __WASI_ERRNO_INVAL;
    }
    return __WASI_ERRNO_SUCCESS;
  }
  
  __wasi_errno_t fd_read(__wasi_fd_t fd, const __wasi_ciovec_t *iovs, __wasi_size_t iovs_len, __wasi_size_t *nread) noexcept
  {
    if (false) {
      return __WASI_ERRNO_BADF;
    }
    if (!iovs || !nread) {
      return __WASI_ERRNO_INVAL;
    }
    return __WASI_ERRNO_SUCCESS;
  }
  
  __wasi_errno_t fd_datasync(__wasi_fd_t fd)
  {
    if (false) {
      return __WASI_ERRNO_BADF;
    }
    return __WASI_ERRNO_SUCCESS;
  }
  
  __wasi_errno_t fd_fdstat_set_flags(__wasi_fd_t fd, __wasi_fdflags_t flags)
  {
    if (false) {
      return __WASI_ERRNO_BADF;
    }
    return __WASI_ERRNO_SUCCESS;
  }
  
  __wasi_errno_t fd_close(__wasi_fd_t fd)
  {
    if (false) {
      return __WASI_ERRNO_BADF;
    }
    return __WASI_ERRNO_SUCCESS;
  }
  
  __wasi_errno_t fd_fdstat_get(__wasi_fd_t fd, __wasi_fdstat_t *stat)
  {
    if (false) {
      return __WASI_ERRNO_BADF;
    }
    if (!stat) {
      return __WASI_ERRNO_INVAL;
    }
    stat->fs_filetype = __WASI_FILETYPE_DIRECTORY;
    stat->fs_flags = 0;
    stat->fs_rights_base = 0xffffffff;
    stat->fs_rights_inheriting = 0;
    return __WASI_ERRNO_SUCCESS;
  }
  
  void proc_exit(int exitcode)
  {
    // TAG: raise trap
    ::exit(exitcode);
  }
#endif
  
  /** Returns the argument. */
  inline const wasm_val_t& getArg()
  {
    /*
    if (!(i < xxx) {
      _throw WebAsseblyException("Missing argument.");
    }
    */
    return args[i++];
  }
};

template<typename... ARGS>
own wasm_trap_t* WASIImpl_forward(void (WASIContext::*function)(ARGS...),
                                  void* env, const wasm_val_t args[], wasm_val_t results[]) noexcept
{
  WebAssembly::Handle::FunctionContext* context = reinterpret_cast<WebAssembly::Handle::FunctionContext*>(env);
  if (!context) {
    return context->getTrap("Missing context.");
  }
  Profiler::Task profile((context->module + "!" + context->name).native(), "WASM");
  context->invocations++;
  try {
    if (sizeof...(ARGS) != context->argSize) { // TAG: allow support for string literal
      _throw WebAssembly::WebAssemblyException("Mismatching arguments.");
    }
    if (0 != context->resultSize) {
      _throw WebAssembly::WebAssemblyException("Mismatching result.");
    }
    WASIContext wasi(context, args);
    (wasi.*function)(NativeValue<ARGS>(wasi.getArg(), context)...);
  } catch (Exception& e) {
    auto trap = context->getTrap(e);
    if (context->handle->getUseLog()) {
      context->handle->writeLog(context->name, false, args, context->argSize, trap);
    }
    return trap;
  } catch (...) {
    auto trap = context->getTrap("Unknown exception throw.");
    if (context->handle->getUseLog()) {
      context->handle->writeLog(context->name, false, args, context->argSize, trap);
    }
    return trap;
  }
  if (context->handle->getUseLog()) {
    context->handle->writeLog(context->name, false, args, context->argSize, results, context->resultSize);
  }
  return nullptr;
}

template<typename RESULT, typename... ARGS>
own wasm_trap_t* WASIImpl_forward(RESULT (WASIContext::*function)(ARGS...),
                                  void* env, const wasm_val_t args[], wasm_val_t results[]) noexcept
{
  WebAssembly::Handle::FunctionContext* context = reinterpret_cast<WebAssembly::Handle::FunctionContext*>(env);
  if (!context) {
    return context->getTrap("Missing context.");
  }
  Profiler::Task profile((context->module + "!" + context->name).native(), "WASM");
  context->invocations++;
  try {
    if (sizeof...(ARGS) != context->argSize) { // TAG: allow support for string literal
      _throw WebAssembly::WebAssemblyException("Mismatching arguments.");
    }
    if (1 != context->resultSize) {
      _throw WebAssembly::WebAssemblyException("Mismatching result.");
    }
    WASIContext wasi(context, args);
    toWASM(results[0], (wasi.*function)(NativeValue<ARGS>(wasi.getArg(), context)...));
  } catch (Exception& e) {
    auto trap = context->getTrap(e);
    if (context->handle->getUseLog()) {
      context->handle->writeLog(context->name, false, args, context->argSize, trap);
    }
    return trap;
  } catch (...) {
    auto trap = context->getTrap("Unknown exception throw.");
    if (context->handle->getUseLog()) {
      context->handle->writeLog(context->name, false, args, context->argSize, trap);
    }
    return trap;
  }
  if (context->handle->getUseLog()) {
    context->handle->writeLog(context->name, false, args, context->argSize, results, context->resultSize);
  }
  return nullptr;
}

#define IMPL_WASI(NAME) \
own wasm_trap_t* WASIImpl_##NAME(void* env, const wasm_val_t args[], wasm_val_t results[]) noexcept \
{ \
  return WASIImpl_forward(&WASIContext::NAME, env, args, results); \
}

// cat base/platforms/backend/wasi_api.h | sed -n '/__wasi_errno_t .*/p'

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME_WASI)
  IMPL_WASI(proc_exit)
  IMPL_WASI(args_sizes_get)
  IMPL_WASI(args_get)
  IMPL_WASI(fd_prestat_dir_name)
  IMPL_WASI(fd_prestat_get)
  IMPL_WASI(fd_fdstat_get)
  IMPL_WASI(fd_read)
  IMPL_WASI(fd_write)
  IMPL_WASI(fd_seek)
  IMPL_WASI(fd_close)
#endif

#define REGISTER_WASI(NAME) \
  registerWASIFunction(&WASIContext::NAME, dest, src, WASIImpl_##NAME, "wasi_unstable", _COM_AZURE_DEV__BASE__STRINGIFY(NAME));

void WebAssembly::Handle::registerWASIImports(const wasm_extern_t** dest, wasm_importtype_vec_t src)
{
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME_WASI)
  REGISTER_WASI(proc_exit);
  REGISTER_WASI(args_sizes_get);
  REGISTER_WASI(args_get);
  REGISTER_WASI(fd_prestat_dir_name);
  REGISTER_WASI(fd_prestat_get);
  REGISTER_WASI(fd_fdstat_get);
  REGISTER_WASI(fd_read);
  REGISTER_WASI(fd_write);
  REGISTER_WASI(fd_seek);
  REGISTER_WASI(fd_close);
#endif
}

own wasm_trap_t* fakeHook(void* env, const wasm_val_t args[], wasm_val_t results[]) noexcept
{
  Profiler::Task profile("WebAssembly::callback()", "WASM");
  WebAssembly::Handle::FunctionContext* context = reinterpret_cast<WebAssembly::Handle::FunctionContext*>(env);
  if (!context) {
    return context->getTrap("Missing context.");
  }
  context->invocations++;
  try {
    for (MemorySize i = 0; i < context->resultSize; ++i) {
      wasm_val_t& v = results[i];
      v.kind = WASM_I32;
      v.of.i32 = 0;
    }
    if (context->handle->getUseLog()) {
      context->handle->writeLog(context->name, false, args, context->argSize, results, context->resultSize);
    }
  } catch (Exception& e) {
    return context->getTrap(e);
  } catch (...) {
    return context->getTrap("Unknown exception throw.");
  }
  return nullptr;
}

own wasm_trap_t* forwardCallback(void* env, const wasm_val_t args[], wasm_val_t _results[]) noexcept
{
  Profiler::Task profile("WebAssembly::callback()", "WASM"); // TAG: add module!name()
  WebAssembly::Handle::FunctionContext* context = reinterpret_cast<WebAssembly::Handle::FunctionContext*>(env);
  if (!context) {
    return context->getTrap("Missing context.");
  }
  context->invocations++;
  try {
    auto handle = context->handle;
    PrimitiveStackArray<WebAssembly::WASMValue> arguments(context->argSize);
    PrimitiveStackArray<WebAssembly::WASMValue> results(context->resultSize);
    for (MemorySize i = 0; i < context->resultSize; ++i) {
      handle->convert(arguments[i], args[i]);
    }
    context->func(context->context, arguments ? arguments : nullptr, results ? results : nullptr);
    for (MemorySize i = 0; i < context->resultSize; ++i) {
      handle->convert(_results[i], results[i]);
    }
    if (handle->getUseLog()) {
      handle->writeLog(context->name, false, args, context->argSize, _results, context->resultSize);
    }
  } catch (Exception& e) {
    return context->getTrap(e);
  } catch (...) {
    return context->getTrap("Unknown exception throw.");
  }
  return nullptr;
}
#endif

#if 0
// Array<VirtualFileSystem*> getMounts();
// void mountFileSystem(const String& path);
// add directory access by paths
// consider ioctl/syskern appoach for all resource access - need security layer
// auto detect dead-lock / timeout
// probe features supported for WASM engine
#endif

bool WebAssembly::FunctionType::operator==(const FunctionType& type) const noexcept
{
  return (results == type.results) && (arguments == type.arguments);
}

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
// TAG: wasmtime: get version from header
MODULE_REGISTER_EXPLICIT(_COM_AZURE_DEV__BASE__THIS_MODULE, "dev.wasmtime", "libwasmtime", "0.8", "https://wasmtime.dev/");
#endif

#if defined(_COM_AZURE_DEV__BASE__TESTS) && defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)

class TEST_CLASS(WebAssembly) : public UnitTest {
public:

  TEST_PRIORITY(1000);
  TEST_PROJECT("base/webassembly");
  TEST_TIMEOUT_MS(30 * 1000);

  static void func1(void* context, WebAssembly::WASMValue* arguments, WebAssembly::WASMValue* results)
  {
    fout << "Hello, World!" << ENDL;
    // TAG: forward to native function
  }

  static void hello()
  {
    fout << "Hello, World!" << ENDL;
  }

  static float func2(int arg1, double arg2, uint64 arg3)
  {
    fout << "Hello, World!" << ENDL;
    return -0.0f;
  }

  void run() override
  {
    WebAssembly wasm;
    TEST_ASSERT(wasm.isSupported());
    String description = wasm.getEngine();
    TEST_ASSERT(description);
    
    const char* addWAT = R"WAT((module
      (type (;0;) (func (param i32 i32) (result i32)))
      (func $add (type 0) (param i32 i32) (result i32)
        get_local 1
        get_local 0
        i32.add)
      (memory 1)
      (export "memory" (memory 0))
      (export "add" (func $add))
    ))WAT";

    TEST_ASSERT(wasm.getFormat(addWAT) == WebAssembly::FORMAT_WAT);
    String binary = WebAssembly::convertWATToWASM(addWAT);
    if (TEST_INLINE_ASSERT(wasm.isValid(binary))) {
      TEST_ASSERT(wasm.load(binary));
      wasm.registerFunction(func1, nullptr, WebAssembly::FunctionType(), "hello", "");
      // wasm.registerFunction(hello, nullptr, WebAssembly::FunctionType(), "hello", "");
      TEST_ASSERT(wasm.makeInstance());
      auto imports = wasm.getImports();
      TEST_ASSERT(!imports);

      auto exports = wasm.getExports();
      TEST_ASSERT(exports.getSize() == 2);

      int result = wasm.invoke<int>("add", -1234, 4567);
      TEST_ASSERT(result == 3333);
    }
    
    const char* importWAT = R"WAT((module
      (import "internal" "write" (func $write (param i32 i32) (result i32)))
      (type (;0;) (func (param i32 i32) (result i32)))
      (data (i32.const 8) "Hello World\n")
      (memory 1)
      (export "memory" (memory 0))
    ))WAT";

#if 0
    (func $main (export "_start")
      (i32.store (i32.const 0) (i32.const 8)) ;; start of string
      (i32.store (i32.const 4) (i32.const 12)) ;; length of string
      ;;(call $write
      ;;  (i32.const 0)
      ;;  (i32.const 1)
      ;;)
      drop ;; Discard result
    )
#endif
    
    {
      WebAssembly wasm2;
      String binary = WebAssembly::convertWATToWASM(importWAT);
      if (TEST_INLINE_ASSERT(wasm2.isValid(binary))) {
        TEST_ASSERT(wasm2.load(binary));

        auto imports = wasm2.getImports();
        TEST_ASSERT(imports.getSize() == 1);
        // fout << imports << ENDL;
      
        auto ft = WebAssembly::getFunctionType(func2);
        // fout << ft << ENDL;

        // TAG: get function type from native
        wasm2.registerFunction(func1, nullptr, WebAssembly::FunctionType(), "write", "internal");
        TEST_ASSERT(wasm2.makeInstance());
        auto exports = wasm2.getExports();
        TEST_ASSERT(exports.getSize() == 1);
      }
    }
  }
};

TEST_REGISTER(WebAssembly);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
