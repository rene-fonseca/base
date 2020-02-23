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
#include <base/Functor.h>
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
#  include <wasm.h>
#  define own
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class Context {}; // TAG: allows forward to function

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
own wasm_trap_t* hook(void* env, const wasm_val_t args[], wasm_val_t results[]) noexcept
{
  Context* context = reinterpret_cast<Context*>(env);
  if (!context) {
    return nullptr; // TAG: trap
  }
  try {
    fout << "Hello, World!" << ENDL;
  } catch (...) {
    // TAG: need to generate trap
  }
  return nullptr;
}

own wasm_trap_t* hello(const wasm_val_t args[], wasm_val_t results[]) noexcept
{
  try {
    fout << "Hello, World!" << ENDL;
  } catch (...) {
    // TAG: need to generate trap
  }
  return nullptr;
}
#endif

bool WebAssembly::isSupported() noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
  return true;
#else
  return false;
#endif
}

WebAssembly::Function::Function()
{
}

WebAssembly::Function::~Function()
{
}

class WebAssembly::Handle : public ReferenceCountedObject {
private:

  MemorySize maximumMemoryUsage = 0;
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
  
  Array<void*> imports;
  wasm_engine_t* engine = nullptr;
  wasm_store_t* store = nullptr;
  own wasm_instance_t* instance = nullptr;
  own wasm_module_t* module = nullptr;
  own wasm_extern_vec_t exports = {0};
  const wasm_func_t* entry = nullptr;
#endif
public:

  void registerFunctionImpl(void* func)
  {
    imports.append(func);
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
#if 0 && defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    wasm_byte_vec_t binary;
    wasm_byte_vec_new_uninitialized(&binary, size);
    // wasmtime: missing symbol bool status = wasm_module_validate(store, &binary);
    wasm_byte_vec_delete(&binary);
    // return status;
#else
    return false;
#endif
  }

  // TAG: add support for journal for bug fixing
  bool load(const uint8* wasm, MemorySize size)
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    wasm_byte_vec_t binary;
#if 1
    wasm_byte_vec_new_uninitialized(&binary, size);
    copy(reinterpret_cast<uint8*>(binary.data), wasm, size);
#else
    clear(binary);
    binary.size = size;
    binary.data = (byte_t*)const_cast<uint8*>(wasm);
#endif
    module = wasm_module_new(store, &binary);
#if 1
    wasm_byte_vec_delete(&binary);
#endif
    
    own wasm_functype_t* hello_type = wasm_functype_new_0_0();
    // own wasm_func_t* hook_func = wasm_func_new_with_env(store, hello_type, hook, nullptr, nullptr);
    own wasm_func_t* hello_func = wasm_func_new(store, hello_type, hello);
    wasm_functype_delete(hello_type);
    // TAG: add template to bind global function/static function
    const wasm_extern_t* imports[] = { wasm_func_as_extern(hello_func), nullptr };

    own wasm_trap_t* traps = nullptr;
    instance = wasm_instance_new(store, module, imports, &traps);
    wasm_func_delete(hello_func);
    if (!instance) {
      return false;
    }

    wasm_instance_exports(instance, &exports);
    if (exports.size > 0) {
      entry = wasm_extern_as_func(exports.data[0]);
    }
    
    return true;
#else
    return false;
#endif
  }
  
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
  Symbol getSymbol(MemorySize i, const wasm_func_t* f)
  {
    Symbol s;
    if (!f) {
      s.index = i;
      s.name = String(format() << i);
      
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
      size_t argSize = wasm_func_param_arity(f);
      s.arguments.setSize(argSize);

      size_t resultSize = wasm_func_result_arity(f);
      s.results.setSize(resultSize);

      wasm_functype_t* functype = nullptr;
      // wasmtime: missing symbol /*own*/ wasm_functype_t* functype = wasm_func_type(f);
      if (functype) {
        const wasm_valtype_vec_t* ps = wasm_functype_params(functype);
        const wasm_valtype_vec_t* rs = wasm_functype_results(functype);
      }
#endif
    }
    return s;
  }
#endif
  
  String toString(const Symbol& s)
  {
    StringOutputStream sos;

    bool first = true;
    sos << "[";
    for (auto type : s.results) {
      if (!first) {
        sos << ", ";
      }
      first = false;
      sos << WebAssembly::toString(type);
    }
    sos << "] ";
    sos << s.name << "(";
    first = true;
    for (auto type : s.arguments) {
      if (!first) {
        sos << ", ";
      }
      first = false;
      sos << WebAssembly::toString(type);
    }
    sos << ")";
    return sos;
  }
  
  Array<Symbol> getExports()
  {
    Array<Symbol> result;
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    for (size_t i = 0; i < exports.size; ++i) {
      result.ensureCapacity(exports.size);
      const wasm_func_t* func = wasm_extern_as_func(exports.data[i]);
      if (!func) {
        auto s = getSymbol(i, func);
        result.append(s);
        fout << toString(s) << ENDL;
      }
    }
#endif
    return result;
  }

  Symbol getSymbol(const String& id)
  {
    unsigned int index = 0;
    try {
      index = UnsignedInteger::parse(id, UnsignedInteger::DEC);
    } catch (InvalidFormat&) {
      return Symbol();
    }
    
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    if (index < exports.size) {
      wasm_externkind_t kind = wasm_extern_kind(exports.data[index]);
      if (kind != WASM_EXTERN_FUNC) {
        return Symbol();
      }
      const wasm_func_t* func = wasm_extern_as_func(exports.data[index]);
      return getSymbol(index, func);
    }
#endif
    return Symbol();
  }
  
  void callEntry()
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    if (wasm_func_call(entry, NULL, NULL)) {
      _throw WebAssemblyException("Failed to call entry function.");
    }
#else
    _throw WebAssemblyException("Failed to call entry function.");
#endif
  }

  FormatOutputStream& writeFunction(FormatOutputStream& stream, const String& id, const Array<AnyValue>& arguments)
  {
    stream << "WebAssemble: call " << id << "(";
    bool first = true;
    for (auto a : arguments) {
      if (!first) {
        stream << ", ";
      }
      first = false;
      stream << a;
    }
    stream << ")" << FLUSH;
    return stream;
  }

  AnyValue call(unsigned int id, const Array<AnyValue>& arguments)
  {
    writeFunction(fout, format() << id, arguments);

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
    if (id < exports.size) {
      wasm_externkind_t kind = wasm_extern_kind(exports.data[id]);
      switch (kind) {
      case WASM_EXTERN_FUNC:
        break;
      case WASM_EXTERN_GLOBAL:
        break;
      case WASM_EXTERN_TABLE:
        break;
      case WASM_EXTERN_MEMORY:
        break;
      default:
        break;
      }

      const wasm_func_t* func = wasm_extern_as_func(exports.data[id]);
      if (!func) {
        // TAG: add support for arguments and result
        if (wasm_func_call(func, NULL, NULL)) {
          _throw WebAssemblyException("Failed to call entry function.");
        }
      }
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
  
#if 0
  // TAG: map arguments to types
  void convert(* dest, const Array<AnyValue>& arguments)
  {
  }
#endif
  
  AnyValue call(const String& id, const Array<AnyValue>& arguments)
  {
    unsigned int index = 0;
    try {
      index = UnsignedInteger::parse(id, UnsignedInteger::DEC);
    } catch (InvalidFormat&) {
      _throw WebAssemblyException("No such function.");
    }
    
    // TAG: add support for any function
    return call(index, arguments);
  }
  
  ~Handle()
  {
#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
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

void WebAssembly::registerFunctionImpl(void* func, const Type* args, unsigned int argsSize)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->registerFunctionImpl(func);
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

Array<WebAssembly::Symbol> WebAssembly::getExports()
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getExports();
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

#if defined(_COM_AZURE_DEV__BASE__USE_WASMTIME)
// TAG: wasmtime: get version from header
MODULE_REGISTER_EXPLICIT(_COM_AZURE_DEV__BASE__THIS_MODULE, "dev.wasmtime", "libwasmtime", "0.8", "https://wasmtime.dev/");
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
