/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__VIRTUAL_MEMORY_H
#define _DK_SDU_MIP__BASE_MEM__VIRTUAL_MEMORY_H

#include <base/MemoryException.h>
#include <base/collection/Array.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Virtual memory.
  
  @see Heap SharedMemory
  @short Virtual memory
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class VirtualMemory {
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

  struct Descriptor {
    const void* address;
    unsigned int flags;
    // Permissions Access access;
    MemorySize size;
  };

  class Module {
  private:
    
    const void* context;
  public:

    inline Module() throw() : context(0) {
    }
    
    inline Module(const void* _context) throw() : context(_context) {
    }
    
    inline Module(const Module& copy) throw() : context(copy.context) {
    }
    
    inline Module operator=(const Module& eq) throw() {
      context = eq.context;
      return *this;
    }

    bool isModule() const throw();

    static Module getProcessModule() throw();

    String getPath() const throw();

    MemorySize getSize() const throw();
  };

  static Array<Module> getModules() throw();
  
  static void query(const void* address) throw();

  static void allocate() throw();
  
  static void release() throw();
  
  static void dump() throw();
};

// TAG: Module is relocateable

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
