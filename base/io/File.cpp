/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/io/File.h>
#include <base/io/EndOfFile.h>
#include <base/concurrency/Thread.h>
#include <base/Base.h>
#include <base/Functor.h>
#include <stdio.h>

#if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  #define _LARGEFILE64_SOURCE 1
#endif

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #include <base/platforms/win32/AsyncReadFileContext.h> // platform specific
  #include <base/platforms/win32/AsyncWriteFileContext.h> // platform specific
  #include <windows.h>
#else // unix
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <sys/time.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <errno.h>
  #include <limits.h>
  #include <string.h> // required by FD_SET on solaris
  #include <sys/mman.h>

  #undef assert
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  inline Date FileTimeToDate(const FILETIME& time) {
    return Date((*pointer_cast<const long long*>(&time) - 116444736000000000LL)/10000000); // TAG: 0x0000001c1a021060LL
  }
#endif // flavor

File::FileHandle::~FileHandle() throw(FileException) {
  // TAG: throw exception if region of file is still locked
  if (isValid()) { // dont try to close if handle is invalidated
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    if (!::CloseHandle(getHandle())) {
      throw FileException("Unable to close file", this);
    }
#else // unix
    if (::close(getHandle())) {
      throw FileException("Unable to close file", this);
    }
#endif // flavor
  }
}

File::File() throw() : fd(File::FileHandle::invalid) {}

File::File(const String& path, Access access, unsigned int options) throw(AccessDenied, FileNotFound) : fd(File::FileHandle::invalid) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD creationFlags;
  switch (options & (CREATE | TRUNCATE)) {
  case 0:
    creationFlags = OPEN_EXISTING;
    break;
  case CREATE:
    creationFlags = (options & EXCLUSIVE) ? CREATE_NEW : OPEN_ALWAYS;
    break;
  case TRUNCATE:
    creationFlags = TRUNCATE_EXISTING;
    break;
  case CREATE | TRUNCATE:
    creationFlags = (options & EXCLUSIVE) ? CREATE_NEW : CREATE_ALWAYS;
    break;
  }

  OperatingSystem::Handle handle = ::CreateFile( // TAG: check out FILE_FLAG_POSIX_SEMANTICS
    path.getElements(),
    (access == READ) ? GENERIC_READ : ((access == WRITE) ? GENERIC_WRITE : (GENERIC_READ | GENERIC_WRITE)),
    (options & EXCLUSIVE) ? 0 : (FILE_SHARE_READ | FILE_SHARE_WRITE),
    0,
    creationFlags,
    FILE_ATTRIBUTE_NORMAL | ((options & ASYNCHRONOUS) ? FILE_FLAG_OVERLAPPED : 0),
    0
  );
  if (handle == OperatingSystem::INVALID_HANDLE) {
    if (::GetLastError() == ERROR_ACCESS_DENIED) {
      throw AccessDenied(this);
    } else {
      throw FileNotFound("Unable to open file", this);
    }
  }
  fd = new FileHandle(handle);
#else // unix
  // TAG: exclusive file locking problem for NFS
#if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  int flags = O_LARGEFILE;
#else
  int flags = 0;
#endif

  switch (access) {
  case READ:
    flags |= O_RDONLY;
    break;
  case WRITE:
    flags |= O_WRONLY;
    break;
  case READWRITE:
    flags |= O_RDWR;
    break;
  }

  if (options & CREATE) {
    flags |= O_CREAT;
  }
  if (options & TRUNCATE) {
    flags |= O_TRUNC;
  }
  if (options & EXCLUSIVE) {
    flags |= O_EXCL;
  }

  OperatingSystem::Handle handle;
  if ((handle = ::open(path.getElements(), flags, S_IRUSR | S_IWUSR | S_IRGRP)) == OperatingSystem::INVALID_HANDLE) {
    if (errno == EACCES) {
      throw AccessDenied(this);
    } else {
      throw FileNotFound("Unable to open file", this);
    }
  }
  fd = new FileHandle(handle);
#endif
  if (options & File::APPEND) {
    setPosition(0, File::END);
  }
}

File& File::operator=(const File& eq) throw() {
  fd = eq.fd; // no need to protect against self-assignment
  return *this;
}

void File::close() throw(FileException) {
  fd = FileHandle::invalid; // invalidate
}

bool File::isClosed() const throw() {
  return !fd->isValid();
}

long long File::getSize() const throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ULARGE_INTEGER size;
  size.LowPart = ::GetFileSize(fd->getHandle(), &size.HighPart);
  if ((size.LowPart == INVALID_FILE_SIZE) && (::GetLastError() != NO_ERROR )) {
    throw FileException("Unable to get file size", this);
  }
//  LARGE_INTEGER size; // TAG: unresolved possible byte order problem for big endian architectures
//  if (!::GetFileSizeEx(fd->getHandle(), &size)) {
//    throw FileException("Unable to get file size", this);
//  }
  return size.QuadPart;
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    if (::fstat64(fd->getHandle(), &status)) {
      throw FileException("Unable to get file size", this);
    }
    fout << "size of status: " << sizeof(status) << ENDL;
    fout << "offset of size: " << (char*)&status.st_size - (char*)&status << ENDL;
    for (int i = 0; i < sizeof(status); ++i) {
      fout << HEX << static_cast<unsigned int>(((const unsigned char*)(&status))[i]) << " ";
    }
    printf("File size = %ld\n", status.st_size);
    return status.st_size;
  #else
    struct stat status;
    if (::fstat(fd->getHandle(), &status)) {
      throw FileException("Unable to get file size", this);
    }
    return status.st_size;
  #endif
#endif
}

long long File::getPosition() const throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  LARGE_INTEGER position;
  position.QuadPart = 0;
  position.LowPart = ::SetFilePointer(fd->getHandle(), 0, &position.HighPart, FILE_CURRENT);
  if ((position.LowPart == INVALID_SET_FILE_POINTER) && (::GetLastError() != NO_ERROR)) {
    throw FileException("Unable to get file position", this);
  }
//  if (!::SetFilePointerEx(fd->getHandle(), 0, &position, FILE_CURRENT)) {
//    throw FileException("Unable to get file position", this);
//  }
  return position.QuadPart;
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    return ::lseek64(fd->getHandle(), 0, SEEK_CUR); // should never fail
  #else
    return ::lseek(fd->getHandle(), 0, SEEK_CUR); // should never fail
  #endif
#endif
}

void File::setPosition(long long position, Whence whence) throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  static DWORD relativeTo[] = {FILE_BEGIN, FILE_CURRENT, FILE_END};
  LARGE_INTEGER temp;
  temp.QuadPart = position;
  temp.LowPart = ::SetFilePointer(fd->getHandle(), temp.LowPart, &temp.HighPart, relativeTo[whence]);
  if ((temp.LowPart == INVALID_SET_FILE_POINTER) && (::GetLastError() != NO_ERROR)) {
    throw FileException("Unable to get file position", this);
  }
//  if (!SetFilePointerEx(fd->getHandle(), position, 0, relativeTo[whence])) {
//    throw FileException("Unable to set position", this);
//  }
#else // unix
  static int relativeTo[] = {SEEK_SET, SEEK_CUR, SEEK_END};
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    ::lseek64(fd->getHandle(), position, relativeTo[whence]); // should never fail
  #else
    assert(position <= PrimitiveTraits<int>::MAXIMUM, FileException("Unable to set position", this));
    ::lseek(fd->getHandle(), position, relativeTo[whence]); // should never fail
  #endif
#endif
}

void File::truncate(long long size) throw(FileException) {
  long long oldSize = File::getSize();
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  setPosition(size);
  if (!::SetEndOfFile(fd->getHandle())) {
    throw FileException("Unable to truncate", this);
  }
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    if (::ftruncate64(fd->getHandle(), size)) {
      throw FileException("Unable to truncate", this);
    }
  #else
    assert((size >= 0) && (size <= PrimitiveTraits<int>::MAXIMUM), FileException("Unable to truncate", this));
    if (::ftruncate(fd->getHandle(), size)) {
      throw FileException("Unable to truncate", this);
    }
  #endif
  if (File::getSize() > oldSize) { // has file been extended
    return;
  }
#endif
  if (size > oldSize) {
    Allocator<char>* buffer = Thread::getLocalStorage();
    fill<char>(buffer->getElements(), buffer->getSize(), 0);
    setPosition(oldSize);
    long long count = size - oldSize;
    while (count > 0) {
      count -= write(buffer->getElements(), minimum<long long>(count, buffer->getSize())); // blocking write
    }
  }
}

void File::flush() throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (!::FlushFileBuffers(fd->getHandle())) {
    throw FileException("Unable to flush", this);
  }
#else // unix
  if (fsync(fd->getHandle())) {
    throw FileException("Unable to flush", this);
  }
#endif
}

void File::lock(const FileRegion& region, bool exclusive = true) throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert((region.getOffset() >= 0) && (region.getSize() >= 0), FileException("Unable to lock region", this));
  LARGE_INTEGER offset;
  offset.QuadPart = region.getOffset();
  OVERLAPPED overlapped;
  overlapped.Offset = offset.LowPart;
  overlapped.OffsetHigh = offset.HighPart;
  overlapped.hEvent = ::CreateEvent(0, TRUE, FALSE, 0);
  if (overlapped.hEvent == 0) {
    throw FileException("Unable to lock region", this);
  }

  LARGE_INTEGER size;
  size.QuadPart = region.getSize();
  if (!::LockFileEx(
    fd->getHandle(),
    exclusive ? LOCKFILE_EXCLUSIVE_LOCK : 0,
    0,
    size.LowPart,
    size.HighPart,
    &overlapped
  )) {
    ::CloseHandle(overlapped.hEvent);
    throw FileException("Unable to lock region", this);
  }
  ::WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for lock
  ::CloseHandle(overlapped.hEvent);
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct flock64 lock;
    lock.l_type = exclusive ? F_WRLCK : F_RDLCK; // request exclusive or shared lock
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    while (true) {
      if (::fcntl(fd->getHandle(), F_SETLKW64, &lock) < 0) {
        if (errno == EINTR) { // interrupted by signal - try again
          continue;
        }
        throw FileException("Unable to lock region", this);
      }
      break;
    }
  #else
    assert(
      (region.getOffset() >= 0) && (region.getOffset() <= PrimitiveTraits<int>::MAXIMUM) && (region.getSize() >= 0) && (region.getSize() <= PrimitiveTraits<int>::MAXIMUM),
      FileException("Unable to lock region", this)
    );
    struct flock lock;
    lock.l_type = exclusive ? F_WRLCK : F_RDLCK; // request exclusive or shared lock
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    while (true) {
      if (fcntl(fd->getHandle(), F_SETLKW, &lock) < 0) {
        if (errno == EINTR) { // interrupted by signal - try again
          continue;
        }
        throw FileException("Unable to lock region", this);
      }
      break;
    }
  #endif
#endif
}

bool File::tryLock(const FileRegion& region, bool exclusive = true) throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert((region.getOffset() >= 0) && (region.getSize() >= 0), FileException("Unable to lock region", this));
  LARGE_INTEGER offset;
  offset.QuadPart = region.getOffset();
  OVERLAPPED overlapped;
  overlapped.Offset = offset.LowPart;
  overlapped.OffsetHigh = offset.HighPart;
  overlapped.hEvent = ::CreateEvent(0, TRUE, FALSE, 0);
  if (overlapped.hEvent == 0) {
    throw FileException("Unable to lock region", this);
  }

  LARGE_INTEGER size;
  size.QuadPart = region.getSize();
  if (!::LockFileEx(
    fd->getHandle(),
    exclusive ? LOCKFILE_EXCLUSIVE_LOCK : 0,
    0,
    size.LowPart,
    size.HighPart,
    &overlapped)) {
    CloseHandle(overlapped.hEvent);
    throw FileException("Unable to lock region", this);
  }
  DWORD result = ::WaitForSingleObject(overlapped.hEvent, 0); // return immediately
  ::CloseHandle(overlapped.hEvent);
  return result == WAIT_OBJECT_0; // was the region locked
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct flock64 lock;
    lock.l_type = exclusive ? F_WRLCK : F_RDLCK; // request exclusive or shared lock
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    if (::fcntl(fd->getHandle(), F_SETLK64, &lock) < 0) {
      // this function is not interrupted by a signal
      if ((errno == EACCES) || (errno == EAGAIN)) {
        return false;
      }
      throw FileException("Unable to lock region", this);
    }
    return true;
  #else
    assert(
      (region.getOffset() >= 0) && (region.getOffset() <= PrimitiveTraits<int>::MAXIMUM) && (region.getSize() >= 0) && (region.getSize() <= PrimitiveTraits<int>::MAXIMUM),
      FileException("Unable to lock region", this)
    );
    struct flock lock;
    lock.l_type = exclusive ? F_WRLCK : F_RDLCK; // request exclusive or shared lock
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    if (::fcntl(fd->getHandle(), F_SETLK, &lock) < 0) {
      // this function is not interrupted by a signal
      if ((errno == EACCES) || (errno == EAGAIN)) {
        return false;
      }
      throw FileException("Unable to lock region", this);
    }
    return true;
  #endif
#endif
}

void File::unlock(const FileRegion& region) throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert((region.getOffset() >= 0) && (region.getSize() >= 0), FileException("Unable to unlock region", this));
  LARGE_INTEGER offset;
  offset.QuadPart = region.getOffset();

  OVERLAPPED overlapped;
  overlapped.Offset = offset.LowPart;
  overlapped.OffsetHigh = offset.HighPart;
  overlapped.hEvent = ::CreateEvent(0, TRUE, FALSE, 0);
  if (overlapped.hEvent == 0) {
    throw FileException("Unable to unlock region", this);
  }

  LARGE_INTEGER size;
  size.QuadPart = region.getSize();
  if (!::UnlockFileEx(fd->getHandle(), 0, size.LowPart, size.HighPart, &overlapped)) {
    ::CloseHandle(overlapped.hEvent);
    throw FileException("Unable to unlock region", this);
  }
  ::WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for unlock
  ::CloseHandle(overlapped.hEvent);
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    assert((region.getOffset() >= 0) && (region.getSize() >= 0), FileException("Unable to unlock region", this));
    struct flock64 lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    while (true) {
      if (::fcntl(fd->getHandle(), F_SETLKW64, &lock) < 0) {
        if (errno == EINTR) { // interrupted by signal - try again
          continue;
        }
        throw FileException("Unable to unlock region", this);
      }
      break;
    }
  #else
    assert(
      (region.getOffset() >= 0) && (region.getOffset() <= PrimitiveTraits<int>::MAXIMUM) && (region.getSize() >= 0) && (region.getSize() <= PrimitiveTraits<int>::MAXIMUM),
      FileException("Unable to unlock region", this)
    );
    struct flock lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    while (true) {
      if (::fcntl(fd->getHandle(), F_SETLKW, &lock) < 0) {
        if (errno == EINTR) { // interrupted by signal - try again
          continue;
        }
        throw FileException("Unable to unlock region", this);
      }
      break;
    }
  #endif
#endif
}

Date File::getLastModification() throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  FILETIME time;
  if (::GetFileTime(fd->getHandle(), 0, 0, &time)) {
    throw FileException("Unable to get file time", this);
  }
  return FileTimeToDate(time);
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct stat64 buffer;
    if (fstat64(fd->getHandle(), &buffer)) {
      throw FileException("Unable to get status", this);
    }
    return Date(buffer.st_mtime);
  #else
    struct stat buffer;
    if (::fstat(fd->getHandle(), &buffer)) {
      throw FileException("Unable to get status", this);
    }
    return Date(buffer.st_mtime);
  #endif // LFS
#endif
}

Date File::getLastAccess() throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  FILETIME time;
  if (::GetFileTime(fd->getHandle(), 0, &time, 0)) {
    throw FileException("Unable to get file time", this);
  }
  return FileTimeToDate(time);
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct stat64 buffer;
    if (::fstat64(fd->getHandle(), &buffer)) {
      throw FileException("Unable to get status", this);
    }
    return Date(buffer.st_atime);
  #else
    struct stat buffer;
    if (::fstat(fd->getHandle(), &buffer)) {
      throw FileException("Unable to get status", this);
    }
    return Date(buffer.st_atime);
  #endif // LFS
#endif
}

Date File::getLastChange() throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  FILETIME time;
  if (::GetFileTime(fd->getHandle(), &time, 0, 0)) {
    throw FileException("Unable to get file time", this);
  }
  return FileTimeToDate(time);
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct stat64 buffer;
    if (::fstat64(fd->getHandle(), &buffer)) {
      throw FileException("Unable to get status", this);
    }
    return Date(buffer.st_ctime);
  #else
    struct stat buffer;
    if (::fstat(fd->getHandle(), &buffer)) {
      throw FileException("Unable to get status", this);
    }
    return Date(buffer.st_ctime);
  #endif // LFS
#endif
}

unsigned int File::read(char* buffer, unsigned int bytesToRead, bool nonblocking) throw(FileException) {
  unsigned int bytesRead = 0;
  while (bytesToRead > 0) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    DWORD result;
    BOOL success = ::ReadFile(fd->getHandle(), buffer, bytesToRead, &result, 0);
    if (!success) { // has error occured
      if (::GetLastError() == ERROR_LOCK_VIOLATION) { // TAG: I'm guessing this error code - please confirm
        if (nonblocking) {
          success = true;
          result = 0;
        } else { // do blocking read
          LARGE_INTEGER position;
          position.QuadPart = 0;
          position.LowPart = ::SetFilePointer(fd->getHandle(), 0, &position.HighPart, FILE_CURRENT);
          if (!((position.LowPart == INVALID_SET_FILE_POINTER) && (::GetLastError() != NO_ERROR))) {
	    // if (SetFilePointerEx(fd->getHandle(), 0, &position, FILE_CURRENT)) { // current position
            OVERLAPPED overlapped;
            overlapped.Offset = position.LowPart;
            overlapped.OffsetHigh = position.HighPart;
            overlapped.hEvent = ::CreateEvent(0, FALSE, FALSE, 0); // auto reset event

            if (overlapped.hEvent) { // was event created
              if (::LockFileEx(fd->getHandle(), 0, 0, bytesToRead, 0, &overlapped)) { // acquire shared lock
                ::WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for lock
                if (::ReadFile(fd->getHandle(), buffer, bytesToRead, &result, 0)) {
                  if (::UnlockFileEx(fd->getHandle(), 0, bytesToRead, 0, &overlapped)) { // // release shared lock
                    ::WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for unlock
                    success = true;
                  }
                }
              }
              ::CloseHandle(overlapped.hEvent);
            }
          }
        }
      }
    }
    if (!success) {
      throw FileException("Unable to read from file", this);
    }
#else // unix
    int result;
    do {
      result = ::read(fd->getHandle(), buffer, minimum<unsigned int>(bytesToRead, SSIZE_MAX));
      if (result < 0) { // has an error occured
        switch (errno) { // remember that errno is local to the thread
        case EINTR: // interrupted by signal before any data was read
          break; // try again
        case EAGAIN: // no data available (only in non-blocking mode)
          result = 0;
          break;
        default:
          throw FileException("Unable to read from file", this);
        }
      }
    } while (result < 0);
#endif
    bytesRead += result;
    buffer += result;
    bytesToRead -= result;
    if (nonblocking) { // accept whatever has been read in nonblocking mode
      break;
    }
    if (result == 0) { // has end been reached
      throw EndOfFile(this); // attempt to read beyond end of stream
    }
  }
  return bytesRead;
}

unsigned int File::write(const char* buffer, unsigned int bytesToWrite, bool nonblocking) throw(FileException) {
  unsigned int bytesWritten = 0;
  while (bytesToWrite > 0) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    DWORD result;
    BOOL success = ::WriteFile(fd->getHandle(), buffer, bytesToWrite, &result, 0);
    if (!success) {
      if (::GetLastError() == ERROR_LOCK_VIOLATION) { // TAG: I'm guessing this error code - please confirm
        if (nonblocking) {
          success = true;
          result = 0;
        } else { // do blocking write
          LARGE_INTEGER position;
          position.QuadPart = 0;
          position.LowPart = ::SetFilePointer(fd->getHandle(), 0, &position.HighPart, FILE_CURRENT);
          if (!((position.LowPart == INVALID_SET_FILE_POINTER) && (::GetLastError() != NO_ERROR))) {
	    // if (SetFilePointerEx(fd->getHandle(), 0, &position, FILE_CURRENT)) { // current position
            OVERLAPPED overlapped;
            overlapped.Offset = position.LowPart;
            overlapped.OffsetHigh = position.HighPart;
            overlapped.hEvent = ::CreateEvent(0, FALSE, FALSE, 0); // auto reset event

            if (overlapped.hEvent) { // was event created
              if (::LockFileEx(fd->getHandle(), 0, 0, bytesToWrite, 0, &overlapped)) { // acquire shared lock
                ::WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for lock
                if (::WriteFile(fd->getHandle(), buffer, bytesToWrite, &result, 0)) {
                  if (::UnlockFileEx(fd->getHandle(), 0, bytesToWrite, 0, &overlapped)) { // // release shared lock
                    WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for unlock
                    success = true;
                  }
                }
              }
              ::CloseHandle(overlapped.hEvent);
            }
          }
        }
      }
    }
    if (!success) {
      throw FileException("Unable to write to file", this);
    }
#else // unix
    int result;
    do {
      result = ::write(fd->getHandle(), buffer, minimum<unsigned int>(bytesToWrite, SSIZE_MAX));
      if (result < 0) { // has an error occured
        switch (errno) {
        case EINTR: // interrupted by signal before any data was written
          break; // try again
        case EAGAIN: // no data could be written without blocking (only in non-blocking mode)
          result = 0;
          break;
        default:
          throw FileException("Unable to write to file", this);
        }
      }
    } while (result < 0);
#endif
    if (result == 0) {
      if (nonblocking) {
        break;
      } else {
        throw FileException("Unable to write to file", this);
      }
    }
    bytesWritten += result;
    buffer += result;
    bytesToWrite -= result;
  }
  return bytesWritten;
}

void File::asyncCancel() throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::CancelIo(getHandle());
#else // unix
#endif // flavor
}

AsynchronousReadOperation File::read(char* buffer, unsigned int bytesToRead, unsigned long long offset, AsynchronousReadEventListener* listener) throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(listener, AsynchronousException(this)); // TAG: fixme
  return new win32::AsyncReadFileContext(getHandle(), buffer, bytesToRead, offset, listener);
#endif // flavor
}

AsynchronousWriteOperation File::write(const char* buffer, unsigned int bytesToWrite, unsigned long long offset, AsynchronousWriteEventListener* listener) throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(listener, AsynchronousException(this)); // TAG: fixme
  return new win32::AsyncWriteFileContext(getHandle(), buffer, bytesToWrite, offset, listener);
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
