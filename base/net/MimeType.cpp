/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/net/MimeType.h>
#include <base/string/StringOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class MimeTypeImpl {
public:

  static const StringLiteral APPLICATION;
  static const StringLiteral AUDIO;
  static const StringLiteral IMAGE;
  static const StringLiteral MESSAGE;
  static const StringLiteral MULTIPART;
  static const StringLiteral TEXT;
  static const StringLiteral VIDEO;

  static inline MimeType::MediaType getWellKnownMediaType(const String& type) throw() {
    if (type.compareToIgnoreCase("application")) { // not very optimal
      return MimeType::APPLICATION;
    } else if (type.compareToIgnoreCase("audio")) {
      return MimeType::AUDIO;
    } else if (type.compareToIgnoreCase("image")) {
      return MimeType::IMAGE;
    } else if (type.compareToIgnoreCase("message")) {
      return MimeType::MESSAGE;
    } else if (type.compareToIgnoreCase("multipart")) {
      return MimeType::MULTIPART;
    } else if (type.compareToIgnoreCase("text")) {
      return MimeType::TEXT;
    } else if (type.compareToIgnoreCase("video")) {
      return MimeType::VIDEO;
    } else {
      return MimeType::EXTENSION;
    }
  }
};

const StringLiteral MimeTypeImpl::APPLICATION = MESSAGE("application");
const StringLiteral MimeTypeImpl::AUDIO = MESSAGE("audio");
const StringLiteral MimeTypeImpl::IMAGE = MESSAGE("image");
const StringLiteral MimeTypeImpl::MESSAGE = MESSAGE("message");
const StringLiteral MimeTypeImpl::MULTIPART = MESSAGE("multipart");
const StringLiteral MimeTypeImpl::TEXT = MESSAGE("text");
const StringLiteral MimeTypeImpl::VIDEO = MESSAGE("video");

MimeType::MimeType() throw(MemoryException) {
  type = MimeTypeImpl::APPLICATION;
  subtype = MESSAGE("octet-stream");
  mediaType = APPLICATION;
}

MimeType::MimeType(const String& _type, const String& _subtype) throw()
  : type(_type), subtype(_subtype) {
}

MimeType::MimeType(MediaType _mediaType, const String& _subtype) throw(OutOfDomain, MemoryException) {
  switch (mediaType) {
  case APPLICATION:
    type = MimeTypeImpl::APPLICATION;
    break;
  case AUDIO:
    type = MimeTypeImpl::AUDIO;
    break;
  case IMAGE:
    type = MimeTypeImpl::IMAGE;
    break;
  case MESSAGE:
    type = MimeTypeImpl::MESSAGE;
    break;
  case MULTIPART:
    type = MimeTypeImpl::MULTIPART;
    break;
  case TEXT:
    type = MimeTypeImpl::TEXT;
    break;
  case VIDEO:
    type = MimeTypeImpl::VIDEO;
    break;
  case EXTENSION:
  case UNINITIALIZED:
  default:
    throw OutOfDomain(this);
  }
  this->mediaType = _mediaType;  
  this->subtype = _subtype;
}

MimeType::MimeType(const String& value) throw(InvalidFormat) : mediaType(UNINITIALIZED) {
  const int index = value.indexOf('/');
  assert(index >= 0, InvalidFormat(this));
  type = value.substring(0, index - 1);
  subtype = value.substring(index + 1);
}

bool MimeType::isValid() const throw() {
  // for grammar see RFC 2045

  {
    // check type
    String::ReadIterator i = type.getBeginReadIterator();
    const String::ReadIterator end = type.getEndReadIterator();
    
    if (i >= end) {
      return false;
    }
    while (i < end) {
      char ch = *i++;
      
      assert(!ASCIITraits::isControl(ch), InvalidFormat(this));
      switch (ch) {
      case ' ':
	// tspecials
      case '(': case ')': case '<': case '>': case '@':
      case ',': case ';': case ':': case '\\': case '"':
      case '/': case '[': case ']': case '?': case '=':
	return false;
      }
    }
  }

  {
    // check subtype
    String::ReadIterator i = subtype.getBeginReadIterator();
    const String::ReadIterator end = subtype.getEndReadIterator();

    if (i >= end) {
      return false;
    }
    while (i < end) {
      char ch = *i++;
      
      assert(!ASCIITraits::isControl(ch), InvalidFormat(this));
      switch (ch) {
      case ' ':
	// tspecials
      case '(': case ')': case '<': case '>': case '@':
      case ',': case ';': case ':': case '\\': case '"':
      case '/': case '[': case ']': case '?': case '=':
	return false;
      }    
    }
  }
  
  return true;
}

bool MimeType::operator==(const MimeType& value) const throw() {
  if ((mediaType == UNINITIALIZED) || (mediaType == EXTENSION) ||
      (value.mediaType == UNINITIALIZED) || ((value.mediaType == EXTENSION))) {
    return type.compareToIgnoreCase(value.type) && subtype.compareToIgnoreCase(value.subtype); // avoid cached media type
  }
  return (mediaType == value.mediaType) && subtype.compareToIgnoreCase(value.subtype);
}

MimeType::MediaType MimeType::getMediaType() const throw() {
  if (this->mediaType == UNINITIALIZED) {
    this->mediaType = MimeTypeImpl::getWellKnownMediaType(type);
  }
  return mediaType;
}

bool MimeType::matches(MediaType mediaType, const String& subtype) const throw() {
  if (this->mediaType == UNINITIALIZED) {
    this->mediaType = MimeTypeImpl::getWellKnownMediaType(this->type);
  }
  return (this->mediaType == mediaType) && (this->mediaType != EXTENSION) && this->subtype.compareToIgnoreCase(subtype);
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const MimeType& value) throw(IOException) {
  StringOutputStream s;
  s << value.getType() << '/' << value.getSubtype() << FLUSH;
  return stream << s.getString();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
