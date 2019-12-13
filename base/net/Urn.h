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
#include <base/string/String.h>
#include <base/string/InvalidFormat.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Implementation of Uniform Resource Name (URN) as specified by RFC 2141.
  Uniform Resource Names (URNs) are intended to serve as persistent,
  location-independent, resource identifiers. The components of the urn are
  stored internally in escaped format.
 
  See https://en.wikipedia.org/wiki/Uniform_Resource_Name.
  
  @code
  Urn urn("urn:NID:NSS")
  @endcode
  
  @short Uniform Resource Name (URN)
  @ingroup net
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Urn : public Object {
private:

  /** The namespace identifier. */
  String nid;
  /** The namespace specific string. */
  String nss;
public:

  /**
    Returns true if the specified string is a valid urn.
  */
  static bool isUrn(const String& urn) noexcept;
  
  /**
    Initializes urn as invalid.
  */
  Urn() noexcept;
  
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
  Urn(const Urn& copy) noexcept;

  /**
    Assignment of URN by URN.
  */
  Urn& operator=(const Urn& assign) noexcept;
  
  /**
    Returns true if the URN are equal.
  */
  bool operator==(const Urn& compare) const noexcept;
  
  /**
    Returns the namespace identifier.
  */
  inline const String& getNID() const throw()
  {
    return nid;
  }
  
  /**
    Returns the namespace specific string.
  */
  inline const String& getNSS() const throw()
  {
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
  const Urn& value) throw(MemoryException, IOException)
{
  return stream << value.getUrn();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
