/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/FileReader.h>
#include <base/Base.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void FileReader::requestRegion(const FileRegion& region) throw(IOException) {
  mapping.remap(fixRegion(region));
  begin = mapping.getBytes();
  end = begin + mapping.getRegion().getSize();
  current = begin + (region.getOffset() - mapping.getRegion().getOffset());
}

FileReader::FileReader(File& file, long long position, unsigned int windowSize = DEFAULT_WINDOW_SIZE) throw(IOException) :
  fileSize(file.getSize()),
  granularity(MappedFile::getGranularity()),
  preferredWindowSize(windowSize),
  mapping(file, fixRegion(FileRegion(position, 0))) {

  begin = mapping.getBytes();
  end = begin + mapping.getRegion().getSize();
  current = begin + (position - mapping.getRegion().getOffset());
}

void FileReader::read(byte* buffer, unsigned int size) throw(IOException) {
  peek(size);
  copy(buffer, current, size);
  skip(size);
}


//FileReader::FileReader(File& f) throw() :
//  granularity(MappedFile::getGranularity()),
//  file(f),
//  mapping(f, FileRegion(0, 0)) {
//
//  unsigned int temp = 1; // make granularity a power of two
//  while (temp < granularity) {
//    temp <<= 1;
//  }
//  granularity = temp;
//  lowMask = granularity - 1;
//  highMask = ~static_cast<unsigned long long>(lowMask);
//}
//
//ReadIterator FileReader::ensure(FileRegion& region) throw(IOException) {
//  const long long offset = region.getOffset() & highMask;
//  const unsigned int index = region.getOffset() & lowMask;
//  const long long totalSize = file.getSize();
//  unsigned int size = region.getSize() + index; // the minimum size
//  long long availableBytes = totalSize - offset;
//  // minimumSize <= finalSize && finalSize <= availableBytes
//  if (availableBytes > 0) {
//    unsigned int MAXIMUM_SIZE = granularity << 1; // set by user or default 4096 <= x <= 40960???
//    maxSize = minimum(MAXIMUM_SIZE, availableBytes);
//    size = (size + granularity - 1)/granularity * granularity;
//    if (size > availableBytes) {
//      size = availableBytes;
//    }
//    size = maximum<unsigned int>(size, maximumSize);
//    size = minimum<unsigned int>(size, maximumSize);
//    if (size < minimumSize) {
//      size = minimumSize;
//    }
//  }
//  setRegion(FileRegion(offset, finalSize));
//  return ReadIterator(begin + index); // no overflow 'cause granularity is small
//}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
