/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A region of a file.

  @author René Møller Fonseca
  @version 1.0
*/

class FileRegion : public Object {
private:

  /** The offset of the region from the beginning of the file. */
  long long offset;
  /** The size of the region (in bytes). */
  unsigned int size;
public:

  FileRegion(long long o, unsigned int s) throw() : offset(o), size(s) {}

  FileRegion(const FileRegion& copy) throw() : offset(copy.offset), size(copy.size) {}

  FileRegion& operator=(const FileRegion& eq) throw() {
    if (&eq != this) {
      offset = eq.offset;
      size = eq.size;
    }
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

class MappedFile;

/**
  File.

  @author René Møller Fonseca
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

    int handle;
  public:

    FileImpl() throw() : handle(-1) {}
    explicit FileImpl(int h) throw() : handle(h) {}
    int getHandle() const throw() {return handle;}
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
    return read((char*)&buffer, sizeof(TYPE));
  }

  /**
    Throws EndOfFile if minimum number of bytes cannot be read without
    exceeding the end of the file.
  */
  unsigned int write(const char* buffer, unsigned int size, bool nonblocking = false) throw(FileException);
};



/**
  Locked file region.

  @author René Møller Fonseca
  @version 1.0
*/

class LockableRegion : public Object {
private:

  /** The file. */
  File file;
  /** The region. */
  FileRegion region;
public:

  inline LockableRegion(const File& f, const FileRegion& r, bool exclusive = true) throw(FileException) : file(f), region(r) {
    file.lock(region, exclusive);
  }

  inline FileRegion getRegion() const throw() {return region;}

  inline ~LockableRegion() throw(FileException) {
    file.unlock(region);
  }
};



///**
//  This class is used to map a specified file region into the address space of the process.
//
//  @short File region mapper.
//  @author René Møller Fonseca
//  @version 1.0
//*/
//
//class MappedFile : public Object {
//private:
//
//  class MappedFileImpl : public ReferenceCountedObject {
//  private:
//
//    File file; // ensure that the file is not closed before map has been closed - may not be required
//    FileRegion region;
//    bool writeable;
//    void* bytes;
//  public:
//
//    MappedFileImpl(const File& file, const FileRegion& region, bool writeable) throw(FileException);
//
//    inline void* getBytes() const throw() {return bytes;}
//
//    inline File& getFile() throw() {return file;}
//
//    inline const FileRegion& getRegion() const throw() {return region;}
//
//    inline bool isWriteable() throw() {return writeable;}
//
//    void flush() const throw(FileException);
//
//    ~MappedFileImpl() throw(FileException);
//  };
//
//  /** The internal mapping representation. */
//  ReferenceCountedObjectPointer<MappedFileImpl> map;
//protected:
//
//  /**
//    Returns the handle of the specified file.
//  */
//  inline static int getHandle(File& file) {return file.fd->getHandle();}
//public:
//
//  /**
//    Returns the required granularity of the file region offset.
//  */
//  static unsigned int getGranularity() throw();
//
//  /**
//  */
//  MappedFile() throw();
//
//  /**
//    Initializes a file mapping.
//
//    @param file The file to be mapped into memory.
//    @param region The file region to be mapped. The offset of the region must honour the granularity returned by getGranularity.
//    @param writeable Specifies that write access is required. Default is false.
//  */
//  MappedFile(const File& file, const FileRegion& region, bool writeable = false) throw(FileException);
//
//  /**
//    Initializes mapping from other mapping.
//  */
//  inline MappedFile(const MappedFile& copy) throw() : map(copy.map) {}
//
//  /**
//    Assignment of mapping by mapping.
//  */
//  MappedFile& operator=(const MappedFile& eq) throw();
//
//  /**
//    Returns the mapped bytes. Do not use the mapping outside the requested
//    mapping range.
//  */
//  inline byte* getBytes() const throw() {return static_cast<byte*>(map->getBytes());}
//
//  /**
//    Returns the mapped file region.
//  */
//  inline const FileRegion& getRegion() const throw() {return map->getRegion();}
//
//  /**
//    Flushes the mapping.
//  */
//  inline void flush() const throw(FileException) {map->flush();}
//
//  /**
//    Maps the specified file region.
//
//    @region The file region to be mapped.
//  */
//  inline void remap(const FileRegion& region) throw(FileException) {
//    map = new MappedFileImpl(map->getFile(), region, map->isWriteable());
//  }
//};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
