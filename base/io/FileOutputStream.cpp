/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/FileOutputStream.h>

#if defined(__win32__)
  #include <windows.h>
#else // __unix
  #include <fcntl.h>
  #include <unistd.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileOutputStream::FileOutputStream(const String& p, unsigned int flags, unsigned int permissions) throw(FileNotFound) :
  FileDescriptorOutputStream(), path(p) {

#if defined(__win32__)
  HANDLE handle;
  handle = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (handle == INVALID_HANDLE_VALUE) {
    throw FileNotFound("Unable to open file for writing");
  }
  setHandle((int)handle);
#else // __unix__
  unsigned int temp = 0;
  if (flags & CREATE) {
    temp |= O_CREAT;
  }
  if (flags & TRUNCATE) {
    temp |= O_TRUNC;
  }
  if (flags & APPEND) {
    temp |= O_APPEND;
  }
  if (flags & NONBLOCK) {
    temp |= O_NONBLOCK;
  }
  if (flags & SYNC) {
    temp |= O_SYNC;
  }

  int handle = ::open(path, temp | O_WRONLY, permissions);
  if (handle == -1) {
    throw FileNotFound("Unable to open file from writing");
  }
  setHandle(handle);
#endif
}

const String& FileOutputStream::getPath() const throw() {
  return path;
}

void FileOutputStream::flush() throw(IOException) {
#if defined(__win32__)
  if (!FlushFileBuffers((HANDLE)getHandle())) {
    throw IOException("Unable to flush file");
  }
#else // __unix__
  if (fsync(getHandle())) {
    throw IOException("Unable to flush file");
  }
#endif
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const FileOutputStream& value) {
  return stream << "class/FileOutputStream{"
                << "path=" << value.getPath() << ";"
                << "fd=" << value.getHandle()
                << "}";
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
