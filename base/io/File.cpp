/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/io/File.h>
#include <base/io/EndOfFile.h>
#include <base/concurrency/Thread.h>
#include <base/Base.h>
#include <base/Functor.h>

#if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  #define _LARGEFILE64_SOURCE 1
#endif

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // __unix__
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
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  inline Date FileTimeToDate(const FILETIME& time) {
    ASSERT(sizeof(FILETIME) == sizeof(long long));
    return Date((*pointer_cast<const long long*>(&time) - 116444736000000000LL)/10000000); // TAG: 0x0000001c1a021060LL
  }
#endif

File::FileImpl::~FileImpl() throw(FileException) {
// TAG: throw exception if region of file is still locked
  if (handle != -1) { // dont try to close if handle is invalidated
  #if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
    if (!CloseHandle((HANDLE)handle)) {
      throw FileException("Unable to close file");
    }
  #else // __unix__
    if (::close(handle)) {
      throw FileException("Unable to close file");
    }
  #endif
  }
}

File::File(const String& path, Access access, unsigned int options) throw(FileNotFound) : fd(0) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
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

  HANDLE handle;
  handle = CreateFile(
    path.getElements(),
    (access == READ) ? GENERIC_READ : ((access == WRITE) ? GENERIC_WRITE : (GENERIC_READ | GENERIC_WRITE)),
    (options & EXCLUSIVE) ? 0 : (FILE_SHARE_READ | FILE_SHARE_WRITE),
    NULL,
    creationFlags,
    FILE_ATTRIBUTE_NORMAL,
    NULL
  );
  if (handle == INVALID_HANDLE_VALUE) {
    throw FileNotFound("Unable to open file");
  }
  fd = new FileImpl((int)handle);
#else // __unix__
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

  int handle;
  if ((handle = ::open(path.getElements(), flags, S_IRUSR | S_IWUSR | S_IRGRP)) == -1) {
    throw FileNotFound("Unable to open file");
  }
  fd = new FileImpl(handle);
#endif
}

File& File::operator=(const File& eq) throw() {
  if (&eq != this) {
    fd = eq.fd;
  }
  return *this;
}

void File::close() throw(FileException) {
  fd = new FileImpl(); // invalidate
}

long long File::getSize() const throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  ULARGE_INTEGER size;
  size.LowPart = GetFileSize((HANDLE)fd->getHandle(), &size.HighPart);
  if ((size.LowPart == INVALID_FILE_SIZE) && (GetLastError() != NO_ERROR )) {
    throw FileException("Unable to get file size");
  }
//  LARGE_INTEGER size; // TAG: unresolved possible byte order problem for big endian architectures
//  if (!GetFileSizeEx((HANDLE)fd->getHandle(), &size)) {
//    throw FileException("Unable to get file size");
//  }
  return size.QuadPart;
#else // __unix__
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    if (fstat64(fd->getHandle(), &status)) {
      throw FileException("Unable to get file size");
    }
    return status.st_size;
  #else
    struct stat status;
    if (fstat(fd->getHandle(), &status)) {
      throw FileException("Unable to get file size");
    }
    return status.st_size;
  #endif
#endif
}

long long File::getPosition() const throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  LARGE_INTEGER position;
  position.QuadPart = 0;
  position.LowPart = SetFilePointer((HANDLE)fd->getHandle(), 0, &position.HighPart, FILE_CURRENT);
  if ((position.LowPart == INVALID_SET_FILE_POINTER) && (GetLastError() != NO_ERROR)) {
    throw FileException("Unable to get file position");
  }
//  if (!SetFilePointerEx((HANDLE)fd->getHandle(), 0, &position, FILE_CURRENT)) {
//    throw FileException("Unable to get file position");
//  }
  return position.QuadPart;
#else // __unix__
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    return lseek64(fd->getHandle(), 0, SEEK_CUR); // should never fail
  #else
    return lseek(fd->getHandle(), 0, SEEK_CUR); // should never fail
  #endif
#endif
}

void File::setPosition(long long position, Whence whence) throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  static DWORD relativeTo[] = {FILE_BEGIN, FILE_CURRENT, FILE_END};
  LARGE_INTEGER temp;
  temp.QuadPart = position;
  temp.LowPart = SetFilePointer((HANDLE)fd->getHandle(), temp.LowPart, &temp.HighPart, relativeTo[whence]);
  if ((temp.LowPart == INVALID_SET_FILE_POINTER) && (GetLastError() != NO_ERROR)) {
    throw FileException("Unable to get file position");
  }
//  if (!SetFilePointerEx((HANDLE)fd->getHandle(), position, 0, relativeTo[whence])) {
//    throw FileException("Unable to set position");
//  }
#else // __unix__
  static int relativeTo[] = {SEEK_SET, SEEK_CUR, SEEK_END};
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    lseek64(fd->getHandle(), position, relativeTo[whence]); // should never fail
  #else
    assert(position <= INT_MAX, FileException("Unable to set position"));
    lseek(fd->getHandle(), position, relativeTo[whence]); // should never fail
  #endif
#endif
}

void File::truncate(long long size) throw(FileException) {
  long long oldSize = File::getSize();
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  setPosition(size);
  if (!SetEndOfFile((HANDLE)fd->getHandle())) {
    throw FileException("Unable to truncate");
  }
#else // __unix__
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    if (ftruncate64(fd->getHandle(), size)) {
      throw FileException("Unable to truncate");
    }
  #else
    assert((size >= 0) && (size <= INT_MAX), FileException("Unable to truncate"));
    if (ftruncate(fd->getHandle(), size)) {
      throw FileException("Unable to truncate");
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
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!FlushFileBuffers((HANDLE)fd->getHandle())) {
    throw FileException("Unable to flush");
  }
#else // __unix__
  if (fsync(fd->getHandle())) {
    throw FileException("Unable to flush");
  }
#endif
}

void File::lock(const FileRegion& region, bool exclusive = true) throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  assert((region.getOffset() >= 0) && (region.getSize() >= 0), FileException("Unable to lock region"));
  LARGE_INTEGER offset;
  offset.QuadPart = region.getOffset();
  OVERLAPPED overlapped;
  overlapped.Offset = offset.LowPart;
  overlapped.OffsetHigh = offset.HighPart;
  overlapped.hEvent = CreateEvent(NULL, true, false, NULL);
  if (overlapped.hEvent == NULL) {
    throw FileException("Unable to lock region");
  }

  LARGE_INTEGER size;
  size.QuadPart = region.getSize();
  if (!LockFileEx(
    (HANDLE)fd->getHandle(),
    exclusive ? LOCKFILE_EXCLUSIVE_LOCK : 0,
    0,
    size.LowPart,
    size.HighPart,
    &overlapped
  )) {
    CloseHandle(overlapped.hEvent);
    throw FileException("Unable to lock region");
  }
  WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for lock
  CloseHandle(overlapped.hEvent);
#else // __unix__
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct flock64 lock;
    lock.l_type = exclusive ? F_WRLCK : F_RDLCK; // request exclusive or shared lock
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    while (true) {
      if (fcntl(fd->getHandle(), F_SETLKW64, &lock) < 0) {
        if (errno == EINTR) { // interrupted by signal - try again
          continue;
        }
        throw FileException("Unable to lock region");
      }
      break;
    }
  #else
    assert(
      (region.getOffset() >= 0) && (region.getOffset() <= INT_MAX) && (region.getSize() >= 0) && (region.getSize() <= INT_MAX),
      FileException("Unable to lock region")
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
        throw FileException("Unable to lock region");
      }
      break;
    }
  #endif
#endif
}

bool File::tryLock(const FileRegion& region, bool exclusive = true) throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  assert((region.getOffset() >= 0) && (region.getSize() >= 0), FileException("Unable to lock region"));
  LARGE_INTEGER offset;
  offset.QuadPart = region.getOffset();
  OVERLAPPED overlapped;
  overlapped.Offset = offset.LowPart;
  overlapped.OffsetHigh = offset.HighPart;
  overlapped.hEvent = CreateEvent(NULL, true, false, NULL);
  if (overlapped.hEvent == NULL) {
    throw FileException("Unable to lock region");
  }

  LARGE_INTEGER size;
  size.QuadPart = region.getSize();
  if (!LockFileEx(
    (HANDLE)fd->getHandle(),
    exclusive ? LOCKFILE_EXCLUSIVE_LOCK : 0,
    0,
    size.LowPart,
    size.HighPart,
    &overlapped)) {
    CloseHandle(overlapped.hEvent);
    throw FileException("Unable to lock region");
  }
  DWORD result = WaitForSingleObject(overlapped.hEvent, 0); // return immediately
  CloseHandle(overlapped.hEvent);
  return result == WAIT_OBJECT_0; // was the region locked
#else // __unix__
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct flock64 lock;
    lock.l_type = exclusive ? F_WRLCK : F_RDLCK; // request exclusive or shared lock
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    if (fcntl(fd->getHandle(), F_SETLK64, &lock) < 0) {
      // this function is not interrupted by a signal
      if ((errno == EACCES) || (errno == EAGAIN)) {
        return false;
      }
      throw FileException("Unable to lock region");
    }
    return true;
  #else
    assert(
      (region.getOffset() >= 0) && (region.getOffset() <= INT_MAX) && (region.getSize() >= 0) && (region.getSize() <= INT_MAX),
      FileException("Unable to lock region")
    );
    struct flock lock;
    lock.l_type = exclusive ? F_WRLCK : F_RDLCK; // request exclusive or shared lock
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    if (fcntl(fd->getHandle(), F_SETLK, &lock) < 0) {
      // this function is not interrupted by a signal
      if ((errno == EACCES) || (errno == EAGAIN)) {
        return false;
      }
      throw FileException("Unable to lock region");
    }
    return true;
  #endif
#endif
}

void File::unlock(const FileRegion& region) throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  assert((region.getOffset() >= 0) && (region.getSize() >= 0), FileException("Unable to unlock region"));
  LARGE_INTEGER offset;
  offset.QuadPart = region.getOffset();

  OVERLAPPED overlapped;
  overlapped.Offset = offset.LowPart;
  overlapped.OffsetHigh = offset.HighPart;
  overlapped.hEvent = CreateEvent(NULL, true, false, NULL);
  if (overlapped.hEvent == NULL) {
    throw FileException("Unable to unlock region");
  }

  LARGE_INTEGER size;
  size.QuadPart = region.getSize();
  if (!UnlockFileEx((HANDLE)fd->getHandle(), 0, size.LowPart, size.HighPart, &overlapped)) {
    CloseHandle(overlapped.hEvent);
    throw FileException("Unable to unlock region");
  }
  WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for unlock
  CloseHandle(overlapped.hEvent);
#else // __unix__
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    assert((region.getOffset() >= 0) && (region.getSize() >= 0), FileException("Unable to unlock region"));
    struct flock64 lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    while (true) {
      if (fcntl(fd->getHandle(), F_SETLKW64, &lock) < 0) {
        if (errno == EINTR) { // interrupted by signal - try again
          continue;
        }
        throw FileException("Unable to unlock region");
      }
      break;
    }
  #else
    assert(
      (region.getOffset() >= 0) && (region.getOffset() <= INT_MAX) && (region.getSize() >= 0) && (region.getSize() <= INT_MAX),
      FileException("Unable to unlock region")
    );
    struct flock lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    while (true) {
      if (fcntl(fd->getHandle(), F_SETLKW, &lock) < 0) {
        if (errno == EINTR) { // interrupted by signal - try again
          continue;
        }
        throw FileException("Unable to unlock region");
      }
      break;
    }
  #endif
#endif
}

Date File::getLastModification() throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  FILETIME time;
  if (GetFileTime((HANDLE)fd->getHandle(), NULL, NULL, &time)) {
    throw FileException("Unable to get file time");
  }
  return FileTimeToDate(time);
#else // Unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct stat64 buffer;
    if (fstat64(fd->getHandle(), &buffer)) {
      throw FileException("Unable to get status");
    }
    return Date(buffer.st_mtime);
  #else
    struct stat buffer;
    if (fstat(fd->getHandle(), &buffer)) {
      throw FileException("Unable to get status");
    }
    return Date(buffer.st_mtime);
  #endif // LFS
#endif
}

Date File::getLastAccess() throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  FILETIME time;
  if (GetFileTime((HANDLE)fd->getHandle(), NULL, &time, NULL)) {
    throw FileException("Unable to get file time");
  }
  return FileTimeToDate(time);
#else // Unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct stat64 buffer;
    if (fstat64(fd->getHandle(), &buffer)) {
      throw FileException("Unable to get status");
    }
    return Date(buffer.st_atime);
  #else
    struct stat buffer;
    if (fstat(fd->getHandle(), &buffer)) {
      throw FileException("Unable to get status");
    }
    return Date(buffer.st_atime);
  #endif // LFS
#endif
}

Date File::getLastChange() throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  FILETIME time;
  if (GetFileTime((HANDLE)fd->getHandle(), &time, NULL, NULL)) {
    throw FileException("Unable to get file time");
  }
  return FileTimeToDate(time);
#else // Unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct stat64 buffer;
    if (fstat64(fd->getHandle(), &buffer)) {
      throw FileException("Unable to get status");
    }
    return Date(buffer.st_ctime);
  #else
    struct stat buffer;
    if (fstat(fd->getHandle(), &buffer)) {
      throw FileException("Unable to get status");
    }
    return Date(buffer.st_ctime);
  #endif // LFS
#endif
}

unsigned int File::read(char* buffer, unsigned int size, bool nonblocking) throw(FileException) {
  unsigned int bytesRead = 0;
  while (bytesRead < size) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
    DWORD result;
    BOOL success = ::ReadFile((HANDLE)fd->getHandle(), buffer, size, &result, NULL);
    if (!success) { // has error occured
      if (GetLastError() == ERROR_LOCK_VIOLATION) { // TAG: I'm guessing this error code - please confirm
        if (nonblocking) {
          success = true;
          result = 0;
        } else { // do blocking read
          LARGE_INTEGER position;
          position.QuadPart = 0;
          position.LowPart = SetFilePointer((HANDLE)fd->getHandle(), 0, &position.HighPart, FILE_CURRENT);
          if (!((position.LowPart == INVALID_SET_FILE_POINTER) && (GetLastError() != NO_ERROR))) {
	    // if (SetFilePointerEx((HANDLE)fd->getHandle(), 0, &position, FILE_CURRENT)) { // current position
            OVERLAPPED overlapped;
            overlapped.Offset = position.LowPart;
            overlapped.OffsetHigh = position.HighPart;
            overlapped.hEvent = CreateEvent(NULL, false, false, NULL); // auto reset event

            if (overlapped.hEvent) { // was event created
              if (LockFileEx((HANDLE)fd->getHandle(), 0, 0, size, 0, &overlapped)) { // acquire shared lock
                WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for lock
                if (::ReadFile((HANDLE)fd->getHandle(), buffer, size, &result, NULL)) {
                  if (UnlockFileEx((HANDLE)fd->getHandle(), 0, size, 0, &overlapped)) { // // release shared lock
                    WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for unlock
                    success = true;
                  }
                }
              }
              CloseHandle(overlapped.hEvent);
            }
          }
        }
      }
    }
    if (!success) {
      throw FileException("Unable to read from file");
    }
#else // __unix__
    int result;
    do {
      result = ::read(fd->getHandle(), buffer, (size <= SSIZE_MAX) ? size : SSIZE_MAX);
      if (result < 0) { // has an error occured
        switch (errno) { // remember that errno is local to the thread
        case EINTR: // interrupted by signal before any data was read
          break; // try again
        case EAGAIN: // no data available (only in non-blocking mode)
          result = 0;
          break;
        default:
          throw FileException("Unable to read from file");
        }
      }
    } while (result < 0);
#endif
    bytesRead += result;
    if (nonblocking) { // accept whatever has been read in nonblocking mode
      break;
    }
    if (result == 0) { // has end been reached
      throw EndOfFile(); // attempt to read beyond end of stream
    }
  }
  return bytesRead;
}

unsigned int File::write(const char* buffer, unsigned int size, bool nonblocking) throw(FileException) {
  unsigned int bytesWritten = 0;
  while (bytesWritten < size) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
    DWORD result;
    BOOL success = ::WriteFile((HANDLE)fd->getHandle(), buffer, size, &result, NULL);
    if (!success) {
      if (GetLastError() == ERROR_LOCK_VIOLATION) { // TAG: I'm guessing this error code - please confirm
        if (nonblocking) {
          success = true;
          result = 0;
        } else { // do blocking write
          LARGE_INTEGER position;
          position.QuadPart = 0;
          position.LowPart = SetFilePointer((HANDLE)fd->getHandle(), 0, &position.HighPart, FILE_CURRENT);
          if (!((position.LowPart == INVALID_SET_FILE_POINTER) && (GetLastError() != NO_ERROR))) {
	    // if (SetFilePointerEx((HANDLE)fd->getHandle(), 0, &position, FILE_CURRENT)) { // current position
            OVERLAPPED overlapped;
            overlapped.Offset = position.LowPart;
            overlapped.OffsetHigh = position.HighPart;
            overlapped.hEvent = CreateEvent(NULL, false, false, NULL); // auto reset event

            if (overlapped.hEvent) { // was event created
              if (LockFileEx((HANDLE)fd->getHandle(), 0, 0, size, 0, &overlapped)) { // acquire shared lock
                WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for lock
                if (WriteFile((HANDLE)fd->getHandle(), buffer, size, &result, 0)) {
                  if (UnlockFileEx((HANDLE)fd->getHandle(), 0, size, 0, &overlapped)) { // // release shared lock
                    WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for unlock
                    success = true;
                  }
                }
              }
              CloseHandle(overlapped.hEvent);
            }
          }
        }
      }
    }
    if (!success) {
      throw FileException("Unable to write to file");
    }
#else // __unix__
    int result;
    do {
      result = ::write(fd->getHandle(), buffer, (size <= SSIZE_MAX) ? size : SSIZE_MAX);
      if (result < 0) { // has an error occured
        switch (errno) {
        case EINTR: // interrupted by signal before any data was written
          break; // try again
        case EAGAIN: // no data could be written without blocking (only in non-blocking mode)
          result = 0;
          break;
        default:
          throw FileException("Unable to write to file");
        }
      }
    } while (result < 0);
#endif
    if (result == 0) {
      if (nonblocking) {
        break;
      } else {
        throw FileException("Unable to write to file");
      }
    }
    bytesWritten += result;
  }
  return bytesWritten;
}



LockableRegion::LockableRegion(const File& f, const FileRegion& r, bool exclusive) throw(FileException) : file(f), region(r) {
  file.lock(region, exclusive);
}

void LockableRegion::lock(const FileRegion& region, bool exclusive) throw(FileException) {
  file.unlock(this->region);
  this->region = region;
  file.lock(this->region, exclusive);
}

LockableRegion::~LockableRegion() throw(FileException) {
  file.unlock(region);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
