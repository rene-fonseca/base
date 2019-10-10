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
  Object Request Broker object identifier (URI).
  
  @short Object Request Broker URI.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OrbIdentifier : public Object {
private:

  /** The scheme. */
  String scheme;
  /** The end point. */
  String endPoint;
  /** The path. */
  String path;
  /** The URI. */
  String identifier;
public:
  
  /**
    Initializes the identifier.
  */
  OrbIdentifier(const String& identifier) throw(InvalidFormat);

  /**
    Returns true if the identifier has a scheme.
  */
  inline bool hasScheme() const throw() {
    return scheme.getLength();
  }

  /**
    Returns true if the identifier has an end-point.
  */
  inline bool hasEndPoint() const throw() {
    return endPoint.getLength();
  }

  /**
    Returns true if the identifier has a path.
  */
  inline  bool hasPath() const throw() {
    return path.getLength();
  }

  /**
    Returns the scheme of the identifier.
  */
  inline const String& getScheme() const throw() {
    return scheme;
  }

  /**
    Returns the end-point of the identifier.
  */
  inline String getFullScheme() const throw() {
    return scheme + Literal("://");
  }

  /**
    Returns the end-point of the identifier.
  */
  inline const String& getEndPoint() const throw() {
    return endPoint;
  }

  /**
    Returns the end-point of the identifier.
  */
  inline String getFullEndPoint() const throw() {
    return scheme + Literal("://") + endPoint;
  }
  
  /**
    Returns the end-point of the identifier.
  */
  inline const String& getPath() const throw() {
    return path;
  }
  
  /**
    Returns the full identifier.
  */
  inline String getIdentifier() const throw() {
    return scheme + Literal("://") + endPoint + Literal("/") + path;
  }

//   inline operator String() const throw() {
//     return identifier;
//   }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
