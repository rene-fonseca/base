/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Architecture.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

unsigned int Architecture::getTechonologies() throw() {
  unsigned int result = 0;
#if (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86)
  unsigned int eflags;
  
  enum Flag {
    CF = 1 << 0,
    PF = 1 << 2,
    AF = 1 << 4,
    ZF = 1 << 6,
    SF = 1 << 7,
    TF = 1 << 8,
    IF = 1 << 9,
    DF = 1 << 10,
    OF = 1 << 11,
    IOPL = 3 << 12,
    NT = 1 << 14,
    RF = 1 << 16,
    VM = 1 << 17,
    AC = 1 << 18,
    VIF = 1 << 19,
    VIP = 1 << 20,
    ID = 1 << 21 // identification flag
  };
  
  asm (
    "        pushfl\n"
    "        pop %0"
    : "=m" (eflags) // output
  );
  if (eflags & ID) { // is CPUID supported
    unsigned int eax, ebx, ecx, edx; // result
    asm (
      "        pushl %%ebx\n" // TAG: fix for gcc under linux
      "        cpuid\n"
      "        movl %%ebx,%1\n"
      "        popl %%ebx\n"
      : "=a" (eax), "=m" (ebx), "=c" (ecx), "=d" (edx) // output
      : "a" (0) // input // ask for maximum input value and vendor string
    );
    const bool genuineIntel = (ebx == 0x756e6547) && (edx == 0x49656e69) && (ecx == 0x6c65746e);
    if (genuineIntel) {
      asm (
        "        pushl %%ebx\n" // TAG: fix for gcc under linux
        "        cpuid\n"
        "        movl %%ebx,%1\n"
        "        popl %%ebx\n"
        : "=a" (eax), "=m" (ebx), "=c" (ecx), "=d" (edx) // output
        : "0" (1) // input // ask for feature flags
      );
      
      result |= ((edx >> 28) & 1) ? HYPER_THREADING : 0;
      result |= ((edx >> 26) & 1) ? SSE2 : 0;
      result |= ((edx >> 25) & 1) ? SSE : 0;
      result |= ((edx >> 23) & 1) ? MMX : 0;
      result |= ((edx >> 0) & 1) ? FPU : 0;
    }
  }
#endif
  return result;
}

Architecture::Major Architecture::getArchitecture() throw() {
#if (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86)
  return X86;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__SPARC)
  return SPARC;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__MIPS)
  return MIPS;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ITANIUM)
  return ITANIUM;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ALPHA)
  return ALPHA;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__PPC)
  return POWER_PC;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__RS)
  return RS;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86_64)
  return X86_64;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__S390)
  return S390;
#else
#  error Unsupported architecture
#endif
}
  
Architecture::Minor Architecture::getMinorArchitecture() throw() {
  Architecture::Minor result = GENERIC;
#if (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86)
  result = X86_386;
  // TAG: add check for X86_486
  
  unsigned int eflags;

  enum Flag {
    CF = 1 << 0,
    PF = 1 << 2,
    AF = 1 << 4,
    ZF = 1 << 6,
    SF = 1 << 7,
    TF = 1 << 8,
    IF = 1 << 9,
    DF = 1 << 10,
    OF = 1 << 11,
    IOPL = 3 << 12,
    NT = 1 << 14,
    RF = 1 << 16,
    VM = 1 << 17,
    AC = 1 << 18,
    VIF = 1 << 19,
    VIP = 1 << 20,
    ID = 1 << 21 // identification flag
  };
  
  asm (
    "        pushfl\n"
    "        pop %0"
    : "=m" (eflags) // output
  );
  if (eflags & ID) { // is CPUID supported
    unsigned int eax, ebx, ecx, edx; // result
    asm (
      "        pushl %%ebx\n" // TAG: fix for gcc under linux
      "        cpuid\n"
      "        movl %%ebx,%1\n"
      "        popl %%ebx\n"
      : "=a" (eax), "=m" (ebx), "=c" (ecx), "=d" (edx) // output
      : "a" (0) // input // ask for maximum input value and vendor string
    );

    const bool genuineIntel = (ebx == 0x756e6547) && (edx == 0x49656e69) && (ecx == 0x6c65746e);
    if (genuineIntel) {

      if (eax > 0) {
        asm (
          "        pushl %%ebx\n" // TAG: fix for gcc under linux
          "        cpuid\n"
          "        movl %%ebx,%0\n"
          "        popl %%ebx\n"
          : "=a" (eax), "=m" (ebx), "=c" (ecx), "=d" (edx) // output
          : "0" (1) // input // ask for version information
        );
        
        const unsigned int type = (eax >> 12) & 0x3;
        const unsigned int family = (eax >> 8) & 0xf;
        const unsigned int model = (eax >> 4) & 0xf;
        const unsigned int step = (eax >> 0) & 0xf;

        switch (family) {
        case 0x4:
          result = X86_486;
          break;
        case 0x5:
          result = X86_PENTIUM;
          break;
        case 0x6:
          switch (model) {
          case 0x1:
            result = X86_PENTIUM_PRO;
            break;
          case 0x3:
          case 0x5:
            result = X86_PENTIUM_II;
            break;
          case 0x7:
          case 0x8:
          case 0x10:
          case 0x11:
            result = X86_PENTIUM_III;
            break;
          }
          break;
        case 0xf:
          result = X86_PENTIUM_IV;
          break;
        }
      }
    }
  }
#endif
  return result;
}

// TAG: need support for multiple processors (and hyper-threading)
// struct ProcessorInformation {major, minor, #logical processors, technologies};

// Array is no good in Architecture header
// Array<Architecture::Minor> Architecture::getProcessors() throw() {
//   Array<Minor> result;
//   return result;
// }

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
