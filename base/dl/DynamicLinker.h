/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_DL__DYNAMIC_LINKER_H
#define _DK_SDU_MIP__BASE_DL__DYNAMIC_LINKER_H

#include <base/dl/LinkerException.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class provides on-demand loading of (shared) objects.

  @short Dynamic linker loader.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class DynamicLinker {
private:

  /** Opaque handle to the module. */
  void* handle;
  //ReferenceCounter<void> handle;

  /** Returns a pointer to the global symbol. */
  static void* getGlobalSymbolImpl(const String& symbol) throw(LinkerException);
public:

  /** Linker options. */
  enum Options {
    LAZY = 1, /**< Resolve symbols during execution of module. */
    GLOBAL = 2 /**< Include symbols of the module in the global symbol pool. */
  };

  template<class TYPE>
  static inline TYPE getGlobalSymbol(const String& symbol) throw(LinkerException) {
    return (TYPE)getGlobalSymbolImpl(symbol);
  }

  /**
    Opens a Dynamic Shared Object (DSO) with the specified options.

    @param module The path of the module to open.
    @param options The linking options. Default is LAZY.
  */
  DynamicLinker(const String& module, unsigned int options = LAZY) throw(LinkerException);

  /**
    Returns the address of the specified symbol.

    @param symbol The symbol to be resolved.
    @return The address of the symbol.
  */
  void* getSymbol(const String& symbol) const throw(LinkerException);

  /**
    Returns the address of the specified symbol.

    @param symbol The symbol to be resolved.
    @return The address of the symbol.
  */
  void* getSymbol(const StringLiteral& symbol) const throw(LinkerException);

  /**
    Closes the module.
  */
  ~DynamicLinker() throw(LinkerException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
