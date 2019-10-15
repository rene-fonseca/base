/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/Application.h>
#include <base/Primitives.h>
#include <base/mem/Allocator.h>
#include <base/io/File.h>
#include <base/UnsignedInteger.h>

// TAG: calculate md5sum of original file and write value as comment
// TAG: protect against duplicate codes

// TAG: problem with: 0x010425, 1, 0x01044d, // 1122:0
//                    0x000000, 0, // 1122:1

using namespace base;

class UnicodeApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  static const unsigned int MODULUS = 1455;

  enum FieldIndex {
    FIELD_CODE = 0,
    FIELD_CATEGORY = 2,
    FIELD_DECIMAL_VALUE = 6,
    FIELD_DIGIT_VALUE = 7,
    FIELD_NUMERIC_VALUE = 8,
    FIELD_UPPER = 12,
    FIELD_LOWER = 13,
    FIELD_TITLE = 14
  };

  enum Category {
    LETTER_UPPER,
    LETTER_LOWER,
    LETTER_TITLE,
    LETTER_MODIFIER,
    LETTER_OTHER,
    MARK_NONSPACING,
    MARK_SPACING_COMBINING,
    MARK_ENCLOSING,
    NUMBER_DECIMAL,
    NUMBER_LETTER,
    NUMBER_OTHER,
    PUNCTUATION_CONNECTOR,
    PUNCTUATION_DASH,
    PUNCTUATION_OPEN,
    PUNCTUATION_CLOSE,
    PUNCTUATION_INITIAL,
    PUNCTUATION_FINAL,
    PUNCTUATION_OTHER,
    SYMBOL_MATH,
    SYMBOL_CURRENCY,
    SYMBOL_MODIFIER,
    SYMBOL_OTHER,
    SEPARATOR_SPACE,
    SEPARATOR_LINE,
    SEPARATOR_PARAGRAPH,
    OTHER_CONTROL,
    OTHER_FORMAT,
    OTHER_SURROGATE,
    OTHER_PRIVATE_USE,
    OTHER
  };

  enum Flag {
    LETTER = 1,
    UPPER = 2,
    LOWER = 4,
    TITLE = 8,
    DECIMAL = 16,
    SPACE = 32,
    NEWLINE = 64,
    CONTROL = 128,
    PUNCTUATION = 256,
    HAS_UPPER = 1 << 16,
    HAS_LOWER = 1 << 17,
    HAS_TITLE = 1 << 18
  };
  
  struct Descriptor {
    ucs4 code;
    unsigned int flags : 28;
    unsigned int value : 4;
    ucs4 upper;
    ucs4 lower;
    ucs4 title;
  };
  
  Array<Descriptor> descriptors;
public:
  
  struct HashEntry {
    unsigned int numberOfCodes; // the current number of codes in the table
    unsigned int hits; // codes for this hash value
    unsigned int index; // index of the first code
  };

  UnicodeApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw() 
    : Application("Unicode", numberOfArguments, arguments, environment) {
  }

  void doit(const String& path) throw() {
    unsigned int totalNumberOfCodes = 0;
    unsigned int totalNumberOfUpper = 0;
    unsigned int totalNumberOfLower = 0;
    unsigned int totalNumberOfTitle = 0;
    unsigned int totalNumberOfSpaces = 0;
    unsigned int totalNumberOfControlCodes = 0;
    unsigned int totalNumberOfDigits = 0;
        
    File file;
    try {
      file = File(path, File::READ, 0);
    } catch (IOException& e) {
      ferr << "Error: " << e << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }

    Allocator<uint8> buffer(4096);
    uint64 bytesToRead = file.getSize();
    String line;
    Array<uint32> codeTable;
//    Map<uint32, unsigned int> codes;
    
    // load all mappings
    while (bytesToRead) {
      unsigned int bytesRead = file.read(
        buffer.getElements(),
        minimum<uint64>(buffer.getSize(), bytesToRead)
      );
      bytesToRead -= bytesRead;
      
      const uint8* src = buffer.getElements();
      const uint8* end = src + bytesRead;
      while ((src < end) || line.isProper()) {
        const uint8* newline = find<uint8>(
          src,
          end - src,
          '\n'
        );
        if (newline) {
          line.append(String(Cast::pointer<const char*>(src), newline - src));
          src = newline + 1;
        } else {
          line.append(String(Cast::pointer<const char*>(src), end - src));
          src = end;
          if (bytesToRead > 0) {
            // TAG: do not read too much
            break;
          }
        }
        int comment = line.indexOf('#');
        if (comment >= 0) {
          line = line.substring(0, comment);
        }

        line.trim();
        if (!line.isProper()) {
          continue; // blank line
        }
        
        Array<String> fields = line.split(';');
        Array<String>::ReadEnumerator enu = fields.getReadEnumerator();
        
        uint32 code = UnsignedInteger(*enu.next(), UnsignedInteger::HEX);
        if (code > 0x10ffff) {
          ferr << "Error: Invalid character code" << ENDL;
          setExitCode(EXIT_CODE_ERROR);
          return;
        }

        unsigned int flags = 0;
        
        String categoryStr = fields[FIELD_CATEGORY];
        Category category;
        if (categoryStr == "Lu") {
          category = LETTER_UPPER;
          flags = LETTER | UPPER;
          ++totalNumberOfUpper;
        } else if (categoryStr == "Ll") {
          category = LETTER_LOWER;
          flags = LETTER | LOWER;
          ++totalNumberOfLower;
        } else if (categoryStr == "Lt") {
          category = LETTER_TITLE;
          flags = LETTER | TITLE;
          ++totalNumberOfTitle;
        } else if (categoryStr == "Lm") {
          category = LETTER_MODIFIER;
        } else if (categoryStr == "Lo") {
          category = LETTER_OTHER;
        } else if (categoryStr == "Mn") {
          category = MARK_NONSPACING;
        } else if (categoryStr == "Mc") {
          category = MARK_SPACING_COMBINING;
        } else if (categoryStr == "Me") {
          category = MARK_ENCLOSING;
        } else if (categoryStr == "Nd") {
          category = NUMBER_DECIMAL;
          flags = DECIMAL;
        } else if (categoryStr == "Nl") {
          category = NUMBER_LETTER;
        } else if (categoryStr == "No") {
          category = NUMBER_OTHER;
        } else if (categoryStr == "Pc") {
          category = PUNCTUATION_CONNECTOR;
          flags = PUNCTUATION;
        } else if (categoryStr == "Pd") {
          category = PUNCTUATION_DASH;
          flags = PUNCTUATION;
        } else if (categoryStr == "Ps") {
          category = PUNCTUATION_OPEN;
          flags = PUNCTUATION;
        } else if (categoryStr == "Pe") {
          category = PUNCTUATION_CLOSE;
          flags = PUNCTUATION;
        } else if (categoryStr == "Pi") {
          category = PUNCTUATION_INITIAL;
          flags = PUNCTUATION;
        } else if (categoryStr == "Pf") {
          category = PUNCTUATION_FINAL;
          flags = PUNCTUATION;
        } else if (categoryStr == "Po") {
          category = PUNCTUATION_OTHER;
          flags = PUNCTUATION;
        } else if (categoryStr == "Sm") {
          category = SYMBOL_MATH;
        } else if (categoryStr == "Sc") {
          category = SYMBOL_CURRENCY;
        } else if (categoryStr == "Sk") {
          category = SYMBOL_MODIFIER;
        } else if (categoryStr == "So") {
          category = SYMBOL_OTHER;
        } else if (categoryStr == "Zs") {
          category = SEPARATOR_SPACE;
          flags = SPACE;
          ++totalNumberOfSpaces;
        } else if (categoryStr == "Zl") {
          category = SEPARATOR_LINE;
          flags = NEWLINE;
        } else if (categoryStr == "Zp") {
          category = SEPARATOR_PARAGRAPH;
        } else if (categoryStr == "Cc") {
          category = OTHER_CONTROL;
          flags = CONTROL;
          ++totalNumberOfControlCodes;
        } else if (categoryStr == "Cf") {
          category = OTHER_FORMAT;
        } else if (categoryStr == "Cs") {
          category = OTHER_SURROGATE;
        } else if (categoryStr == "Co") {
          category =  OTHER_PRIVATE_USE;
        } else { // "Cn"
          category = OTHER;
        }

        ucs4 upperCharacter = 0;
        if (static_cast<String>(fields[FIELD_UPPER]).isProper()) {
          flags |= HAS_UPPER;
          upperCharacter = UnsignedInteger(fields[FIELD_UPPER], UnsignedInteger::HEX);
        }
        bassert(WideString::isUCS4(upperCharacter), InvalidFormat());
        
        ucs4 lowerCharacter = 0;
        if (static_cast<String>(fields[FIELD_LOWER]).isProper()) {
          flags |= HAS_LOWER;
          lowerCharacter = UnsignedInteger(fields[FIELD_LOWER], UnsignedInteger::HEX);
        }
        bassert(WideString::isUCS4(lowerCharacter), InvalidFormat());

        ucs4 titleCharacter = 0;
        if (static_cast<String>(fields[FIELD_TITLE]).isProper()) {
          flags |= HAS_TITLE;
          titleCharacter = UnsignedInteger(fields[FIELD_TITLE], UnsignedInteger::HEX);
        }
        bassert(WideString::isUCS4(titleCharacter), InvalidFormat());

        unsigned int value = 0;
        if (flags & DECIMAL) {
          value = UnsignedInteger(fields[FIELD_DECIMAL_VALUE], UnsignedInteger::DEC);
        }
        
        if (flags & DECIMAL) {
          ++totalNumberOfDigits;
        }

        if (flags != 0) {
          ++totalNumberOfCodes;
          
          Descriptor descriptor;
          descriptor.code = code;
          descriptor.flags = flags;
          descriptor.value = value;
          descriptor.lower = lowerCharacter;
          descriptor.upper = upperCharacter;
          descriptor.title = titleCharacter;
          descriptors.append(descriptor);
        }
        
//         String mapping = *enu.next();
//         mapping.trim();
//         Array<String> mappings = mapping.split(' '); //, true);
        
//         codeTable.append(code);
//         codeTable.append(mappings.getSize());

//         {
//           Array<String>::ReadEnumerator enu = mappings.getReadEnumerator();
//           while (enu.hasNext()) {    
//             uint32 code = UnsignedInteger(*enu.next(), UnsignedInteger::HEX);
//             if (code > 0x10ffff) {
//               ferr << "Error: Invalid character code" << ENDL;
//               setExitCode(EXIT_CODE_ERROR);
//               return;
//             }
//             codeTable.append(code); // mapping code
//           }
//         }
        
        line = String();
      }
    }

    // generate: header
    fout << "// This is an automatically generated file. Do NOT edit." << EOL
         << "// This source was generated from the file " << path << EOL
         << "// At " << Date::getNow() << EOL
         << ENDL;

    // generate: controlCharacters
    {
      Array<Descriptor>::ReadIterator ite = descriptors.getBeginIterator();
      
      unsigned int totalNumberOfRanges = 0;
      for (unsigned int i = 0; i < totalNumberOfControlCodes;) {
        while ((ite->flags & CONTROL) == 0) {
          ++ite;
        }
        
        uint32 firstCode = ite->code;
        uint32 expectedCode = firstCode + 1;
        ++ite;
        while ((ite->flags & CONTROL) && (ite->code == expectedCode)) {
          ++expectedCode;
          ++ite;
        }
        i += expectedCode - firstCode;
        ++totalNumberOfRanges;
      }
     
      ite = descriptors.getBeginIterator();
      fout << "namespace base {" << EOL
           << EOL
           << indent(2) << "const WideTraits::Range WideTraits::controlCharacters["
           << totalNumberOfRanges << "] = {" << EOL; // TAG: fixme
      for (unsigned int i = 0; i < totalNumberOfControlCodes;) {
        while ((ite->flags & CONTROL) == 0) {
          ++ite;
        }

        // check range
        uint32 firstCode = ite->code;
        uint32 expectedCode = firstCode + 1;
        ++ite;
        while ((ite->flags & CONTROL) && (ite->code == expectedCode)) {
          ++expectedCode;
          ++ite;
        }
        unsigned int range = expectedCode - firstCode;
        
        fout << indent(4) << '{' << HEX << firstCode << ',' << ' ' << HEX << (firstCode+range-1) << '}';
        if (range > 1) {
          fout << (((i+range-1) < (totalNumberOfControlCodes - 1)) ? ", // " : " // ") << i << '-' << (i+range-1) << EOL;
        } else {
          fout << (((i+range-1) < (totalNumberOfControlCodes - 1)) ? ", // " : " // ") << i << EOL;
        }
        
        i += range;
      }
      
      fout << indent(2) << "}; // end of controlCharacters" << EOL
           << "}; // end of base namespace" << ENDL;
    }

    fout << EOL;
    
    // generate: upperCharacters
    {
      Array<Descriptor>::ReadIterator ite = descriptors.getBeginIterator();

      unsigned int totalNumberOfRanges = 0;
      for (unsigned int i = 0; i < totalNumberOfUpper;) {
        while ((ite->flags & (LETTER|UPPER)) != (LETTER|UPPER)) {
          ++ite;
        }
        
        uint32 firstCode = ite->code;
        uint32 expectedCode = firstCode + 1;
        ++ite;
        while (((ite->flags & (LETTER|UPPER)) == (LETTER|UPPER)) && (ite->code == expectedCode)) {
          ++expectedCode;
          ++ite;
        }
        i += expectedCode - firstCode;
        ++totalNumberOfRanges;
      }

      ite = descriptors.getBeginIterator();
      
      fout << "namespace base {" << EOL
           << EOL
           << indent(2) << "const WideTraits::Range WideTraits::upperCharacters["
           << totalNumberOfRanges << "] = {" << EOL; // TAG: fixme
      for (unsigned int i = 0; i < totalNumberOfUpper;) {
        while ((ite->flags & (LETTER|UPPER)) != (LETTER|UPPER)) {
          ++ite;
        }

        // check range
        uint32 firstCode = ite->code;
        uint32 expectedCode = firstCode + 1;
        ++ite;
        while (((ite->flags & (LETTER|UPPER)) == (LETTER|UPPER)) && (ite->code == expectedCode)) {
          ++expectedCode;
          ++ite;
        }
        unsigned int range = expectedCode - firstCode;
        
        fout << indent(4) << '{' << HEX << firstCode << ',' << ' ' << HEX << (firstCode+range-1) << '}';
        if (range > 1) {
          fout << (((i+range-1) < (totalNumberOfUpper - 1)) ? ", // " : " // ") << i << '-' << (i+range-1) << EOL;
        } else {
          fout << (((i+range-1) < (totalNumberOfUpper - 1)) ? ", // " : " // ") << i << EOL;
        }
        
        i += range;        
      }
      fout << indent(2) << "}; // end of upperCharacters" << EOL
           << "}; // end of base namespace" << ENDL;
    }

    fout << EOL;

    // generate: lowerCharacters
    {
      Array<Descriptor>::ReadIterator ite = descriptors.getBeginIterator();

      unsigned int totalNumberOfRanges = 0;
      for (unsigned int i = 0; i < totalNumberOfLower;) {
        while ((ite->flags & (LETTER|LOWER)) != (LETTER|LOWER)) {
          ++ite;
        }
        
        uint32 firstCode = ite->code;
        uint32 expectedCode = firstCode + 1;
        ++ite;
        while (((ite->flags & (LETTER|LOWER)) == (LETTER|LOWER)) && (ite->code == expectedCode)) {
          ++expectedCode;
          ++ite;
        }
        i += expectedCode - firstCode;
        ++totalNumberOfRanges;
      }

      ite = descriptors.getBeginIterator();
      
      fout << "namespace base {" << EOL
           << EOL
           << indent(2) << "const WideTraits::Range WideTraits::lowerCharacters["
           << totalNumberOfRanges << "] = {" << EOL; // TAG: fixme
      for (unsigned int i = 0; i < totalNumberOfLower;) {
        while ((ite->flags & (LETTER|LOWER)) != (LETTER|LOWER)) {
          ++ite;
        }

        // check range
        uint32 firstCode = ite->code;
        uint32 expectedCode = firstCode + 1;
        ++ite;
        while (((ite->flags & (LETTER|LOWER)) == (LETTER|LOWER)) && (ite->code == expectedCode)) {
          ++expectedCode;
          ++ite;
        }
        unsigned int range = expectedCode - firstCode;
        
        fout << indent(4) << '{' << HEX << firstCode << ',' << ' ' << HEX << (firstCode+range-1) << '}';
        if (range > 1) {
          fout << (((i+range-1) < (totalNumberOfLower - 1)) ? ", // " : " // ") << i << '-' << (i+range-1) << EOL;
        } else {
          fout << (((i+range-1) < (totalNumberOfLower - 1)) ? ", // " : " // ") << i << EOL;
        }
        
        i += range;        
      }
      fout << indent(2) << "}; // end of lowerCharacters" << EOL
           << "}; // end of base namespace" << ENDL;
    }

    fout << EOL;
    
    // generate: titleCharacters
    {
      Array<Descriptor>::ReadIterator ite = descriptors.getBeginIterator();

      unsigned int totalNumberOfRanges = 0;
      for (unsigned int i = 0; i < totalNumberOfTitle;) {
        while ((ite->flags & (LETTER|TITLE)) != (LETTER|TITLE)) {
          ++ite;
        }
        
        uint32 firstCode = ite->code;
        uint32 expectedCode = firstCode + 1;
        ++ite;
        while (((ite->flags & (LETTER|TITLE)) == (LETTER|TITLE)) && (ite->code == expectedCode)) {
          ++expectedCode;
          ++ite;
        }
        i += expectedCode - firstCode;
        ++totalNumberOfRanges;
      }

      ite = descriptors.getBeginIterator();
      
      fout << "namespace base {" << EOL
           << EOL
           << indent(2) << "const WideTraits::Range WideTraits::titleCharacters["
           << totalNumberOfRanges << "] = {" << EOL; // TAG: fixme
      for (unsigned int i = 0; i < totalNumberOfTitle;) {
        while ((ite->flags & (LETTER|TITLE)) != (LETTER|TITLE)) {
          ++ite;
        }

        // check range
        uint32 firstCode = ite->code;
        uint32 expectedCode = firstCode + 1;
        ++ite;
        while (((ite->flags & (LETTER|TITLE)) == (LETTER|TITLE)) && (ite->code == expectedCode)) {
          ++expectedCode;
          ++ite;
        }
        unsigned int range = expectedCode - firstCode;
        
        fout << indent(4) << '{' << HEX << firstCode << ',' << ' ' << HEX << (firstCode+range-1) << '}';
        if (range > 1) {
          fout << (((i+range-1) < (totalNumberOfTitle - 1)) ? ", // " : " // ") << i << '-' << (i+range-1) << EOL;
        } else {
          fout << (((i+range-1) < (totalNumberOfTitle - 1)) ? ", // " : " // ") << i << EOL;
        }
        
        i += range;
      }
      fout << indent(2) << "}; // end of titleCharacters" << EOL
           << "}; // end of base namespace" << ENDL;
    }

    fout << EOL;

    // generate: spaceCharacters
    {
      Array<Descriptor>::ReadIterator ite = descriptors.getBeginIterator();

      unsigned int totalNumberOfRanges = 0;
      for (unsigned int i = 0; i < totalNumberOfSpaces;) {
        while ((ite->flags & SPACE) == 0) {
          ++ite;
        }
        
        uint32 firstCode = ite->code;
        uint32 expectedCode = firstCode + 1;
        ++ite;
        while ((ite->flags & SPACE) && (ite->code == expectedCode)) {
          ++expectedCode;
          ++ite;
        }
        i += expectedCode - firstCode;
        ++totalNumberOfRanges;
      }

      ite = descriptors.getBeginIterator();
      
      fout << "namespace base {" << EOL
           << EOL
           << indent(2) << "const WideTraits::Range WideTraits::spaceCharacters["
           << totalNumberOfRanges << "] = {" << EOL;
      for (unsigned int i = 0; i < totalNumberOfSpaces;) {
        while ((ite->flags & SPACE) == 0) {
          ++ite;
        }

        // check range
        uint32 firstCode = ite->code;
        uint32 expectedCode = firstCode + 1;
        ++ite;
        while ((ite->flags & SPACE) && (ite->code == expectedCode)) {
          ++expectedCode;
          ++ite;
        }
        unsigned int range = expectedCode - firstCode;
        
        fout << indent(4) << '{' << HEX << firstCode << ',' << ' ' << HEX << (firstCode+range-1) << '}';
        if (range > 1) {
          fout << (((i+range-1) < (totalNumberOfSpaces - 1)) ? ", // " : " // ") << i << '-' << (i+range-1) << EOL;
        } else {
          fout << (((i+range-1) < (totalNumberOfSpaces - 1)) ? ", // " : " // ") << i << EOL;
        }
        
        i += range;
      }
      fout << indent(2) << "}; // end of spaceCharacters" << EOL
           << "}; // end of base namespace" << ENDL;
    }

    fout << EOL;
    
    // generate: characters
    {
      Array<Descriptor>::ReadIterator ite = descriptors.getBeginIterator();
      
      fout << "namespace base {" << EOL
           << EOL
           << indent(2) << "const WideTraits::CharacterDescriptor WideTraits::characters["
           << totalNumberOfCodes << "] = {" << EOL;
      for (unsigned int i = 0; i < totalNumberOfCodes;) {
        unsigned int flags = ite->flags;
        if (flags) {
          fout << indent(4) << '{' << HEX << ite->code << ',';
          bool first = true;
          if (flags & LETTER) {
            fout << ((first) ? ' ' : '|') << "LETTER";
            first = false;
          }
          if (flags & UPPER) {
            fout << ((first) ? ' ' : '|') << "UPPER";
            first = false;
          }
          if (flags & LOWER) {
            fout << ((first) ? ' ' : '|') << "LOWER";
            first = false;
          }
          if (flags & TITLE) {
            fout << ((first) ? ' ' : '|') << "TITLE";
            first = false;
          }
          if (flags & DECIMAL) {
            fout << ((first) ? ' ' : '|') << "DIGIT";
            first = false;
          }
          if (flags & CONTROL) {
            fout << ((first) ? ' ' : '|') << "CONTROL";
            first = false;
          }
          if (flags & PUNCTUATION) {
            fout << ((first) ? ' ' : '|') << "PUNCTUATION";
            first = false;
          }
          if (flags & SPACE) {
            fout << ((first) ? ' ' : '|') << "SPACE";
            first = false;
          }
          if (flags & NEWLINE) {
            fout << ((first) ? ' ' : '|') << "NEWLINE";
            first = false;
          }
          if (first) {
            fout << ' ' << 0;
            first = false;
          }
          fout << ',' << ' ' << HEX << ite->upper
               << ',' << ' ' << HEX << ite->lower
               << ',' << ' ' << HEX << ite->title
               << ((i < (totalNumberOfCodes - 1)) ? "}, // " : "} // ") << i << EOL;
          ++i;
        }
        ++ite;
      }
      fout << indent(2) << "}; // end of characters" << EOL
           << "}; // end of base namespace" << ENDL;
    }

    fout << EOL;
    
    // generate: digitCharacters
    {
      Array<Descriptor>::ReadIterator ite = descriptors.getBeginIterator();

      unsigned int totalNumberOfRanges = 0;
      for (unsigned int i = 0; i < totalNumberOfDigits;) {
        while ((ite->flags & DECIMAL) == 0) {
          ++ite;
        }
        
        unsigned int firstCode = ite->code;
        unsigned int expectedCode = firstCode + 1;
        unsigned int expectedValue = ite->value + 1;
        ++ite;
        while ((ite->flags & DECIMAL) && (ite->code == expectedCode) && (ite->value == expectedValue)) {
          ++expectedCode;
          ++expectedValue;
          ++ite;
        }
        i += expectedCode - firstCode;
        ++totalNumberOfRanges;
      }
      
      ite = descriptors.getBeginIterator();
      
      fout << "namespace base {" << EOL
           << EOL
           << indent(2) << "const WideTraits::DigitCharacterDescriptor WideTraits::digitCharacters["
           << totalNumberOfRanges << "] = {" << EOL;
      for (unsigned int i = 0; i < totalNumberOfDigits;) {
        while ((ite->flags & DECIMAL) == 0) {
          ++ite;
        }

        unsigned int firstCode = ite->code;
        unsigned int firstValue = ite->value;
        unsigned int expectedCode = firstCode + 1;
        unsigned int expectedValue = firstValue + 1;
        ++ite;
        while ((ite->flags & DECIMAL) && (ite->code == expectedCode) && (ite->value == expectedValue)) {
          ++expectedCode;
          ++expectedValue;
          ++ite;
        }
        unsigned int range = expectedCode - firstCode;
        
        fout << indent(4) << '{' << HEX << firstCode << ',' << ' ' << HEX << (firstCode+range-1) << ',' << ' ' << firstValue << '}';
        if (range > 1) {
          fout << (((i+range-1) < (totalNumberOfDigits - 1)) ? ", // " : " // ") << i << '-' << (i+range-1) << EOL;
        } else {
          fout << (((i+range-1) < (totalNumberOfDigits - 1)) ? ", // " : " // ") << i << EOL;
        }
        
        i += range;
      }
      fout << indent(2) << "}; // end of digitCharacters" << EOL
           << "}; // end of base namespace" << ENDL;
    }
    
    return;
    
    // check all codes here
    
    HashEntry hashTable[MODULUS];
    fill<char>(Cast::getCharAddress(hashTable), sizeof(hashTable), 0);
    
    Array<uint32>::ReadIterator i = codeTable.getBeginIterator();
    Array<uint32>::ReadIterator end = codeTable.getEndIterator();

    // determine number of codes per hash table slot
    totalNumberOfCodes = 0;
    while (i != end) {
      ++totalNumberOfCodes;
      uint32 code = *i++;
      uint32 numberOfMappings = *i++;
      unsigned int hash = code % MODULUS; // TAG: optimize
      hashTable[hash].hits++;
      for (unsigned int mapping = 0; mapping < numberOfMappings; ++mapping) {
        *i++; // skip mappings
      }
    }
    
    // reserve code indices
    unsigned int totalHits = 0;
    for (unsigned int i = 0; i < MODULUS; ++i) {
      hashTable[i].index = totalHits;
      totalHits += hashTable[i].hits;
    }
    
    Allocator<unsigned int> codeIndexTable(totalNumberOfCodes);

    // fill code indices
    i = codeTable.getBeginIterator();
    unsigned int* indexTable = codeIndexTable.getElements();
    unsigned int index = 0;
    while (i != end) {
      uint32 code = *i++;
      uint32 numberOfMappings = *i++;
      unsigned int hash = code % MODULUS; // TAG: optimize
      indexTable[hashTable[hash].index] = index;
      ++index;
      ++index;
      hashTable[hash].numberOfCodes++;
      for (unsigned int mapping = 0; mapping < numberOfMappings; ++mapping) {
        *i++; // skip mappings
        ++index;
      }
    }
    
    unsigned int stat[999];
    fill<unsigned int>(stat, 999, 0);

    // dump information
    fout << "// This is an automatically generated file. Do NOT edit." << EOL
         << "// This source was generated from the file " << path << EOL
         << "// At " << Date::getNow() << EOL
         << ENDL;

    fout << "namespace base {" << EOL
         << EOL;
    
    // dump hash table
    fout << indent(2) << "WideString::HashEntry WideString::hashTable[]" << " = {" << EOL;
    for (unsigned int i = 0; i < MODULUS; ++i) {
      if (hashTable[i].numberOfCodes < 999) {
        stat[hashTable[i].numberOfCodes]++;
      }
      fout << indent(4) << '{'
           << hashTable[i].numberOfCodes << ',' << ' ' << hashTable[i].index
           << '}';
      if (i < (MODULUS - 1)) {
        fout << ',';
        }
      fout << " // " << i << EOL;
    }
    fout << indent(2) << "};" << EOL
         << EOL;
    
    unsigned int totalCost = 0;
    for (unsigned int i = 0; i < 999; ++i) {
      totalCost += i * i * stat[i];
      if (stat[i]) {
        fout << indent(2) << "// number of hash table entries with " << i << " hit(s): " << stat[i] << EOL;
      }
    }
    fout << indent(2) << "// total cost: " << totalCost << EOL
         << indent(2) << "// modulus: " << MODULUS << EOL
         << indent(2) << "// cost/modulus ratio: " << static_cast<double>(totalCost)/MODULUS << EOL
         << indent(2) << "// number of codes: " << totalNumberOfCodes << EOL
         << EOL;
    
    // dump mapping table
    {
      Array<uint32>::ReadIterator ite = codeTable.getBeginIterator();

      int lastHash = MODULUS - 1;
      while (hashTable[lastHash].numberOfCodes == 0) {
        --lastHash;
      }
      
      fout << indent(2) << "WideString::MappingEntry WideString::mappingTable[]" << " = {" << EOL;
      for (int hash = 0; hash < lastHash; ++hash) {
        if (hashTable[hash].numberOfCodes == 0) {
          continue;
        }
        unsigned int index = indexTable[hashTable[hash].index];
        int numberOfCodes = hashTable[hash].numberOfCodes;
        for (int i = 0; i < numberOfCodes; ++i) {
          uint32 code = ite[index++];
          uint32 numberOfMappings = ite[index++];
          fout << indent(4) << HEX << PREFIX << ZEROPAD << setWidth(8) << code << ',' << ' ' << numberOfMappings;
          for (unsigned int mapping = 0; mapping < numberOfMappings; ++mapping) {
            fout << ',' << ' ' << HEX << PREFIX << ZEROPAD << setWidth(8) << ite[index++];
          }
          fout << ',';
          fout << " // " << hash << ":" << i << EOL; 
        }
      }
      fout << indent(4) << "0 // end" << EOL;
    }
    
    fout << indent(2) << "};" << EOL
         << "}; // end of base namespace" << EOL
         << EOL;
  }
  
  void main() throw() {
    Array<String> arguments = getArguments();
    
    switch (arguments.getSize()) {
    case 0: // use default
      setExitCode(Application::EXIT_CODE_ERROR);
      ferr << "Usage: [source file] or [source file] [output file]" << ENDL;
      return;
    case 1:
      {
        String argument = arguments[0];
        if (argument == "--version") {
          fout << getFormalName() << " version " << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
               << "The Base Framework (Test Suite)" << EOL
               << "http://www.mip.sdu.dk/~fonseca/base" << EOL
               << "Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>" << EOL
               << ENDL;
          return;
        }
        doit(argument); // file path
      }
      break;
    default:
      ferr << "Error: Invalid argument(s)" << ENDL;
      setExitCode(Application::EXIT_CODE_ERROR);
      return;
    }
  }
  
  ~UnicodeApplication() throw() {
  }
};

STUB(UnicodeApplication);
