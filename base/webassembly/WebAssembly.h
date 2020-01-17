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
  static const char* toString(Type type) noexcept
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

  /*
  bool -> i32
  int -> i32
  int64 -> i64
  float -> f32
  double -> f64
  long double -> f64
  String/WideString -> string
  */

  /*
  string -> String/WideString
  i32 -> int
  i64 -> int64
  f32 -> float
  f64 -> double
  */

  /** Description for symbol. */
  class Symbol {
  public:

    String id;
    Type type = TYPE_FUNCTION;
    Array<Type> arguments;
    Type returnType = TYPE_i32;
  };

  class _COM_AZURE_DEV__BASE__API WebAssemblyException : public Exception {
  public:
  };
  
  // CompileError, LinkError, RuntimeError

  // add directory access by paths
  // callbacks
  // register host functions - need to parse and build this automatically
  // consider ioctl/syskern appoach for all resource access - need security layer
  // auto detect dead-lock / timeout
  // probe features supported for WASM engine
  
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

  /** Loads the given wasm module. */
  bool load(const String& path);
  
  /** Loads the given wasm module in buffer. */
  bool load(const uint8* wasm, MemorySize size);

  // TAG: force full compilation instead of JIT

  /** Returns the exported symbols. */
  Array<Symbol> getExports();

  /** Returns information about the given id. */
  Symbol getSymbol(const String& id);

  /** Calls the function with the given id and arguments. */
  AnyValue call(const String& id, const Array<AnyValue>& arguments);

  // void readMemory(uint8* dest, unsigned int address, unsigned int size);
  // void writeMemory(unsigned int address, const uint8* src, unsigned int size);
  // String getString(unsigned int address, unsigned int length);

  // TAG: handle imports
  // TAG: StackTrace getStackTrace()
  // TAG: call async to start thread
  // TAG: add template for automatic conversion

  // Array<VirtualFileSystem*> getMounts();
  // void mountFileSystem(const String& path);
  
  ~WebAssembly();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
