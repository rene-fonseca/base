/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/net/Urn.h>
#include <base/string/ASCIITraits.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Urn::Urn() throw() {
}

Urn::Urn(const String& urn) throw(InvalidFormat, MemoryException) {
  String::ReadIterator i = urn.getBeginReadIterator();
  String::ReadIterator end = urn.getEndReadIterator();
  
  // check prefix
  assert(
    ((end - i) >= 4) &&
    (ASCIITraits::toLower(*i++) == 'u') &&
    (ASCIITraits::toLower(*i++) == 'r') &&
    (ASCIITraits::toLower(*i++) == 'n') &&
    (*i++ == ':'),
    InvalidFormat(this)
  );
  
  // check nid
  if (((end - i) >= 4) &&
      (ASCIITraits::toLower(i[0]) == 'u') &&
      (ASCIITraits::toLower(i[1]) == 'r') &&
      (ASCIITraits::toLower(i[2]) == 'n') &&
      (i[3] == ':')) {
    throw InvalidFormat(this);
  }
  assert(
    (i != end) && (ASCIITraits::isAlphaNum(*i++)),
    InvalidFormat(this)
  );
  for (
    unsigned int j = 31;
    (j > 0) && (i != end) && ((ASCIITraits::isAlphaNum(*i)) || (*i == '-'));
    --j, ++i
  );
  assert(
    (i != end) && (*i++ == ':'),
    InvalidFormat(this)
  );

  // check nss  
  String::ReadIterator j = i;
  while (i != end) {
    if (ASCIITraits::isAlphaNum(*i)) {
      ++i;
      continue;
    }
    switch (*i) {
    case '(':
    case ')':
    case '+':
    case ',':
    case '-':
    case '.':
    case ':':
    case '=':
    case '@':
    case ';':
    case '$':
    case '_':
    case '!':
    case '*':
    case '\'':
      ++i;
      break;
    case '%': // reserved
      assert(
        ((end - i) >= 2) &&
        (ASCIITraits::isHexDigit(i[0])) &&
        (ASCIITraits::isHexDigit(i[1])),
        InvalidFormat(this)
      );
      ++i;
      ++i;
      break;
    default:
      throw InvalidFormat(this);
    }
  };
  assert(i != j, InvalidFormat(this));
}

Urn::Urn(const String& nid, const String& nss) throw(InvalidFormat) {
  setNID(nid);
  setNSS(nss);
}

Urn::Urn(const Urn& copy) throw() :
  nid(copy.nid),
  nss(copy.nss) {
}

Urn& Urn::operator=(const Urn& eq) throw() {
  nid = eq.nid;
  nss = eq.nss;
  return *this;
}

bool Urn::operator==(const Urn& eq) const throw() {
  if (nid.compareToIgnoreCase(eq.nid) != 0) {
    return false;
  }
  if (nss.getLength() != eq.nss.getLength()) {
    return false;
  }
  String::ReadIterator i = nss.getBeginReadIterator();
  String::ReadIterator j = eq.nss.getBeginReadIterator();
  const String::ReadIterator iEnd = nss.getEndReadIterator();
  const String::ReadIterator jEnd = eq.nss.getEndReadIterator();
  while ((i != iEnd) && (j != jEnd)) {
    char left = *i++;
    if (left == '%') {
      left = (ASCIITraits::digitToValue(*i++) << 4) + ASCIITraits::digitToValue(*i++);
    }
    char right = *j++;
    if (right == '%') {
      right = (ASCIITraits::digitToValue(*j++) << 4) + ASCIITraits::digitToValue(*j++);
    }
    if (ASCIITraits::toLower(left) != (ASCIITraits::toLower(right))) {
      return false;
    }
  }
  return true;
}

String Urn::getUnescapedNSS() const throw(MemoryException) {
  String result(nss.getLength());
  const String::ReadIterator end = nss.getEndReadIterator();
  for (String::ReadIterator i = nss.getBeginReadIterator(); i < end; ++i) {
    char ch = *i;
    if (ch == '%') {
      assert(
        end - i >= 2,
        InvalidFormat("Invalid encoding", this)
      );
      char high = *++i;
      char low = *++i;
      assert(
        ASCIITraits::isHexDigit(high) && ASCIITraits::isHexDigit(low),
        InvalidFormat("Invalid encoding", this)
      );
      ch = (ASCIITraits::digitToValue(high) << 4) +
        ASCIITraits::digitToValue(low); // replace with decoded char
    }
    result += ch;
  }
  return result;
}

void Urn::setNID(const String& value) throw(InvalidFormat) {
  String::ReadIterator i = value.getBeginReadIterator();
  const String::ReadIterator end = value.getEndReadIterator();
  
  if (((end - i) == 3) &&
      (ASCIITraits::toLower(i[0]) == 'u') &&
      (ASCIITraits::toLower(i[1]) == 'r') &&
      (ASCIITraits::toLower(i[2]) == 'n')
  ) {
    throw InvalidFormat(this);
  }
  assert(
    (i != end) && (ASCIITraits::isAlphaNum(*i++)),
    InvalidFormat(this)
  );
  for (
    unsigned int j = 31;
    (j > 0) && (i != end) && ((ASCIITraits::isAlphaNum(*i)) || (*i == '-'));
    --j, ++i
  );
  assert(i == end, InvalidFormat(this));
  this->nid = nid;
}

void Urn::setNSS(const String& value) throw(InvalidFormat) {
  String::ReadIterator i = value.getBeginReadIterator();
  const String::ReadIterator end = value.getEndReadIterator();
  
  assert(i != end, InvalidFormat(this));
  while (i != end) {
    if (ASCIITraits::isAlphaNum(*i)) {
      ++i;
      continue;
    }
    switch (*i) {
    case '(':
    case ')':
    case '+':
    case ',':
    case '-':
    case '.':
    case ':':
    case '=':
    case '@':
    case ';':
    case '$':
    case '_':
    case '!':
    case '*':
    case '\'':
      ++i;
      break;
    case '%': // reserved
      assert(
        ((end - i) >= 2) &&
        (ASCIITraits::isHexDigit(i[0])) &&
        (ASCIITraits::isHexDigit(i[1])),
        InvalidFormat(this)
      );
      ++i;
      ++i;
      break;
    default:
      throw InvalidFormat(this);
    }
  };
  this->nss = nss;
}

String Urn::getUrn() const throw(MemoryException) {
  return MESSAGE("urn:") + nid + MESSAGE(":") + nss;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
