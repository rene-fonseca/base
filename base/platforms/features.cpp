/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>

#if ((_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__UNIX) || \
     (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32))
#else
#  error Invalid flavor (FLAVOR) specification
#endif

#if ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__UNSPECIFIED) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WINNT4) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__W2K) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WXP) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__BEOS) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__AIX) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREEBSD) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__OPENBSD) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WEBASSEMBLY) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__EMCC) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IOS))
#else
#  error Invalid operating system (OS) specification
#endif

#if ((_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__UNSPECIFIED) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__SPARC32) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__SPARC64) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__MIPS) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__MIPS64) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ITANIUM) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ALPHA) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__POWERPC32) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__POWERPC64) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__S390X) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__RS) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86_64) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__S390) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ARM32) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ARM64) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__WASM32) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__WASM64))
#else
#  error Invalid architecture (ARCH) specification
#endif

#if (!defined(_COM_AZURE_DEV__BASE__CPP_BOOL))
#  error Compiler does not support the bool primitive
#endif

#if (!defined(_COM_AZURE_DEV__BASE__CPP_CONST))
#  error Compiler does not support the const qualifier
#endif

#if (!defined(_COM_AZURE_DEV__BASE__CPP_RESTRICT) && !defined(restrict))
#  if defined(_COM_AZURE_DEV__BASE__COMPILER_RESTRICT)
#    warning Using alternative restrict qualifier keyword
#  else
#    warning Compiler does not support restrict qualifier
#  endif
#endif

#if (!defined(_COM_AZURE_DEV__BASE__CPP_INLINE) && !defined(inline))
#  if defined(_COM_AZURE_DEV__BASE__COMPILER_INLINE)
#    warning Using alternative inline specifier keyword
#  else
#    warning Compiler does not support inline specifier
#  endif
#endif

#if (!defined(_COM_AZURE_DEV__BASE__CPP_STATIC_CONST))
#  error Compiler does not support initialization of static constants inside class definition
#endif

#if (!defined(_COM_AZURE_DEV__BASE__CPP_ANONYMOUS))
#  warning Compiler does not support anonymous structures (and unions)
#endif

#if (!defined(_COM_AZURE_DEV__BASE__CPP_SPECIFICATION))
#  error Compiler does not support exception specifications
#endif

#if 0
#if (_COM_AZURE_DEV__BASE__COMPILER_ABI != _COM_AZURE_DEV__BASE__ABI)
#  warning ABI of compiler is different from configured ABI
#endif

#if (!defined(_COM_AZURE_DEV__BASE__ARCH_ABI))
#  warning Unsupported architecture ABI
#endif
#endif

_COM_AZURE_DEV__BASE__DUMMY_SYMBOL

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#include <string.h>
#include <stdlib.h>

void dumpMemory(size_t offset, size_t size)
{
  const auto end = offset + size;
  char* src = 0;
  for (auto i = offset/64; i < end/64; ++i) {
    _COM_AZURE_DEV__BASE__PRINT("%6lx: ", i * 64);
    for (unsigned int j = 0; j < 64; ++j) {
      char ch = src[i * 64 + j];
      if (ch < 0x20) {
        ch = '.';
      } else if (ch >= 0x7f) {
        ch = '.';
      }
      _COM_AZURE_DEV__BASE__PRINT("%c", ch);
    }
    _COM_AZURE_DEV__BASE__PRINT("\n");
  }
}

void recurse(unsigned int count)
{
  if (count == 0) {
    return;
  }
  unsigned int a = (count & 1) ? 0x32323232 : 0x42424242;
  //void* p = (void*)&recurse;
  unsigned int q = (count & 1) ? 0x31313131 : 0x41414141;
  _COM_AZURE_DEV__BASE__PRINT("!!! recurse1 stack=%p count=%d func=%p\n", (void*)&recurse, count, (void*)&recurse);
  unsigned int* src = (unsigned int*)&a;
  for (unsigned int i = 0; i < 16; ++i) {
    _COM_AZURE_DEV__BASE__PRINT("  frame %2d = %08x = %d\n", i, ((unsigned int*)&a)[i], ((unsigned int*)&a)[i]);
  }
  recurse(count - 1);
}

extern "C" void* __cxa_allocate_exception(size_t thrown_size) noexcept
{
  _COM_AZURE_DEV__BASE__PRINT("__cxa_allocate_exception %ld\n", thrown_size);
  recurse(10);
  dumpMemory(1024, 1024);
  return malloc(thrown_size);
}

extern "C" void __cxa_throw(void* thrown_exception, void* /*struct std::type_info **/ tinfo, void (*dest)(void*))
{
  _COM_AZURE_DEV__BASE__PRINT("__cxa_throw\n");
}

extern "C" void __cxa_rethrow()
{
  _COM_AZURE_DEV__BASE__PRINT("__cxa_rethrow\n");
}

#endif
