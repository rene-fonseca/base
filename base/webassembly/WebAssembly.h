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
protected:

  class Handle;

  Reference<ReferenceCountedObject> handle;
public:

  enum Type {
    TYPE_FUNCTION,
    TYPE_i32,
    TYPE_i64,
    TYPE_f32,
    TYPE_f64,
    TYPE_STRING
  };
  
  // TAG: map types
  // bool -> i32
  // int -> i32
  // int64 -> i64
  // float -> f32
  // double -> f64
  // long double -> f64

  /** Description for symbol. */
  class Symbol {
  public:

    Type type = TYPE_FUNCTION;
    Array<Type> arguments;
    Type returnType = TYPE_i32;
  };

  class _COM_AZURE_DEV__BASE__API WebAssemblyException : public Exception {
  public:
  };

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

  // TAG: get stack trace
  // TAG: call async to start thread
  // TAG: add template for automatic conversion

  ~WebAssembly();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
