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

#include <base/Primitives.h>
#include <base/Base.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class provides common casting methods.
  
  @short Type casting support.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Cast {
private:
  
  /** Casting error codes. */
  enum {
    /**
      This error specifies that the impersonator type does not occupy the exact
      amount of memory as the impersonated type.
    */
    MISMATCH_OF_IMPERSONATOR_FOOTPRINT,
    /**
      Specifies that the container type occupies less memory than the element
      type.
    */
    CONTAINER_FOOTPRINT_TOO_SMALL,
    /**
      Specifies that the element type occupies more memory than the container
      type.
    */
    ELEMENT_FOOTPRINT_TOO_LARGE,
    /**
      Specifies that the type is not a pointer.
    */
    NOT_A_POINTER
  };
  
  template<class RESULT>
  class Up {
  public:
    
    inline RESULT operator()(RESULT value) const throw() {
      // Constraint<false, Cast>::NOT_A_POINTER;
    }
  };
  
  template<class RESULT>
  class Up<RESULT*> {
  public:
    
    inline RESULT* operator()(RESULT* value) throw() {
      return value;
    }
  };
  
  template<class RESULT>
  class Up<const RESULT*> {
  public:
    
    inline const RESULT* operator()(const RESULT* value) throw() {
      return value;
    }
  };

  template<class RESULT, class ORIGINAL>
  class ExplicitCast {
  public:
    
    inline RESULT operator()(ORIGINAL value) throw() {
      return value;
    }
  };
  
  template<class ORIGINAL>
  class ExplicitCast<void, ORIGINAL> {
  public:
    
    inline void operator()(ORIGINAL value) throw() {
    }
  };
  
  template<class ORIGINAL>
  class ExplicitCast<const void, ORIGINAL> {
  public:
    
    inline const void operator()(ORIGINAL value) throw() {
    }
  };
  
  template<class ORIGINAL>
  class ExplicitCast<volatile const void, ORIGINAL> {
  public:
    
    inline volatile const void operator()(ORIGINAL value) throw() {
    }
  };

  template<class RESULT, class ORIGINAL>
  class PointerCast {
  public:

#if 0 // best to disallow non pointers for now
    static inline RESULT cast(const ORIGINAL& value) throw() { // force input to pointer
      const void* _dummy = value;
      return reinterpret_cast<RESULT>(value);
    }

    static inline RESULT cast(ORIGINAL& value) throw() { // force input to pointer
      const void* _dummy = value;
      return reinterpret_cast<RESULT>(value);
    }
#endif
  };
  
  template<class RESULT, class ORIGINAL>
  class PointerCast<RESULT*, ORIGINAL*> {
  public:
    
    static inline RESULT* cast(ORIGINAL* value) throw() {
_COM_AZURE_DEV__BASE__PACKED__BEGIN
      union {
        ORIGINAL* original;
        RESULT* result;
      } _COM_AZURE_DEV__BASE__PACKED temp;
_COM_AZURE_DEV__BASE__PACKED__END
      temp.original = value;
      return temp.result;
    }
  };
  
  template<class RESULT, class ORIGINAL>
  class PointerCast<const RESULT*, const ORIGINAL*> {
  public:
    
    static inline const RESULT* cast(const ORIGINAL* value) throw() {
_COM_AZURE_DEV__BASE__PACKED__BEGIN
      union {
        const ORIGINAL* original;
        const RESULT* result;
      } _COM_AZURE_DEV__BASE__PACKED temp;
_COM_AZURE_DEV__BASE__PACKED__END
      temp.original = value;
      return temp.result;
    }
  };

  template<class RESULT, class ORIGINAL>
  class PointerCast<volatile RESULT*, volatile ORIGINAL*> {
  public:
    
    static inline volatile RESULT* cast(volatile ORIGINAL* value) throw() {
_COM_AZURE_DEV__BASE__PACKED__BEGIN
      union {
        volatile ORIGINAL* original;
        volatile RESULT* result;
      } _COM_AZURE_DEV__BASE__PACKED temp;
_COM_AZURE_DEV__BASE__PACKED__END
      temp.original = value;
      return temp.result;
    }
  };
  
  template<class RESULT, class ORIGINAL>
  class PointerCast<volatile const RESULT*, volatile const ORIGINAL*> {
  public:
    
    static inline volatile const RESULT* cast(
      volatile const ORIGINAL* value) throw() {
_COM_AZURE_DEV__BASE__PACKED__BEGIN
      union {
        volatile const ORIGINAL* original;
        volatile const RESULT* result;
      } _COM_AZURE_DEV__BASE__PACKED temp;
_COM_AZURE_DEV__BASE__PACKED__END
      temp.original = value;
      return temp.result;
    }
  };
  
  template<class RESULT, class ORIGINAL>
  class AnyPointerCast {
  public:
    
    static inline RESULT cast(ORIGINAL value) throw() {
      // Constraint<false, Cast>::NOT_A_POINTER;
    }
  };
  
  template<class RESULT, class ORIGINAL>
  class AnyPointerCast<RESULT*, ORIGINAL*> {
  public:
    
    static inline RESULT* cast(ORIGINAL* value) throw() {
_COM_AZURE_DEV__BASE__PACKED__BEGIN
      union {
        ORIGINAL* original;
        RESULT* result;
      } _COM_AZURE_DEV__BASE__PACKED temp;
_COM_AZURE_DEV__BASE__PACKED__END
      temp.original = value;
      return temp.result;
    }
  };
  
  template<class RESULT, class ORIGINAL>
  class ReinterpretCast {
  };
  
  template<class RESULT, class ORIGINAL>
  class ReinterpretCast<RESULT*, ORIGINAL*> {
  public:
    
    inline RESULT* operator()(ORIGINAL* value) const throw() {
_COM_AZURE_DEV__BASE__PACKED__BEGIN
      union {
        ORIGINAL* original;
        RESULT* result;
      } _COM_AZURE_DEV__BASE__PACKED temp;
_COM_AZURE_DEV__BASE__PACKED__END
      temp.original = value;
      return temp.result;
    }
  };
  
  template<class RESULT, class ORIGINAL>
  class ReinterpretCast<const RESULT*, const ORIGINAL*> {
  public:
    
    inline const RESULT* operator()(const ORIGINAL* value) const throw() {
_COM_AZURE_DEV__BASE__PACKED__BEGIN
      union {
        const ORIGINAL* original;
        const RESULT* result;
      } _COM_AZURE_DEV__BASE__PACKED temp;
_COM_AZURE_DEV__BASE__PACKED__END
      temp.original = value;
      return temp.result;
    }
  };
  
  template<class TYPE>
  class GetPointerCast {
  };
  
  template<class TYPE>
  class GetPointerCast<TYPE*> {
  public:
    
    static inline TYPE* cast(MemorySize value) throw() {
      return static_cast<TYPE*>(
        static_cast<void*>(static_cast<char*>(nullptr) + value)
      );
    }
  };
public:

  /**
    Implicit cast to the specified type.
  */
  template<class RESULT>
  static inline RESULT implicit(RESULT argument) throw() {
    return argument;
  }
  
  /**
    Returns the address of the specified object as a byte pointer
    (i.e. uint8*).
  */
  template<class TYPE>
  static inline uint8* getAddress(TYPE& value) throw() {
    return reinterpret_cast<uint8*>(&value);
  }
  
  /**
    Returns the address of the specified object as a byte pointer
    (i.e. uint8*).
  */
  template<class TYPE>
  static inline const uint8* getAddress(const TYPE& value) throw() {
    return reinterpret_cast<const uint8*>(&value);
  }
  
  /**
    Returns the address of the specified object as a byte pointer
    (i.e. uint8*).
  */
  template<class TYPE>
  static inline volatile uint8* getAddress(volatile TYPE& value) throw() {
    return reinterpret_cast<volatile uint8*>(&value);
  }

  /**
    Returns the address of the specified object as a byte pointer
    (i.e. uint8*).
  */
  template<class TYPE>
  static inline volatile const uint8* getAddress(
    volatile const TYPE& value) throw() {
    return reinterpret_cast<volatile const uint8*>(&value);
  }

  /**
    Returns the address of the specified array as a byte pointer (i.e. uint8*).
  */
  template<class TYPE>
  static inline uint8* getAddress(TYPE value[]) throw() {
    return reinterpret_cast<uint8*>(value);
  }
  
  /**
    Returns the address of the specified object as a byte pointer
    (i.e. uint8*).
  */
  template<class TYPE>
  static inline const uint8* getAddress(const TYPE value[]) throw() {
    return reinterpret_cast<const uint8*>(value);
  }
  
  /**
    Returns the address of the specified object as a byte pointer
    (i.e. uint8*).
  */
  template<class TYPE>
  static inline volatile uint8* getAddress(volatile TYPE value[]) throw() {
    return reinterpret_cast<volatile uint8*>(value);
  }
  
  /**
    Returns the address of the specified object as a byte pointer
    (i.e. uint8*).
  */
  template<class TYPE>
  static inline volatile const uint8* getAddress(
    volatile const TYPE value[]) throw() {
    return reinterpret_cast<volatile const uint8*>(value);
  }
  
  /**
    Returns the address of the specified object as a char pointer.
  */
  template<class TYPE>
  static inline char* getCharAddress(TYPE& value) throw() {
    return reinterpret_cast<char*>(&value);
  }
  
  /**
    Returns the address of the specified object as a char pointer.
  */
  template<class TYPE>
  static inline const char* getCharAddress(const TYPE& value) throw() {
    return reinterpret_cast<const char*>(&value);
  }
  
  /**
    Returns the address of the specified object as a char pointer.
  */
  template<class TYPE>
  static inline volatile char* getCharAddress(volatile TYPE& value) throw() {
    return reinterpret_cast<volatile char*>(&value);
  }

  /**
    Returns the address of the specified object as a char pointer.
  */
  template<class TYPE>
  static inline volatile const char* getCharAddress(
    volatile const TYPE& value) throw() {
    return reinterpret_cast<volatile const char*>(&value);
  }

  /**
    Returns the address of the specified object as a char pointer.
  */
  template<class TYPE>
  static inline char* getCharAddress(TYPE value[]) throw() {
    return reinterpret_cast<char*>(value);
  }
  
  /**
    Returns the address of the specified object as a char pointer.
  */
  template<class TYPE>
  static inline const char* getCharAddress(const TYPE value[]) throw() {
    return reinterpret_cast<const char*>(value);
  }
  
  /**
    Returns the address of the specified object as a char pointer.
  */
  template<class TYPE>
  static inline volatile char* getCharAddress(volatile TYPE value[]) throw() {
    return reinterpret_cast<volatile char*>(value);
  }
  
  /**
    Returns the address of the specified object as a char pointer.
  */
  template<class TYPE>
  static inline volatile const char* getCharAddress(
    volatile const TYPE value[]) throw() {
    return reinterpret_cast<volatile const char*>(value);
  }

  /**
    Returns the memory offset of the specified pointer. Any qualifiers are lost
    in the process!
  */
  static inline MemorySize getOffset(const volatile void* value) throw() {
    return static_cast<const volatile char*>(value) -
      static_cast<const volatile char*>(nullptr);
  }

  /**
    Returns the pointer value corresponding to the specified memory offset.
  */
  template<class TYPE>
  static inline TYPE getPointer(MemorySize value) throw() {
    return GetPointerCast<TYPE>::cast(value);
  }
  
  /**
    This cast function is used to up cast pointers (i.e. you can explicitly
    specify the desired pointer type). This function only works if there exists
    implicit rules which convert the input type to the desired type.
  */
  template<class RESULT>
  static inline RESULT up(RESULT value) throw() {
    return Up<RESULT>(value);
  }
  
  template<class RESULT, class ORIGINAL>
  static inline RESULT explicitly(ORIGINAL value) throw() {
    return ExplicitCast<RESULT, ORIGINAL>();
  }

  /**
    This method casts any pointer type to any other pointer type (at compile
    time). You should definitely avoid this function when possible. However, it
    is not as dangerous as using reinterpret_cast directly. This function does
    not cast away the qualifiers (e.g. const).
  */
  template<class RESULT, class ORIGINAL>
  static inline RESULT pointer(ORIGINAL value) throw() {
    return PointerCast<RESULT, ORIGINAL>::cast(value); // TAG: how to handle PrimitiveArray<char> and similar when operator needs to be called?
  }
  
  template<class RESULT, class ORIGINAL>
  static inline RESULT anypointer(ORIGINAL value) throw() {
    return AnyPointerCast<RESULT, ORIGINAL>::cast(value);
  }
  
  template<class RESULT, class ORIGINAL>
  static inline RESULT reinterpret(ORIGINAL value) throw() {
    return ReinterpretCast<RESULT, ORIGINAL>(value);
  }
  
  template<class RESULT>
  class Impersonate {
  public:
    
    template<class ARGUMENT>
    static inline RESULT& cast(ARGUMENT& value) throw() {
      if (Constraint<sizeof(RESULT) == sizeof(ARGUMENT), Cast>::MISMATCH_OF_IMPERSONATOR_FOOTPRINT) {}
      return *reinterpret_cast<RESULT*>(&value);
    }
    
    template<class ARGUMENT>
    static inline const RESULT& cast(const ARGUMENT& value) throw() {
      if (Constraint<sizeof(RESULT) == sizeof(ARGUMENT), Cast>::MISMATCH_OF_IMPERSONATOR_FOOTPRINT) {}
      return *reinterpret_cast<const RESULT*>(&value);
    }
  };

  /**
    Casts the argument to the specified type. The argument and result types
    must have the exact same memory size.
    
    @see container
  */
  template<class RESULT, class ARGUMENT>
  static inline RESULT impersonate(ARGUMENT value) throw() {
    return Impersonate<RESULT>::cast(value);
  }

  template<class RESULT>
  class Container {
  public:
    
    template<class ARGUMENT>
    static inline RESULT cast(ARGUMENT& value) throw() {
      if (Constraint<sizeof(RESULT) >= sizeof(ARGUMENT), Cast>::CONTAINER_FOOTPRINT_TOO_SMALL) {}
// _COM_AZURE_DEV__BASE__PACKED__BEGIN
      union {
        ARGUMENT argument;
        RESULT result; // make sure we do not get an access violation in the cast
      } /*_COM_AZURE_DEV__BASE__PACKED*/ temp;
// _COM_AZURE_DEV__BASE__PACKED__END
      temp.argument = value;
      return *reinterpret_cast<RESULT*>(&temp.argument); // partial garbage
    }
    
    template<class ARGUMENT>
    static inline const RESULT cast(const ARGUMENT& value) throw() {
      if (Constraint<sizeof(RESULT) >= sizeof(ARGUMENT), Cast>::CONTAINER_FOOTPRINT_TOO_SMALL) {}
// _COM_AZURE_DEV__BASE__PACKED__BEGIN
      union {
        const ARGUMENT argument;
        const RESULT result; // make sure we do not get an access violation in the cast
      } /*_COM_AZURE_DEV__BASE__PACKED*/ temp;
// _COM_AZURE_DEV__BASE__PACKED__END
      temp.argument = value;
      return *reinterpret_cast<const RESULT*>(&temp.argument); // partial garbage
    }
  };

  /**
    Casts the argument to the specified type. The result type must not occupy
    less memory than the argument type.
    
    @see impersonate
  */
  template<class RESULT, class ARGUMENT>
  static inline RESULT container(ARGUMENT value) throw() {
    return Container<RESULT>::cast(value);
  }

  template<class RESULT>
  class Extract {
  public:
    
    template<class ARGUMENT>
    static inline RESULT cast(ARGUMENT& value) throw() {
      if (Constraint<sizeof(RESULT) <= sizeof(ARGUMENT), Cast>::ELEMENT_FOOTPRINT_TOO_LARGE) {}
      return *reinterpret_cast<RESULT*>(&value);
    }
    
    template<class ARGUMENT>
    static inline const RESULT cast(const ARGUMENT& value) throw() {
      if (Constraint<sizeof(RESULT) <= sizeof(ARGUMENT), Cast>::ELEMENT_FOOTPRINT_TOO_LARGE) {}
      return *reinterpret_cast<const RESULT*>(&value);
    }
  };

  /**
    Casts the argument to the specified type. The result type must not occupy
    more memory than the argument type.
    
    @see container
  */
  template<class RESULT, class ARGUMENT>
  static inline RESULT extract(ARGUMENT value) throw() {
    return Extract<RESULT>::cast(value);
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
