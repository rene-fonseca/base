/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COMMUNICATION__EUI_64_H
#define _DK_SDU_MIP__BASE_COMMUNICATION__EUI_64_H

#include <base/OutOfDomain.h>
#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A 64 bit globally unique identifier (GUID) composed of a 24 bit company id and
  an 48 bit extension identifier.
  
  @ingroup communications
  @short IEEE EUI-64 identifier.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/
class EUI64 {
private:
  
  /** Unique 64 bit identifier. */
  unsigned char id[8];
protected:

  /**
    Swaps the 4 bit nibbles of a 8 bit word.
  */
  static inline unsigned char swapNibbles(unsigned char value) throw() {
    return ((value & 0x0f) << 4) | ((value & 0xf0) >> 4);
  }
public:
  
  /**
    Initializes identifier as 00:00:00-00:00:00:00:00.
  */
  EUI64() throw();
  
  /**
    Initializes identifier.
  */
  EUI64(const unsigned char value[8]) throw();
  
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
    Returns true if the identifier is 00:00:00-00:00:00:00:00.
  */
  bool isInvalid() const throw();
  
  /**
    Returns true if the id is an encapsulated EUI-48 identifier.
  */
  inline bool isEUI48() const throw() {
    return (id[3] == 0xff) && (id[4] == 0xfe);
  }
  
  /**
    Returns true if the id is an encapsulated MAC-48 identifier.
  */
  inline bool isMAC48() const throw() {
    return (id[3] == 0xff) && (id[4] == 0xff);
  }
  
  /**
    Fills the specified array with the EUI-48 identifier.
  */
  void getEUI48(unsigned char (&eui48)[6]) const throw();

  /**
    Sets the identifier by an EUI-48 identifier.
  */
  void setEUI48(const unsigned char (&eui48)[6]) throw();
  
  /**
    Fills specified array with the MAC idenfier.
  */
  void getMAC48(unsigned char (&mac)[6]) const throw();
  
  /**
    Sets the identifier by an MAC-48 identifier.
  */
  void setMAC48(const unsigned char (&mac)[6]) throw();
  
  /**
    Returns the identifier.
  */
  inline const unsigned char* getBytes() const throw() {
    return reinterpret_cast<const unsigned char*>(&id);
  }
};

/**
  Writes the globally unique identifier (EUI-64) to the format output stream (e.g. 12:34:56-09:87:65:43:21).
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const EUI64& value) throw(IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
