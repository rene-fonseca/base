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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Guid. Used as unique identifier.
*/

class _COM_AZURE_DEV__BASE__API Guid {
public:

  enum {
    SIZE = 16
  };
private:

  /** The guid. */
  uint8 guid[SIZE];
public:

  /** Returns a likely unique guid. */
  static Guid createGuid() noexcept;

  /** Returns a likely unique guid as a string. */
  static String createGuidAsString();

  /** Converts string to guid. */
  static Guid getGuid(const char* _guid);

  /** Converts string to guid. */
  static Guid getGuid(const String& _guid);

  /** Returns true if string is a guid. */
  static bool isGuid(const char* _guid);

  /** Returns true if string is a guid. */
  static bool isGuid(const String& _guid);

  /** Initializes as null. */
  Guid() noexcept;
  
  Guid(const uint8 (&_guid)[SIZE]) noexcept;

  Guid(const Guid& _copy) noexcept;
  
  Guid& operator=(const Guid& assign) noexcept;

  /**
    Returns true if the identifiers are identical.
  */
  bool operator==(const Guid& compare) const noexcept;

  /**
    Returns true if the identifiers are different.
  */
  bool operator!=(const Guid& compare) const noexcept;

  /** Returns true if the guid is null. */
  bool isNull() const noexcept;

  /** Converts the guid to a string. E.g. "{B429A864-CDFC-4044-BCC1-BDDE18EAF8DE}" */
  String toString() const;
  
  /** Converts the guid to a string. E.g. "{B429A864-CDFC-4044-BCC1-BDDE18EAF8DE}" */
  inline operator String() const
  {
    return toString();
  }

  /**
    Returns the identifier.
  */
  inline const uint8* getBytes() const noexcept
  {
    return guid;
  }
};

/**
  Writes the guid to the format output stream. E.g. {B429A864-CDFC-4044-BCC1-BDDE18EAF8DE}.
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Guid& value);

_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE(Guid)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
