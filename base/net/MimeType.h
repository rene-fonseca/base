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

#include <base/string/String.h>
#include <base/string/InvalidFormat.h>
#include <base/OutOfDomain.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Multipurpose Internet Mail Extensions (MIME) (see RFC 2046).

  @short Internet Media Type
  @ingroup net
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API MimeType : public Object {
public:
  
  /** Top-level media types. */
  enum MediaType {
    APPLICATION, /**< Application type. */
    AUDIO, /**< Audio type. */
    IMAGE, /**< Image type. */
    MESSAGE, /**< Message type. */
    MULTIPART, /**< Multipart type. */
    TEXT, /**< Text type. */
    VIDEO, /**< Video type. */
    EXTENSION, /**< Unspecified type. */
    UNINITIALIZED, /**< Used internally. */
  };
private:
  
  /** The media type. */
  String type;
  /** The subtype. */
  String subtype;
  /** The cached identifier of the media type. */
  mutable MediaType mediaType = UNINITIALIZED;
public:
  
  /**
    Initializes the mime type as "application/octet-stream".
  */
  MimeType() throw(MemoryException);
  
  /**
    Initializes the mime object with specified values. Use the isValid() method
    to validate against grammar.

    @param type The type of the mime object (e.g. image).
    @param subtype The subtype of the mime object (e.g. jpeg).
  */
  MimeType(const String& type, const String& subtype) noexcept;
  
  /**
    Initializes the mime object with specified values. Use the isValid() method
    to validate against grammar. The media type must be APPLICATION, AUDIO,
    IMAGE, MESSAGE, MULTIPART, TEXT, or VIDEO. OutOfDomain is raised for all
    other values.

    @param mediaType The media type of the mime object (e.g. VIDEO).
    @param subtype The subtype of the mime object (e.g. mpeg).
  */
  MimeType(MediaType mediaType, const String& subtype) throw(OutOfDomain, MemoryException);
  
  /**
    Initializes the mime object with specified values. Only raises
    InvalidFormat is '/' is missing. Use the isValid() method to validate
    against grammar.

    @param value The string representation of the mime object (e.g. "text/html").
  */
  MimeType(const String& value) throw(InvalidFormat);

  /**
    Assignment of MIME type to MIME type.
  */
  MimeType& operator=(const MimeType& assign) noexcept;
  
  /**
    Returns true if the type and subtype complies with the grammar described in
    RFC 2045.
  */
  bool isValid() const noexcept;
  
  /**
    Returns true if the objects are equal.
  */
  bool operator==(const MimeType& value) const noexcept;
  
  /**
    Returns false if the objects are different.
  */
  inline bool operator!=(const MimeType& value) const noexcept
  {
    return !(operator==(value));
  }
  
  /**
    Returns the type.
  */
  inline const String& getType() const noexcept
  {
    return type;
  }
  
  /**
    Returns the subtype.
  */
  inline const String& getSubtype() const noexcept
  {
    return subtype;
  }
  
  /**
    Returns the top-level type.
    
    @return EXTENSION if the media type is unknown.
  */
  MediaType getMediaType() const noexcept;
  
  /**
    Returns true if the media type and subtype matches.

    @return Always false if specified type is EXTENSION.
  */
  bool matches(MediaType mediaType, const String& subtype) const noexcept;
};

/**
  Writes MimeType object to the format output stream (e.g. "text/plain").
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const MimeType& value);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
