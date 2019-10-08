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

#include <base/OutOfDomain.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/InvalidFormat.h>
#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A 64 bit globally unique identifier (GUID) composed of a 24 bit company id
  and an 48 bit extension identifier. See
  http://standards.ieee.org/regauth/oui/tutorials/EUI64.html and RFC 2373.
  
  @ingroup communications
  @short IEEE EUI-64 identifier.
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API EUI64 {
private:
  
  /** Unique 64 bit identifier. */
  uint8 id[8];
protected:

  /**
    Swaps the 4 bit nibbles of a 8 bit word.
  */
  static inline uint8 swapNibbles(uint8 value) throw() {
    return ((value & 0x0f) << 4) | ((value & 0xf0) >> 4);
  }
public:
  
  /**
    Initializes identifier as ff:ff:ff-00:00:00:00:00.
  */
  EUI64() throw();
  
  /**
    Initializes identifier from array.
  */
  EUI64(const uint8 value[8]) throw();

  /**
    Initializes identifier from string.
  */
  EUI64(const String& value) throw(InvalidFormat);
  
  /**
    Initializes identifier from other identifier.
  */
  EUI64(const EUI64& copy) throw();
  
  /**
    Assignment of identifier by identifier.
  */
  EUI64& operator=(const EUI64& eq) throw();
  
  /**
    Returns true if the identifiers are identical.
  */
  bool operator==(const EUI64& eq) const throw();
  
  /**
    Returns true if the identifiers are different.
  */
  bool operator!=(const EUI64& eq) const throw();
  
  /**
    Returns the 24 bit company id.
  */
  unsigned int getCompanyId() const throw();
  
  /**
    Sets the 24 bit company id.
  */
  void setCompanyId(unsigned int companyId) throw(OutOfDomain);
  
  /**
    Returns the extension identifier of this EUI-64 id.
  */
  uint64 getExtensionId() const throw();
  
  /**
    Sets the extension identifier of this EUI-64 id.
  */
  void setExtensionId(uint64 extensionId) throw(OutOfDomain);
  
  /**
    Returns true if the identifier is ff:ff:ff-00:00:00:00:00.
  */
  bool isInvalid() const throw();
  
  /**
    Returns true if the id is an encapsulated EUI-48 identifier.
  */
  inline bool isEUI48() const throw() {
    return (id[3] == 0xff) && (id[4] == 0xfe); // TAG: conflict with RFC 2373
  }
  
  /**
    Returns true if the id is an encapsulated MAC-48 identifier.
  */
  inline bool isMAC48() const throw() {
    return (id[3] == 0xff) && (id[4] == 0xff); // TAG: conflict with RFC 2373
  }

  /**
    Returns true if the individual/group bit is set.
  */
  inline bool isIndividual() const throw() {
    return (id[0] & 0x01) == 0;
  }
  
  /**
    Returns true if the universal/local bit is set.
  */
  inline bool isGlobal() const throw() {
    return (id[0] & 0x02) == 0;
  }

  /**
    Resets the universal/local bit.
  */
  inline void makeGlobal() throw() {
    id[0] &= ~0x02;
  }
  
  /**
    Sets the universal/local bit.
  */
  inline void makeLocal() throw() {
    id[0] |= 0x02;
  }
  
  /**
    Fills the specified array with the EUI-48 identifier.
  */
  void getEUI48(uint8* eui48) const throw();

  /**
    Sets the identifier by an EUI-48 identifier.
  */
  void setEUI48(const uint8* eui48) throw();
  
  /**
    Fills specified array with the MAC idenfier.
  */
  void getMAC48(uint8* mac) const throw();
  
  /**
    Sets the identifier by an MAC-48 identifier.
  */
  void setMAC48(const uint8* mac) throw();
  
  /**
    Returns the identifier.
  */
  inline const uint8* getBytes() const throw() {
    return reinterpret_cast<const uint8*>(&id);
  }
};

/**
  Writes the globally unique identifier (EUI-64) to the format output stream
  (e.g. 12:34:56-09:87:65:43:21).
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const EUI64& value) throw(IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
