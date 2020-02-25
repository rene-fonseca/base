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
#include <base/UnitTest.h>
#include <base/build.h>

// #undef _COM_AZURE_DEV__BASE__USE_WASMTIME

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
#  include <wasm.h>
#  define own
#endif

// TAG: use runtime linking like opengl
// TAG: add support for multiple engines - but adding Engine class

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
void dumpValues(const wasm_val_t args[], MemorySize size)
{
  fout << "(";
  for (MemorySize i = 0; i < size; ++i) {
    const wasm_val_t& arg = args[i];
    switch (arg.kind) {
    case WASM_I32:
      fout << arg.of.i32;
      break;
    case WASM_I64:
      fout << arg.of.i64;
      break;
    case WASM_F32:
      fout << arg.of.f32;
      break;
    case WASM_F64:
      fout << arg.of.f64;
      break;
    case WASM_ANYREF:
      fout << "<REF>";
      break;
    case WASM_FUNCREF:
      fout << "<FUNCTION>";
      break;
    default:
      fout << "<BAD>";
    }
  }
  fout << ")";
}

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

  /** Call context. */
  class FunctionContext {
  public:
    
    WebAssembly::Handle* handle = nullptr;
    MemorySize argSize = 0;
    
    FunctionContext(WebAssembly::Handle* _handle, MemorySize _argSize) noexcept
      : handle(_handle), argSize(_argSize)
    {
    }
    
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    /** Returns trap for given message. */
    own wasm_trap_t* getTrap(const char* _message)
    {
      wasm_message_t message;
      wasm_store_t* store = handle->store;
      const MemorySize length = getNullTerminatedLength(_message);
      message.size = length;
      message.data = (wasm_byte_t*)_message;
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
  
  class ImportFunction {
  public:
  
    void* func = nullptr;
    WebAssembly::Type result = TYPE_UNSPECIFIED;
    Array<WebAssembly::Type> arguments;
    String name;
  };

  Array<ImportFunction> imports;
  Array<FunctionContext*> functionContexts;
public:

  void registerFunctionImpl(void* func, Type result, const Type* args, unsigned int argsSize, const String& name)
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
        String msg(reinterpret_cast<const char*>(message.data), message.size);
        _throw WebAssemblyException(msg.native());
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
  
  FunctionType getFunctionType(wasm_functype_t* functype)
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
    // wasmtime: missing symbol /*own*/ wasm_functype_t* functype = wasm_func_type(f);
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
  
  bool makeInstance()
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    if (!module) {
      _throw WebAssemblyException("Module not loaded.");
    }
    
    own wasm_importtype_vec_t _imports;
    wasm_module_imports(module, &_imports);
    PrimitiveArray<const wasm_extern_t*> imports(_imports.size);
#if 1
    // TAG: can we dynamically register a function
    own wasm_functype_t* hello_type = wasm_functype_new_0_0();
    FunctionContext* functionContext = new FunctionContext(this, 0);
    functionContexts.append(functionContext);
    own wasm_func_t* hook_func = wasm_func_new_with_env(store, hello_type, hook, functionContext, nullptr);
    /*
    wasm_func_callback_with_env_t f = (wasm_func_callback_with_env_t)this->imports[0].func;
    if (f) {
      own wasm_func_t* func1 = wasm_func_new_with_env(store, hello_type, f, functionContext, nullptr);
      imports[0] = wasm_func_as_extern(func1);
    }
     */
    // own wasm_func_t* hello_func = wasm_func_new(store, hello_type, hello);
    wasm_functype_delete(hello_type);
    // TAG: add template to bind global function/static function
    for (MemorySize i = 0; i < _imports.size; ++i) {
      imports[i] = wasm_func_as_extern(hook_func);
    }
#endif

    // TAG: add support for WASI instance
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
  Symbol getSymbol(MemorySize i, const wasm_func_t* f, const String& name, const String& moduleName)
  {
    Symbol s;
    s.index = i;
    s.name = name;
    s.moduleName = moduleName;
    s.externType = EXTERN_FUNCTION;
    if (f) { // TAG: how do we get function type when f is null for importtype
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
      const wasm_externtype_t* type = wasm_importtype_type(import);
      wasm_externkind_t kind = wasm_externtype_kind(type);

      switch (kind) {
      case WASM_EXTERN_FUNC:
        {
          // TAG: get type
          result[i] = getSymbol(i, nullptr, name, moduleName);
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
          result[i].externType = EXTERN_MEMORY;
          const wasm_memory_t* memory = wasm_extern_as_memory(e);
          if (memory) {
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
  
  FormatOutputStream& writeFunction(FormatOutputStream& stream, const String& id, const Array<AnyValue>& arguments)
  {
    stream << "WebAssemble: calling: " << id << "(";
    bool first = true;
    for (auto a : arguments) {
      if (!first) {
        stream << ", ";
      }
      first = false;
      stream << a;
    }
    stream << ")" << ENDL;
    return stream;
  }

  AnyValue call(MemorySize id, const Array<AnyValue>& arguments)
  {
    String name = exportNames[id];
    if (!name) {
      name = String(format() << id);
    }
    writeFunction(fout, name, arguments);

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    if (id < exports.size) {
      // wasm_externkind_t kind = wasm_extern_kind(exports.data[id]);
      const wasm_func_t* func = wasm_extern_as_func(exports.data[id]);
      if (!func) {
        _throw WebAssemblyException("Not a function.");
      }

      const size_t size = wasm_func_param_arity(func);
      const size_t resultSize = wasm_func_result_arity(func);
      wasm_val_t args[256];
      BASSERT(arguments.getSize() <= getArraySize(args)); // TAG: handle 2 vals for strings
      wasm_val_t* end = convert(args, arguments);
      // own wasm_functype_t* type = wasm_func_type(func);
      wasm_val_t results[2];
      if (own wasm_trap_t* trap = wasm_func_call(func, args, results)) {
        onTrap(trap);
      }
      return convertToAnyValue(results, resultSize);
    }
#endif
    _throw WebAssemblyException("No such function.");
  }
  
  AnyValue call(WebAssembly::Function f, const Array<AnyValue>& arguments)
  {
    writeFunction(fout, "?", arguments);

#if 0
    if (!f) {
      _throw WebAssemblyException("Invalid function");
    }
#endif
    
#if 0 && defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    // TAG: handle exceptions
    if (wasm_func_call(f, NULL, NULL)) {
      return 1;
    }
    
    if (argSize != arguments.getSize()) {
      _throw OutOfRange("Invalid number of arguments.");
    }
    if (resultSize > 8) {
      _throw OutOfRange("Invalid number of results.");
    }
    // String would map to 2 words - mem+size
    wasm_val_t args[64];
    wasm_val_t results[8];
    /*own*/ wasm_trap_t* trap = wasm_func_call(func, args, results);

    if (trap) {
    #if 0
      wasm_message_t*
      wasm_trap_message(const wasm_trap_t*, own wasm_message_t* out);
      own wasm_frame_t* wasm_trap_origin(const wasm_trap_t*);
      wasm_trap_trace(const wasm_trap_t*, own wasm_frame_vec_t* out);
    #endif
    }
#endif
    
    return AnyValue();
  }
  
  void callEntry()
  {
    call(0, {});
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
  
  wasm_val_t* convert(wasm_val_t* dest, const Array<AnyValue>& arguments)
  {
    for (const auto& a : arguments) {
      dest = convert(dest, a);
    }
    return dest;
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
  
  AnyValue call(const String& id, const Array<AnyValue>& arguments)
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
    
    return call(index, arguments);
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
    sos << setForeground(ANSIEscapeSequence::RED) << dim() << italic();
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
    sos << normal();
    sos << setForeground(ANSIEscapeSequence::BLUE);
  }
  if (name) {
    sos << name;
  } else {
    sos << "(*)";
  }
  if (colorize) {
    sos << dim() << italic();
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

void WebAssembly::registerFunctionImpl(void* func, Type result, const Type* args, unsigned int argsSize, const String& name)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->registerFunctionImpl(func, result, args, argsSize, name);
}

bool WebAssembly::load(const String& path)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  
  Allocator<uint8> buffer;
  {
    fout << "WebAssembly::load(): " << path << ENDL;
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

bool WebAssembly::makeInstance()
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->makeInstance();
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

AnyValue WebAssembly::call(const String& id, const Array<AnyValue>& arguments)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->call(id, arguments);
}

AnyValue WebAssembly::call(unsigned int id, const Array<AnyValue>& arguments)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->call(id, arguments);
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
own wasm_trap_t* hook(void* env, const wasm_val_t args[], wasm_val_t results[]) noexcept
{
  WebAssembly::Handle::FunctionContext* context = reinterpret_cast<WebAssembly::Handle::FunctionContext*>(env);
  if (!context) {
    return context->getTrap("Missing context.");
  }
  dumpValues(args, context->argSize);
  try {
    fout << "hook(): Hello, World!" << ENDL;
  } catch (Exception& e) {
    return context->getTrap(e);
  } catch (...) {
    return context->getTrap("Unknown exception throw.");
  }
  return nullptr;
}

own wasm_trap_t* hello(const wasm_val_t args[], wasm_val_t results[]) noexcept
{
  try {
    fout << "hello(): Hello, World!" << ENDL;
  } catch (...) {
    // TAG: need to generate trap
  }
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
