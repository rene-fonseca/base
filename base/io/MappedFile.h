/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__MAPPED_FILE_H
#define _DK_SDU_MIP__BASE_IO__MAPPED_FILE_H

#include <base/Object.h>
#include <base/io/File.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class is used to map a specified file region into the address space of the process.

  @short File region mapper.
  @author René Møller Fonseca
  @version 1.0
*/

class MappedFile : public Object {
private:

  class MappedFileImpl : public ReferenceCountedObject {
  private:

    File file; // ensure that the file is not closed before map has been closed - may not be required
    FileRegion region;
    bool writeable;
    void* bytes;
  public:

    MappedFileImpl(const File& file, const FileRegion& region, bool writeable) throw(FileException);

    inline void* getBytes() const throw() {return bytes;}

    inline File& getFile() throw() {return file;}

    inline const FileRegion& getRegion() const throw() {return region;}

    inline bool isWriteable() throw() {return writeable;}

    void flush() const throw(FileException);

    ~MappedFileImpl() throw(FileException);
  };

  friend MappedFileImpl;

  /** The internal mapping representation. */
  ReferenceCountedObjectPointer<MappedFileImpl> map;
protected:

  /**
    Returns the handle of the specified file.
  */
  inline static int getHandle(File& file) {return file.fd->getHandle();}
public:

  /**
    Returns the required granularity of the file region offset.
  */
  static unsigned int getGranularity() throw();

  /**
    Initializes a file mapping.

    @param file The file to be mapped into memory.
    @param region The file region to be mapped. The offset of the region must honour the granularity returned by getGranularity.
    @param writeable Specifies that write access is required. Default is false.
  */
  MappedFile(const File& file, const FileRegion& region, bool writeable = false) throw(FileException);

  /**
    Initializes mapping from other mapping.
  */
  inline MappedFile(const MappedFile& copy) throw() : map(copy.map) {}

  /**
    Assignment of mapping by mapping.
  */
  MappedFile& operator=(const MappedFile& eq) throw();

  /**
    Returns the mapped bytes. Do not use the mapping outside the requested
    mapping range.
  */
  inline byte* getBytes() const throw() {return static_cast<byte*>(map->getBytes());}

  /**
    Returns the mapped file region.
  */
  inline const FileRegion& getRegion() const throw() {return map->getRegion();}

  /**
    Flushes the mapping.
  */
  inline void flush() const throw(FileException) {map->flush();}

  /**
    Maps the specified file region.

    @region The file region to be mapped.
  */
  inline void remap(const FileRegion& region) throw(FileException) {
    map = new MappedFileImpl(map->getFile(), region, map->isWriteable());
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
