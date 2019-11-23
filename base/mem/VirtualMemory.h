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

#include <base/MemoryException.h>
#include <base/collection/Array.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Virtual memory.
  
  @short Virtual memory
  @ingroup memory
  @see Heap SharedMemory
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API VirtualMemory {
public:

  enum State {
    RESERVED,
    FREE,
    COMMITTED
  };

  enum Flags {
    NO_ACCESS = 0,
    EXECUTE = 1,
    READ = 2,
    WRITE = 4,
    GUARD = 8,
    NO_CACHE = 16,
    IMAGE = 32,
    MAPPED = 64,
    PRIVATE = 128
  };

#if 0
  class Descriptor {
  public:
  
    const void* address = nullptr;
    unsigned int flags = 0;
    // Permissions Access access;
    MemorySize size = 0;
  };
#endif

  class _COM_AZURE_DEV__BASE__API Module {
  private:
    
    const void* context = nullptr;
  public:

    inline Module() noexcept {
    }
    
    inline Module(const void* _context) noexcept : context(_context) {
    }
    
    inline Module(const Module& copy) noexcept : context(copy.context) {
    }
    
    inline Module operator=(const Module& eq) noexcept {
      context = eq.context;
      return *this;
    }

    bool isModule() const noexcept;

    static Module getProcessModule() noexcept;

    String getPath() const noexcept;

    MemorySize getSize() const noexcept;
  };

  static Array<Module> getModules() noexcept;
  
  static void query(const void* address) noexcept;

  /**
    Returns the base address for the address.
  */
  static void* getBase(const void* address) noexcept;

  static void allocate() noexcept;
  
  static void release() noexcept;
  
  static void dump() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
