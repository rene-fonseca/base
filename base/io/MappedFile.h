/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/io/File.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class is used to map a specified file region into the address space of
  the process.

  @short File region mapper.
  @ingroup io
  @version 1.1
*/

class _COM_AZURE_DEV__BASE__API MappedFile : public Object {
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
      bool writeable);
    
    inline void* getBytes() const noexcept
    {
      return bytes;
    }

    inline File& getFile() noexcept
    {
      return file;
    }

    inline const FileRegion& getRegion() const noexcept
    {
      return region;
    }

    inline bool isWriteable() noexcept
    {
      return writeable;
    }

    void synchronize();

    ~MappedFileImpl();
  };

  friend class MappedFileImpl;

  /** The internal mapping representation. */
  Reference<MappedFileImpl> map;
public:

  /**
    Returns the required granularity of the file region offset.
  */
  static unsigned int getGranularity() noexcept;

  /**
    Initializes a file mapping.

    @param file The file to be mapped into memory.
    @param region The file region to be mapped. The offset of the region must honour the granularity returned by getGranularity.
    @param writeable Specifies that write access is required. Default is false.
  */
  MappedFile(const File& file, const FileRegion& region, bool writeable = false);

  /**
    Initializes mapping from other mapping.
  */
  inline MappedFile(const MappedFile& copy) noexcept
    : map(copy.map) {
  }

  /**
    Assignment of mapping by mapping.
  */
  MappedFile& operator=(const MappedFile& assign) noexcept;

  /**
    Returns the mapped bytes. Do not use the mapping outside the requested
    mapping range.
  */
  inline uint8* getBytes() const noexcept
  {
    return static_cast<uint8*>(map->getBytes());
  }

  /**
    Returns the mapped file region.
  */
  inline const FileRegion& getRegion() const noexcept
  {
    return map->getRegion();
  }

  /**
    Flushes the mapping.
  */
  inline void synchronize()
  {
    map->synchronize();
  }

  /**
    Maps the specified file region.

    @param region The file region to be mapped.
  */
  inline void remap(const FileRegion& region)
  {
    map = new MappedFileImpl(map->getFile(), region, map->isWriteable());
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
