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

#include <base/dl/LinkerException.h>
#include <base/string/String.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class provides on-demand loading of (shared) objects.

  @short Dynamic linker loader.
  @version 1.2
*/

class _COM_AZURE_DEV__BASE__API DynamicLinker {
private:

  /** Opaque handle to the module. */
  void* handle = nullptr;
  
  /** Returns a pointer to the global symbol. */
  static void* getGlobalSymbolImpl(const String& symbol) throw(LinkerException);
public:

  typedef void (*Function)();

  /* Function descriptor. */
  struct StaticFunctionDescriptor {
    /** The symbol. */
    const char* symbol;
    /** The function. */
    Function* function;
  };
  
  /** Linker options. */
  enum Options {
    LAZY = 1, /**< Resolve symbols during execution of module. */
    GLOBAL = 2 /**< Include symbols of the module in the global symbol pool. */
  };

  template<class TYPE>
  static inline TYPE getGlobalSymbol(const String& symbol) throw(LinkerException)
  {
    return (TYPE)getGlobalSymbolImpl(symbol);
  }

  /**
    Opens a Dynamic Shared Object (DSO) with the specified options. LACY
    relocations may not be supported the particular platform in which case all
    relocations will be resolved during object initialization.
    

    @param module The path of the module to open.
    @param options The linking options. Default is LAZY.
  */
  DynamicLinker(
    const String& module, unsigned int options = LAZY) throw(LinkerException);

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
  void* getSymbol(const Literal& symbol) const throw(LinkerException);
  
  /**
    Returns the address of the specified symbol.

    @param symbol The symbol to be resolved.
    @return The address of the symbol (0 is not available).
  */
  void* getUncertainSymbol(const String& symbol) const noexcept;
  
  /**
    Returns the address of the specified symbol.

    @param symbol The symbol to be resolved.
    @return The address of the symbol (0 is not available).
  */
  void* getUncertainSymbol(const Literal& symbol) const noexcept;

  /** Flags for use with import method. */
  enum Flags {
    /** Continue to import symbols when symbol is not available. */
    CONTINUE = 1
  };
  
  /**
    Imports the addresses of the specified symbols/functions.

    @param functions The desired functions.
    @param numberOfFunctions The number of functions.
    @param flags The flags.
    
    @return True if all functions are available.
  */
  bool import(
    StaticFunctionDescriptor* functions,
    unsigned int numberOfFunctions, bool flags = 0) noexcept;

  /** Returns the image address of the executable. */
  static void* getProcessImage() noexcept;

  /** Returns the image address of the Base Framework model (can be the executable module for static linking). */
  static void* getBaseFrameworkImage() noexcept;

  /** Returns the image address for the given process address. */
  static void* getImageAddress(const void* address) noexcept;

  /** Returns the path of the image for the given process address. */
  static String getImagePath(const void* address);

  /** Returns the closest symbol for the given process address. */
  static void* getSymbolAddress(const void* address) noexcept;

  /** Returns the closest symbol for the given process address. */
  static String getSymbolName(const void* address);

  /** Symbol info. */
  class SymbolInfo {
  public:

    void* address = nullptr;
    void* imageAddress = nullptr;
    String name;
    // could add some flags/type info

    inline operator bool() const noexcept
    {
      return address;
    }
  };

  /** Returns the closest symbol for the given process address. */
  static SymbolInfo getSymbolInfo(const void* address) noexcept;

  /**
    Closes the module.
  */
  ~DynamicLinker();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
