/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/io/File.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class is used to map a specified file region into the address space of
  the process.

  @short File region mapper.
  @ingroup io
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/

class MappedFile : public Object {
private:

  class MappedFileImpl : public ReferenceCountedObject {
  private:
    
    File file; // ensure that the file is not closed before map has been closed - may not be required
    FileRegion region;
    bool writeable = false;
    void* bytes = nullptr;
  public:

    MappedFileImpl(
      const File& file,
      const FileRegion& region,
      bool writeable) throw(FileException);
    
    inline void* getBytes() const throw() {
      return bytes;
    }

    inline File& getFile() throw() {
      return file;
    }

    inline const FileRegion& getRegion() const throw() {
      return region;
    }

    inline bool isWriteable() throw() {
      return writeable;
    }

    void synchronize() throw(FileException);

    ~MappedFileImpl();
  };

  friend class MappedFileImpl;

  /** The internal mapping representation. */
  Reference<MappedFileImpl> map;
protected:

  /**
    Returns the handle of the specified file.
  */
  inline static OperatingSystem::Handle getHandle(File& file) throw() {
    return file.fd->getHandle();
  }
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
  inline MappedFile(const MappedFile& copy) throw()
    : map(copy.map) {
  }

  /**
    Assignment of mapping by mapping.
  */
  MappedFile& operator=(const MappedFile& eq) throw();

  /**
    Returns the mapped bytes. Do not use the mapping outside the requested
    mapping range.
  */
  inline uint8* getBytes() const throw() {
    return static_cast<uint8*>(map->getBytes());
  }

  /**
    Returns the mapped file region.
  */
  inline const FileRegion& getRegion() const throw() {
    return map->getRegion();
  }

  /**
    Flushes the mapping.
  */
  inline void synchronize() throw(FileException) {
    map->synchronize();
  }

  /**
    Maps the specified file region.

    @param region The file region to be mapped.
  */
  inline void remap(const FileRegion& region) throw(FileException) {
    map = new MappedFileImpl(map->getFile(), region, map->isWriteable());
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
