/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_H
#define _DK_SDU_MIP__BASE_IO__FILE_H

#include <base/Object.h>
#include <base/io/FileException.h>
#include <base/io/FileNotFound.h>
#include <base/mem/ReferenceCountedObject.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/string/String.h>
#include <base/Date.h>
#include <base/Type.h>
#include <base/OperatingSystem.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Description of a file region represented by an offset (64 bit) and a size (32 bit).

  @short A region of a file.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class FileRegion : public Object {
private:

  /** The offset of the region from the beginning of the file. */
  long long offset;
  /** The size of the region (in bytes). */
  unsigned int size;
public:

  /**
    Initializes the file region.

    @param offset The offset (from the beginning of the file) of the region.
    @param size The size (in bytes) of the region.
  */
  FileRegion(long long offset, unsigned int size) throw();

  /**
    Initializes region from other region.
  */
  inline FileRegion(const FileRegion& copy) throw() : offset(copy.offset), size(copy.size) {}

  /**
    Assignment of region by region.
  */
  inline FileRegion& operator=(const FileRegion& eq) throw() {
    // no need to protect against self-assignment
    offset = eq.offset;
    size = eq.size;
    return *this;
  }

  /**
    Returns the offset of the file region.
  */
  inline long long getOffset() const throw() {return offset;}

  /**
    Returns the size of the file region.
  */
  inline unsigned int getSize() const throw() {return size;}

  /**
    Sets the offset of the file region.
  */
  inline void setOffset(long long offset) throw() {this->offset = offset;}

  /**
    Sets the size of the file region.
  */
  inline void setSize(unsigned int size) throw() {this->size = size;}

  /**
    Returns true if the specified region is contained in this region.
  */
  inline bool isWithin(const FileRegion& region) const throw() {
    const long long difference = region.offset - offset;
    return (difference >= 0) && ((difference + region.size - size) <= 0);
  }
};

inline FileRegion::FileRegion(long long o, unsigned int s) throw() : offset(o), size(s) {}

class MappedFile;

/**
  File.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class File : public Object {
public:

  /** File access. */
  typedef enum {
    READ, /**< Request read access. */
    WRITE, /**< Request write access. */
    READWRITE /**< Request read and write access. */
  } Access;
  /** Type used to specify the relative offset. */
  typedef enum {
    BEGIN, /**< Position is relative to begining of file. */
    CURRENT, /**< Position is relative to current position of file. */
    END /**< Position is relative to end of file. */
  } Whence;
  /** File initialization options. */
  enum Options {
    CREATE = 1, /**< Specifies that the file should be created if it doesn't exist. */
    TRUNCATE = 2, /**< Specifies that the file should be truncated if it already exists. */
    EXCLUSIVE = 4 /**< Specifies that the file should be opened in exclusive mode. */
  };
private:

  friend class MappedFile;

  class FileImpl : public ReferenceCountedObject {
  private:

    OperatingSystem::Handle handle;
  public:

    FileImpl() throw() : handle(-1) {}
    explicit FileImpl(int h) throw() : handle(h) {}
    OperatingSystem::Handle getHandle() const throw() {return handle;}
    ~FileImpl() throw(FileException);
  };

  /** The handle of the file. */
  ReferenceCountedObjectPointer<FileImpl > fd;
public:

  /**
    Initializes a file. If no options are specified the initialization will
    fail if the file doesn't exist.

    @param path the path of the file.
    @param access The desired access to the files.
    @param option Additional options (i.e. CREATE, TRUNCATE, EXCLUSIVE).
  */
  File(const String& path, Access access, unsigned int options) throw(FileNotFound);

  /**
    Initialize a new file object from other file object.
  */
  File(const File& copy) throw() : fd(copy.fd) {}

  /**
    Assignment of file object.
  */
  File& operator=(const File& eq) throw();



  /**
    Closes the file.
  */
  void close() throw(FileException);

  /**
    Returns the size of the file in bytes.
  */
  long long getSize() const throw(FileException);

  /**
    Returns the current position.
  */
  long long getPosition() const throw(FileException);

  /**
    Sets the position.

    @param position The relative offset from the specified base offset.
    @param whence Specifies the base offset. The default is the beginning of the file.
  */
  void setPosition(long long position, Whence whence = BEGIN) throw(FileException);

  /**
    Truncates the file to the specified size. The file must have been opened
    with write access. If the file previously was larger than the specified
    size, the extra data is discarded. If the file was previously shorter, it
    is extended with zeros.
  */
  void truncate(long long size) throw(FileException);

  /**
    Flushes any buffered data.
  */
  void flush() throw(FileException);



  /**
    Locks the specified region of the file. Blocks until lock has been
    acquired. Exclusive locks require the file to be opened with write access.

    @param region The file region to be locked.
    @param exclusive Specifies that the region should be locked exclusively. Default is true.
  */
  void lock(const FileRegion& region, bool exclusive = true) throw(FileException);

  /**
    Tries to lock specified region of the file. Exclusive locks require the
    file to be opened with write access.

    @param region The file region to be locked.
    @param exclusive Specifies that the region should be locked exclusively. Default is true.

    @return True if the lock was acquired.
  */
  bool tryLock(const FileRegion& region, bool exclusive = true) throw(FileException);

  /**
    Unlocks the specified region of the file.

    @param region The file region to be unlocked.
  */
  void unlock(const FileRegion& region) throw(FileException);



  /**
    Returns the last modification time of the file.
  */
  Date getLastModification() throw(FileException);

  /**
    Returns the last access time of the file.
  */
  Date getLastAccess() throw(FileException);

  /**
    Returns the last chnage time of the file.
  */
  Date getLastChange() throw(FileException);



  /**
    If another process has acquired an exclusive lock on part of the file
    region to be read, the read request will block until the lock has been
    released.

    @return 0 if the end of the file has been reached (or if requested to read 0 bytes).
  */
  unsigned int read(char* buffer, unsigned int size, bool nonblocking = false) throw(FileException);

  /**
    Read the specified type.
  */
  template<class TYPE>
  inline unsigned int read(TYPE& buffer) throw(FileException) {
    return read(getCharAddress(buffer), sizeof(TYPE));
  }

  /**
    Throws EndOfFile if minimum number of bytes cannot be read without
    exceeding the end of the file.
  */
  unsigned int write(const char* buffer, unsigned int size, bool nonblocking = false) throw(FileException);
};



/**
  @short Locked file region.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class LockableRegion : public Object {
private:

  /** The file. */
  File file;
  /** The region. */
  FileRegion region;
public:

  /**
    Initializes the file region lock.

    @param file The file.
    @param region The file region to be locked.
    @param exclusive Specifies that the region should be locked exclusively (if false the region is locked shared). This argument is true by default.
  */
  LockableRegion(const File& file, const FileRegion& region, bool exclusive) throw(FileException);

  /**
    Returns the locked file region.
  */
  inline FileRegion getRegion() const throw() {return region;}

  /**
    Lock the specified file region (the old region is unlocked first).

    @param region The new file region to be locked.
    @param exclusive Specifies that the region should be locked exclusively (if false the region is locked shared). This argument is true by default.
  */
  void lock(const FileRegion& region, bool exclusive = true) throw(FileException);

  /**
    Destroys the file region lock (unlocks the region).
  */
  ~LockableRegion() throw(FileException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
