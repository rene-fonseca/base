/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__CHARACTER_SET_H
#define _DK_SDU_MIP__BASE_STRING__CHARACTER_SET_H

#include <base/Object.h>
#include <base/mem/ReferenceCountedAllocator.h>
#include <base/string/String.h>
#include <base/string/WideString.h>
#include <base/string/InvalidFormat.h>
#include <base/io/FileException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  8 bit character set.
  
  @short 8 bit character set.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class CharacterSet : public Object {
private:

  /** The character representation. */
  typedef WideString::ucs4 ucs4;
  /** Container type for the character set. */
  typedef ReferenceCountedAllocator<ucs4> CharacterSetImpl;
  
  /** The character set. */
  ReferenceCountedObjectPointer<CharacterSetImpl> characterSet;
public:
  
  /**
    Loads the character set from the specified file.
  */
  static CharacterSet load(const String& path) throw(FileException, InvalidFormat);
  
  /**
    Loads the character set.

    @param serachPaths The serach paths.
    @param path The path of the character set (normally relative).
  */
  static CharacterSet load(const Array<String>& searchPaths, const String& path) throw(FileException, InvalidFormat);
  
  /**
    Initializes the character set filled with 0.
  */
  CharacterSet() throw(MemoryException);
  
  /**
    Initializes the character set from the specified file.
  */
  inline CharacterSet(const String& path) throw(FileException, InvalidFormat) {
    load(path);
  }

  /**
    Initalizes the character set from other character set.
  */
  inline CharacterSet(const CharacterSet& copy) throw() : characterSet(copy.characterSet) {
  }
  
  /**
    Assignment of character set by character set.
  */
  inline CharacterSet& operator=(const CharacterSet& eq) throw() {
    characterSet = eq.characterSet;
  }
  
  /**
    Returns the character set.
  */
  inline const ucs4* getCharacterSet() const throw() {
    return characterSet->getElements();
  }

  /**
    Returns the Unicode value of the specified character in the character set.

    @param character The character within the character set.
  */
  inline ucs4 getCharacter(uint8 value) const throw() {
    return characterSet->getElements()[value];
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
