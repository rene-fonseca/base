/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/CharacterSet.h>
#include <base/Architecture.h>
#include <base/io/MappedFile.h>
#include <base/ByteOrder.h>
#include <base/filesystem/FileSystem.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

CharacterSet CharacterSet::load(const String& path) throw(FileException, InvalidFormat) {
  try {
    // binary format
    
    File file(path, File::READ, 0);
    FileRegion region(0, minimum<uint64>(file.getSize(), 4096 * 4));
    MappedFile mappedFile(file, region);
    const uint8* data = mappedFile.getBytes();

    // check format (EUI-64)
    // OID <ff> <ff> <ff> // company
    // <12> <34> <56> <67> <89> // format
      
    // check version
    uint8 majorVersion = *data++;
    uint8 minorVersion = *data++;
    uint8 microVersion = *data++;
    uint8 reserved = *data++;
    assert(
      (majorVersion == 1) && (minorVersion >= 0) && (microVersion >= 0) && (reserved >= 0),
      InvalidFormat(Type::getType<CharacterSet>())
    );
    
    ASSERT(Cast::getOffset(data) % sizeof(uint32) == 0);
    
    CharacterSet characterSet;
    ucs4* dest = characterSet.characterSet->getElements();
    
    // byte order
    uint32 BOM = WideString::BOM;
    Architecture::ByteOrder byteOrder;
    if ((data[0] == (BOM >> 0)) && (data[1] == (BOM >> 8)) && (data[2] == (BOM >> 16)) && (data[3] == (BOM >> 24))) {
      byteOrder = Architecture::LE;
    } else if ((data[0] == (BOM >> 24)) && (data[1] == (BOM >> 16)) && (data[2] == (BOM >> 8)) && (data[3] == (BOM >> 0))) {
      byteOrder = Architecture::BE;
    } else {
      throw InvalidFormat(Type::getType<CharacterSet>());
    }
    
    if (byteOrder == Architecture::LE) {
      // load data lot (little endian)
      const LittleEndian<uint32>* src = Cast::pointer<const LittleEndian<uint32>*>(data);
      for (unsigned int i = 0; i < 256; ++i) {
        *dest++ = *src++;
      }
    } else if (byteOrder == Architecture::BE) {
      // load data lot (big endian)
      const BigEndian<uint32>* src = Cast::pointer<const BigEndian<uint32>*>(data);
      for (unsigned int i = 0; i < 256; ++i) {
        *dest++ = *src++;
      }
    }
    
    // check if UCS-4 values are valid
//      const ucs4* src = characterSet;
    for (unsigned int i = 0; i < 256; ++i) {
//        assert(isUCS4(*src++), Exception(Type::getType<CharacterSet>()));
    }

    // ignore any other data
  } catch (IOException& e) {
    throw FileException(Type::getType<CharacterSet>());
  }
}

CharacterSet CharacterSet::load(const Array<String>& searchPaths, const String& path) throw(FileException, InvalidFormat) {
  // TAG: default path "/usr/share/mip.sdu.dk/base/charset/" and name "8859-1"
  if (FileSystem::isAbsolutePath(path)) {
    return load(path);
  } else {
    Array<String>::ReadEnumerator enu = searchPaths.getReadEnumerator();
    while (enu.hasNext()) {
      String absolutePath = FileSystem::toAbsolutePath(*enu.next(), path);
      if (FileSystem::fileExists(absolutePath)) {
        try {
          return load(absolutePath);
        } catch (FileException& e) {
          // TAG: } catch (InvalidFormat& e) {
        }
      }
    }
    throw FileException(Type::getType<CharacterSet>());
  }
}
  
CharacterSet::CharacterSet() throw(MemoryException)
  : characterSet(new CharacterSetImpl(256)) {
  fill<ucs4>(characterSet->getElements(), characterSet->getSize(), 0);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
