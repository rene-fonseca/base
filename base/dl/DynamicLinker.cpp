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
#include <base/dl/DynamicLinker.h>
#include <base/Application.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#  include <dbghelp.h>
#else // unix
#  include <dlfcn.h>
#if 0
#  if !defined(RDLD_LAZY) || !defined(RDLD_NOW) || !defined(RTLD_GLOBAL) || !defined(RTDL_LOCAL)
#    warning dlfcn.h is not compliant with IEEE Std 1003.1-2001
#  endif
#endif
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)

namespace {

  typedef BOOL (__stdcall* SymInitializeFunc)(HANDLE hProcess, PCSTR UserSearchPath, BOOL fInvadeProcess);
  typedef BOOL (__stdcall* SymCleanupFunc)(HANDLE hProcess);
  typedef BOOL (__stdcall* SymFromAddrFunc)(HANDLE hProcess, DWORD64 Address, PDWORD64 Displacement, PSYMBOL_INFO Symbol);
  typedef DWORD(__stdcall* SymGetOptionsFunc)();
  typedef DWORD(__stdcall* SymSetOptionsFunc)(DWORD SymOptions);

  bool initialized = false;
  HMODULE handle = nullptr;
  SymInitializeFunc symInitialize = nullptr;
  SymCleanupFunc symCleanup = nullptr;
  SymFromAddrFunc symFromAddr = nullptr;
  SymGetOptionsFunc symGetOptions = nullptr;
  SymSetOptionsFunc symSetOptions = nullptr;

  class Initialize {
  public:

    ~Initialize() {
      if (symCleanup) {
        symCleanup(GetCurrentProcess());

        symInitialize = nullptr;
        symCleanup = nullptr;
        symFromAddr = nullptr;
      }
      if (handle) {
        FreeLibrary(handle);
        handle = nullptr;
      }
    }
  };

  Initialize initialize;

  void loadDbgHelp()
  {
    if (!initialized) {
      initialized = true;
      handle = ::LoadLibraryExW(L"dbghelp.dll", 0, 0); // TAG: use DynamicLibrary class
      symInitialize = (SymInitializeFunc)GetProcAddress(handle, "SymInitialize");
      if (symInitialize) {
        symCleanup = (SymCleanupFunc)GetProcAddress(handle, "SymCleanup");
        if (symCleanup) {
          BOOL status = symInitialize(GetCurrentProcess(), NULL, TRUE || FALSE); // we want to load on demand instead

          symGetOptions = (SymGetOptionsFunc)GetProcAddress(handle, "SymGetOptions");
          symSetOptions = (SymSetOptionsFunc)GetProcAddress(handle, "SymSetOptions");
          if (symSetOptions && symGetOptions) {
            symSetOptions((symGetOptions() | SYMOPT_UNDNAME | 0*SYMOPT_LOAD_LINES | SYMOPT_DEBUG | 0*SYMOPT_LOAD_ANYTHING) /*& ~SYMOPT_UNDNAME*/);
          }

          symFromAddr = (SymFromAddrFunc)GetProcAddress(handle, "SymFromAddr");
        }
      }
    }
  }
}

#endif

void* DynamicLinker::getGlobalSymbolImpl(const String& symbol) throw(LinkerException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // GetModuleHandle does not increment reference count
  void* result = (void*)(::GetProcAddress(::GetModuleHandle(0), symbol.getElements()));
  bassert(result != 0, LinkerException("Unable to resolve symbol"));
  return result;
#else // unix
  #if defined(RTLD_LAZY)
    void* handle = ::dlopen(nullptr, RTLD_LAZY);
  #else
    void* handle = ::dlopen(nullptr, 0); // use unspecified linking
  #endif
  if (handle == 0) {
    throw LinkerException("Unable to open module");
  }
  void* result = ::dlsym(handle, symbol.getElements());
  bassert(dlerror() == 0, LinkerException("Unable to resolve symbol"));
  ::dlclose(handle); // is this required
  return result;
#endif // flavor
}

// TAG: get path of module support
// TAG: enumerate modules support

//String DynamicLinker::getPath() noexcept
//{
//#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
//  String path(MAX_PATH); // maximum path length in ANSI mode
//  DWORD length = ::GetModuleFileNameEx(
//                   ::GetCurrentProcess(), // get pseudo handle
//                   ::GetModuleHandle(0), // get handle of process image
//                   path.getBytes(), // path buffer
//                   MAX_PATH
//  );
//  if (length == 0) {
//    throw LinkerException(this);
//  }
//  path.setLength(length);
//  return path;
//#else // unix
//  return String();
//#endif // flavor
//}

DynamicLinker::DynamicLinker(const String& path, unsigned int options) throw(LinkerException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if ((handle = ::LoadLibraryEx(toWide(path).c_str(), 0, 0)) == nullptr) {
    throw LinkerException("Unable to open module", this);
  }
#else // unix
  #if defined(RTLD_LAZY)
    int flags = (options & LAZY) ? RTLD_LAZY : RTLD_NOW;
  #else
    int flags = RTLD_NOW; // we only end up here if dlfcn.h is non-compliant
  #endif
  #if defined(RTLD_LOCAL)
    flags |= (options & GLOBAL) ? RTLD_GLOBAL : RTLD_LOCAL;
  #else
    flags |= RTLD_GLOBAL;
  #endif
  if ((handle = ::dlopen(path.getElements(), flags)) == 0) {
    throw LinkerException("Unable to open module", this);
  }
#endif // flavor
}

void* DynamicLinker::getSymbol(const Literal& symbol) const throw(LinkerException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  void* result = (void*)(::GetProcAddress((HMODULE)handle, symbol.getValue()));
  bassert(result != nullptr, LinkerException("Unable to resolve symbol", this));
  return result;
#else // unix
  void* result = ::dlsym(handle, symbol.getValue());
  bassert(dlerror() == 0, LinkerException("Unable to resolve symbol", this));
  return result;
#endif // flavor
}

void* DynamicLinker::getSymbol(const String& symbol) const throw(LinkerException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  void* result = (void*)(::GetProcAddress((HMODULE)handle, symbol.getElements()));
  bassert(result != 0, LinkerException("Unable to resolve symbol", this));
  return result;
#else // unix
  void* result = ::dlsym(handle, symbol.getElements());
  bassert(dlerror() == 0, LinkerException("Unable to resolve symbol", this));
  return result;
#endif // flavor
}

void* DynamicLinker::getUncertainSymbol(const Literal& symbol) const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return (void*)(::GetProcAddress((HMODULE)handle, symbol.getValue()));
#else // unix
  return ::dlsym(handle, symbol.getValue());
#endif // flavor
}

void* DynamicLinker::getUncertainSymbol(const String& symbol) const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return (void*)(::GetProcAddress((HMODULE)handle, symbol.getElements()));
#else // unix
  return ::dlsym(handle, symbol.getElements());
#endif // flavor
}

bool DynamicLinker::import(
  StaticFunctionDescriptor* functions,
  unsigned int numberOfFunctions,
  bool flags) noexcept
{
  bool result = true;
  for (unsigned int i = 0; i < numberOfFunctions; ++i) {
    void* address = getUncertainSymbol(NativeString(functions[i].symbol));
    if (!address) {
      result = false;
      if ((flags & CONTINUE) == 0) {
        break;
      }
    }
    *functions[i].function = (Function)address;
  }
  return result;
}

DynamicLinker::~DynamicLinker()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(
    ::FreeLibrary((HMODULE)handle),
    LinkerException("Unable to close module", this)
  );
#else // unix
  bassert(
    ::dlclose(handle) == 0,
    LinkerException("Unable to close module", this)
  );
#endif // flavor
}

String DynamicLinker::getImagePath(const void* address)
{
  String result;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  MutualExclusion::Sync _guard(Application::getApplication()->getLock()); // dbghelp is not MT-safe
  loadDbgHelp();
  if (symFromAddr) {
    DWORD64 displacement = 0;
    SYMBOL_INFO info;
    info.SizeOfStruct = sizeof(SYMBOL_INFO);
    info.MaxNameLen = 0;
    if (INLINE_ASSERT(symFromAddr(GetCurrentProcess(), reinterpret_cast<MemorySize>(address), &displacement, &info))) {
      HMODULE handle = reinterpret_cast<HMODULE>(static_cast<MemorySize>(info.ModBase));
      PrimitiveArray<wchar> buffer(1024);
      while (buffer.size() < (64 * 1024)) {
        DWORD length = GetModuleFileNameW(handle, buffer, buffer.size());
        if (length == 0) {
          if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
            buffer.resize(buffer.size() * 2);
            continue;
          }
          break;
        }
        result = String(buffer, length);
        break;
      }
    }
  }
#elif ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS) || \
       (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX))
  Dl_info info;
  int status = dladdr(address, &info);
  if (status) {
    return info.dli_fname;
  }
#else
  BASSERT(!"Not implemented");
#endif
  return result;
}

void* DynamicLinker::getImageAddress(const void* address) noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  MutualExclusion::Sync _guard(Application::getApplication()->getLock()); // dbghelp is not MT-safe
  loadDbgHelp();
  if (symFromAddr) {
    DWORD64 displacement = 0;
    SYMBOL_INFO info;
    info.SizeOfStruct = sizeof(SYMBOL_INFO);
    info.MaxNameLen = 0;
    if (INLINE_ASSERT(symFromAddr(GetCurrentProcess(), reinterpret_cast<MemorySize>(address), &displacement, &info))) {
      return reinterpret_cast<void*>(static_cast<MemorySize>(info.ModBase));
    }
  }
#elif ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS) || \
       (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX))
  Dl_info info;
  int status = dladdr(address, &info);
  if (status) {
    return info.dli_fbase;
  }
#else
  BASSERT(!"Not implemented");
#endif
  return nullptr;
}

void* DynamicLinker::getSymbolAddress(const void* address) noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  MutualExclusion::Sync _guard(Application::getApplication()->getLock()); // dbghelp is not MT-safe
  loadDbgHelp();
  if (symFromAddr) {
    DWORD64 displacement = 0;
    SYMBOL_INFO info;
    info.SizeOfStruct = sizeof(SYMBOL_INFO);
    info.MaxNameLen = 0;
    BASSERT(sizeof(info.Name[0]) == 1); // char - not wchar

    // TAG: ensure we load symbols if available and not yet loaded
    BOOL status = symFromAddr(GetCurrentProcess(), reinterpret_cast<MemorySize>(address), &displacement, &info);
    if (!status) {
      DWORD error = ::GetLastError();
      return nullptr;
    }
    return reinterpret_cast<void*>(static_cast<MemorySize>(info.Address));
  }
#elif ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS) || \
       (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX))
  Dl_info info;
  int status = dladdr(address, &info);
  if (status) {
    return info.dli_saddr;
  }
#else
  BASSERT(!"Not implemented");
#endif
  return nullptr;
}

#if 0 && (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)

#pragma comment(lib, "dbghelp.lib") // TAG: temp test

namespace {

  // TAG: just get symbol from dll export table if available? - map address -> index in export table -> to name
  String demangle(const char* decorated)
  {
    char buffer[1024];
    DWORD flags = UNDNAME_NO_THISTYPE | UNDNAME_NO_MS_KEYWORDS | UNDNAME_NO_SPECIAL_SYMS | UNDNAME_NO_THROW_SIGNATURES |
      UNDNAME_NO_ACCESS_SPECIFIERS | // no public/private
      UNDNAME_NO_MEMBER_TYPE; // no virtual/static
    DWORD size = UnDecorateSymbolName(decorated, buffer, getArraySize(buffer), flags);
    if (size > 0) {
      return String(buffer, size);
    }
    return String();
  }

  // unsigned int getIndexByAddress(HMODULE handle, const void* address);
  // const char* getNameByIndex(HMODULE handle, unsigned int index);
  // const char* getNameByAddress(HMODULE handle, const void* address);

  unsigned int dumpNames(HMODULE handle, const void* address)
  {
    IMAGE_DOS_HEADER* dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(handle);
    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
      return 0;
    }
    const uint8* start = reinterpret_cast<const uint8*>(dosHeader);
    const IMAGE_NT_HEADERS* ntHeaders = reinterpret_cast<const IMAGE_NT_HEADERS*>(start + dosHeader->e_lfanew);
    if (ntHeaders->Signature != 0x00004550) {
      return 0;
    }
    const IMAGE_OPTIONAL_HEADER* optionalHeader = &ntHeaders->OptionalHeader;
    if (optionalHeader->NumberOfRvaAndSizes < IMAGE_DIRECTORY_ENTRY_EXPORT) {
      return 0;
    }
    if (optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size == 0) {
      return 0;
    }
    const IMAGE_DATA_DIRECTORY* dataDirectory = &optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    const IMAGE_EXPORT_DIRECTORY* exportDirectory = reinterpret_cast<const IMAGE_EXPORT_DIRECTORY*>(start + dataDirectory->VirtualAddress);

    const DWORD count = exportDirectory->NumberOfNames;
    const DWORD* functions = reinterpret_cast<const DWORD*>(start + exportDirectory->AddressOfFunctions);
    const DWORD* names = reinterpret_cast<const DWORD*>(start + exportDirectory->AddressOfNames);

    WORD ordinalBase = 1; // TAG: get from exportDirectory?
    for (DWORD i = 1; i < count; ++i) {
      // LPCSTR ordinal = reinterpret_cast<LPCSTR>(static_cast<MemorySize>(i + ordinalBase));
      // void* fromproc = GetProcAddress(handle, ordinal);

      const void* _address = functions[i] + start;
      const char* _name = reinterpret_cast<const char*>(names[i] + start);
      const String name(_name);
      const String demangled = demangle(_name);
      if (name.indexOf("OutOfRange") >= 0) {
        printf("%d = %p = %s\n", i, _address, demangled.native()/*, _name*/);
      }
      // TAG: is this a forwarded address pointer?
      if (_address == address) {
        return i - 1;
      }
    }

    return 0;
  }
}
#endif

String DynamicLinker::getSymbolName(const void* address)
{
  String result;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  MutualExclusion::Sync _guard(Application::getApplication()->getLock()); // dbghelp is not MT-safe
  loadDbgHelp();
  if (symFromAddr) {
    DWORD64 displacement = 0;
    const unsigned int MAXIMUM_NAME = 4096; // nested templates can give very long names
    PrimitiveArray<uint8> buffer(MAXIMUM_NAME + sizeof(SYMBOL_INFO) - 1);
    SYMBOL_INFO* info = reinterpret_cast<SYMBOL_INFO*>(static_cast<uint8*>(buffer));
    info->SizeOfStruct = sizeof(SYMBOL_INFO);
    info->MaxNameLen = MAXIMUM_NAME;
    BOOL status = symFromAddr(GetCurrentProcess(), reinterpret_cast<MemorySize>(address), &displacement, info);
    // TAG: missing type info

    if (!status) {
      DWORD error = ::GetLastError();
    } else {

#if 0
      static bool first = true;
      if (first) {
        first = false;
        HMODULE module = (HMODULE)(info->ModBase);// GetModuleHandle(L"base.dll");
        dumpNames(module, reinterpret_cast<uint8*>(info->Address));
      }
#endif

#if 0
      // TAG: add support for source and line info in stack trace
      IMAGEHLP_LINE64 line;
      line.SizeOfStruct = sizeof(line);
      DWORD disp = 0;
      status = SymGetLineFromAddr(GetCurrentProcess(), reinterpret_cast<MemorySize>(address), &disp, &line);
#endif
      result = String(info->Name, info->NameLen);
    }
  }
#elif ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS) || \
       (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX))
  Dl_info info;
  int status = dladdr(address, &info);
  if (status) {
    return info.dli_sname;
  }
#else
  BASSERT(!"Not implemented");
#endif
  return result;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
