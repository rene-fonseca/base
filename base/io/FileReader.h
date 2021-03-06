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

#include <base/io/MappedFile.h>
#include <base/iterator/SequenceIterator.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  File reader utilizing the file mapping support.
  
  @short Optimal file reader.
  @ingroup io
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API FileReader : public Object {
public:

  /**
    Iterator used to traverse the elements within the current file mapping
    window.
  */
  typedef SequenceIterator<ReadIteratorTraits<uint8> > ReadIterator;
  /** The default preferred window size. */
  static constexpr unsigned int DEFAULT_WINDOW_SIZE = 0x10000;
private:

  /** The size of the file. */
  const long long fileSize = 0;
  /** The required granularity of the file region offset. */
  unsigned int granularity = 0;
  /** The preferred window size. */
  unsigned int preferredWindowSize = 0;
  /** The current mapping. */
  MappedFile mapping;
  /** The beginning of the window. */
  uint8* begin = nullptr;
  /** The end of the window. */
  uint8* end = nullptr;
  /** The current position in the window. */
  uint8* current = nullptr;
protected:
  
  inline FileRegion fixRegion(const FileRegion& region) noexcept
  {
    unsigned int index = region.getOffset() % granularity;
    long long offset = region.getOffset()/granularity*granularity;
    unsigned int minimumSize = region.getSize() + index;
    unsigned int size = (maximum(minimumSize, preferredWindowSize) + granularity - 1)/granularity*granularity;
    long long availableBytes = fileSize - offset;
    if ((availableBytes > 0) && (size > availableBytes)) {
      size = static_cast<unsigned int>(availableBytes);
    }
    if (size < minimumSize) { // force IOException if minimumSize bytes is not available
      size = minimumSize;
    }
    return FileRegion(offset, size);
  }
  
  /**
    Requests mapping of the specified region.

    @param region The region to be mapped.
  */
  void requestRegion(const FileRegion& region);
public:
  
  /**
    Initializes the object.

    @param file The file.
    @param position The initial position.
    @param windowSize The preferred size of the mapping window. The actual
    window size used may be less or greater.
  */
  FileReader(
    File& file,
    long long position,
    unsigned int windowSize = DEFAULT_WINDOW_SIZE);
  
  /**
    Returns the bytes of the entire window.
  */
  inline const uint8* getBytes() const noexcept
  {
    return begin;
  }
  
  /**
    Returns the available bytes.
  */
  inline MemorySize getSize() const noexcept
  {
    return end - begin;
  }
  
  /**
    Returns the current position indexed from the beginning of the file.
  */
  inline long long getPosition() const noexcept
  {
    return mapping.getRegion().getOffset() + (current - begin);
  }

  /**
    Ensures that the specified number of bytes are mapped without changing the
    current position. Raises IOException if requested number of bytes cannot be
    mapped.

    @param size The number of bytes to peek at.
    @return A ReadIterator to the first element.
  */
  inline ReadIterator peek(unsigned int size)
  {
    if (current + size > end) { // make sure all bytes are in the current window
      requestRegion(
        FileRegion(mapping.getRegion().getOffset() + (current - begin), size)
      );
    }
    return ReadIterator(current);
  }

  /**
    Fills the specified buffer with the requested number of bytes and
    increments the position, respectively.

    @param buffer The buffer.
    @param size The number of bytes the copy to the buffer.
  */
  void read(uint8* buffer, unsigned int size);

  /**
    Sets the current position indexed from the beginning of the file. Used this
    method if you want to skip a large number of bytes. Raises IOException if
    the postion exceeds the end of the file.

    @param position The desired postion.
  */
  inline void seek(long long position)
  {
    requestRegion(FileRegion(position, 0));
  }

  /**
    Skips the specified number of bytes. Normally used to skip a small number
    of bytes.

    @param size The number of bytes to skip.
  */
  inline void skip(unsigned int size)
  {
    current += size;
    if (current > end) {
      requestRegion(
        FileRegion(
          mapping.getRegion().getOffset() + mapping.getRegion().getSize() +
            (current - end),
          0
        )
      );
    }
  }
};

//template<class TYPE>
//class MappedSequence {
//protected:
//
//  /** The file map. */
//  MappedFile map;
//  const TYPE* elements = nullptr;
//public:
//
//  /**
//    @param position The position of the sequence.
//  */
//  inline MappedSequence(File& file, long long position, unsigned int size) {
//    long long granularity = MappedFile::getGranularity();
//    const unsigned int index = position % granularity; // offset to the sequence within the window
//    map = MappedFile(file, FileRegion(position/granularity*granularity, size * sizeof(TYPE) + index));
//    elements = Cast::pointer<const TYPE*>(map.getBytes() + index);
//  }
//
//  inline MappedSequence(const MappedSequence& copy) noexcept : map(copy.map), elements(copy.elements) {
//  }
//
//  /**
//    Returns the sequence.
//  */
//  inline const TYPE* getElements() const noexcept {
//    return elements;
//  }
//};
//
///**
//  @short File reader.
//  @version 1.0
//*/
//
//class FileReader : public Object {
//public:
//
//  /** Iterator used to traverse the elements within the current file mapping window. */
//  typedef SequenceIterator<ReadIteratorTraits<uint8> > ReadIterator;
//protected:
//
//  /** The granularity of the position. */
//  unsigned int granularity = 0;
//  /** Granularity mask. */
//  unsigned long long highMask = 0;
//  unsigned int lowMask = 0;
//  /** The file. */
//  File file;
//  /** The file providing the stream. */
//  MappedFile mapping;
//  /** The beginning of the internal buffer. */
//  uint8* begin = nullptr;
//  /** The end of the internal buffer. */
//  uint8* end = nullptr;
//protected:
//
//  /**
//    Sets the region.
//  */
//  inline void setRegion(const FileRegion& region) {
//    mapping = MappedFile(file, region);
//    begin = mapping.getBytes();
//    end = begin + region.getSize();
//  }
//public:
//
//  /**
//    Initializes the file reader.
//
//    @param file The file to be read.
//  */
//  FileReader(File& file) noexcept;
//
//  /**
//    Returns the first element of the file mapping window.
//  */
//  inline ReadIterator getBegin() const noexcept {
//    return ReadIterator(begin);
//  }
//
//  /**
//    Returns the end of the file mapping window.
//  */
//  inline ReadIterator getEnd() const noexcept {
//    return ReadIterator(end);
//  }
//
//  /**
//    Returns the first byte of the mapping.
//  */
//  inline const uint8* getBytes() const noexcept {
//    return begin;
//  }
//
//  /**
//    Returns the current position of the window.
//  */
//  inline const FileRegion& getRegion() const noexcept {
//    return mapping.getRegion();
//  }
//
//  /**
//    Moves the window to the next region.
//  */
//  inline ReadIterator next() {
//    const long long totalSize = file.getSize();
//    const long long offset = getRegion().getOffset() + granularity;
//    setRegion(FileRegion(offset, minimum<unsigned int>(granularity, totalSize - offset)));
//    return ReadIterator(begin);
//  }
//
//  /**
//    Moves the window to the previous region.
//  */
//  inline ReadIterator previous() {
//    const long long totalSize = file.getSize();
//    const long long offset = getRegion().getOffset() - granularity;
//    setRegion(FileRegion(offset, minimum<unsigned int>(granularity, totalSize - offset)));
//    return ReadIterator(begin);
//  }
//
//  /**
//    Maps the file region corresponding to the specified position into memory.
//
//    @return The index within the mapped window of the specified file position.
//  */
//  inline ReadIterator setPosition(long long position) {
//    const long long totalSize = file.getSize();
//    const long long offset = position & highMask;
//    setRegion(FileRegion(offset, minimum<unsigned int>(granularity, totalSize - offset)));
//    return ReadIterator(begin + (position & lowMask)); // no overflow 'cause granularity is small
//  }
//
//  /**
//  */
//  ReadIterator ensure(const FileRegion& region);
//
//  /**
//    Ensures that the specified region is in the window. Raises IOException if
//    the specified region cannot be completely mapped. The size of the window is
//    adjusted according to the specified region and the internal granularity.
//
//    @param region The file region.
//  */
//  inline ReadIterator niceEnsure(const FileRegion& region) {
//    if (mapping.getRegion().isWithin(region)) { // is the region already in the current window
//      return ReadIterator(begin + (region.getOffset() - mapping.getRegion().getOffset())); // no overflow
//    } else {
//      return ensure(region);
//    }
//  }
//
//  /**
//    Maps a sequence into memory.
//
//    @param position The offset of the sequence.
//    @param size The number of elements in the sequence.
//  */
//  template<class TYPE>
//  inline MappedSequence<TYPE> getMapping(long long position, unsigned int size) {
//    const unsigned int index = position & lowMask; // offset to the sequence within the window
//    return MappedSequence<TYPE>(
//      MappedFile(file, FileRegion(position & highMask, size * sizeof(TYPE) + index)),
//      index
//    );
//  }
//};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
