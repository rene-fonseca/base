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
#include <base/string/WideString.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#  include <stdlib.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

String VirtualMemory::Module::getPath() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  PrimitiveStackArray<wchar> buffer(1024);
  while (buffer.size() < (64 * 1024)) {
    DWORD length = GetModuleFileNameW((HMODULE)context, buffer, buffer.size());
    if (length == 0) {
      if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        buffer.resize(buffer.size() * 2);
        continue;
      }
      break;
    }
    return String(buffer, length);
  }
  return String();
#else // unix
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif // flavor
}

VirtualMemory::Module VirtualMemory::Module::getProcessModule() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return Module((HMODULE)::GetModuleHandle(nullptr));
#else // unix
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif // flavor
}

bool VirtualMemory::Module::isModule() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  wchar buffer[1];
  unsigned int length = ::GetModuleFileName((HMODULE)context, buffer, getArraySize(buffer));
  return length > 0;
#else // unix
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif // flavor
}

MemorySize VirtualMemory::Module::getSize() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
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
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif // flavor
}

Array<VirtualMemory::Module> VirtualMemory::getModules() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  Array<Module> result;
  SYSTEM_INFO systemInformation;
  ::GetSystemInfo(&systemInformation);
  const unsigned int pageSize = systemInformation.dwPageSize;
  const uint8* address = nullptr;
  MEMORY_BASIC_INFORMATION info;
  while (false /*address < (const uint8*)0x80000000*/) { // TAG: OS limit
    if (::VirtualQuery(address, &info, sizeof(info)) == 0) {
      address += pageSize;
      continue;
    }
    if (info.Type & MEM_IMAGE) {
      wchar buffer[1];
      unsigned int length = ::GetModuleFileName((HMODULE)address, buffer, getArraySize(buffer));
      if (length > 0) {
        result.append(Module(address));
      }
    }
    address += (info.RegionSize+pageSize-1)/pageSize*pageSize;
  }
  return result;
#else // unix
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif // flavor
}

void VirtualMemory::query(const void* address) noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
  // DWORD result = ::VirtualQuery(address, &info, sizeof(info));
#else // unix
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif // flavor
}

void* VirtualMemory::getBase(const void* address) noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  MEMORY_BASIC_INFORMATION mbi;
  if (VirtualQuery(address, &mbi, sizeof(mbi))) {
    return mbi.AllocationBase;
  }
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
  return nullptr;
}

void VirtualMemory::dump() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // get page size
  SYSTEM_INFO info;
  ::GetSystemInfo(&info);
  const unsigned int pageSize = info.dwPageSize;
  
  const uint8* address = nullptr;
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
 
  fout << FLUSH;
#else // unix
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif // flavor
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
