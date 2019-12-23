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

#include <base/features.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Architecture information.
  
  @short Architecture information.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Architecture {
public:

  /** Supported byte orders. */
  enum ByteOrder {
    BE = 0, /**< Big endian. */
    LE = 1, /**< Little endian. */
#if (_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN)
    NATIVE = BE /**< Native byte order. */
#elif (_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN)
    NATIVE = LE /**< Native byte order. */
#else
    NATIVE = 0 /**< Native byte order. */
#endif
  };

  /** Technology flags. */
  enum Technology {
    FPU = 1U << 0, /**< Floating Point Unit (FPU). */
    MMX = 1U << 1, /**< Multi-Media Extentions (MMX) */
    SSE = 1U << 2, /**< Streaming SIMD Extensions (SSE). */
    SSE2 = 1U << 3, /**< Streaming SIMD Extensions (SSE) 2. */
    SSE3 = 1U << 4, /**< Streaming SIMD Extensions (SSE) 3. */
    NOW3D = 1U << 5, /**< 3DNow. */
    NOW3D2 = 1U << 6, /**< 3DNow 2. */
    HYPER_THREADING = 1U << 7 /**< Hyper-threading. */
  };

  /** The major architecture. */
  enum Major {
    ALPHA, /**< Alpha. */
    X86, /**< Intel 32 bit. */
    X86_64, /**< AMD 64-bit. */
    ITANIUM, /**< Itanium. */
    MIPS, /**< MIPS. */
    MIPS64, /**< MIPS 64-bit. */
    POWER_PC_32, /**< Power PC 32-bit. */
    POWER_PC_64, /**< Power PC 64-bit. */
    S390X, /** IBM subsystem 64-bit. */
    RS, /**< RS. */
    SPARC32, /**< Sparc 32-bit. */
    SPARC64, /**< Sparc 64-bit. */
    S390, /**< S390. */
    ARM32, /**< ARM 32-bit. */
    ARM64, /**< ARM 64-bit. */
    WASM32, /**< WebAssembly 32-bit. */
    WASM64 /**< WebAssembly 64-bit. */
  };
  
  /** The minor architecture. */
  enum Minor {
    GENERIC /**< Unspecified processor. */
  };
  
  /**
    Returns true if the native byte order is big endian.
  */
  static bool isBigEndian() noexcept
  {
    return NATIVE == BE;
  }

  /**
    Returns true if the native byte order is little endian.
  */
  static bool isLittleEndian() noexcept
  {
    return NATIVE == LE;
  }

  /**
    Returns the supported technologies.
  */
  static unsigned int getTechonologies() noexcept;
  
  /**
    Returns the major architecture.
  */
  static Major getArchitecture() noexcept;
  
  /**
    Returns the processor (lowest common denominator is multiple are present).
  */
  static Minor getMinorArchitecture() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
