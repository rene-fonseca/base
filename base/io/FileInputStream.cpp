/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/FileInputStream.h>

#if defined(__win32__)
  #include <windows.h>
#else // __unix
  #include <sys/types.h>
  #include <fcntl.h>
  #include <unistd.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileInputStream::FileInputStream(const String<>& p, unsigned int flags) throw(FileNotFound) :
  path(p) {
#if defined(__win32__)
  HANDLE handle; 
  handle = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL,
    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (handle == INVALID_HANDLE_VALUE) {
    throw FileNotFound("Unable to open file for reading");
  }
  setHandle((int)handle);
#else // __unix__
  unsigned int temp = 0;
  if (flags & NONBLOCK) {
    temp |= O_NONBLOCK;
  }
  int handle = ::open(path, temp | O_RDONLY);
  if (handle == -1) {
    throw FileNotFound("Unable to open file for reading");
  }
  setHandle(handle);
#endif
}

unsigned int FileInputStream::available() const throw(IOException) {
#if defined(__win32__)
  // are 'long long' and LARGE_INTEGER compatible on big endian architectures
  DWORD size = GetFileSize((HANDLE)getHandle(), NULL);
  if ((size == INVALID_FILE_SIZE) && (GetLastError() != NO_ERROR)) {
    throw IOException("Unable to get available bytes");
  }
  DWORD position = SetFilePointer((HANDLE)getHandle(), 0, NULL, FILE_CURRENT);
  // INVALID_SET_FILE_POINTER not defined
  if ((position == 0xffffffff) && (GetLastError() != NO_ERROR)) {
    throw IOException("Unable to get available bytes");
  }
// Windows 2000 only
//  long long size;
//  long long position;
//    if (!GetFileSizeEx((HANDLE)getHandle(), &size) || !SetFilePointerEx((HANDLE)getHandle(), 0, &position, FILE_CURRENT)) {
//      throw IOException("Unable to get available bytes");
//    }
  return size - position;
#else // __unix__
  struct stat status;
  off_t position;
  if ((fstat(getHandle(), &status) != 0) || ((position = ::lseek(getHandle(), 0, SEEK_CUR)) == -1)) {
    throw IOException("Unable to get available bytes");
  }
  return status.st_size - position;
#endif
}

const String<>& FileInputStream::getPath() const throw() {
  return path;
}

void FileInputStream::wait() const throw(IOException) {
  // now what
}

bool FileInputStream::wait(unsigned int timeout) const throw(IOException) {
  // now what
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const FileInputStream& value) {
  return stream << "class/FileInputStream{"
                << "path=" << value.getPath() << ";"
                << "fd=" << value.getHandle()
                << "}";
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
