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
#include <base/Architecture.h>
#include <base/mem/Reference.h>
#include <base/mem/ReferenceCountedAllocator.h>
#include <base/string/String.h>
#include <base/string/WideString.h>
#include <base/string/InvalidFormat.h>
#include <base/io/FileException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  8 bit character set.
  
  @short 8 bit character set.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API CharacterSet : public Object {
private:
  
  /** Container type for the character set. */
  typedef ReferenceCountedAllocator<ucs4> CharacterSetImpl;
  
  /** The character set. */
  Reference<CharacterSetImpl> characterSet;
public:
  
  /**
    Loads the character set from the specified file.
  */
  static CharacterSet load(const String& path);
  
  /**
    Loads the character set.

    @param serachPaths The serach paths.
    @param path The path of the character set (normally relative).
  */
  static CharacterSet load(const Array<String>& searchPaths, const String& path);
  
  /**
    Initializes the character set filled with 0.
  */
  CharacterSet();
  
  /**
    Initializes the character set from the specified file.
  */
  inline CharacterSet(const String& path)
  {
    load(path);
  }

  /**
    Initalizes the character set from other character set.
  */
  inline CharacterSet(const CharacterSet& copy) noexcept
    : characterSet(copy.characterSet)
  {
  }
  
  /**
    Assignment of character set by character set.
  */
  inline CharacterSet& operator=(const CharacterSet& assign) noexcept
  {
    characterSet = assign.characterSet;
    return *this;
  }
  
  /**
    Returns the character set.
  */
  inline const ucs4* getCharacterSet() const noexcept
  {
    return characterSet->getElements();
  }

  /**
    Returns the Unicode value of the specified character in the character set.

    @param character The character within the character set.
  */
  inline ucs4 getCharacter(uint8 value) const noexcept
  {
    return characterSet->getElements()[value];
  }

  /**
    Sets the Unicode value of the specified character in the character set.
    
    @param value The 8 bit character within the character set.
    @param code The Unicode value.
  */
  inline void setCharacter(uint8 value, ucs4 code) noexcept
  {
    characterSet->getElements()[value] = value;
  }
  
  /**
    Saves the character set to the specified file.
    
    @param path The file path.
    @param byteOrder The desired byte order. Default is Architecture::NATIVE.
  */
  void save(const String& path, Architecture::ByteOrder byteOrder = Architecture::NATIVE) const;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
