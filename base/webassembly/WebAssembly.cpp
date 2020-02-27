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
#  include <wasm.h>
#  define own
#endif

// TAG: use runtime linking like opengl
// TAG: add support for multiple engines - but adding Engine class

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME_WASI)
__wasi_errno_t __wasi_fd_prestat_get(__wasi_fd_t fd, __wasi_prestat_t *buf)
{
  return 0;
}

own wasm_trap_t* __wasiimpl_fd_prestat_get(void* env, const wasm_val_t args[], wasm_val_t results[]) noexcept;
#endif

#if 0
// https://github.com/WebAssembly/WASI/blob/master/phases/snapshot/witx/typenames.witx
// https://github.com/bytecodealliance/wasmtime/blob/master/docs/WASI-api.md
// https://github.com/CraneStation/wasi-libc/blob/master/libc-bottom-half/headers/public/wasi/api.h
// TAG: add security manifest
// TAG: lookup each API - from imports - add auto generate manifest - allow to run with disapproved security slots

int __wasi_fd_close(__wasi_fd_t fd)
{
  return -1;
}
#endif

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
      sos << "i32: " << arg.of.i32 << " = " << HEX << arg.of.i32;
      break;
    case WASM_I64:
      sos << "i64: " << arg.of.i64 << " = " << HEX << arg.of.i64;
      break;
    case WASM_F32:
      sos << "f32: " << arg.of.f32 << " = " << FHEX << arg.of.f32;
      break;
    case WASM_F64:
      sos << "f64: " << arg.of.f64 << " = " << FHEX << arg.of.f64;
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
// TAG: add support for forwarding hook to allow tracking
own wasm_trap_t* fakeHook(void* env, const wasm_val_t args[], wasm_val_t results[]) noexcept;

own wasm_trap_t* hook(void* env, const wasm_val_t args[], wasm_val_t results[]) noexcept;

own wasm_trap_t* hello(const wasm_val_t args[], wasm_val_t results[]) noexcept;
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
  case TYPE_STRING:
    return "STRING";
  default:
    BASSERT(!"Not supported.");
    return "UNSPECIFIED";
  }
}

WebAssembly::Function::Function()
{
}

WebAssembly::Function::~Function()
{
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
    String fullname;
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
  own wasm_module_t* module = nullptr;
  own wasm_extern_vec_t exports = {0};
  wasm_memory_t* memory = nullptr;
#endif
  Array<String> exportNames;
  MemorySize maximumMemoryUsage = 0;

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
  
    void* func = nullptr;
    WebAssembly::Type result = TYPE_UNSPECIFIED;
    Array<WebAssembly::Type> arguments;
    String name;
    bool nothrow = false;
  };

  /** Registered import functions. */
  Array<ImportFunction> imports;
  /** Function contexts. */
  Array<FunctionContext*> functionContexts;
public:

  void registerFunctionImpl(
    void* func, Type result, const Type* args, unsigned int argsSize, const String& name, bool nothrow)
  {
    if (!func) {
      _throw NullPointer();
    }
    if (argsSize != 0) {
      if (!args) {
        _throw NullPointer();
      }
    }

    ImportFunction f;
    f.result = result;
    f.arguments.setSize(argsSize);
    // copy(&f.arguments[0], args, args + argsSize);
    for (MemorySize i = 0; i < argsSize; ++i) {
      f.arguments[i] = args[i];
    }
    f.name = name;
    f.func = func;
    f.nothrow = nothrow;
    imports.append(f);
  }
  
  Handle()
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    static Engine* _engine = new Engine();

    engine = _engine->engine;
    if (!engine) {
      _throw ResourceException("Failed to initialize WASM.");
    }
    store = wasm_store_new(engine);
    if (!store) {
      _throw ResourceException("Failed to initialize WASM.");
    }
#endif
  }
  
  String getEngine()
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    return String("Wasmtime 0.8"); // TAG: wasmtime: get version from header
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
  [[noreturn]] void onTrap(own wasm_trap_t* trap)
  {
    if (trap) {
      own wasm_message_t message;
      wasm_trap_message(trap, &message);
      if (message.size) {
        // TAG: wasmtime: why do we get an extra null-terminator added - https://github.com/bytecodealliance/wasmtime/issues/994
        while ((message.size > 0) && !message.data[message.size - 1]) {
          --message.size;
        }
        // fout << MemoryDump((const uint8*)message.data, message.size) << ENDL;
        String msg(reinterpret_cast<const char*>(message.data), message.size);
        _throw WebAssemblyException(msg.native());
      } else {
        _throw WebAssemblyException("Trap was raised.");
      }
    }
    _throw WebAssemblyException("Unsupported trap.");
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
          if (moduleName) {
            functionContext->fullname = moduleName + "!" + name;
          } else {
            functionContext->fullname = name;
          }
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
          if (moduleName) {
            functionContext->fullname = moduleName + "!" + name;
          } else {
            functionContext->fullname = name;
          }
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
    
    if (wasi) {
      registerWASIImports(imports.begin(), _imports);
    }

    if (fake) {
      registerFakeImports(imports.begin(), _imports);
    }
    
#if 0
    // TAG: can we dynamically register a function
    #if 0
      wasm_valtype_t* ps[n] = {p1, ...};
      wasm_valtype_t* rs[m] = {r1, ...};
      wasm_valtype_vec_t params, results;
      wasm_valtype_vec_new(&params, getArraySize(ps), ps);
      wasm_valtype_vec_new(&results, getArraySize(rs), rs);
      own wasm_functype_t* = wasm_functype_new(&params, &results);
    #endif
    
    if (!fake) {
      own wasm_functype_t* hello_type = wasm_functype_new_0_0();
      FunctionContext* functionContext = new FunctionContext(this, 0, 0);
      functionContexts.append(functionContext);
      own wasm_func_t* hook_func = wasm_func_new_with_env(store, hello_type, hook, functionContext, nullptr);
      // own wasm_func_t* hello_func = wasm_func_new(store, hello_type, hello);
      wasm_functype_delete(hello_type);
      // TAG: add template to bind global function/static function
      for (MemorySize i = 0; i < _imports.size; ++i) {
        imports[i] = wasm_func_as_extern(hook_func);
      }
    }
#endif

    // TAG: add support for WASI instance - waiting for wasi.h support
    // TAG: add support for wasm text from memory
    own wasm_trap_t* trap = nullptr;
    instance = wasm_instance_new(store, module, imports, &trap);
    if (trap) {
      onTrap(trap);
    }
    // wasm_func_delete(hello_func);
    if (!instance) {
      return false;
    }
    
    wasm_instance_exports(instance, &exports);
    
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
  void registerWASIFunction(RESULT (*func)(ARGS...),
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

#define REGISTER_WASI(NAME) \
  registerWASIFunction(__wasi_##NAME, dest, src, __wasiimpl_##NAME, "wasi_unstable", _COM_AZURE_DEV__BASE__STRINGIFY(NAME));

  void registerWASIImports(const wasm_extern_t** dest, wasm_importtype_vec_t src)
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME_WASI)
    REGISTER_WASI(fd_prestat_get);
#endif
  }
#endif
  
  bool makeWASIInstance(InputStream* _stdin, OutputStream* _stdout, OutputStream* _stderr)
  {
    return makeInstance(true, true);
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
      s.functionType = getFunctionType(wasm_func_type(f));
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
      // TAG: should we add isIndex()? exportnames.isIndex(index), maybe getAt(index, default)
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

  AnyValue call(MemorySize id, const AnyValue* arguments, MemorySize argumentsSize)
  {
    String name = exportNames[id];
    if (!name) {
      name = String(format() << id);
    }
    writeFunction(fout, name, arguments, argumentsSize);

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
      convert(args, args + size, arguments, argumentsSize);
      // own wasm_functype_t* type = wasm_func_type(func);
      if (own wasm_trap_t* trap = wasm_func_call(func, args, results)) {
        onTrap(trap);
      }
      return convertToAnyValue(results, resultSize);
    }
#endif
    _throw WebAssemblyException("No such function.");
  }
  
  AnyValue call(WebAssembly::Function f, const AnyValue* arguments, MemorySize argumentsSize)
  {
    _throw WebAssemblyException("Invalid function");
  }
  
  void callEntry()
  {
    call(0, nullptr, 0);
  }

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
  wasm_val_t* convert(wasm_val_t* dest, const AnyValue& value)
  {
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
  }
  
   void convert(wasm_val_t* dest, const wasm_val_t* end, const AnyValue* arguments, MemorySize argumentsSize)
  {
    for (MemorySize i = 0; i < argumentsSize; ++i) {
      if (dest == end) {
        _throw WebAssemblyException("Arguments mismatch.");
      }
      const AnyValue& a = arguments[i];
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
  
  AnyValue call(const String& id, const AnyValue* arguments, MemorySize argumentsSize)
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
    
    return call(index, arguments, argumentsSize);
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

String WebAssembly::convertToText(const String& bytes)
{
  _throw WebAssemblyException("Unsupported conversion to WAT.");
}

String WebAssembly::convertToWASM(const String& text)
{
  _throw WebAssemblyException("Unsupported conversion to WASM.");
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
  if (s.name) {
    return toString(s.functionType, s.name, s.moduleName, colorize);
  } else {
    return toString(s.functionType, format() << s.index, s.moduleName, colorize);
  }
}

WebAssembly::FunctionType WebAssembly::getFunctionType(unsigned int id)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getFunctionType(id);
}

void WebAssembly::registerFunctionImpl(
  void* func, Type result, const Type* args, unsigned int argsSize, const String& name, bool nothrow)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->registerFunctionImpl(func, result, args, argsSize, name, nothrow);
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

void WebAssembly::setArguments(const Array<String>& arguments)
{
  _throw WebAssemblyException("Arguments not supported.");
}

void WebAssembly::setEnvironment(const Map<String, String>& environment)
{
  _throw WebAssemblyException("Environment not supported.");
}

void WebAssembly::setUseProfiler(bool useProfiler)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->setUseProfiler(useProfiler);
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

void WebAssembly::callEntry()
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->callEntry();
}

AnyValue WebAssembly::call(const String& id, const AnyValue* arguments, MemorySize argumentsSize)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->call(id, arguments, argumentsSize);
}

AnyValue WebAssembly::call(const String& id, const Array<AnyValue>& arguments)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  // TAG: add Array::getFirst()
  return handle->call(id, arguments ? &arguments[0] : nullptr, arguments.getSize());
}

AnyValue WebAssembly::call(unsigned int id, const Array<AnyValue>& arguments)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->call(id, arguments ? &arguments[0] : nullptr, arguments.getSize());
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
  WebAssembly::Handle::FunctionContext* context = reinterpret_cast<WebAssembly::Handle::FunctionContext*>(env);
  if (!context) {
    return context->getTrap("Missing context.");
  }
  context->invocations++;
  auto& stream = fout;
  try {
    stream << context->fullname << getValuesAsString(args, context->argSize);
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

own wasm_trap_t* __wasiimpl_fd_prestat_get(void* env, const wasm_val_t args[], wasm_val_t results[]) noexcept
{
  WebAssembly::Handle::FunctionContext* context = reinterpret_cast<WebAssembly::Handle::FunctionContext*>(env);
  if (!context) {
    return context->getTrap("Missing context.");
  }
  context->invocations++;
  auto& stream = fout;
  try {
    stream << context->fullname << getValuesAsString(args, context->argSize);
    // TAG: is types correct for input or do we need to set explicitly
    stream << " -> " << getValuesAsString(results, context->resultSize);
    stream << " INVOKES=" << context->invocations << ENDL;
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME_WASI)
    int32 fd = toNative<int32>(args[0]);
    __wasi_prestat_t* prestat = toNativePointer<__wasi_prestat_t*>(args[1], context);
    (void)prestat->u.dir.pr_name_len;

    for (MemorySize i = 0; i < context->resultSize; ++i) {
      wasm_val_t& v = results[i];
      v.kind = WASM_I32;
      v.of.i32 = __WASI_ERRNO_INVAL;
    }
#endif
  } catch (Exception& e) {
    return context->getTrap(e);
  } catch (...) {
    return context->getTrap("Unknown exception throw.");
  }
  return nullptr;
}

own wasm_trap_t* fakeHook(void* env, const wasm_val_t args[], wasm_val_t results[]) noexcept
{
  WebAssembly::Handle::FunctionContext* context = reinterpret_cast<WebAssembly::Handle::FunctionContext*>(env);
  if (!context) {
    return context->getTrap("Missing context.");
  }
  context->invocations++;
  // TAG: colorize
  auto& stream = fout;
  try {
    stream << context->fullname << getValuesAsString(args, context->argSize);
    // TAG: is types correct for input or do we need to set explicitly
    stream << " -> " << getValuesAsString(results, context->resultSize);
    stream << " INVOKES=" << context->invocations << ENDL;
  return context->getTrap("Force stop.");

    MemorySize slot = 0;
    if (context->fullname == "wasi_unstable!fd_prestat_get") {
      slot = getMemorySlot(args[1]);
    } else if (context->fullname == "wasi_unstable!fd_prestat_dir_name") {
      slot = getMemorySlot(args[1]);
    } else if (context->fullname == "wasi_unstable!fd_fdstat_get") {
      slot = getMemorySlot(args[1]);
    } else if (context->fullname == "wasi_unstable!proc_exit") {
      return context->getTrap("Force stop.");
    }
    if (slot) {
      uint8* data = context->write(slot, 64);
      MemoryDump dump(data, 64); // TAG: add slot as offset
      // dump.setGlobalOffset((slot) & ~0xf);
      // TAG: add ANSI support - highlight span support
      fout << dump << ENDL;
    }
  
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
  
#if 0
own wasm_trap_t* hook(void* env, const wasm_val_t args[], wasm_val_t results[]) noexcept
{
  WebAssembly::Handle::FunctionContext* context = reinterpret_cast<WebAssembly::Handle::FunctionContext*>(env);
  if (!context) {
    return context->getTrap("Missing context.");
  }
  fout << getValuesAsString(args, context->argSize) << ENDL;
  try {
    fout << "hook(): Hello, World!" << ENDL;
  } catch (Exception& e) {
    return context->getTrap(e);
  } catch (...) {
    return context->getTrap("Unknown exception throw.");
  }
  return nullptr;
}
#endif

own wasm_trap_t* hello(const wasm_val_t args[], wasm_val_t results[]) noexcept
{
  // no access to store so we cannot make trap
  fout << "hello(): Hello, World!" << ENDL;
  return nullptr;
}
#endif

#if 0
// void readMemory(uint8* dest, unsigned int address, unsigned int size);
// void writeMemory(unsigned int address, const uint8* src, unsigned int size);
// String getString(unsigned int address, unsigned int length);

// TAG: handle imports
// TAG: StackTrace getStackTrace()
// TAG: call async to start thread
// TAG: add template for automatic conversion

// Array<VirtualFileSystem*> getMounts();
// void mountFileSystem(const String& path);
// CompileError, LinkError, RuntimeError

// add directory access by paths
// consider ioctl/syskern appoach for all resource access - need security layer
// auto detect dead-lock / timeout
// probe features supported for WASM engine
#endif

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

  static void hello()
  {
    fout << "Hello, World!" << ENDL;
  }

  void run() override
  {
    WebAssembly wasm;
    TEST_ASSERT(wasm.isSupported());
    wasm.registerFunction(hello, "hello");
    // TAG: add support for text wasm
    // wasm.load();
    // wasm.call()
  }
};

TEST_REGISTER(WebAssembly);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
