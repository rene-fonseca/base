/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

CharacterSet CharacterSet::load(
  const String& path) throw(FileException, InvalidFormat) {
  try {
    File file(path, File::READ, 0);
    FileRegion region(0, minimum<uint64>(file.getSize(), 4096 * 4));
    MappedFile mappedFile(file, region);
    const uint8* data = mappedFile.getBytes();

    // check format (EUI-64)
    // <ff> <ff> <ff> // company
    // <12> <34> <56> <67> <89> // format
    data += 8; // skip id
    
    // check version
    uint8 majorVersion = *data++;
    uint8 minorVersion = *data++;
    uint8 microVersion = *data++;
    uint8 reserved = *data++;
    bassert(
      (majorVersion == 1) /* && (minorVersion >= 0) && (microVersion >= 0) && (reserved >= 0) */,
      InvalidFormat(Type::getType<CharacterSet>())
    );
    
    BASSERT(Cast::getOffset(data) % sizeof(uint32) == 0);
    
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
      const BigEndian<uint32>* src =
        Cast::pointer<const BigEndian<uint32>*>(data);
      for (unsigned int i = 0; i < 256; ++i) {
        *dest++ = *src++;
      }
    }
    
    // check if UCS-4 values are valid
    {
      const ucs4* src = characterSet.characterSet->getElements();
      for (unsigned int i = 0; i < 256; ++i) {
        // duplicates are allowed
        bassert(
          Unicode::isUCS4(*src++),
          InvalidFormat(Type::getType<CharacterSet>())
        );
      }
    }
    
    // ignore any other data
    
    return characterSet;
  } catch (IOException&) {
    throw FileException(Type::getType<CharacterSet>());
  }
}

CharacterSet CharacterSet::load(
  const Array<String>& searchPaths,
  const String& path) throw(FileException, InvalidFormat) {
  // TAG: default path "/usr/share/base/charset/" and name "8859-1"
  if (FileSystem::isAbsolutePath(path)) {
    return load(path);
  } else {
    Array<String>::ReadEnumerator enu = searchPaths.getReadEnumerator();
    while (enu.hasNext()) {
      String absolutePath = FileSystem::toAbsolutePath(*enu.next(), path);
      if (FileSystem::fileExists(absolutePath)) {
        try {
          return load(absolutePath);
        } catch (Exception&) {
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

void CharacterSet::save(const String& path, Architecture::ByteOrder byteOrder) const throw(FileException) {
  try {
    File file(path, File::WRITE, File::CREATE|File::TRUNCATE|File::EXCLUSIVE);
    
    // check format (EUI-64)
    uint8 buffer[8 + 4 + sizeof(ucs4) + 256 * sizeof(ucs4)]; // id + version + bom + chars
    uint8* header = buffer;
    
    // id
    *header++ = 0xff; // TAG: not reserved
    *header++ = 0xff;
    *header++ = 0xff;
    *header++ = 0x12; // TAG: not allocated
    *header++ = 0x34;
    *header++ = 0x56;
    *header++ = 0x78;
    *header++ = 0x90;
    
    // check version
    *header++ = 1; // majorVersion
    *header++ = 0; // minorVersion
    *header++ = 0; // microVersion
    *header++ = 0; // reserved
    
    const ucs4* src = characterSet->getElements();
    
    if (byteOrder == Architecture::LE) {
      // load data lot (little endian)
      LittleEndian<uint32>* dest =
        Cast::pointer<LittleEndian<uint32>*>(header);
      *dest++ = WideString::BOM;
      for (unsigned int i = 0; i < 256; ++i) {
        *dest++ = *src++;
      }
    } else if (byteOrder == Architecture::BE) {
      // load data lot (big endian)
      BigEndian<uint32>* dest = Cast::pointer<BigEndian<uint32>*>(header);
      *dest++ = WideString::BOM;
      for (unsigned int i = 0; i < 256; ++i) {
        *dest++ = *src++;
      }
    }
    file.write(buffer, sizeof(buffer));
  } catch (IOException&) {
    throw FileException(Type::getType<CharacterSet>());
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
