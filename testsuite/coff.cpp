/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/Application.h>
#include <base/mem/Allocator.h>
#include <base/mem/MemoryDump.h>
#include <base/io/File.h>
#include <base/ByteOrder.h>
#include <base/Literal.h>
#include <base/string/StringOutputStream.h>

using namespace com::azure::dev::base;

class CommonObjectFileFormat {
public:

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  /** DOS EXE header. */
  struct ImageDosHeader {
    LittleEndian<uint16> magic; // 0x5a4d ~ "MZ"
    LittleEndian<uint16> bytesOnLastPage;
    LittleEndian<uint16> pagesInFile;
    LittleEndian<uint16> relocations;
    LittleEndian<uint16> headerParagraphs;
    LittleEndian<uint16> minExtraParagraphs;
    LittleEndian<uint16> maxExtraParagraphs;
    LittleEndian<uint16> ss;
    LittleEndian<uint16> sp;
    LittleEndian<uint16> checksum;
    LittleEndian<uint16> ip;
    LittleEndian<uint16> cs;
    LittleEndian<uint16> relocationTableOffset;
    LittleEndian<uint16> overlayNumber;
    LittleEndian<uint16> reserved0[4];
    LittleEndian<uint16> identifier;
    LittleEndian<uint16> information;
    LittleEndian<uint16> reserved1[10];
    LittleEndian<uint32> exeHeaderOffset; // PE header offset
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct COFFHeader {
    LittleEndian<uint16> machine;
    LittleEndian<uint16> numberOfSections;
    LittleEndian<uint32> timeStamp;
    LittleEndian<uint32> symbolTable;
    LittleEndian<uint32> numberOfSymbols;
    LittleEndian<uint16> sizeOfOptionalHeader;
    LittleEndian<uint16> characteristics;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

  /** Architecture. */
  enum MachineType {
    MACHINE_UNKNOWN = 0x0000,
    MACHINE_ALPHA = 0x0184,
    MACHINE_ARM = 0x01c0,
    MACHINE_ALPHA64 = 0x0284,
    MACHINE_I386 = 0x014c,
    MACHINE_IA64 = 0x0200,
    MACHINE_M68K = 0x0268,
    MACHINE_MIPS16 = 0x0266,
    MACHINE_MIPSFPU = 0x0366,
    MACHINE_MIPSFPU16 = 0x0466,
    MACHINE_POWERPC = 0x01f0,
    MACHINE_R3000 = 0x0162,
    MACHINE_R4000 = 0x0166,
    MACHINE_R10000 = 0x0168,
    MACHINE_SH3 = 0x01a2,
    MACHINE_SH4 = 0x01a6,
    MACHINE_THUMB = 0x01c2
  };

  /** Flags. */
  enum Characteristic {
    IMAGE_RELOCATIONS_STRIPPED = 1 << 0,
    IMAGE_EXECUTABLE_IMAGE = 1 << 1,
    IMAGE_LINE_NUMBERS_STRIPPED = 1 << 2,
    IMAGE_LOCAL_SYMBOLS_STRIPPED = 1 << 3,
    IMAGE_AGGRESSIVE_WORKING_SET = 1 << 4,
    IMAGE_LARGE_ADDRESS_AWARE = 1 << 5,
    IMAGE_16BIT_MACHINE = 1 << 6,
    IMAGE_LITTLE_ENDIAN = 1 << 7,
    IMAGE_32BIT_MACHINE = 1 << 8,
    IMAGE_DEBUG_STRIPPED = 1 << 9,
    IMAGE_REMOVABLE_RUN_FROM_SWAP = 1 << 10,
    IMAGE_SYSTEM = 1 << 11,
    IMAGE_DLL = 1 << 12,
    IMAGE_UP_SYSTEM_ONLY = 1 << 13,
    IMAGE_BIG_ENDIAN = 1 << 14
  };

  /** Magic numbers. */
  enum Magic {
    MAGIC_ROM = 0x0107,
    MAGIC_PE32 = 0x010b,
    MAGIC_PE32_PLUS = 0x020b
  };

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct PEHeader32 {
    LittleEndian<uint16> magic;
    uint8 majorLinkerVersion;
    uint8 minorLinkerVersion;
    LittleEndian<uint32> sizeOfCodeSections;
    LittleEndian<uint32> sizeOfInitializedData;
    LittleEndian<uint32> sizeOfUninitializedData;
    LittleEndian<uint32> addressOfEntryPoint;
    LittleEndian<uint32> baseOfCode;
    LittleEndian<uint32> baseOfData;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct PEHeader64 {
    LittleEndian<uint16> magic;
    uint8 majorLinkerVersion;
    uint8 minorLinkerVersion;
    LittleEndian<uint32> sizeOfCodeSections;
    LittleEndian<uint32> sizeOfInitializedData;
    LittleEndian<uint32> sizeOfUninitializedData;
    LittleEndian<uint32> addressOfEntryPoint;
    LittleEndian<uint64> baseOfCode;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct PEWindowsHeader32 {
    LittleEndian<uint32> imageBase;
    LittleEndian<uint32> sectionAlignment;
    LittleEndian<uint32> fileAlignment;
    LittleEndian<uint16> majorOperatingSystemVersion;
    LittleEndian<uint16> minorOperatingSystemVersion;
    LittleEndian<uint16> majorImageVersion;
    LittleEndian<uint16> minorImageVersion;
    LittleEndian<uint16> majorSubsystemVersion;
    LittleEndian<uint16> minorSubsystemVersion;
    LittleEndian<uint32> reserved;
    LittleEndian<uint32> sizeOfImage;
    LittleEndian<uint32> sizeOfHeaders;
    LittleEndian<uint32> checkSum;
    LittleEndian<uint16> subsystem;
    LittleEndian<uint16> dllCharacteristics;
    LittleEndian<uint32> sizeOfReservedStack;
    LittleEndian<uint32> sizeOfCommittedStack;
    LittleEndian<uint32> sizeOfReservedHeap;
    LittleEndian<uint32> sizeOfComittedHeap;
    LittleEndian<uint32> flags;
    LittleEndian<uint32> numberOfDirectoryEntries;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct PEWindowsHeader64 {
    LittleEndian<uint64> imageBase;
    LittleEndian<uint32> sectionAlignment;
    LittleEndian<uint32> fileAlignment;
    LittleEndian<uint16> majorOperatingSystemVersion;
    LittleEndian<uint16> minorOperatingSystemVersion;
    LittleEndian<uint16> majorImageVersion;
    LittleEndian<uint16> minorImageVersion;
    LittleEndian<uint16> majorSubsystemVersion;
    LittleEndian<uint16> minorSubsystemVersion;
    LittleEndian<uint32> reserved;
    LittleEndian<uint32> sizeOfImage;
    LittleEndian<uint32> sizeOfHeaders;
    LittleEndian<uint32> checkSum;
    LittleEndian<uint16> subsystem;
    LittleEndian<uint16> dllCharacteristics;
    LittleEndian<uint64> sizeOfReservedStack;
    LittleEndian<uint64> sizeOfCommittedStack;
    LittleEndian<uint64> sizeOfReservedHeap;
    LittleEndian<uint64> sizeOfComittedHeap;
    LittleEndian<uint32> flags;
    LittleEndian<uint32> numberOfDirectoryEntries;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

  /** Subsystem. */
  enum Subsystem {
    /** Unknown subsystem. */
    SUBSYSTEM_UNKNOWN = 0,
    /** Driver and native processes.*/
    SUBSYSTEM_NATIVE = 1,
    /** Windows GUI subsystem. */
    SUBSYSTEM_WINDOWS_GUI = 2,
    /** Windows character subsystem. */
    SUBSYSTEM_WINDOWS_CUI = 3,
    /** POSIX character subsystem. */
    SUBSYSTEM_POSIX_CUI = 7,
    /** Windows CE. */
    SUBSYSTEM_WINDOWS_CE_GUI = 9,
    /** EFI application. */
    SUBSYSTEM_EFI_APPLICATION = 10,
    /** EFI driver providing boot services. */
    SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER = 11,
    /** EFI driver providing runtime services. */
    SUBSYSTEM_EFI_RUNTIME_DRIVER = 12
  };

  static String getSubsystemDescription(unsigned int subsystem) throw() {
    switch (subsystem) {
    case SUBSYSTEM_UNKNOWN:
      return Literal("unknown subsystem");
    case SUBSYSTEM_NATIVE:
      return Literal("driver and native process subsystem");
    case SUBSYSTEM_WINDOWS_GUI:
      return Literal("Windows graphical user interface subsystem");
    case SUBSYSTEM_WINDOWS_CUI:
      return Literal("Windows character subsystem");
    case SUBSYSTEM_POSIX_CUI:
      return Literal("POSIX subsystem");
    case SUBSYSTEM_WINDOWS_CE_GUI:
      return Literal("Windows CE subsystem");
    case SUBSYSTEM_EFI_APPLICATION:
      return Literal("EFI application");
    case SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:
      return Literal("EFI driver providing boot services");
    case SUBSYSTEM_EFI_RUNTIME_DRIVER:
      return Literal("EFI driver providing runtime services");
    default:
      return Literal("unsupported subsystem");
    }
  }
  
  /** DLL characteristics. */
  enum DLLCharacteristic {
    /** No binding. */
    DLL_CHARACTERISTIC_NO_BIND = 0x0800,
    /** WDM driver. */
    DLL_CHARACTERISTIC_WDM_DRIVER = 0x2000,
    /** Terminal server aware. */
    DLL_CHARACTERISTIC_TERMINAL_SERVER_AWARE = 0x8000
  };

 _COM_AZURE_DEV__BASE__PACKED__BEGIN
  /** Directory entry. */
  struct DirectoryEntry {
    LittleEndian<uint32> rva;
    LittleEndian<uint32> size;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct DirectoryHeader {
    DirectoryEntry exportTable;
    DirectoryEntry importTable;
    DirectoryEntry resourceTable;
    DirectoryEntry exceptionTable;
    DirectoryEntry certificateTable;
    DirectoryEntry baseRelocationTable;
    DirectoryEntry debugData;
    DirectoryEntry architectureData;
    DirectoryEntry globalPointer;
    DirectoryEntry threadLocalStorageTable;
    DirectoryEntry loadConfigTable;
    DirectoryEntry boundImportTable;
    DirectoryEntry importAddressTable;
    DirectoryEntry delayImportDescriptor;
    DirectoryEntry runtimeHeader;
    DirectoryEntry reserved;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct SectionHeader {
    char name[8];
    LittleEndian<uint32> virtualSize;
    LittleEndian<uint32> virtualAddress;
    LittleEndian<uint32> sizeOfRawData;
    LittleEndian<uint32> pointerToRawData;
    LittleEndian<uint32> pointerToRelocations;
    LittleEndian<uint32> pointerToLineNumbers;
    LittleEndian<uint16> numberOfRelocations;
    LittleEndian<uint16> numberOfLineNumbers;
    LittleEndian<uint32> characteristics;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

  static String getMachineDescription(uint32 machine) throw() {
    switch (machine) {
    case MACHINE_UNKNOWN:
      return Literal("unknown");
    case MACHINE_ALPHA:
      return Literal("Alpha AXP");
    case MACHINE_ARM:
      return Literal("ARM");
    case MACHINE_ALPHA64:
      return Literal("Alpha AXP 64");
    case MACHINE_I386:
      return Literal("x86");
    case MACHINE_IA64:
      return Literal("IA64");
    case MACHINE_M68K:
      return Literal("Motorola 68K");
    case MACHINE_MIPS16:
      return Literal("MIPS16");
    case MACHINE_MIPSFPU:
      return Literal("MIPS with FPU");
    case MACHINE_MIPSFPU16:
      return Literal("MIPS16 with FPU");
    case MACHINE_POWERPC:
      return Literal("PowerPC (little endian)");
    case MACHINE_R3000:
      return Literal("R3000");
    case MACHINE_R4000:
      return Literal("R4000 (little endian)");
    case MACHINE_R10000:
      return Literal("R10000");
    case MACHINE_SH3:
      return Literal("Hitachi SH3");
    case MACHINE_SH4:
      return Literal("Hitachi SH4");
    case MACHINE_THUMB:
      return Literal("thumb");
    default:
      return Literal("unsupported machine type");
    }
  }

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct SymbolEntry {
    char name[8];
    LittleEndian<uint32> value;
    LittleEndian<uint16> section; // 1 indexed
    LittleEndian<uint16> type;
    uint8 storageClass;
    uint8 numberOfAuxSymbols;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

  static bool isCOFF(const String& file) throw() {
    return false;
  }
  
  static bool isPE(const String& file) throw() {
    return false;
    // if stub avail and offset is ok and signature is PE\0\0 then assume PE format
  }
  
};

FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  const CommonObjectFileFormat::DirectoryEntry& entry) throw(IOException) {
  stream << '['
         << HEX << PREFIX << ZEROPAD << setWidth(10) << entry.rva << ':'
         << HEX << PREFIX << ZEROPAD << setWidth(10) << entry.size
         << ']';
  return stream;
}

class COFFApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  enum {
    COMMAND_DUMP_SECTION_DATA
  };
  
  COFFApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw() 
    : Application("coff", numberOfArguments, arguments, environment) {
  }

  // TAG: need function to check alignment

  void error(const String& message) throw() {
    ferr << "Error: " << message << ENDL;
    setExitCode(EXIT_CODE_ERROR);
  }

  void dump(const String& message, uint32 value) throw() {
    fout << message << ':' << SP
         << HEX << PREFIX << ZEROPAD << setWidth(10) << value << ENDL;
  }

  /**
    Dump raw data for specified file region.
  */
  void dumpFileRegion(
    File file,
    uint64 offset,
    unsigned int size) throw(IOException) {
    
    uint64 originalPosition = file.getPosition();
    file.setPosition(offset);
    
    unsigned int bytesToRead = size;
    uint64 globalPosition = 0;
    
    uint8 buffer[4096];
    while (bytesToRead) {
      unsigned int bytesRead = file.read(
        buffer,
        minimum<unsigned int>(sizeof(buffer), bytesToRead)
      );
      MemoryDump dump(buffer, bytesRead);
      dump.setGlobalOffset(globalPosition);
      fout << dump << ENDL;
      globalPosition += bytesRead;
      bytesToRead -= bytesRead;         
    }   
    file.setPosition(originalPosition);
  }

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct ExportTableDirectory {
    LittleEndian<uint32> flags;
    LittleEndian<uint32> timeStamp;
    LittleEndian<uint16> majorVersion;
    LittleEndian<uint16> minorVersion;
    LittleEndian<uint32> nameRVA;
    LittleEndian<uint32> ordinalBase;
    LittleEndian<uint32> addressTableEntries;
    LittleEndian<uint32> namePointers;
    LittleEndian<uint32> exportAddressTableRVA;
    LittleEndian<uint32> namePointerRVA;
    LittleEndian<uint32> ordinalTableRVA;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

  void dumpExportSection(
    File file,
    const uint32 virtualAddress,
    const unsigned int rawOffset,
    const unsigned int rawSize) throw(IOException)
  {
    if ((virtualAddress & 3) != 0) {
      throw OutOfDomain(this);
    }
    
    Allocator<uint8> buffer(rawSize);
    
    uint64 originalPosition = file.getPosition();
    file.setPosition(rawOffset);
    try {
      file.read(buffer.getElements(), buffer.getSize());
    } catch (IOException&) {
      file.setPosition(originalPosition);
      throw;
    }
    file.setPosition(originalPosition);
    // TAG: zero pad buffer
    
    const ExportTableDirectory* directory =
      Cast::pointer<const ExportTableDirectory*>(buffer.getElements());

    fout << "flags: "
         << HEX << PREFIX << ZEROPAD << setWidth(10)
         << directory->flags << EOL
      
         << "time stamp: "
         << HEX << PREFIX << ZEROPAD << setWidth(10)
         << directory->timeStamp << EOL
      
         << "version: "
         << directory->majorVersion << '.' << directory->minorVersion << EOL
      
         << "module name: "
         << String(NativeString(Cast::pointer<const char*>(buffer.getElements() + directory->nameRVA - virtualAddress)), buffer.getSize() - (directory->nameRVA - virtualAddress)) << EOL
      
         << "ordinal base: "
         << HEX << PREFIX << ZEROPAD << setWidth(10)
         << directory->ordinalBase << EOL
      
         << "number of address table entries: "
         << HEX << PREFIX << ZEROPAD << setWidth(10)
         << directory->addressTableEntries << EOL
      
         << "number of name pointers: "
         << HEX << PREFIX << ZEROPAD << setWidth(10)
         << directory->namePointers << EOL
      
         << "export address table RVA: "
         << HEX << PREFIX << ZEROPAD << setWidth(10)
         << directory->exportAddressTableRVA << EOL
      
         << "name pointer RVA: "
         << HEX << PREFIX << ZEROPAD << setWidth(10)
         << directory->namePointerRVA << EOL
      
         << "ordinal table RVA: "
         << HEX << PREFIX << ZEROPAD << setWidth(10)
         << directory->ordinalTableRVA << EOL
         << ENDL;

    // MemoryDump dump(buffer.getElements(), buffer.getSize());
    // fout << dump << ENDL;
    
    // TAG: check range
    const LittleEndian<uint32>* exportTable =
      Cast::pointer<const LittleEndian<uint32>*>(
        buffer.getElements() + directory->exportAddressTableRVA - virtualAddress
      );
    // TAG: check range
    const LittleEndian<uint16>* ordinalTable =
      Cast::pointer<const LittleEndian<uint16>*>(
        buffer.getElements() + directory->ordinalTableRVA - virtualAddress
      );
    // TAG: check range
    const LittleEndian<uint32>* nameTable =
      Cast::pointer<const LittleEndian<uint32>*>(
        buffer.getElements() + directory->namePointerRVA - virtualAddress
      );
    
    const unsigned int base = directory->ordinalBase;
    for (unsigned int i = 0; i < directory->namePointers; ++i) {
      const int o = *ordinalTable;
      BASSERT(static_cast<unsigned int>(o) >= base);
      const uint32 address = exportTable[o - base];

      String name = String(
        NativeString(
          Cast::pointer<const char*>(
            buffer.getElements() + (*nameTable - virtualAddress)
          )
        ),
        buffer.getSize() - (*nameTable - virtualAddress)
      );
      // TAG: name must be ascending lexical order
      
      fout << setWidth(10) << HEX << ZEROPAD << PREFIX << address << SP
           << name << SP << '[' << i + base << ']' << ENDL;
      ++nameTable;
      ++ordinalTable;
    }
  }

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct ImportTableDirectory {
    LittleEndian<uint32> importLookupTableRVA;
    LittleEndian<uint32> timeStamp;
    LittleEndian<uint32> forwarderChain;
    LittleEndian<uint32> nameRVA;
    LittleEndian<uint32> importAddressTableRVA;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

  void dumpImportSection(
    File file,
    const uint32 virtualAddress,
    const unsigned int rawOffset,
    const unsigned int rawSize) throw(IOException)
  {
    if ((virtualAddress & 3) != 0) {
      throw OutOfDomain(this);
    }
    
    Allocator<uint8> buffer(rawSize);
    
    uint64 originalPosition = file.getPosition();
    file.setPosition(rawOffset);
    try {
      file.read(buffer.getElements(), buffer.getSize());
    } catch (IOException&) {
      file.setPosition(originalPosition);
      throw;
    }
    file.setPosition(originalPosition);
    // TAG: zero pad buffer
    
    MemoryDump dump(buffer.getElements(), buffer.getSize());
    fout << dump << ENDL;
    
    const ImportTableDirectory* directory =
      Cast::pointer<const ImportTableDirectory*>(buffer.getElements());
    
    while (true) {
      if (!directory->importLookupTableRVA &&
          !directory->timeStamp &&
          !directory->forwarderChain &&
          !directory->nameRVA &&
          !directory->importAddressTableRVA) {
        break;
      }
      
      fout << "import lookup table RVA: "
           << HEX << PREFIX << ZEROPAD << setWidth(10)
           << directory->importLookupTableRVA << EOL
        
           << "time stamp: "
           << HEX << PREFIX << ZEROPAD << setWidth(10)
           << directory->timeStamp << EOL
        
           << "forwarder chain: "
           << directory->forwarderChain << EOL
        
           << "name RVA: "
           << HEX << PREFIX << ZEROPAD << setWidth(10)
           << directory->nameRVA << EOL
        
           << "import address table RVA: "
           << HEX << PREFIX << ZEROPAD << setWidth(10)
           << directory->importAddressTableRVA << EOL
           << ENDL;

      fout << "module name: " << String(
        NativeString(
          Cast::pointer<const char*>(
            buffer.getElements() + directory->nameRVA - virtualAddress
          )
        ),
        buffer.getSize() - (directory->nameRVA - virtualAddress)
      ) << ENDL;
      
      // TAG: check range
      const LittleEndian<uint32>* importLookupTable =
        Cast::pointer<const LittleEndian<uint32>*>(
          buffer.getElements() + directory->importLookupTableRVA - virtualAddress
        );

      // TAG: check for mismatch between lookup and address tables
      while (*importLookupTable) {
        if (*importLookupTable & 0x80000000) {
          fout << "import by ordinal" << SP;
          uint32 ordinal = *importLookupTable & 0x7fffffff;
        } else {
          fout << "import by name" << SP;
          uint32 addressRVA = *importLookupTable & 0x7fffffff;
          bassert((addressRVA & 1) == 0, InvalidFormat(this));
          const void* hintTable =
            buffer.getElements() + addressRVA - virtualAddress;
          const LittleEndian<uint16>* hint =
            Cast::pointer<LittleEndian<uint16>*>(hintTable);
          const NativeString name(
            Cast::pointer<const char*>(hintTable) + sizeof(*hint)
          );
          // TAG: must be terminated
          fout << SP << name << SP;
        }
        fout << ENDL;
        ++importLookupTable;
      }
      ++directory;
    }
  }

  void dumpRelocationsSection(
    File file,
    const uint32 virtualAddress,
    const unsigned int rawOffset,
    const unsigned int rawSize) throw(IOException)
  {
    if ((virtualAddress & 3) != 0) {
      throw OutOfDomain(this);
    }
    
    Allocator<uint8> buffer(rawSize);
    
    uint64 originalPosition = file.getPosition();
    file.setPosition(rawOffset);
    try {
      file.read(buffer.getElements(), buffer.getSize());
    } catch (IOException&) {
      file.setPosition(originalPosition);
      throw;
    }
    file.setPosition(originalPosition);
    // TAG: zero pad buffer
    
    MemoryDump dump(buffer.getElements(), buffer.getSize());
    fout << dump << ENDL;
  }
  
  void dumpDOSHeader(const String& path) throw() {
    CommonObjectFileFormat::ImageDosHeader header;
    File file(path, File::READ, File::EXCLUSIVE);

    file.read(Cast::getAddress(header), sizeof(header));
    fout << "header.magic: "
         << HEX << PREFIX << ZEROPAD << setWidth(6) << header.magic << EOL
         << "PE header offset: "
         << HEX << PREFIX << ZEROPAD << setWidth(10)
         << header.exeHeaderOffset << EOL
         << ENDL;

    if (header.magic != 0x5a4d) {
      error("Unknown file type");
      return;
    }
    
    if ((header.exeHeaderOffset % 8) != 0) {
      error("Unknown file type");
      return;
    }
    
    LittleEndian<uint32> signature;
    file.setPosition(header.exeHeaderOffset);
    file.read(Cast::getAddress(signature), sizeof(signature));
    
    if (signature != 0x00004550) {
      error("Invalid signature");
      return;
    }
    fout << "file offset: " << HEX << file.getPosition() << ENDL;

    bool symbolTableAvailable = false;
    unsigned int symbolTableOffset = 0;
    unsigned int numberOfSymbols = 0;
    bool optionalHeaderAvailable = false;
    unsigned int sizeOfOptionalHeader = 0;
    unsigned int numberOfSections = 0;
    
    {
      CommonObjectFileFormat::COFFHeader header;
      file.read(Cast::getAddress(header), sizeof(header));
      numberOfSections = header.numberOfSections;
      
      fout << "machine: "
           << HEX << PREFIX << ZEROPAD << setWidth(6)
           << header.machine << SP
           << CommonObjectFileFormat::getMachineDescription(header.machine) << EOL
        
           << "number of sections: "
           << HEX << PREFIX << ZEROPAD << setWidth(6)
           << header.numberOfSections << EOL
        
           << "time stamp: "
           << HEX << PREFIX << ZEROPAD << setWidth(10)
           << header.timeStamp << EOL
        
           << "symbol table: "
           << HEX << PREFIX << ZEROPAD << setWidth(10)
           << header.symbolTable << EOL
        
           << "number of symbols: "
           << HEX << PREFIX << ZEROPAD << setWidth(10)
           << header.numberOfSymbols << EOL
        
           << "size of optional header: "
           << HEX << PREFIX << ZEROPAD << setWidth(6)
           << header.sizeOfOptionalHeader << EOL
        
           << ENDL;

      symbolTableAvailable = header.symbolTable != 0;
      symbolTableOffset = header.symbolTable;
      numberOfSymbols = header.numberOfSymbols;
      optionalHeaderAvailable = header.sizeOfOptionalHeader != 0;
      sizeOfOptionalHeader = header.sizeOfOptionalHeader;
      
      bool relocationsStripped =
        header.characteristics & CommonObjectFileFormat::IMAGE_RELOCATIONS_STRIPPED;
      bool executable =
        header.characteristics & CommonObjectFileFormat::IMAGE_EXECUTABLE_IMAGE;
      bool linesStripped =
        header.characteristics & CommonObjectFileFormat::IMAGE_LINE_NUMBERS_STRIPPED;
      bool symbolsStripped =
        header.characteristics & CommonObjectFileFormat::IMAGE_LOCAL_SYMBOLS_STRIPPED;
      bool aggressiveWorkingSet =
        header.characteristics & CommonObjectFileFormat::IMAGE_AGGRESSIVE_WORKING_SET;
      bool largeAddress =
        header.characteristics & CommonObjectFileFormat::IMAGE_LARGE_ADDRESS_AWARE;
      bool archWord16 =
        header.characteristics & CommonObjectFileFormat::IMAGE_16BIT_MACHINE;
      bool littleEndian =
        header.characteristics & CommonObjectFileFormat::IMAGE_LITTLE_ENDIAN;
      bool archWord32 =
        header.characteristics & CommonObjectFileFormat::IMAGE_32BIT_MACHINE;
      bool debugStripped =
        header.characteristics & CommonObjectFileFormat::IMAGE_DEBUG_STRIPPED;
      bool removable =
        header.characteristics & CommonObjectFileFormat::IMAGE_REMOVABLE_RUN_FROM_SWAP;
      bool systemFile =
        header.characteristics & CommonObjectFileFormat::IMAGE_SYSTEM;
      bool libraryFile =
        header.characteristics & CommonObjectFileFormat::IMAGE_DLL;
      bool onlyOnUpSystem =
        header.characteristics & CommonObjectFileFormat::IMAGE_UP_SYSTEM_ONLY;
      bool bigEndian =
        header.characteristics & CommonObjectFileFormat::IMAGE_BIG_ENDIAN;

//       if (littleEndian && bigEndian || (!littleEndian && !bigEndian)) {
//         error("Invalid file");
//         return;
//       }
      
      StringOutputStream stream;
      stream << (bigEndian ? Literal("BE ") : Literal(""))
             << (littleEndian ? Literal("LE ") : Literal(""))
             << (executable ? Literal("EXE ") : Literal(""))
             << (relocationsStripped ? Literal("") : Literal("RELOCS "))
             << (symbolsStripped ? Literal("") : Literal("SYMBOLS "))
             << (linesStripped ? Literal("") : Literal("LINES "))
             << (debugStripped ? Literal("") : Literal("DEBUG "))
             << (removable ? Literal("REMOVABLE ") : Literal(""))
             << (systemFile ? Literal("SYS ") : Literal(""))
             << (libraryFile ? Literal("DLL ") : Literal(""))
             << (archWord16 ? Literal("16BIT ") : Literal(""))
             << (archWord32 ? Literal("32BIT ") : Literal(""))
             << (linesStripped ? Literal("") : Literal("LINES "))
             << FLUSH;
      fout << "characteristics: "
           << HEX << PREFIX << ZEROPAD << setWidth(6) << header.characteristics
           << SP << '[' << stream.getString() << ']' << ENDL;
    }

    unsigned int subsystem = CommonObjectFileFormat::SUBSYSTEM_UNKNOWN;
    unsigned int numberOfDirectoryEntries = 0;
    
    if (optionalHeaderAvailable) {
      LittleEndian<uint16> magic;
      file.read(Cast::getAddress(magic), sizeof(magic));

      fout << "magic: " << HEX << ZEROPAD << PREFIX << setWidth(6)
           << magic << ENDL;

      switch (magic) {
      case CommonObjectFileFormat::MAGIC_ROM:
        fout << "File type: ROM image" << ENDL;
        return;
      case CommonObjectFileFormat::MAGIC_PE32:
        {
          file.setPosition(-static_cast<MemoryDiff>(sizeof(magic)), File::CURRENT);
          CommonObjectFileFormat::PEHeader32 header;
          file.read(Cast::getAddress(header), sizeof(header));
          
          fout << "linker version: "
               << header.majorLinkerVersion << '.'
               << header.minorLinkerVersion << EOL
            
               << "text section (bytes): "
               << HEX << PREFIX << ZEROPAD << setWidth(10)
               << header.sizeOfCodeSections << EOL
            
               << "initialized data (bytes): "
               << HEX << PREFIX << ZEROPAD << setWidth(10)
               << header.sizeOfInitializedData << EOL
            
               << "uninitialized data (bytes): "
               << HEX << PREFIX << ZEROPAD << setWidth(10)
               << header.sizeOfUninitializedData << EOL

               << "entry point: "
               << HEX << PREFIX << ZEROPAD << setWidth(10)
               << header.addressOfEntryPoint << EOL

               << "base of code: "
               << HEX << PREFIX << ZEROPAD << setWidth(10)
               << header.baseOfCode << EOL
            
               << "base of data: "
               << HEX << PREFIX << ZEROPAD << setWidth(10)
               << header.baseOfData << EOL
            
               << ENDL;

          CommonObjectFileFormat::PEWindowsHeader32 windowsHeader;
          file.read(
            Cast::getAddress(windowsHeader),
            sizeof(windowsHeader)
          );
          subsystem = windowsHeader.subsystem;
          numberOfDirectoryEntries = windowsHeader.numberOfDirectoryEntries;
          
          BASSERT(
            (windowsHeader.sizeOfImage % windowsHeader.sectionAlignment) == 0
          );
          BASSERT(
            (windowsHeader.sizeOfHeaders % windowsHeader.fileAlignment) == 0
          );
          
          fout << "image base: "
               << HEX << PREFIX << ZEROPAD << setWidth(10)
               << windowsHeader.imageBase << EOL

               << "section alignment: "
               << HEX << PREFIX << ZEROPAD << setWidth(10)
               << windowsHeader.sectionAlignment << EOL
               
               << "file alignment: "
               << HEX << PREFIX << ZEROPAD << setWidth(10)
               << windowsHeader.fileAlignment << EOL
               
               << "os version: "
               << windowsHeader.majorOperatingSystemVersion << '.'
               << windowsHeader.minorOperatingSystemVersion << EOL
               
               << "image version: "
               << windowsHeader.majorImageVersion << '.'
               << windowsHeader.minorImageVersion << EOL
            
               << "subsystem version: "
               << windowsHeader.majorSubsystemVersion << '.'
               << windowsHeader.minorSubsystemVersion << EOL
               
               << "image size (bytes): "
               << windowsHeader.sizeOfImage << EOL
               
               << "headers size (bytes): "
               << windowsHeader.sizeOfHeaders << EOL
               
               << "checksum: "
               << HEX << PREFIX << ZEROPAD << setWidth(10)
               << windowsHeader.checkSum << EOL
               
               << "subsystem: "
               << HEX << PREFIX << ZEROPAD << setWidth(6)
               << subsystem << SP
               << CommonObjectFileFormat::getSubsystemDescription(subsystem)
               << EOL
               
               << "DLL characteristics: "
               << HEX << PREFIX << ZEROPAD << setWidth(6)
               << windowsHeader.dllCharacteristics << EOL
               
               << "reserved stack (bytes): "
               << windowsHeader.sizeOfReservedStack << EOL
               
               << "comitted stack (bytes): "
               << windowsHeader.sizeOfCommittedStack << EOL
               
               << "reserved heap (bytes): "
               << windowsHeader.sizeOfReservedHeap << EOL
               
               << "committed heap (bytes): "
               << windowsHeader.sizeOfComittedHeap << EOL
               
               << "number of directory entries: "
               << numberOfDirectoryEntries << EOL
               
               << ENDL;
        }
        break;
      case CommonObjectFileFormat::MAGIC_PE32_PLUS:
        break;
      default:
        error("Unsupported format");
        return;
      }
    }

    const uint64 offsetOfSectionTable = file.getPosition() +
      sizeof(CommonObjectFileFormat::DirectoryEntry) *
      numberOfDirectoryEntries;
    if (true && (numberOfDirectoryEntries > 0)) { // if image file
      CommonObjectFileFormat::DirectoryHeader header;
      file.read(Cast::getAddress(header), sizeof(header)); // TAG: fixme
      // TAG: use numberOfDirectoryEntries
      
      fout << "export table: " << header.exportTable << EOL
           << "import table: " << header.importTable << EOL
           << "resource table: " << header.resourceTable << EOL
           << "exception table: " << header.exceptionTable << EOL
           << "certificate table: " << header.certificateTable << EOL
           << "base relocation table: " << header.baseRelocationTable << EOL
           << "debug data: " << header.debugData << EOL
           << "architecture data: " << header.architectureData << EOL
           << "global pointer: " << header.globalPointer << EOL
           << "TLS table: " << header.threadLocalStorageTable << EOL
           << "load config table: " << header.loadConfigTable << EOL
           << "bound import table: " << header.boundImportTable << EOL
           << "import address table: " << header.importAddressTable << EOL
           << "delay import descriptor: " << header.delayImportDescriptor << EOL
           << "COM+ runtime header: " << header.runtimeHeader << EOL
           << ENDL;
    }

    fout << "Section table offset: " << HEX << file.getPosition() << ENDL;
    if (numberOfSections > 0) { // section table
        enum NamedSection {
          UNKNOWN_SECTION,
          BSS_SECTION,
          DATA_SECTION,
          EXPORT_SECTION,
          IMPORT_SECTION,
          READ_ONLY_DATA_SECTION,
          RELOCATIONS_SECTION,
          TEXT_SECTION,
          THREAD_LOCAL_STORAGE_SECTION
        };
        
        static const char
          SECTION_NAMES[][sizeof(CommonObjectFileFormat::SectionHeader)] = {
            {0, 0, 0, 0, 0, 0, 0, 0},
            {'.', 'b', 's', 's', 0, 0, 0, 0},
            {'.', 'd', 'a', 't', 'a', 0, 0, 0},
            {'.', 'e', 'd', 'a', 't', 'a', 0, 0},
            {'.', 'i', 'd', 'a', 't', 'a', 0, 0},
            {'.', 'r', 'd', 'a', 't', 'a', 0, 0},
            {'.', 'r', 'e', 'l', 'o', 'c', 0, 0},
            {'.', 't', 'e', 'x', 't', 0, 0, 0},
            {'.', 't', 'l', 's', 0, 0, 0, 0}
        };

        static const Literal SECTION_DESCRIPTIONS[] = {
          Literal("unknown section"),
          Literal("uninitialized data section"),
          Literal("initialized data section"),
          Literal("export tables"),
          Literal("import tables"),
          Literal("read only data section"),
          Literal("relocations section"),
          Literal("executable code section"),
          Literal("thread local storage section")
        };



      file.setPosition(offsetOfSectionTable);
      
      for (unsigned int i = 0; i < numberOfSections; ++i) {
        CommonObjectFileFormat::SectionHeader header;
        file.read(Cast::getAddress(header), sizeof(header));

        BASSERT(header.sizeOfRawData <= header.virtualSize);

        NamedSection section = UNKNOWN_SECTION;
        for (unsigned int j = BSS_SECTION; j <= THREAD_LOCAL_STORAGE_SECTION; ++j) {
          if (compare(header.name, SECTION_NAMES[j], 8) == 0) {
            section = static_cast<NamedSection>(j);
            break;
          }
        }

        // TAG: String getBinaryName(const char* name, unsigned int length) throw();
        
        // TAG: $ not allowed in name
        fout << "section: " << i << EOL
          
             << "name: "
             << header.name[0] // TAG: fixme - handle 0 and /
             << header.name[1]
             << header.name[2]
             << header.name[3]
             << header.name[4]
             << header.name[5]
             << header.name[6]
             << header.name[7] << SP << SECTION_DESCRIPTIONS[section] << EOL
          
             << "virtual size: "
             << HEX << PREFIX << ZEROPAD << setWidth(10)
             << header.virtualSize << EOL
          
             << "virtual address: "
             << HEX << PREFIX << ZEROPAD << setWidth(10)
             << header.virtualAddress << EOL
          
             << "raw data (bytes): "
             << HEX << PREFIX << ZEROPAD << setWidth(10)
             << header.sizeOfRawData << EOL
          
             << "raw data offset: "
             << HEX << PREFIX << ZEROPAD << setWidth(10)
             << header.pointerToRawData << EOL
          
             << "relocations offset: "
             << HEX << PREFIX << ZEROPAD << setWidth(10)
             << header.pointerToRelocations << EOL
        
             << "line numbers offset: "
             << HEX << PREFIX << ZEROPAD << setWidth(10)
             << header.pointerToLineNumbers << EOL
          
             << "number of relocations: "
             << HEX << PREFIX << ZEROPAD << setWidth(6)
             << header.numberOfRelocations << EOL
        
             << "number of line numbers: "
             << HEX << PREFIX << ZEROPAD << setWidth(6)
             << header.numberOfLineNumbers << EOL
        
             << "characteristics: "
             << HEX << PREFIX << ZEROPAD << setWidth(10)
             << header.characteristics << EOL        
             << EOL
             << ENDL;
        
        switch (section) {
        case BSS_SECTION:
          break;
        case EXPORT_SECTION:
          dumpExportSection(
            file,
            header.virtualAddress,
            header.pointerToRawData,
            header.sizeOfRawData
          );
          break;
        case IMPORT_SECTION:
          dumpImportSection(
            file,
            header.virtualAddress,
            header.pointerToRawData,
            header.sizeOfRawData
          );   
          break;
        default:
          break;
        }
        
        if (false) { // TAG: fixme
          uint64 originalPosition = file.getPosition();
          dumpFileRegion(file, header.pointerToRawData, header.sizeOfRawData);
          file.setPosition(originalPosition);
          
          if (header.sizeOfRawData < header.virtualSize) {
            fout << "in memory zero padding (bytes): "
                 << header.virtualSize - header.sizeOfRawData << ENDL;
          }
        }
      }
    }

    if (symbolTableAvailable) { // symbol table
      Allocator<uint8> stringTable;
      
      {
        const uint64 stringTableOffset =
          static_cast<uint64>(symbolTableOffset) +
          static_cast<uint64>(numberOfSymbols) *
          sizeof(CommonObjectFileFormat::SymbolEntry);
        file.setPosition(stringTableOffset);
        LittleEndian<uint32> temp;
        file.read(Cast::getAddress(temp), sizeof(temp));
        unsigned int size = temp;
        BASSERT(size >= sizeof(temp));
        size -= sizeof(temp);
        stringTable.setSize(size);
        file.read(stringTable.getElements(), size);
        // dumpFileRegion(file, symbolTableOffset, numberOfSymbols * sizeof(CommonObjectFileFormat::SymbolEntry));
      }
      
      
      fout << "symbol table: " << EOL
           << "number of symbols: " << numberOfSymbols << EOL
           << ENDL;
      
      file.setPosition(symbolTableOffset);
      for (unsigned int i = 0; i < numberOfSymbols; ++i) {
        CommonObjectFileFormat::SymbolEntry entry;
        file.read(Cast::getAddress(entry), sizeof(entry));
        String name;
        
        if ((entry.name[0] == 0) && (entry.name[1] == 0) &&
            (entry.name[2] == 0) && (entry.name[3] == 0)) {
          uint32 offset =
            (static_cast<uint32>(static_cast<uint8>(entry.name[7])) << 24) +
            (static_cast<uint32>(static_cast<uint8>(entry.name[6])) << 16) +
            (static_cast<uint32>(static_cast<uint8>(entry.name[5])) << 8) +
            (static_cast<uint32>(static_cast<uint8>(entry.name[4])) << 0);
          BASSERT(offset < stringTable.getSize());
          name = String(
            NativeString(
              Cast::pointer<const char*>(stringTable.getElements() + offset)
            ),
            stringTable.getSize() - offset
          );
        } else {
          name = String(NativeString(entry.name), sizeof(entry.name));
        }
        
        fout << "name: " << name << SP
          
             << "value:"
             << HEX << PREFIX << ZEROPAD << setWidth(10) << entry.value << SP

             << "section:" << entry.section << SP
             << "type:" << entry.type << SP
             << "class:" << entry.storageClass << ENDL;
        
      }
    }
  }
  
  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;

    Array<String> arguments = getArguments();
    
    switch (arguments.getSize()) {
    case 0: // use default
      fout << "Usage: " << getFormalName() << " [OPTIONS] [FILE]" << ENDL;
    case 1:
      {
        String sourcePath = arguments[0]; // file path
        dumpDOSHeader(sourcePath);
      }
      break;
    default:
      setExitCode(EXIT_CODE_ERROR);
    }
  }
};

APPLICATION_STUB(COFFApplication);
