/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__URN_H
#define _DK_SDU_MIP__BASE_NET__URN_H

#include <base/Object.h>
#include <base/string/String.h>
#include <base/string/InvalidFormat.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Implementation of Uniform Resource Name (URN) as specified by RFC 2141.
  Uniform Resource Names (URNs) are intended to serve as persistent,
  location-independent, resource identifiers. The components of the urn are
  stored internally in escaped format.
  
  @code
  Urn urn("urn:NID:NSS")
  @endcode
  
  @short Uniform Resource Name (URN)
  @ingroup net
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Urn : public Object {
private:

  /** The namespace identifier. */
  String nid;
  /** The namespace specific string. */
  String nss;
public:

  /**
    Returns true if the specified string is a valid urn.
  */
  static bool isUrn(const String& urn) throw();
  
  /**
    Initializes urn as invalid.
  */
  Urn() throw();
  
  /**
    Initializes urn from a string representation.
    
    @param urn The string representation of the URN.
  */
  Urn(const String& urn) throw(InvalidFormat, MemoryException);
  
  /**
    Initializes URN from nid and nss strings.
    
    @param nid The namespace identifier.
    @param nss The namespace specific string.
  */
  Urn(const String& nid, const String& nss) throw(InvalidFormat);
  
  /**
    Initializes urn from other urn.
  */
  Urn(const Urn& copy) throw();

  /**
    Assignment of URN by URN.
  */
  Urn& operator=(const Urn& eq) throw();
  
  /**
    Returns true if the URN are equal.
  */
  bool operator==(const Urn& eq) const throw();
  
  /**
    Returns the namespace identifier.
  */
  inline String getNID() const throw() {
    return nid;
  }
  
  /**
    Returns the namespace specific string.
  */
  inline String getNSS() const throw() {
    return nss;
  }
  
  /**
    Returns the unescaped namespace specific string.
  */
  String getUnescapedNSS() const throw(MemoryException);

  /**
    Sets the nid of the URN.
  */
  void setNID(const String& value) throw(InvalidFormat);
  
  /**
    Sets the namespace specific string of the URN.
  */
  void setNSS(const String& value) throw(InvalidFormat);
  
  /**
    Returns the URN in escaped format.
  */
  String getUrn() const throw(MemoryException);
};

/**
  Writes the specified URN to the format output stream.
*/
inline FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  const Urn& value) throw(MemoryException, IOException) {
  return stream << value.getUrn();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
