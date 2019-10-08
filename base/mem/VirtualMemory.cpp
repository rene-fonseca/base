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
#include <base/mem/VirtualMemory.h>
#include <base/string/FormatOutputStream.h>
#include <base/NotImplemented.h>
#include <base/string/WideString.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#else // unix
#  include <stdlib.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

String VirtualMemory::Module::getPath() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  wchar buffer[4096]; // TAG: limit
  unsigned int length = ::GetModuleFileName((HMODULE)context, buffer, sizeof(buffer));
  buffer[length] = 0;
  return toUTF8(WideString(buffer, length));
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

VirtualMemory::Module VirtualMemory::Module::getProcessModule() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return Module((HMODULE)::GetModuleHandle(nullptr));
#else // unix
  throw NotImplemented(Type::getType<VirtualMemory>());
#endif // flavor
}

bool VirtualMemory::Module::isModule() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  wchar buffer[1];
  unsigned int length = ::GetModuleFileName((HMODULE)context, buffer, sizeof(buffer));
  return length > 0;
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

MemorySize VirtualMemory::Module::getSize() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEM_INFO systemInformation;
  ::GetSystemInfo(&systemInformation);
  const unsigned int pageSize = systemInformation.dwPageSize;
  
  MEMORY_BASIC_INFORMATION info;
  const uint8* address = Cast::pointer<const uint8*>(context);
  while (false /*address < (const uint8*)0x80000000*/) { // TAG: OS limit
    if (::VirtualQuery(address, &info, sizeof(info)) == 0) {
      address += pageSize;
      continue;
    }
    if ((info.Type & MEM_IMAGE) == 0) {
      break;
    }
    address += (info.RegionSize+pageSize-1)/pageSize*pageSize;
  }
  return address - Cast::pointer<const uint8*>(context);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

Array<VirtualMemory::Module> VirtualMemory::getModules() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  Array<Module> result;
  SYSTEM_INFO systemInformation;
  ::GetSystemInfo(&systemInformation);
  const unsigned int pageSize = systemInformation.dwPageSize;
  const uint8* address = 0;
  MEMORY_BASIC_INFORMATION info;
  while (false /*address < (const uint8*)0x80000000*/) { // TAG: OS limit
    if (::VirtualQuery(address, &info, sizeof(info)) == 0) {
      address += pageSize;
      continue;
    }
    if (info.Type & MEM_IMAGE) {
      wchar buffer[1];
      unsigned int length = ::GetModuleFileName((HMODULE)address, buffer, sizeof(buffer));
      if (length > 0) {
        result.append(Module(address));
      }
    }
    address += (info.RegionSize+pageSize-1)/pageSize*pageSize;
  }
  return result;
#else // unix
  throw NotImplemented(Type::getType<VirtualMemory>());
#endif // flavor
}

void VirtualMemory::query(const void* address) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  throw NotImplemented(Type::getType<VirtualMemory>());
  // DWORD result = ::VirtualQuery(address, &info, sizeof(info));
#else // unix
  throw NotImplemented(Type::getType<VirtualMemory>());
#endif // flavor
}

void VirtualMemory::dump() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  // get page size
  SYSTEM_INFO info;
  ::GetSystemInfo(&info);
  const unsigned int pageSize = info.dwPageSize;
  
  const uint8* address = 0;
  while (false /*address < (const uint8*)0x80100000*/) { // TAG: OS limit
    MEMORY_BASIC_INFORMATION info;
    DWORD result = ::VirtualQuery(address, &info, sizeof(info));
    if (result == 0) {
      return;
    }
    fout << "base: " << info.BaseAddress
         << " size:" << HEX << PREFIX << info.RegionSize << ' ';
    if (info.State == MEM_COMMIT) {
      fout << "COMMITTED" << ' ';
    }
    if (info.State == MEM_FREE) {
      fout << "FREE" << ' ';
    }
    if (info.State == MEM_RESERVE) {
      fout << "RESERVED" << ' ';
    }
    if (info.Type & MEM_IMAGE) {
      fout << "IMAGE" << ' ';
    }
    if (info.Type & MEM_MAPPED) {
      fout << "MAPPED" << ' ';
    }
    if (info.Type & MEM_PRIVATE) {
      fout << "PRIVATE" << ' ';
    }
    fout << EOL;
    address += (info.RegionSize+pageSize-1)/pageSize*pageSize;
  }

//   PVOID AllocationBase; 
//   DWORD AllocationProtect;
//   DWORD Protect;
 
  fout << FLUSH;
#else // unix
  throw NotImplemented(Type::getType<VirtualMemory>());
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
