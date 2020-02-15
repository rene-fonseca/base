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
#include <base/OutOfRange.h>
#include <base/mem/ReferenceCountedAllocator.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringException.h>
#include <base/mem/AllocatorEnumeration.h>
#include <base/collection/Hash.h>
#include <base/WideLiteral.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class StringOutputStream;

/**
  Default wide-character properties and manipulators.

  @short Wide-character description.
  @ingroup string
  @version 1.0
*/
class _COM_AZURE_DEV__BASE__API WideTraits {
public:
  
  /** Specifies the terminator for NULL-terminated strings. */
  static constexpr ucs4 TERMINATOR = 0;
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
  static unsigned int getFlags(ucs4 character) noexcept;
  
  /** Returns true if the character a letter. */
  static inline bool isLetter(ucs4 character) noexcept
  {
    return getFlags(character) & LETTER;
  }

  /** Returns true if the character is a letter or a digit. */
  static inline bool isLetterDigit(ucs4 character) noexcept
  {
    return getFlags(character) & (LETTER|DIGIT);
  }
  
  /** Returns true if the character is in lower case. */
  static bool isLower(ucs4 character) noexcept;
  
  /** Returns true if the character is in upper case. */
  static bool isUpper(ucs4 character) noexcept;
  
  /** Returns true if the character is in title case. */
  static bool isTitle(ucs4 character) noexcept;

  /** Returns true if the character is cased. */
  // static inline bool isCased(ucs4 character) noexcept;
  
  /** Returns true if the character is a digit. */
  static bool isDigit(ucs4 character) noexcept;
  
  /** Returns true if the character is a hex digit. */
  static inline bool isHexDigit(ucs4 character) noexcept
  {
    if (character < 0x80) { // TAG: need support for all "abcdefABCDEF"
      return ASCIITraits::isHexDigit(character);
    }
    return isDigit(character);
  }
  
  /** Returns true if the character is a white space. */
  static bool isSpace(ucs4 character) noexcept; // TAG: inline check for most common space (i.e. ' ')
  
  /** Returns true if the character is a punctuation mark. */
  static inline bool isPunctuation(ucs4 character) noexcept
  {
    // TAG: check UNIX spec
    // TAG: use table with only punctuation codes
    return getFlags(character) & PUNCTUATION;
  }
  
  /** Returns true if the character is a control character. */
  static bool isControl(ucs4 character) noexcept;
  
  /** Returns true if the character is an ASCII character. */
  static inline bool isASCII(ucs4 character) noexcept
  {
    return character < 0x80;
  }

  /** Maps the character to lower case. */
  static ucs4 toLower(ucs4 character) noexcept;
  
  /** Maps the character to upper case. */
  static ucs4 toUpper(ucs4 character) noexcept;
  
  /** Maps the character to title case. */
  static ucs4 toTitle(ucs4 character) noexcept;

  /**
    Returns the value of the specified character. The character must be a digit
    approved by isDigit(), isOctal(), isLowerHex(), isUpperHex(), or
    isHexDigit(). If the character is not a digit an unspecified value is
    returned.
  */
  int digitToValue(ucs4 character) noexcept;
  
  class _COM_AZURE_DEV__BASE__API ToLowerCase {
  public:
    
    inline ucs4 operator()(ucs4 character) const noexcept
    {
      return toLower(character);
    }
  };

  class _COM_AZURE_DEV__BASE__API ToUpperCase {
  public:
    
    inline ucs4 operator()(ucs4 character) const noexcept
    {
      return toUpper(character);
    }
  };

  class _COM_AZURE_DEV__BASE__API ToTitleCase {
  public:
    
    inline ucs4 operator()(ucs4 character) const noexcept
    {
      return toTitle(character);
    }
  };
};

/**
  Wide-character string implementation. The first modifying operation on a
  string may force the internal buffer to be duplicated. The implementation is
  currently NOT MT-safe.

  @code
  WideString myString = L"Hello, World!";
  WideString myOtherString = myString;
  @endcode

  @short Wide-character string.
  @ingroup string
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API WideString /*: public virtual Object*/ { // string are too common to use Object
  friend void swapper<WideString>(WideString& a, WideString& b);
public:

  /** Character specific properties and manipulators. */
  typedef WideTraits Traits;

  // char32_t
  typedef ucs4 Char;

  struct HashEntry {
    uint8 numberOfCodes;
    unsigned int index;
  };

  /** Specifies the maximum length of any string. Guarantees that an int can hold the length of the string. */
  static constexpr MemorySize MAXIMUM_LENGTH = (PrimitiveTraits<int>::MAXIMUM/sizeof(ucs4) - 1);
  /** Hash modulus. */
  static constexpr unsigned int HASH_MODULUS = 1455;
  /** Character folding hash table. */
  static const HashEntry hashTable[];
  /** Character mapping table for caseless matching. */
  static const uint32 mappingTable[];
  
  /** The type of the modifying string iterator. */
  typedef ReferenceCountedAllocator<ucs4>::Iterator Iterator;
  /** The type of the non-modifying string iterator. */
  typedef ReferenceCountedAllocator<ucs4>::ReadIterator ReadIterator;
  /** The type of the modifying string enumerator. */
  typedef ReferenceCountedAllocator<ucs4>::Enumerator Enumerator;
  /** The type of the non-modifying string enumerator. */
  typedef ReferenceCountedAllocator<ucs4>::ReadEnumerator ReadEnumerator;

  /** Wide character encoding. */
  enum WideEncoding {
    UTF16, /**< UTF-16 encoding. */
    UCS4 /**< UCS-4 encoding. */
  };

  /** Encoding flags. */
  enum EncodingFlags {
    /** Specifies that a BOM should be inserted when encoding to UTF. */
    ADD_BOM = 1,
    /** Specifies that the BOM should be skipped if present. */
    EAT_BOM = 2,
    /** Specifies that the BOM must be present. */
    EXPECT_BOM = 4,
    /** Specifies that the encoding is in native byte order. */
    ASSUME_NATIVE_BYTE_ORDER = 8,
    /**
      Specifies that the encoding is in big endian byte order (ignored if
      ASSUME_NATIVE_BYTE_ORDER is set).
    */
    ASSUME_BE = 16,
    /**
      Specifies that the encoding is in little endian byte order (ignored if
      ASSUME_NATIVE_BYTE_ORDER or ASSUME_BE are set).
    */
    ASSUME_LE = 32
  };
private:

  /**
    The default string. This is used to avoid multiple allocations of empty
    string buffers.
  */
  static const WideString DEFAULT_STRING;

  /*
    Reference to an element within a string.
  */
  class _COM_AZURE_DEV__BASE__API Element {
    friend class WideString;
  private:
    
    WideString& string;
    MemorySize index = 0;
    
    Element(const Element& copy) noexcept;
    Element& operator=(const Element& assign) noexcept;
    
    inline Element(WideString& _string, MemorySize _index) noexcept
      : string(_string), index(_index)
    {
    }
  public:
    
    inline Element& operator=(ucs4 value)
    {
      string.setAt(index, value);
      return *this;
    }
    
    inline operator ucs4() const
    {
      return string.getAt(index);
    }
  };

  /**
    Reference counted buffer holding NULL-terminated string. The array is
    guarantied to be non-empty when the string has been initialized. elements
    will never be nullptr.
  */
  Reference<ReferenceCountedAllocator<ucs4> > elements;
protected:

  /**
    Initializes string.
  */
  void initialize(const char* string, MemorySize length);

  /**
    Initializes string.
  */
  void initialize(const wchar* string, MemorySize length);

  /**
    Initializes string.
  */
  void initialize(const char16_t* string, MemorySize length);

  /**
    Initializes string.
  */
  void initialize(const ucs4* string, MemorySize length);

  /**
    Returns a modifiable buffer. Forces copy of internal buffer if shared by
    multiple strings.
  */
  Char* getBuffer();
  
  /** Returns buffer for modification and resizes at the same time. */
  Char* getBuffer(MemorySize length);

  /**
    Returns a non-modifiable buffer.
  */
  inline const Char* getBuffer() const noexcept
  {
    return elements->getElements();
  }
  
  /**
    Sets the length of the string.
  */
  inline void setLength(MemorySize length)
  {
    getBuffer(length);
  }

  /**
    Compare the NULL-terminated strings ignoring the case.
  */
  static int compareToIgnoreCase(const ucs4* left, const ucs4* right) noexcept;
private:
  
  /** Prevent accidential use with bool operators. */
  WideString(bool) = delete;
public:

  /**
    Specifies the maximum number of bytes per character for any supported encoding.
  */
  static constexpr unsigned int MAXIMUM_MULTIBYTES = 6;
  /** Specifies the byte order mark. */
  static constexpr ucs4 BOM = 0x0000feff;

  /**
    Initializes an empty string. No allocation done.
  */
  WideString() noexcept;

  inline WideString(Reference<ReferenceCountedAllocator<Char> > string)
    : elements(string ? string : DEFAULT_STRING.elements)
  {
    BASSERT(elements && !elements->isEmpty());
    auto index = elements->getSize() - 1;
    BASSERT(elements->getElements()[index] == 0); // check null terminator
  }

  /**
    Initializes a string with no characters in it and the given initial capacity.
    
    @param capacity The initial capacity.
  */
  explicit WideString(MemorySize capacity);

  /** Returns a new string of the given length. */
  static inline WideString makeLength(MemorySize length)
  {
    WideString result;
    result.forceToLength(length);
    return result;
  }

  /** Returns an empty string with the given capacity. */
  static inline WideString makeCapacity(MemorySize capacity)
  {
    return WideString(capacity);
  }

  class _COM_AZURE_DEV__BASE__API Default {};
  
  /** Forces a non-null empty string. Avoid this. */
  WideString(Default d);

  /**
    Initializes the string from a string literal. The string literal is not
    copied into internal buffer. Implicit initialization is allowed.

    @param string The string.
  */
  WideString(const Literal& string);

  /**
    Initializes the string from a string literal. The string literal is not
    copied into internal buffer. Implicit initialization is allowed.

    @param string The string.
  */
  WideString(const WideLiteral& string);

  /**
    Initializes the string from a string literal. The string literal is not
    copied into internal buffer. Implicit initialization is allowed.

    @param string The string.
  */
  WideString(const char* string);

  /**
    Initializes the string from a string literal. The string literal is not
    copied into internal buffer. Implicit initialization is allowed.

    @param string The string.
    @param length The length of the string.
  */
  WideString(const char* string, MemorySize length);

#if defined(_COM_AZURE_DEV__BASE__CPP_CHAR8_T)
  /**
    Initializes the string from a string literal. The string literal is not
    copied into internal buffer. Implicit initialization is allowed.

    @param string The string.
  */
  WideString(const char8_t* string);

  /**
    Initializes the string from a string literal. The string literal is not
    copied into internal buffer. Implicit initialization is allowed.

    @param string The string.
    @param length The length of the string.
  */
  WideString(const char8_t* string, MemorySize length);
#endif

  /**
    Initializes the string from a string literal. The string literal is not
    copied into internal buffer. Implicit initialization is allowed.

    @param string The string.
  */
  WideString(const wchar* string);

  /**
    Initializes the string from a string literal. The string literal is not
    copied into internal buffer. Implicit initialization is allowed.

    @param string The string.
    @param length The length of the string.
  */
  WideString(const wchar* string, MemorySize length);

  /**
    Initializes the string from a string literal. The string literal is not
    copied into internal buffer. Implicit initialization is allowed.

    @param string The string.
  */
  WideString(const char16_t* string);

  /**
    Initializes the string from a string literal. The string literal is not
    copied into internal buffer. Implicit initialization is allowed.

    @param string The string.
    @param length The length of the string.
  */
  WideString(const char16_t* string, MemorySize length);

  /**
    Initializes the string from a string literal. The string literal is not
    copied into internal buffer. Implicit initialization is allowed.

    @param string The string.
  */
  WideString(const char32_t* string);

  /**
    Initializes the string from a string literal. The string literal is not
    copied into internal buffer. Implicit initialization is allowed.

    @param string The string.
    @param length The length of the string.
  */
  WideString(const char32_t* string, MemorySize length);

  /**
    Initializes the string from UTF-8 string.
  */
  WideString(const String& string);

#if 0 // TAG: bad for now due to match with structs also - or static_cast<const wchar*>() is required
  /**
    Initializes the string from a string literal. Implicit initialization is
    allowed.
    
    @param literal String literal.
  */
  template<MemorySize SIZE>
  inline WideString(const wchar (&literal)[SIZE])
  {
    if (Constraint<(SIZE > 0)>::UNSPECIFIED) {}
    initialize(literal, SIZE - 1);
  }
#endif
  
  /**
    Initializes the string from a NULL-terminated string.

    @param string NULL-terminated string. If NULL, the string is initialized
    with no characters in it.
  */
  WideString(const NativeWideString& string);
  
  /**
    Initializes the string from a NULL-terminated string. If the length of the
    specified string (string) exceeds the maximum length (n) only the first n
    characters are used. Raises StringException is string exceeds the maximum
    allowed length.
    
    @param string NULL-terminated string. If NULL, the string is initialized
    with no characters in it.
    @param maximum Specifies the maximum length.
  */
  WideString(const NativeWideString& string, MemorySize maximum);

  /**
    Initializes string from a NULL-terminated multibyte character string.

    @param string The NULL-terminated string.
  */
  template<MemorySize SIZE>
  inline WideString(const char (&string)[SIZE])
  {
    if (Constraint<(SIZE > 0)>::UNSPECIFIED) {}
    initialize(string, SIZE - 1);
  }
  
  /**
    Initializes the string from a NULL-terminated string.

    @param string NULL-terminated string. If nullptr, the string is initialized
    with no characters in it.
  */
  WideString(const NativeString& string);
  
  /**
    Initializes string from a NULL-terminated multibyte string.

    @param string The NULL-terminated string.
    @param maxmimum The maximum length of the string.
  */
  WideString(const NativeString& string, MemorySize maximum);
  
  /**
    Initializes string from other string.
  */
  inline WideString(const WideString& copy) noexcept
    : elements(copy.elements)
  {
  }

  /**
    Initializes string from other string.
  */
  WideString(WideString&& move) noexcept
    : elements(move.elements)
  {
    move.elements = DEFAULT_STRING.elements; // make empty so we may avoid future copyOnWrite()
  }

  /**
    Initializes string from string output stream. Forces flush().
  */
  WideString(StringOutputStream& stream);

  /**
    Initializes string from string output stream via FormatOutputStream. Forces flush(). Throws exception if not a
    StringOutputStream.
  */
  WideString(FormatOutputStream& stream);

  /**
    Assignment of string to string.
  */
  inline WideString& operator=(const WideString& assign) noexcept
  {
    elements = assign.elements; // self assignment handled by automation pointer
    return *this;
  }

  WideString& operator=(WideString&& move) noexcept
  {
    if (this != &move) { // self assigment not allowed
      elements = moveObject(move.elements);
      move.elements = DEFAULT_STRING.elements; // make empty so we may avoid future copyOnWrite()
    }
    return *this;
  }

  /**
    Assignment of string to string.
  */
  WideString& operator=(const Literal& assign);

  /**
    Assignment of string to string.
  */
  WideString& operator=(const WideLiteral& assign);

  /**
    Assignment of string to string.
  */
  WideString& operator=(const char* assign);

  /**
    Assignment of string to string.
  */
  WideString& operator=(const wchar* assign);

#if defined(_COM_AZURE_DEV__BASE__CPP_CHAR8_T)
  /**
    Assignment of string to string.
  */
  WideString& operator=(const char8_t* assign);
#endif

  /**
    Assignment of string to string.
  */
  WideString& operator=(const char16_t* assign);

  /**
    Assignment of string to string.
  */
  WideString& operator=(const char32_t* assign);

  /**
    Assignment of string to string.
  */
  WideString& operator=(const String& assign);

  /**
    Assignment of string to string.
  */
  WideString& operator=(const StringOutputStream& assign);

  /**
    Assignment of string to string.
  */
  template<MemorySize SIZE>
  inline WideString& operator=(const Char (&literal)[SIZE])
  {
    return operator=(WideLiteral(literal));
  }

  /**
    Returns the number of characters in the string.
  */
  inline MemorySize getLength() const noexcept
  {
    return elements->getSize() - 1; // exclude null terminator
  }

  /**
    Returns true if the string does not contain characters.
  */
  inline bool isEmpty() const noexcept
  {
    return elements->getSize() == 1;
  }

  /**
    Returns true if the string contains characters.
  */
  inline bool isProper() const noexcept
  {
    return elements->getSize() > 1;
  }

  /**
    Returns true if the string is an ASCII string.
  */
  bool isASCII() const noexcept;

  /**
    Returns true if the string has multiple references.
  */
  bool isMultiReferenced() const noexcept;

  /**
    Returns the capacity of the string.
  */
  inline MemorySize getCapacity() const noexcept
  {
    return elements->getCapacity();
  }

  /**
    Ensures that the capacity of the buffer is at least equal to the specified
    minimum. This applies to all shared strings.

    @param capacity Specifies the minimum capacity of the string.
  */
  void ensureCapacity(MemorySize capacity);

  /** Clears the string. */
  void clear();

  /** Returns a forced copy of the string. */
  WideString copy() const;

  /**
    Releases any unused capacity of the string. This applies to all shared
    strings.
  */
  void garbageCollect();

  /**
    Sets the length of the string without initializing the elements.
  */
  void forceToLength(MemorySize length);

// *************************************************************************
//   TRAVERSE SECTION
// *************************************************************************

  /**
    Returns the first element of the string as a modifying iterator.
  */
  inline Iterator getBeginIterator() noexcept
  {
    return elements->getBeginIterator();
  }

  /**
    Returns the end of the string as a modifying iterator.
  */
  inline Iterator getEndIterator() noexcept
  {
    return elements->getEndIterator() - 1; // remember terminator
  }

  /**
    Returns the first element of the string as a non-modifying iterator.
  */
  inline ReadIterator getBeginReadIterator() const noexcept
  {
    return elements->getBeginReadIterator();
  }

  /**
    Returns the end of the string as a non-modifying iterator.
  */
  inline ReadIterator getEndReadIterator() const noexcept
  {
    return elements->getEndReadIterator() - 1; // remember terminator
  }

  /**
    Returns the first element of the string as a non-modifying iterator.
  */
  inline ReadIterator begin() const noexcept
  {
    return elements->getBeginReadIterator();
  }

  /**
    Returns the end of the string as a non-modifying iterator.
  */
  inline ReadIterator end() const noexcept
  {
    return elements->getEndReadIterator() - 1; // remember terminator
  }

  /**
    Returns a modifying enumerator of the string.
  */
  inline Enumerator getEnumerator() noexcept
  {
    return elements->getEnumerator();
  }

  /**
    Returns a non-modifying enumerator of the string.
  */
  inline ReadEnumerator getReadEnumerator() const noexcept
  {
    return elements->getReadEnumerator();
  }

  // TAG: add method to check compliance with Unicode and ISO codes
  
// *************************************************************************
//   CHARACTER SECTION
// *************************************************************************

  /**
    Returns the character at the specified index in this string. Raises
    OutOfRange if index exceeds the length of the string.
  */
  ucs4 getAt(MemorySize index) const;

  /**
    Sets the character at the specified index of this string. If the new
    character is the string terminator ('\\0') then the string is cut off from
    the specified index. Raises OutOfRange if index exceeds the length of
    the string.

    @param index The index of the character to set.
    @param value The new character value.
  */
  void setAt(MemorySize index, ucs4 value);

  /**
    Returns a reference to character at the specified index. Raises
    OutOfRange if index exceeds the length of the string.
  */
  Element operator[](MemorySize index)
  {
    return Element(*this, index);
  }

  /** Returns true if non-empty. */
  inline operator bool() const noexcept
  {
    return getLength() != 0;
  }

  /**
    Returns the character at the specified index. Raises OutOfRange if index
    exceeds the length of the string.
  */
  inline char operator[](MemorySize index) const
  {
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
  WideString& remove(MemorySize start, MemorySize end);

  /**
    Removes the characters from the specified index to the end of the string.

    @param start Specifies the start of the string.
  */
  WideString& removeFrom(MemorySize start);

  /**
    Removes the character at the specified position in this string.

    @param index Specifies the character to be removed.
  */
  inline WideString& removeAt(MemorySize index)
  {
    return remove(index, index + 1);
  }

  /**
    Appends the character to this string.

    @param ch The character to be appended.
  */
  inline WideString& append(ucs4 ch)
  {
    return insert(getLength(), ch);
  }

  /**
    Appends the string to this string.

    @param string The string to be appended.
  */
  inline WideString& append(const WideString& string)
  {
    return insert(getLength(), string);
  }

  /**
    Appends the string to this string.

    @param string The string to be appended.
  */
  inline WideString& append(const String& string)
  {
    return insert(getLength(), string.native());
  }

  /**
    Appends the string literal to this string.

    @param string The string to be appended.
  */
  WideString& append(const WideLiteral& string);
  
  /**
    Appends the NULL-terminated string to this string.

    @param string The string to be appended.
  */
  inline WideString& append(const NativeWideString& string)
  {
    return insert(getLength(), string);
  }

  /** Returns the span. */
  static inline ConstSpan<wchar> toSpan(const wchar* string) noexcept
  {
    return ConstSpan<wchar>(string, getNullTerminatedLength(string));
  }

  /**
    Appends the native string to this string.

    @param string The native string to be appended.
  */
  inline WideString& append(const char* string)
  {
    return insert(getLength(), string);
  }

#if defined(_COM_AZURE_DEV__BASE__CPP_CHAR8_T)
  /**
    Appends the native string to this string.

    @param string The native string to be appended.
  */
  inline WideString& append(const char8_t* string)
  {
    return insert(getLength(), reinterpret_cast<const char*>(string));
  }
#endif

  /**
    Appends the native string to this string.

    @param string The native string to be appended.
  */
  inline WideString& append(const wchar* string)
  {
    return insert(getLength(), string);
  }

#if 0
  /**
    Appends the string to this string.

    @param string The string to be appended.
  */
  WideString& append(const WideLiteral& string);

  template<MemorySize SIZE>
  inline WideString& append(const Char (&literal)[SIZE])
  {
    return append(WideLiteral(literal));
  }
#endif

  /**
    Appends the string literal to this string.

    @param string The string to be appended.
    @param maximum The maximum length of the string to be appended.
  */
  WideString& append(const WideLiteral& string, MemorySize maximum);

  /**
    Appends the NULL-terminated string to this string.

    @param string The string to be appended.
    @param maximum The maximum length of the string to be appended.
  */
  WideString& append(const NativeWideString& string, MemorySize maximum);

  /**
    Appends span.
  */
  WideString& append(const ConstSpan<Char>& buffer);

  /**
    Appends the NULL-terminated string to this string.

    @param string The string to be appended.
    @param maximum The maximum length of the to be appended string.
  */
  WideString& append(const wchar* string, MemorySize maximum);

  /**
    Prepends the character to this string.

    @param ch The character to be prepended.
  */
  inline WideString& prepend(ucs4 ch)
  {
    return insert(0, ch);
  }

  /**
    Prepends the string to this string.

    @param string The string to be prepended.
  */
  inline WideString& prepend(const WideString& string)
  {
    return insert(0, string);
  }

  /**
    Inserts the span into this string.

    @param index Specifies the position to insert the character. If the index
    exceeds the end of this string the character is inserted at the end.
    @param span The characters to be inserted.
  */
  WideString& insert(MemorySize index, const ConstSpan<Char>& span);
  
  /**
    Inserts the span into this string.

    @param index Specifies the position to insert the character. If the index
    exceeds the end of this string the character is inserted at the end.
    @param span The characters to be inserted.
  */
  WideString& insert(MemorySize index, const ConstSpan<wchar>& src);

  /**
    Inserts the character into this string.

    @param index Specifies the position to insert the character. If the index
    exceeds the end of this string the character is inserted at the end.
    @param ch The character to be inserted.
  */
  WideString& insert(MemorySize index, ucs4 ch);

  /**
    Inserts the string into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.
    @param string The string to be inserted.
  */
  WideString& insert(MemorySize index, const WideString& string);

  /**
    Inserts the string literal into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.
    @param string The string literal to be inserted.
  */
  WideString& insert(MemorySize index, const WideLiteral& string);

  template<MemorySize SIZE>
  inline WideString& insert(MemorySize index, const Char (&literal)[SIZE])
  {
    return insert(index, WideLiteral(literal));
  }
  
  /**
    Inserts NULL-terminated string into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.
    @param string The NULL-terminated string to be inserted.
  */
  WideString& insert(MemorySize index, const NativeWideString& string);

  /**
    Inserts NULL-terminated string into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.
    @param string The NULL-terminated string to be inserted.
  */
  inline WideString& insert(MemorySize index, const wchar* string)
  {
    return insert(index, NativeWideString(string));
  }

  /**
    Inserts NULL-terminated string into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.
    @param string The NULL-terminated string to be inserted.
  */
  WideString& insert(MemorySize index, const char* string)
  {
    return insert(index, WideString(string)); // TAG: avoid temp copy
  }

  /**
    Replaces the characters in a substring of this string with the characters
    in the specified string.

    @param start The start of the substring.
    @param end The end of the substring.
    @param string The string to replace with.
  */
  WideString& replace(MemorySize start, MemorySize end, const WideString& string);

  /**
    Replaces all occurances of the specified substring with another string in
    this string. Only does one pass of this string.

    @param fromStr The substring to be replaced.
    @param toStr The new string.
    @return The number of substrings that was replaced.
  */
  MemorySize replaceAll(const WideString& fromStr, const WideString& toStr);

  /**
    Returns a new string that contains a subsequence of characters currently
    contained in this string. If 'end' is less than 'start' an empty string is
    returned.

    @param start Specifies the start of the substring.
    @param end Specifies the end of the substring.
  */
  WideString substring(MemorySize start, MemorySize end) const;

  /**
    Returns a new string that contains a subsequence of characters currently
    contained in this string. The substring begins at the specified index and
    extends to the end of the string.

    @param start Specifies the start of the substring.
  */
  inline WideString substring(MemorySize start) const
  {
    return substring(start, getLength());
  }

  /**
    Appends the string to this string.

    @param suffix The string to be appended.
  */
  inline WideString& operator+=(const WideString& suffix)
  {
    return append(suffix);
  }

  /**
    Appends the string to this string.

    @param suffix The string to be appended.
  */
  inline WideString& operator+=(const String& suffix)
  {
    return append(suffix);
  }

  /**
    Appends the literal to this string.

    @param suffix The character to be appended.
  */
  inline WideString& operator+=(const WideLiteral& suffix)
  {
    return append(suffix);
  }

  /**
    Appends the string to this string.

    @param suffix The character to be appended.
  */
  inline WideString& operator+=(const wchar* suffix)
  {
    return append(NativeWideString(suffix));
  }

  /**
    Appends the string to this string.

    @param suffix The character to be appended.
  */
  inline WideString& operator+=(const NativeWideString& suffix)
  {
    return append(suffix);
  }
  
  /**
    Appends the character to this string.

    @param suffix The character to be appended.
  */
  inline WideString& operator+=(Char suffix)
  {
    return append(suffix);
  }

  /**
    String reduction operator. Removes suffix from this string if and only if
    it ends with the suffix (e.g. ("presuf"-"suf") results in a new string
    "pre" whereas ("pre"-"suf") results in "pre").

    @param suffix The suffix to be removed.
  */
  WideString& operator-=(const WideString& suffix);

// *************************************************************************
//   UNARY SECTION
// *************************************************************************

  /**
    The character sequence contained in this string is replaced by the reverse
    sequence.
  */
  WideString& reverse() noexcept;

  /**
    Converts the characters of this string into the lower case equivalents.
  */
  WideString& toLowerCase() noexcept;

  /**
    Converts the characters of this string into the upper case equivalents.
  */
  WideString& toUpperCase() noexcept;

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
  int compareTo(const WideString& string) const noexcept;

  /**
    Compares this string to with string literal.

    @param string The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is
    found, respectively, to be less than, equal to, or greater than the
    specified string.
  */
  int compareTo(const WideLiteral& string) const /*noexcept*/;

  /**
    Compares this string with NULL-terminated string.

    @param string The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is
    found, respectively, to be less than, equal to, or greater than the
    specified string.
  */
  int compareTo(const wchar* string) const /*noexcept*/;

  /**
    Compares this string with a NULL-terminated string.
    
    @param string The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is
    found, respectively, to be less than, equal to, or greater than the
    specified string. False, if string is 0.
  */
  int compareTo(const NativeWideString& string) const /*noexcept*/
  {
    return compareTo(string.getValue());
  }

  /**
    Compares this string with NULL-terminated string ignoring the case of the
    characters.

    @param string The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is
    found, respectively, to be less than, equal to, or greater than the
    specified string.
  */
  int compareToIgnoreCase(const WideString& string) const noexcept;

  template<MemorySize SIZE>
  inline int compareToIgnoreCase(const Char (&literal)[SIZE]) const noexcept
  {
    return compareToIgnoreCase(NativeWideString(literal));
  }
  
  /**
    Returns true if this string starts with the specified prefix.

    @param prefix The string to compare start of this string with.
  */
  bool startsWith(const WideString& prefix) const /*noexcept*/;

  /**
    Returns true if this string starts with the specified prefix.

    @param prefix The string to compare start of this string with.
  */
  bool startsWith(const WideLiteral& prefix) const /*noexcept*/;

  /**
    Returns true if this string starts with the specified prefix.

    @param prefix The string to compare start of this string with.
  */
  template<MemorySize SIZE>
  inline bool startsWith(const Char (&prefix)[SIZE]) const /*noexcept*/
  {
    return startsWith(Literal(prefix));
  }
  
  /**
    Returns true if this string ends with the specified suffix.

    @param suffix The string to compare end of this string with.
  */
  bool endsWith(const WideString& suffix) const /*noexcept*/;

  /**
    Returns true if this string ends with the specified suffix.

    @param suffix The string to compare end of this string with.
  */
  bool endsWith(const WideLiteral& suffix) const /*noexcept*/;

  /**
    Returns true if this string ends with prefix.

    @param suffix The string to compare end of this string with.
  */
  template<MemorySize SIZE>
  inline bool endsWith(const char (&suffix)[SIZE]) const /*noexcept*/
  {
    return endsWith(Literal(suffix));
  }
  
  /**
    Equality operator.
  */
  inline bool operator==(const WideString& string) const noexcept
  {
    return compareTo(string) == 0;
  }

  /**
    Inequality operator.
  */
  inline bool operator!=(const WideString& string) const noexcept
  {
    return compareTo(string) != 0;
  }

  /**
    Equality operator.
  */
  inline bool operator==(const WideLiteral& string) const noexcept
  {
    return compareTo(string) == 0;
  }


  /**
    Less than operator.
  */
  inline bool operator<(const WideString& string) const noexcept
  {
    return compareTo(string) < 0;
  }

  /**
    Less than or equal operator.
  */
  inline bool operator<=(const WideString& string) const noexcept
  {
    return compareTo(string) <= 0;
  }

  /**
    Greater than or equal operator.
  */
  inline bool operator>=(const WideString& string) const noexcept
  {
    return compareTo(string) >= 0;
  }

  /**
    Greater than operator.
  */
  inline bool operator>(const WideString& string) const noexcept
  {
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
  MemoryDiff indexOf(ucs4 ch, MemorySize start = 0) const noexcept;

  /**
    Returns the index of the first substring that matches the specified string
    after the start position.

    @param string The substring to find.
    @param start Specifies the start position of the search. Default is 0.
    @return Index of the first match if any otherwise -1. Also returns -1 if
    substring is empty.
  */
  MemoryDiff indexOf(const WideString& string, MemorySize start = 0) const noexcept;

  /**
    Returns the index of the last character that matches the specified
    character before the start position.

    @param ch The character to find.
    @param start Specifies the start position of the search. Default is end of
    string.
    @return Index of the last match if any otherwise -1.
  */
  MemoryDiff lastIndexOf(ucs4 ch, MemorySize start) const noexcept;

  /**
    Returns the index of the last character that matches the specified character.

    @param ch The character to find.
    @return Index of the last match if any otherwise -1.
  */
  inline MemoryDiff lastIndexOf(ucs4 ch) const noexcept
  {
    return lastIndexOf(ch, getLength());
  }

  /**
    Returns the index of the last substring that matches the specified string
    before the start position.

    @param string The substring to find.
    @param start Specifies the start position of the search.
    @return Index of the last match if any otherwise -1. Also returns -1 if
    substring is empty.
  */
  MemoryDiff lastIndexOf(const WideString& string, MemorySize start) const noexcept;

  /**
    Returns the index of the last string that matches the specified string
    starting from the end of the string.

    @param string The substring to find.

    @return Index of the last match if any otherwide -1. Also returns -1 if the substring is empty.
  */
  inline MemoryDiff lastIndexOf(const WideString& string) const noexcept
  {
    return lastIndexOf(string, getLength());
  }

  /**
    Returns the number of occurances of the specified character in this string.

    @param ch The character to be counted.
    @param start The start position. Default is 0.
    @return The number of occurances of the character.
  */
  MemorySize count(ucs4 ch, MemorySize start = 0) const noexcept;

  /**
    Counts the number of occurances of the specified substring in this string.

    @param string The substring to be counted.
    @param start The start position. Default is 0.
    @return The number of occurances of the substring.
  */
  MemorySize count(const WideString& string, MemorySize start = 0) const noexcept;

  /**
    Returns true if the string is upper cased.
  */
  bool isUpperCased() const noexcept;

  /**
    Returns true if the string is lower case.
  */
  bool isLowerCased() const noexcept;

  /**
    Returns true if the string is title cased.
  */
  bool isTitleCased() const noexcept;

#if 0
  /**
    Returns the string as UTF-8 string.
  */
  inline operator String() const
  {
    return String(*this);
  }
#endif

#if 0
  /**
    Returns the string as UTF-8 string.
  */
  inline operator Unicode::WCharString() const
  {
    return Unicode::WCharString(*this);
  }
#endif
  
  /**
    Trims the string.

    @param character The character to remove.
  */
  WideString& trim(ucs4 character = ' ');

  /**
    Returns the index of the first substring that matches the specified string
    after the start position. The implementation is based on the "Boyer-Moore
    Fast String Searching Algorithm".
    
    @see indexOf
    @param string The substring to find.
    @param start Specifies the start position of the search. Default is 0.
    @return Index of the first match if any otherwise -1. Also returns -1 if
    substring is empty.
  */
  MemoryDiff search(const WideString& substring, MemorySize start = 0) const noexcept;

  /**
    Returns the substrings between the specified separator.

    @param separator Separator.
    @param group Group separators. Default is false.
  */
  Array<WideString> split(Char separator, bool group = false) const;

  /** Returns the internal container. */
  const Reference<ReferenceCountedAllocator<Char> >& getContainer() const
  {
    return elements;
  }
  
  /**
    Returns NULL-terminated string for modifying access.
  */
  Char* getElements();

  /** Returns true if state is valid. */
  inline bool invariant() const noexcept
  {
    if (!elements) {
      return false;
    }
    const MemorySize length = getLength();
    if (length == 0) {
      return false;
    }
    const Char* buffer = elements->getElements();
    return (buffer[length] == Traits::TERMINATOR);
  }
  
  /**
    Returns NULL-terminated string.
  */
  inline const Char* getElements() const noexcept
  {
    const Char* result = elements->getElements();
    return result;
  }

  /**
    Returns the end of string.
  */
  inline const Char* getEnd() const noexcept
  {
    const MemorySize length = getLength();
    const Char* result = elements->getElements();
    return result + length;
  }

  // we do not support this const uint8* getBytes() const noexcept;

  /**
    Returns true if the NULL is used within the string.
  */
  bool hasNull() const noexcept;

  /**
    Returns the internal buffer.
  */
  inline ConstSpan<ucs4> getSpan() const noexcept
  {
    return ConstSpan<ucs4>(getBuffer(), getLength());
  }

  /**
    Returns the characters of the string for non-modifying access.
  */
  inline const Char* native() const noexcept
  {
    const Char* result = elements->getElements();
    return result;
  }
  
  /**
    Returns true if the string has valid Unicode codes.
   
    @iso Specifies the ISO codes should be allowed.
  */
  bool isValidUnicode(bool iso = false) const noexcept;
  
  /**
    Returns the string with all invalid Unicode codes removed.
   
    @iso Specifies the ISO codes should be allowed.
  */
  WideString getValidUnicode(bool iso = false) const;
};

template<>
int compare<WideString>(const WideString& left, const WideString& right);

template<>
class _COM_AZURE_DEV__BASE__API Hash<WideString> {
public:

  unsigned long operator()(const WideString& value) noexcept;
};

/**
  Writes string to format stream.
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const WideString& value);

/**
  Returns a new string that is the concatenation of the two specified strings.
*/
inline WideString operator+(const WideString& left, const WideString& right)
{
  return WideString(left.getLength() + right.getLength()).append(left).append(right);
}

#if 0
/**
  Returns a new string that is the concatenation of the two specified strings.
*/
inline WideString operator+(const WideString& left, const String& right)
{
  return WideString(left.getLength() + right.getLength()).append(left).append(right);
}

/**
  Returns a new string that is the concatenation of the two specified strings.
*/
inline WideString operator+(const String& left, const WideString& right)
{
  return WideString(left.getLength() + right.getLength()).append(left).append(right);
}
#endif

/**
  Returns a new string that is the concatenation of the two specified strings.
*/
_COM_AZURE_DEV__BASE__API WideString operator+(const WideString& left, const char* right);

/**
  Returns a new string that is the concatenation of the two specified strings.
*/
_COM_AZURE_DEV__BASE__API WideString operator+(const char* left, const WideString& right);

/**
  Returns a new string that is the concatenation of the two specified strings.
*/
_COM_AZURE_DEV__BASE__API WideString operator+(const WideString& left, const wchar* right);

/**
  Returns a new string that is the concatenation of the two specified strings.
*/
_COM_AZURE_DEV__BASE__API WideString operator+(const wchar* left, const WideString& right);

/**
  String reduction. Removes suffix from string if and only if it ends with the
  suffix (e.g. ("presuf"-"suf") results in a new string "pre" whereas
  ("pre"-"suf") results in "pre").
*/
_COM_AZURE_DEV__BASE__API WideString operator-(const WideString& left, const WideString& right);

/**
  String reduction. Removes suffix from string if and only if it ends with the
  suffix (e.g. ("presuf"-"suf") results in a new string "pre" whereas
  ("pre"-"suf") results in "pre").
*/
inline WideString operator-(const WideString& left, const char* right)
{
  return left - WideString(right);
}

/**
  String reduction. Removes suffix from string if and only if it ends with the
  suffix (e.g. ("presuf"-"suf") results in a new string "pre" whereas
  ("pre"-"suf") results in "pre").
*/
inline WideString operator-(const WideString& left, const wchar* right)
{
  return left - WideString(right);
}

/**
  Returns a string that is the concatenation of the given string the given number of times.
*/
_COM_AZURE_DEV__BASE__API WideString operator*(const WideString& src, unsigned int count);

/**
  Returns a string that is the concatenation of the given string the given number of times.
  Returns empty string for 0 or negative.
*/
inline WideString operator*(const WideString& src, int count)
{
  return (count > 0) ? (src * static_cast<unsigned int>(count)) : WideString();
}

template<>
inline void swapper<WideString>(WideString& a, WideString& b)
{
  swapper(a.elements, b.elements); // self swap allowed
}

/** Creates String. */
inline WideString operator "" _ws(const char* text, size_t length)
{
  return WideString(text, length);
}

/** Creates String. */
inline WideString operator "" _ws(const wchar* text, size_t length)
{
  return WideString(text, length);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
