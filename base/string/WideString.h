/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__WIDE_STRING_H
#define _DK_SDU_MIP__BASE_STRING__WIDE_STRING_H

#include <base/Object.h>
#include <base/OutOfRange.h>
#include <base/OutOfDomain.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/mem/ReferenceCountedCapacityAllocator.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/MultibyteException.h>
#include <base/string/WideStringException.h>
#include <base/mem/AllocatorEnumeration.h>
#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class FormatOutputStream;

/**
  This class binds together a wide string literal and its length. Use the macro
  WIDEMESSAGE to construct an object of this class for a given wide string
  literal (e.g. WIDEMESSAGE("Hello World")). Do not call the constructor
  manually.
  
  @short Wide string literal
  @ingroup string
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/
class WideStringLiteral {
private:
  
  /** The number of characters occupied by the message without the terminator. */
  const unsigned int length;
  /** NULL-terminated message. */
  const wchar* message;
public:
  
  /** Initializes message. Automatically invocated by the macro WIDEMESSAGE. */
  inline WideStringLiteral(unsigned int _length, const wchar* _message) throw()
    : length(_length), message(_message) {
  }
  
  /** Cast to the usual message type. */
  inline operator const wchar*() const throw() {
    return message;
  }
  
  /** Returns the length of the string. */
  inline unsigned int getLength() const throw() {
    return length;
  }
};

/** This macro constructs a WideStringLiteral object from the given string literal. */
#define WIDEMESSAGE(message) \
  WideStringLiteral(sizeof(L ## message)/sizeof(wchar) - 1, L ## message)



/**
  Default wide-character properties and manipulators.

  @short Wide-character description.
  @ingroup string
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/
class WideTraits {
public:

  /** The type of a single character. */
  typedef ucs4 Character;
  /** Specifies the terminator for NULL-terminated strings. */
  static const ucs4 TERMINATOR = 0;
private:

  struct Range {
    uint32 first;
    uint32 last;
  };
  
  static const Range controlCharacters[];
  static const Range upperCharacters[];
  static const Range lowerCharacters[];
  static const Range titleCharacters[];
  static const Range spaceCharacters[];

  struct CharacterDescriptor {
    uint32 code;
    uint32 flags;
    uint32 lower;
    uint32 upper;
    uint32 title;
  };
  
  static const CharacterDescriptor characters[];

  struct DigitCharacterDescriptor {
    uint32 first;
    uint32 last;
    uint32 value;
  };

  static const DigitCharacterDescriptor digitCharacters[];
public:

  /** Character characteristics. */
  enum Characteristics {
    LETTER = 1, /**< Character is a letter (e.g. "a" or "A"). */
    LOWER = 2, /**< Character is lower cased. */
    UPPER = 4, /**< Character is upper cased. */
    TITLE = 8, /**< Character is title cased. */
    DIGIT = 16, /**< Any of the characters "0123456789". */
    CONTROL = 32, /**< The characters from 0x00 to 0x1f and 0x7f. */
    SPACE = 64, /**< Tab, newline, vertical-tab, form-feed, carriage-return, and space. */
    NEWLINE = 128, /**< Line separator. */
    PUNCTUATION = 256 /**< Punctuation characters. */
  };
  
  /** Returns the flags describing the specified character code. */
  static unsigned int getFlags(Character character) throw();
  
  /** Returns true if the character a letter. */
  static inline bool isLetter(Character character) throw() {
    return getFlags(character) & LETTER;
  }

  /** Returns true if the character is a letter or a digit. */
  static inline bool isLetterDigit(Character character) throw() {
    return getFlags(character) & (LETTER|DIGIT);
  }
  
  /** Returns true if the character is in lower case. */
  static bool isLower(Character character) throw();
  
  /** Returns true if the character is in upper case. */
  static bool isUpper(Character character) throw();
  
  /** Returns true if the character is in title case. */
  static bool isTitle(Character character) throw();

  /** Returns true if the character is cased. */
  // static inline bool isCased(Character character) throw();
  
  /** Returns true if the character is a digit. */
  static bool isDigit(Character character) throw();
  
  /** Returns true if the character is a hex digit. */
  static inline bool isHexDigit(Character character) throw() {
    if (character < 0x80) { // TAG: need support for all "abcdefABCDEF"
      return ASCIITraits::isHexDigit(character);
    }
    return isDigit(character);
  }
  
  /** Returns true if the character is a white space. */
  static bool isSpace(Character character) throw(); // TAG: inline check for most common space (i.e. ' ')
  
  /** Returns true if the character is a punctuation mark. */
  static inline bool isPunctuation(Character character) throw() {
    // TAG: check UNIX spec
    // TAG: use table with only punctuation codes
    return getFlags(character) & PUNCTUATION;
  }
  
  /** Returns true if the character is a control character. */
  static bool isControl(Character character) throw();
  
  /** Returns true if the character is an ASCII character. */
  static inline bool isASCII(Character character) throw() {
    return character < 0x80;
  }

  /** Maps the character to lower case. */
  static Character toLower(Character character) throw();
  
  /** Maps the character to upper case. */
  static Character toUpper(Character character) throw();
  
  /** Maps the character to title case. */
  static Character toTitle(Character character) throw();

  /**
    Returns the value of the specified character. The character must be a digit
    approved by isDigit(), isOctal(), isLowerHex(), isUpperHex(), or
    isHexDigit(). If the character is not a digit an unspecified value is
    returned.
  */
  int digitToValue(Character character) throw();
  
  class ToLowerCase {
  public:
    
    inline Character operator()(Character character) const throw() {
      return toLower(character);
    }
  };

  class ToUpperCase {
  public:
    
    inline Character operator()(Character character) const throw() {
      return toUpper(character);
    }
  };

  class ToTitleCase {
  public:
    
    inline Character operator()(Character character) const throw() {
      return toTitle(character);
    }
  };
};

/**
  Wide-character string implementation. The first modifying operation on a
  string may force the internal buffer to be duplicated. The implementation is
  currently NOT MT-safe.

  <pre>
  WideString myString = WIDEMESSAGE("Hello, World!");
  WideString myOtherString = myString;
  </pre>

  @short Wide-character string.
  @ingroup string
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class WideString : public virtual Object {
public:

  /** Character specific properties and manipulators. */
  typedef WideTraits Traits;
  /** The type of a single character. */
  typedef Traits::Character Character;

  struct HashEntry {
    uint8 numberOfCodes;
    unsigned int index;
  };

  /** Specifies the granularity of the capacity. Guaranteed to be greater than 0. */
  static const unsigned int GRANULARITY = 16;
  /** Specifies the maximum length of any string. Guarantees that an int can hold the length of the string. */
  static const unsigned int MAXIMUM_LENGTH = ((PrimitiveTraits<int>::MAXIMUM/sizeof(Character) - 1)/GRANULARITY)*GRANULARITY;
  /** Hash modulus. */
  static const unsigned int HASH_MODULUS = 1455;
  /** Character folding hash table. */
  static const HashEntry hashTable[];
  /** Character mapping table for caseless matching. */
  static const uint32 mappingTable[];
  
  /** The type of the modifying string iterator. */
  typedef ReferenceCountedCapacityAllocator<Character>::Iterator Iterator;
  /** The type of the non-modifying string iterator. */
  typedef ReferenceCountedCapacityAllocator<Character>::ReadIterator ReadIterator;
  /** The type of the modifying string enumerator. */
  typedef ReferenceCountedCapacityAllocator<Character>::Enumerator Enumerator;
  /** The type of the non-modifying string enumerator. */
  typedef ReferenceCountedCapacityAllocator<Character>::ReadEnumerator ReadEnumerator;

  /** Multibyte encoding. */
  enum MultibyteEncoding {
    UTF7, /**< Unicode transformation format (UTF-7). */
    UTF8, /**< Unicode transformation format (UTF-8). */
    UTF16, /**< Unicode transformation format (UTF-16). */
    UTF16BE, /**< Unicode transformation format (UTF-16) with big endian byte order. */
    UTF16LE, /**< Unicode transformation format (UTF-16) with little endian byte order. */
    UTF32, /**< Unicode transformation format (UTF-32). */
    UTF32BE, /**< Unicode transformation format (UTF-32) with big endian byte order. */
    UTF32LE /**< Unicode transformation format (UTF-32) with little endian byte order. */
  };

  /** Wide character encoding. */
  enum WideEncoding {
    UCS2, /**< UCS-2 encoding. */
    UCS4 /**< UCS-4 encoding. */
  };

  /** Encoding flags. */
  enum EncodingFlags {
    ADD_BOM = 1, /**< Specifies that a BOM should be inserted when encoding to UTF. */
    EAT_BOM = 2, /**< Specifies that the BOM should be skipped if present. */
    EXPECT_BOM = 4, /**< Specifies that the BOM must be present. */
    ASSUME_NATIVE_BYTE_ORDER = 8, /**< Specifies the the encoding is in native byte order. */
    ASSUME_BE = 16, /**< Specifies the the encoding is in big endian byte order (ignored if ASSUME_NATIVE_BYTE_ORDER is set). */
    ASSUME_LE = 32 /**< Specifies the the encoding is in little endian byte order (ignored if ASSUME_NATIVE_BYTE_ORDER or ASSUME_BE are set). */
  };  
private:

  /** The default wide string. This is used to avoid multiple allocations of empty string buffers. */
  static const WideString DEFAULT_STRING;

  /**
    Reference to an element within a wide string.
  */
  class Reference {
    friend class WideString;
  private:
    
    WideString& string;
    unsigned int index;
    
    Reference(const Reference& copy); // prohibit default copy initialization
    Reference& operator=(const Reference& eq); // prohibit default assignment
    
    inline Reference(WideString& _string, unsigned int _index)
      : string(_string), index(_index) {
    }
  public:
    
    inline Reference& operator=(char value) throw(OutOfRange) {
      string.setAt(index, value);
      return *this;
    }
    
    inline operator Character() const throw(OutOfRange) {
      return string.getAt(index);
    }
  };

  /**
    Reference counted buffer holding the character sequence (the sequence is not
    guarantied to be NULL-terminated). However, the attribute is guarantied to
    point to a valid buffer even for empty strings.
  */
  ReferenceCountedObjectPointer<ReferenceCountedCapacityAllocator<Character> > elements;

  /**
    Compare the NULL-terminated strings ignoring the case.
  */
  static int compareToIgnoreCase(const Character* left, const Character* right) throw();
protected:

  /**
    Returns a modifiable buffer. Forces the internal buffer to be copied if
    shared by multiple strings.
  */
  inline Character* getBuffer() throw(MemoryException) {
    elements.copyOnWrite();
    return elements->getElements();
  }

  /**
    Returns a non-modifiable buffer.
  */
  inline const Character* getBuffer() const throw() {
    return elements->getElements();
  }

  /**
    Sets the length of the string.
  */
  inline void setLength(unsigned int length) throw(WideStringException, MemoryException) {
    assert(length <= MAXIMUM_LENGTH, WideStringException(this));
    elements.copyOnWrite(); // we are about to modify the buffer
    elements->setSize(length + 1);
  }
public:

  /** Specifies the maximum number of bytes per character for any supported encoding. */
  static const unsigned int MAXIMUM_MULTIBYTES = 6;
  /** Specifies the byte order mark. */
  static const ucs4 BOM = 0x0000feff;

  /** The style. */
  enum Style {
    STYLE_JAVA, /**< Java format: \u1234 */
    STYLE_C = STYLE_JAVA, /**< C format: \u1234 */
    STYLE_CPP = STYLE_C, /**< C++ format: \u1234 */
    STYLE_HTML, /**< HTML format &#x1234; or &#1234; dependent on the current base integer. */
    STYLE_XML = STYLE_HTML, /**< XML format &#x1234; or &#1234; dependent on the current base integer. */
    STYLE_PERL /**< Perl format: \x{1234} */
  };

  class UnicodeCharacter {
  private:
    
    static const Style DEFAULT_STYLE = STYLE_CPP;
    ucs4 character;
    Style style;
  public:
    
    inline UnicodeCharacter(ucs4 _character, Style _style = DEFAULT_STYLE) throw()
      : character(_character), style(_style) {
    }

    inline ucs4 getCode() const throw() {
      return character;
    }
    
    inline Style getStyle() const throw() {
      return style;
    }
    
    inline UnicodeCharacter& setStyle(Style style) throw() {
      this->style = style;
      return *this;
    }
  };
  
  /**
     Returns the maximum number of bytes required to represent any UCS-4 character.
  */
  static inline unsigned int getMaximumNumberOfMultibytes(MultibyteEncoding encoding) throw() {
    static const unsigned int MAXIMUM_MULTIBYTES[] = {0, 6, 4, 4, 4, 4, 4, 4};
    return MAXIMUM_MULTIBYTES[encoding];
  }

  /**
    Returns true if the specified value is a valid UCS-4 value (ISO/IEC 10646).
  */
  static inline bool isUCS4(unsigned int value) throw() {
    return value <= 0x7fffffffU;
  }

  /**
    Returns the multibyte encoding of the specifies multibyte encoded string.
    This method will not examine the entire string. An invalid encoding may
    return UTF8.
    
    @param src The multibyte encoded string.
    @param size The number of bytes in the multibyte encoded string.
  */
  MultibyteEncoding getMultibyteEncoding(const uint8* src, unsigned int size) throw();

  /**
    Returns a MIME charsets for the specified encoding.

    @param encoding The multibyte encoding.
  */
  static StringLiteral toString(MultibyteEncoding encoding) throw();
  
  /**
    Low-level method which converts an UCS-2 encoded string to UTF-8. A
    null-terminator is NOT appended to the string. The destination buffer must
    have room for enough bytes (guaranteed to not exceed
    (size + 1) * getMaximumNumberOfMultibytes(UTF8)).
    
    @param dest The destination buffer (may be 0).
    @param src The UCS-2 encoded string.
    @param size The number of characters in the UCS-2 encoded string.
    @param flags The encoding flags. The default is 0.

    @return The number of bytes occupied by the UTF-8 encoded string.
  */
  static unsigned int UCS2ToUTF8(uint8* dest, const ucs2* src, unsigned int size, unsigned int flags = 0) throw(WideStringException);
  
  /**
    Low-level method which converts an UCS-4 encoded string to UTF-8. A
    null-terminator is NOT appended to the string. The destination buffer must
    have room for enough bytes (guaranteed to not exceed
    (size + 1) * getMaximumNumberOfMultibytes(UTF8)).
    
    @param dest The destination buffer (may be 0).
    @param src The UCS-4 encoded string.
    @param size The number of characters in the UCS-4 encoded string.
    @param flags The encoding flags. The default is 0.

    @return The number of bytes occupied by the UTF-8 encoded string.
  */
  static unsigned int UCS4ToUTF8(uint8* dest, const ucs4* src, unsigned int size, unsigned int flags = 0) throw(WideStringException);

  // TAG: UTF-16 see RFC 2781  Unicode Standard, version 3.0
  
  /**
    Low-level method which converts an UTF-8 encoded string to UCS-2 encoding.
    The destination buffer must have room for enough characters (guaranteed to
    not exceed size).
    
    @param dest The destination buffer (may be 0).
    @param src The UTF-8 encoded string.
    @param size The number of bytes in the UTF-8 encoded string.
    @param flags The encoding flags. The default is EAT_BOM.
    
    @return The number of characters in the UCS-2 encoded string.
  */
  static unsigned int UTF8ToUCS2(ucs2* dest, const uint8* src, unsigned int size, unsigned int flags = EAT_BOM) throw(MultibyteException);

  /**
    Low-level method which converts an UTF-8 encoded string to UCS-4 encoding.
    The destination buffer must have room for enough characters (guaranteed to
    not exceed size).
    
    @param dest The destination buffer (may be 0).
    @param src The UTF-8 encoded string.
    @param size The number of bytes in the UTF-8 encoded string.
    @param flags The encoding flags. The default is EAT_BOM.
    
    @return The number of characters in the UCS-4 encoded string.
  */
  static unsigned int UTF8ToUCS4(ucs4* dest, const uint8* src, unsigned int size, unsigned int flags = EAT_BOM) throw(MultibyteException);

  /**
    Low-level method which converts an UTF-16 encoded string to UCS-4 encoding.
    The destination buffer must have room for enough characters (guaranteed to
    not exceed size). The UCS-4 characters are restricted to values in the range
    0x00000000-0x0010ffff.
    
    @param dest The destination buffer (may be 0).
    @param src The UTF-16 encoded string.
    @param size The number of bytes in the UTF-16 encoded string.
    @param flags The encoding flags. The default is EAT_BOM|EXPECT_BOM.
    
    @return The number of characters in the UCS-4 encoded string.
  */
  static unsigned int UTF16ToUCS4(ucs4* dest, const uint8* src, unsigned int size, unsigned int flags = EAT_BOM|EXPECT_BOM) throw(MultibyteException);

  /**
    Low-level method which converts an UCS-2 encoded string to UCS-4 encoding.
    The destination buffer must have room for enough characters (guaranteed to
    not exceed size). The UCS-4 characters are restricted to values in the range
    0x00000000-0x0010ffff.
    
    @param dest The destination buffer (may be 0).
    @param src The UCS-2 encoded string.
    @param size The number of characters in the UCS-2 encoded string.
    @param flags The encoding flags.
    
    @return The number of characters in the UCS-4 encoded string.
  */
  static unsigned int UCS2ToUCS4(ucs4* dest, const ucs2* src, unsigned int size, unsigned int flags = 0) throw(WideStringException);

  /**
    Low-level method which converts an UCS-4 encoded string to UCS-2 encoding.
    The destination buffer must have room for enough characters (guaranteed to
    not exceed size).
    
    @param dest The destination buffer (may be 0).
    @param src The UCS-4 encoded string.
    @param size The number of characters in the UCS-4 encoded string.
    @param flags The encoding flags.
    
    @return The number of characters in the UCS-2 encoded string.
  */
  static unsigned int UCS4ToUCS2(ucs2* dest, const ucs4* src, unsigned int size, unsigned int flags = 0) throw(WideStringException);
  
  /**
    Low-level method which converts an UCS-2 encoded string to UTF-16BE. A
    null-terminator is NOT appended to the string. The destination buffer must
    have room for enough bytes (guaranteed to not exceed
    (size + 1) * getMaximumNumberOfMultibytes(UTF16BE)).
    
    @param dest The destination buffer (may be 0).
    @param src The UCS-2 encoded string.
    @param size The number of characters in the UCS-2 encoded string.
    @param flags The encoding flags. The default is ADD_BOM.

    @return The number of bytes occupied by the UTF-16BE encoded string.
  */
  static unsigned int UCS2ToUTF16BE(uint8* dest, const ucs2* src, unsigned int size, unsigned int flags = ADD_BOM) throw(WideStringException);

  /**
    Low-level method which converts an UCS-2 encoded string to UTF-16LE. A
    null-terminator is NOT appended to the string. The destination buffer must
    have room for enough bytes (guaranteed to not exceed
    (size + 1) * getMaximumNumberOfMultibytes(UTF16LE)).
    
    @param dest The destination buffer (may be 0).
    @param src The UCS-2 encoded string.
    @param size The number of characters in the UCS-2 encoded string.
    @param flags The encoding flags. The default is ADD_BOM.

    @return The number of bytes occupied by the UTF-16LE encoded string.
  */
  static unsigned int UCS2ToUTF16LE(uint8* dest, const ucs2* src, unsigned int size, unsigned int flags = ADD_BOM) throw(WideStringException);

  /**
    Low-level method which converts an UCS-4 encoded string to UTF-16BE. A
    null-terminator is NOT appended to the string. The destination buffer must
    have room for enough bytes (guaranteed to not exceed
    (size + 1) * getMaximumNumberOfMultibytes(UTF16BE)).
    
    @param dest The destination buffer (may be 0).
    @param src The UCS-4 encoded string.
    @param size The number of characters in the UCS-4 encoded string.
    @param flags The encoding flags. The default is ADD_BOM.

    @return The number of bytes occupied by the UTF-16BE encoded string.
  */
  static unsigned int UCS4ToUTF16BE(uint8* dest, const ucs4* src, unsigned int size, unsigned int flags = ADD_BOM) throw(WideStringException);

  /**
    Low-level method which converts an UCS-4 encoded string to UTF-16LE. A
    null-terminator is NOT appended to the string. The destination buffer must
    have room for enough bytes (guaranteed to not exceed
    (size + 1) * getMaximumNumberOfMultibytes(UTF16LE)).
    
    @param dest The destination buffer (may be 0).
    @param src The UCS-4 encoded string.
    @param size The number of characters in the UCS-4 encoded string.
    @param flags The encoding flags. The default is ADD_BOM.

    @return The number of bytes occupied by the UTF-16LE encoded string.
  */
  static unsigned int UCS4ToUTF16LE(uint8* dest, const ucs4* src, unsigned int size, unsigned int flags = ADD_BOM) throw(WideStringException);

  /**
    Low-level method which converts an UCS-4 encoded string to UTF-32BE. A
    null-terminator is NOT appended to the string. The destination buffer must
    have room for enough bytes (guaranteed to not exceed
    (size + 1) * getMaximumNumberOfMultibytes(UTF32BE)).
    
    @param dest The destination buffer (may be 0).
    @param src The UCS-4 encoded string.
    @param size The number of characters in the UCS-4 encoded string.
    @param flags The encoding flags. The default is ADD_BOM.

    @return The number of bytes occupied by the UTF-32BE encoded string.
  */
  static unsigned int UCS4ToUTF32BE(uint8* dest, const ucs4* src, unsigned int size, unsigned int flags = ADD_BOM) throw(WideStringException);
  
  /**
    Low-level method which converts an UCS-4 encoded string to UTF-32LE. A
    null-terminator is NOT appended to the string. The destination buffer must
    have room for enough bytes (guaranteed to not exceed
    (size + 1) * getMaximumNumberOfMultibytes(UTF32LE)).
    
    @param dest The destination buffer (may be 0).
    @param src The UCS-4 encoded string.
    @param size The number of characters in the UCS-4 encoded string.
    @param flags The encoding flags. The default is ADD_BOM.

    @return The number of bytes occupied by the UTF-32LE encoded string.
  */
  static unsigned int UCS4ToUTF32LE(uint8* dest, const ucs4* src, unsigned int size, unsigned int flags = ADD_BOM) throw(WideStringException);

  /**
    Low-level method which converts an UTF-32 encoded string to UCS-4 encoding.
    The destination buffer must have room for enough characters (guaranteed to
    not exceed size). See the technical report available at
    http://www.unicode.org/unicode/reports/tr19. The UCS-4 characters are
    restricted to values in the range 0x00000000-0x0010ffff.
    
    @param dest The destination buffer (may be 0).
    @param src The UTF-32 encoded string.
    @param size The number of bytes in the UTF-32 encoded string.
    @param flags The encoding flags. The default is EAT_BOM|EXPECT_BOM.
    
    @return The number of characters in the UCS-4 encoded string.
  */
  static unsigned int UTF32ToUCS4(ucs4* dest, const uint8* src, unsigned int size, unsigned int flags = EAT_BOM|EXPECT_BOM) throw(MultibyteException);

  /**
    Returns a multibyte string from a NULL-terminated wide-string.
  */
  static String getMultibyteString(const wchar* string) throw(NullPointer, MultibyteException, WideStringException);

  /**
    Returns the length of the NULL-terminated string.

    @param string The NULL-terminated string.
    @param maximum The maximum length of the string. The default is MAXIMUM_LENGTH.
  */
  static inline unsigned int getLengthOfMustBeTerminated(const wchar* string, unsigned int maximum = MAXIMUM_LENGTH) throw(StringException) {
    assert(string, StringException(Type::getType<String>()));
    const wchar* terminator = find<wchar>(string, maximum, 0);
    assert(terminator, StringException(Type::getType<String>()));
    return terminator - string;
  }

  /**
    Returns the length of the NULL-terminated string.

    @param string The NULL-terminated string.
    @param maximum The maximum length of the string. The default is MAXIMUM_LENGTH.

    @return maximum if terminator is not found. 0 if string is invalid (i.e. 0).
  */
  static inline unsigned int getLengthOfTerminated(const wchar* string, unsigned int maximum = MAXIMUM_LENGTH) throw() {
    if (!string) {
      return 0;
    }
    const wchar* terminator = find<wchar>(string, maximum, 0);
    return terminator ? (terminator - string) : maximum;
  }
  
  /**
    Initializes an empty string.
  */
  WideString() throw();

  /**
    Initializes a string with no characters in it, initial capacity and granularity of capacity.

    @param capacity The initial capacity of the internal buffer.
  */
  explicit WideString(unsigned int capacity) throw(MemoryException);

  /**
    Initializes the string from a string literal. The string literal is not copied into internal buffer. Implicit initialization is allowed.

    @param string String literal generated by the macro WIDEMESSAGE (e.g. WIDEMESSAGE("My string"))
  */
  WideString(const WideStringLiteral& string) throw(WideStringException, MemoryException);

  /**
    Initializes the string from a NULL-terminated string.

    @param string NULL-terminated string. If NULL, the string is initialized with
    no characters in it.
  */
  WideString(const wchar* string) throw(WideStringException, MemoryException);

  /**
    Initializes the string from a NULL-terminated string. If the length of the
    specified string exceeds the maximum length (n) only the first n
    characters are used.

    @param string NULL-terminated string. If NULL, the string is initialized with
    no characters in it.
    @param maximum Specifies the maximum length.
  */
  WideString(const wchar* string, unsigned int maximum) throw(OutOfDomain, MemoryException);

  /**
    Initializes string from a NULL-terminated multibyte character string.

    @param string The NULL-terminated string.
  */
  WideString(const char* string) throw(MultibyteException, MemoryException);

  /**
    Initializes string from a NULL-terminated multibyte string.

    @param string The NULL-terminated string.
    @param maxmimum The maximum length of the string.
  */
  WideString(const char* string, unsigned int maximum) throw(OutOfDomain, MultibyteException, MemoryException);

  /**
    Initializes string from other string.
  */
  inline WideString(const WideString& copy) throw() : elements(copy.elements) {
  }
  
  /**
    Initialized wide string from multibyte string.
  */
  WideString(const String& string) throw(MultibyteException, MemoryException);
  
  /**
    Assignment of string with string.
  */
  WideString& operator=(const WideString& eq) throw() {
    elements = eq.elements; // self assignment handled by automation pointer
    return *this;
  }

  /**
    Assignment of string literal to string.
  */
  WideString& operator=(const WideStringLiteral& eq) throw(WideStringException, MemoryException);

  /**
    Returns the number of characters in the string.
  */
  inline unsigned int getLength() const throw() {
    return elements->getSize() - 1;
  }

  /**
    Returns true if the string contains no characters.
  */
  inline bool isEmpty() const throw() {
    return elements->getSize() == 1;
  }

  /**
    Returns true if the string contains characters.
  */
  inline bool isProper() const throw() {
    return elements->getSize() > 1;
  }

  /**
    Returns true if the string is an ASCII string.
  */
  inline bool isASCII() const throw();
  
  /**
    Returns the capacity of the string.
  */
  inline unsigned int getCapacity() const throw() {
    return elements->getCapacity();
  }  
  
  /**
    Ensures that the capacity of the buffer is at least equal to the specified
    minimum. This applies to all shared strings.

    @param capacity Specifies the minimum capacity of the string.
  */
  void ensureCapacity(unsigned int capacity) throw(MemoryException);

  /**
    Releases any unused capacity of the string. This applies to all shared
    strings.
  */
  void optimizeCapacity() throw();

  /**
    Returns the granularity.
  */
  unsigned int getGranularity() const throw();

  /**
    Sets the granularity.
  */
  void setGranularity(unsigned int granularity) throw();

// *************************************************************************
//   TRAVERSE SECTION
// *************************************************************************

  /**
    Returns the first element of the string as a modifying iterator.
  */
  inline Iterator getBeginIterator() throw() {
    return elements->getBeginIterator();
  }

  /**
    Returns the end of the string as a modifying iterator.
  */
  inline Iterator getEndIterator() throw() {
    return elements->getEndIterator() - 1;
  }

  /**
    Returns the first element of the string as a non-modifying iterator.
  */
  inline ReadIterator getBeginReadIterator() const throw() {
    return elements->getBeginReadIterator();
  }

  /**
    Returns the end of the string as a non-modifying iterator.
  */
  inline ReadIterator getEndReadIterator() const throw() {
    return elements->getEndReadIterator() - 1;
  }

  /**
    Returns a modifying enumerator of the string.
  */
  inline Enumerator getEnumerator() throw() {
    return elements->getEnumerator();
  }

  /**
    Returns a non-modifying enumerator of the string.
  */
  inline ReadEnumerator getReadEnumerator() const throw() {
    return elements->getReadEnumerator();
  }

// *************************************************************************
//   CHARACTER SECTION
// *************************************************************************

  /**
    Returns the character at the specified index in this string. Raises
    OutOfRange if index exceeds the length of the string.
  */
  Character getAt(unsigned int index) const throw(OutOfRange);

  /**
    Sets the character at the specified index of this string. If the new
    character is the string terminator ('\0') then the string is cut off from
    the specified index. Raises OutOfRange if index exceeds the length of
    the string.

    @param index The index of the character to set.
    @param value The new character value.
  */
  void setAt(unsigned int index, Character value) throw(OutOfRange);

  /**
    Returns a reference to character at the specified index. Raises
    OutOfRange if index exceeds the length of the string.
  */
  Reference operator[](unsigned int index) throw(OutOfRange) {
    return Reference(*this, index);
  }

  /**
    Returns the character at the specified index. Raises OutOfRange if index
    exceeds the length of the string.
  */
  inline char operator[](unsigned int index) const throw(OutOfRange) {
    return getAt(index);
  }

// *************************************************************************
//   SUBSTRING SECTION
// *************************************************************************

  /**
    Removes the characters in a substring of this string.

    @param start Specifies the start of the substring.
    @param end Specifies the end of the substring.
  */
  WideString& remove(unsigned int start, unsigned int end) throw(MemoryException);

  /**
    Removes the characters from the specified index to the end of the string.

    @param start Specifies the start of the string.
  */
  WideString& removeFrom(unsigned int start) throw(MemoryException);

  /**
    Removes the character at the specified position in this string.

    @param index Specifies the character to be removed.
  */
  inline WideString& removeCharacter(unsigned int index) throw(MemoryException) {
    return remove(index, index);
  }

  /**
    Appends the character to this string.

    @param ch The character to be appended.
  */
  inline WideString& append(Character ch) throw(WideStringException, MemoryException) {
    return insert(getLength(), ch);
  }

  /**
    Appends the string to this string.

    @param string The string to be appended.
  */
  inline WideString& append(const WideString& string) throw(WideStringException, MemoryException) {
    return insert(getLength(), string);
  }

  /**
    Appends the string literal to this string.

    @param string The string to be appended.
  */
  WideString& append(const WideStringLiteral& string) throw(WideStringException, MemoryException);

  /**
    Appends the string literal to this string.

    @param string The string to be appended.
    @param maximum The maximum length of the to be appended string.
  */
  WideString& append(const WideStringLiteral& string, unsigned int maximum) throw(OutOfDomain, WideStringException, MemoryException);

  /**
    Appends the NULL-terminated string to this string.

    @param string The string to be appended.
    @param maximum The maximum length of the to be appended string.
  */
  WideString& append(const wchar* string, unsigned int maximum) throw(OutOfDomain, WideStringException, MemoryException);

  /**
    Prepends the character to this string.

    @param ch The character to be prepended.
  */
  inline WideString& prepend(Character ch) throw(WideStringException, MemoryException) {
    return insert(0, ch);
  }

  /**
    Prepends the string to this string.

    @param string The string to be prepended.
  */
  inline WideString& prepend(const WideString& string) throw(WideStringException, MemoryException) {
    return insert(0, string);
  }

  /**
    Inserts the character into this string.

    @param index Specifies the position to insert the character. If the index
    exceeds the end of this string the character is inserted at the end.

    @param ch The character to be inserted.
  */
  WideString& insert(unsigned int index, Character ch) throw(WideStringException, MemoryException);

  /**
    Inserts the string into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.

    @param string The string to be inserted.
  */
  WideString& insert(unsigned int index, const WideString& string) throw(WideStringException, MemoryException);

  /**
    Inserts NULL-terminated string into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.

    @param string The NULL-terminated string to be inserted.
  */
  WideString& insert(unsigned int index, const WideStringLiteral& string) throw(WideStringException, MemoryException);

  /**
    Replaces the characters in a substring of this string with the characters
    in the specified string.

    @param start The start of the substring.
    @param end The end of the substring.
    @param string The string to replace with.
  */
  WideString& replace(unsigned int start, unsigned int end, const WideString& string) throw(WideStringException, MemoryException);

  /**
    Replaces all occurances of the specified substring with another string in
    this string. Only does one pass of this string.

    @param fromStr The substring to be replaced.
    @param toStr The new string.
    @return The number of substrings that was replaced.
  */
  unsigned int replaceAll(const WideString& fromStr, const WideString& toStr) throw(WideStringException, MemoryException);

  /**
    Returns a new string that contains a subsequence of characters currently
    contained in this string. If 'end' is less than 'start' an empty string is
    returned.

    @param start Specifies the start of the substring.
    @param end Specifies the end of the substring.
  */
  WideString substring(unsigned int start, unsigned int end) const throw(MemoryException);

  /**
    Returns a new string that contains a subsequence of characters currently
    contained in this string. The substring begins at the specified index and
    extends to the end of the string.

    @param start Specifies the start of the substring.
  */
  inline WideString substring(unsigned int start) const throw(MemoryException) {
    return substring(start, getLength());
  }

  /**
    Appends the string to this string.

    @param suffix The string to be appended.
  */
  inline WideString& operator+=(const WideString& suffix) throw(MemoryException) {
    return append(suffix);
  }

  /**
    WideString reduction operator. Removes suffix from this string if and only if
    it ends with the suffix (e.g. ("presuf"-"suf") results in a new string
    "pre" whereas ("pre"-"suf") results in "pre").

    @param suffix The suffix to be removed.
  */
  WideString& operator-=(const WideString& suffix) throw(MemoryException);
  
// *************************************************************************
//   UNARY SECTION
// *************************************************************************

  /**
    The character sequence contained in this string is replaced by the reverse sequence.
  */
  WideString& reverse() throw();

  /**
    Converts the characters of this string into the lower case equivalents.
  */
  WideString& toLowerCase() throw();

  /**
    Converts the characters of this string into the upper case equivalents.
  */
  WideString& toUpperCase() throw();

// *************************************************************************
//   COMPARE SECTION
// *************************************************************************

  /**
    Compare this string with another string.

    @param string The string to compare this string with.
    
    @return Integer less than, equal to, or greater than zero if this string is
    found, respectively, to be less than, equal to, or greater than the
    specified string.
  */
  int compareTo(const WideString& string) const throw();

  /**
    Compare this string with a string literal.

    @param string The string literal to compare this string with.
    
    @return Integer less than, equal to, or greater than zero if this string is
    found, respectively, to be less than, equal to, or greater than the
    specified string.
  */
  int compareTo(const WideStringLiteral& string) const throw();
  
  /**
    Compare this string with NULL-terminated string.

    @param string The string to compare this string with.
    
    @return Integer less than, equal to, or greater than zero if this string is
    found, respectively, to be less than, equal to, or greater than the
    specified string.
  */
  int compareTo(const wchar* string) const throw();

  /**
    Compares this string with other string ignoring the case of the characters.

    @param string The string to compare this string with.
    
    @return Integer less than, equal to, or greater than zero if this string is
    found, respectively, to be less than, equal to, or greater than the
    specified string.
  */
  int compareToIgnoreCase(const WideString& string) const throw();

  /**
    Returns true if this string starts with the specified prefix.

    @param prefix The string to compare start of this string with.
  */
  bool startsWith(const WideString& prefix) const throw();

  /**
    Returns true if this string starts with the specified prefix.

    @param prefix The string to compare start of this string with.
  */
  bool startsWith(const WideStringLiteral& prefix) const throw();

  /**
    Returns true if this string ends with the specified suffix.

    @param suffix The string to compare end of this string with.
  */
  bool endsWith(const WideString& suffix) const throw();

  /**
    Returns true if this string ends with the specified suffix.

    @param suffix The string to compare end of this string with.
  */
  bool endsWith(const WideStringLiteral& suffix) const throw();

  /**
    Equality operator.
  */
  inline bool operator==(const WideString& string) const throw() {
    return compareTo(string) == 0;
  }

  /**
    Equality operator.
  */
  inline bool operator==(const WideStringLiteral& string) const throw() {
    return compareTo(string) == 0;
  }

  /**
    Inequality operator.
  */
  inline bool operator!=(const WideString& string) const throw() {
    return compareTo(string) != 0;
  }

  /**
    Less than operator.
  */
  inline bool operator<(const WideString& string) const throw() {
    return compareTo(string) < 0;
  }

  /**
    Less than or equal operator.
  */
  inline bool operator<=(const WideString& string) const throw() {
    return compareTo(string) <= 0;
  }

  /**
    Greater than or equal operator.
  */
  inline bool operator>=(const WideString& string) const throw() {
    return compareTo(string) >= 0;
  }

  /**
    Greater than operator.
  */
  inline bool operator>(const WideString& string) const throw() {
    return compareTo(string) > 0;
  }

// *************************************************************************
//   FIND SECTION
// *************************************************************************

  /**
    Returns the index of the first character that matches the specified
    character after the start position.

    @param ch The character to find.
    @param start Specifies the start position of the search. Default is 0.
    @return Index of the first match if any otherwise -1.
  */
  int indexOf(Character ch, unsigned int start = 0) const throw();

  /**
    Returns the index of the first substring that matches the specified string
    after the start position.

    @param string The substring to find.
    @param start Specifies the start position of the search. Default is 0.
    @return Index of the first match if any otherwise -1. Also returns -1 if substring is empty.
  */
  int indexOf(const WideString& string, unsigned int start = 0) const throw();

  /**
    Returns the index of the last character that matches the specified character
    before the start position.

    @param ch The character to find.
    @param start Specifies the start position of the search. Default is end of string.
    @return Index of the last match if any otherwise -1.
  */
  int lastIndexOf(Character ch, unsigned int start) const throw();

  /**
    Returns the index of the last character that matches the specified character
    starting from the end of the string.
  */
  inline int lastIndexOf(Character ch) const throw() {
    return lastIndexOf(ch, getLength());
  }

  /**
    Returns the index of the last substring that matches the specified string
    before the start position.

    @param string The substring to find.
    @param start Specifies the start position of the search. Default is end of string.
    @return Index of the last match if any otherwise -1. Also returns -1 if the substring is empty.
  */
  int lastIndexOf(const WideString& string, unsigned int start) const throw();

  /**
    Returns the index of the last string that matches the specified string
    starting from the end of the string.

    @param string The substring to find.

    @return Index of the last match if any otherwide -1. Also returns -1 if the substring is empty.
  */
  inline int lastIndexOf(const WideString& string) const throw() {
    return lastIndexOf(string, getLength());
  }

  /**
    Returns the number of occurances of the specified character in this string.

    @param ch The character to be counted.
    @param start The start position. Default is 0.
    @return The number of occurances of the character.
  */
  unsigned int count(Character ch, unsigned int start = 0) const throw();

  /**
    Counts the number of occurances of the specified substring in this string.

    @param string The substring to be counted.
    @param start The start position. Default is 0.
    @return The number of occurances of the substring.
  */
  unsigned int count(const WideString& string, unsigned int start = 0) const throw();

// *************************************************************************
//   END SECTION
// *************************************************************************

  /**
    Returns a multibyte string from this wide string.
  */
  String getMultibyteString() const throw(MultibyteException, MemoryException);
  
  /**
    Returns NULL-terminated wide string.
  */
  inline const Character* getElements() const throw() {
    Character* result = elements->getElements(); // no need to copy on write
    result[getLength()] = Traits::TERMINATOR;
    return result;
  }

  /**
    Returns true if the string is upper cased.
  */
  bool isUpperCased() const throw();

  /**
    Returns true if the string is lower case.
  */
  bool isLowerCased() const throw();

  /**
    Returns true if the string is title cased.
  */
  bool isTitleCased() const throw();
  
// *************************************************************************
//   FRIEND SECTION
// *************************************************************************

  /**
    Writes string to format stream.
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const WideString& value) throw(MultibyteException, IOException);
};

template<>
int compare<WideString>(const WideString& left, const WideString& right) throw();

/**
  Writes wide string to format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const WideString& value) throw(MultibyteException, IOException);

/**
  Returns a new string that is the concatenation of the two specified strings.
*/
inline WideString operator+(const WideString& left, const WideString& right) throw(MemoryException) {
  return WideString(left.getLength() + right.getLength()).append(left).append(right);
}

/**
  String reduction. Removes suffix from string if and only if it ends with the suffix (e.g. ("presuf"-"suf") results in a new string "pre" whereas ("pre"-"suf") results in "pre").
*/
inline WideString operator-(const WideString& left, const WideString& right) throw(MemoryException) {
  if (left.endsWith(right)) {
    return left.substring(0, left.getLength() - right.getLength() - 1); // return copy of left without suffix
  } else {
    return WideString(left); // return copy of left
  }
}

/**
  Writes a wide character to the format output stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, WideString::UnicodeCharacter character) throw(IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
