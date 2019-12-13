/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/net/Urn.h>
#include <base/string/ASCIITraits.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

bool Urn::isUrn(const String& urn) noexcept {
  String::ReadIterator i = urn.getBeginReadIterator();
  String::ReadIterator end = urn.getEndReadIterator();
  
  // check prefix
  if (!(((end - i) >= 4) &&
        (ASCIITraits::toLower(*i++) == 'u') &&
        (ASCIITraits::toLower(*i++) == 'r') &&
        (ASCIITraits::toLower(*i++) == 'n') &&
        (*i++ == ':'))) {
    return false;
  }
  
  // check nid
  if (((end - i) >= 4) &&
      (ASCIITraits::toLower(i[0]) == 'u') &&
      (ASCIITraits::toLower(i[1]) == 'r') &&
      (ASCIITraits::toLower(i[2]) == 'n') &&
      (i[3] == ':')) {
    return false;
  }
  
  if ((i == end) || !ASCIITraits::isAlphaNum(*i++)) {
    return false;
  }
  
  for (
    unsigned int j = 31;
    (j > 0) && (i != end) && ((ASCIITraits::isAlphaNum(*i)) || (*i == '-'));
    --j, ++i
  );
  
  if ((i == end) || (*i++ != ':')) {
    return false;
  }

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
      if (!(((end - i) >= 2) &&
            (ASCIITraits::isHexDigit(i[0])) &&
            (ASCIITraits::isHexDigit(i[1])))) {
        return false;
      }
      ++i;
      ++i;
      break;
    default:
      return false;
    }
  };
  return i != j;
}

Urn::Urn() noexcept {
}

Urn::Urn(const String& urn) throw(InvalidFormat, MemoryException) {
  String::ReadIterator i = urn.getBeginReadIterator();
  String::ReadIterator end = urn.getEndReadIterator();
  
  // check prefix
  bassert(
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
  bassert(
    (i != end) && (ASCIITraits::isAlphaNum(*i++)),
    InvalidFormat(this)
  );
  for (
    unsigned int j = 31;
    (j > 0) && (i != end) && ((ASCIITraits::isAlphaNum(*i)) || (*i == '-'));
    --j, ++i
  );
  bassert(
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
      bassert(
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
  bassert(i != j, InvalidFormat(this));
}

Urn::Urn(const String& nid, const String& nss) throw(InvalidFormat)
{
  setNID(nid);
  setNSS(nss);
}

Urn::Urn(const Urn& copy) noexcept
  : nid(copy.nid),
    nss(copy.nss)
{
}

Urn& Urn::operator=(const Urn& assign) noexcept
{
  nid = assign.nid;
  nss = assign.nss;
  return *this;
}

bool Urn::operator==(const Urn& compare) const throw()
{
  if (nid.compareToIgnoreCase(compare.nid) != 0) {
    return false;
  }
  if (nss.getLength() != compare.nss.getLength()) {
    return false;
  }
  String::ReadIterator i = nss.getBeginReadIterator();
  String::ReadIterator j = compare.nss.getBeginReadIterator();
  const String::ReadIterator iEnd = nss.getEndReadIterator();
  const String::ReadIterator jEnd = compare.nss.getEndReadIterator();
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

String Urn::getUnescapedNSS() const throw(MemoryException)
{
  String result(nss.getLength());
  const String::ReadIterator end = nss.getEndReadIterator();
  for (String::ReadIterator i = nss.getBeginReadIterator(); i < end; ++i) {
    char ch = *i;
    if (ch == '%') {
      bassert(
        end - i >= 2,
        InvalidFormat("Invalid encoding", this)
      );
      char high = *++i;
      char low = *++i;
      bassert(
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
  bassert(
    (i != end) && (ASCIITraits::isAlphaNum(*i++)),
    InvalidFormat(this)
  );
  for (
    unsigned int j = 31;
    (j > 0) && (i != end) && ((ASCIITraits::isAlphaNum(*i)) || (*i == '-'));
    --j, ++i
  );
  bassert(i == end, InvalidFormat(this));
  this->nid = value;
}

void Urn::setNSS(const String& value) throw(InvalidFormat) {
  String::ReadIterator i = value.getBeginReadIterator();
  const String::ReadIterator end = value.getEndReadIterator();
  
  bassert(i != end, InvalidFormat(this));
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
      bassert(
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
  this->nss = value;
}

String Urn::getUrn() const throw(MemoryException) {
  return Literal("urn:") + nid + Literal(":") + nss;
}

#if 0 && defined(_COM_AZURE_DEV__BASE__TESTS) // TAG: not implemented fully

class TEST_CLASS(Urn) : public UnitTest {
public:

  TEST_PRIORITY(100);
  TEST_PROJECT("base/net");
  TEST_IMPACT(PRIVACY);

  void run() override
  {
    // TAG: use static instead of constructor
    Urn urn1("urn:isbn:0451450523");
    Urn urn2("urn:isan:0000-0000-2CEA-0000-1-0000-0000-Y");
    Urn urn3("urn:uuid:6e8bc430-9c3a-11d9-9669-0800200c9a66");

    TEST_ASSERT(urn1.getNID() == "isbn");
    TEST_ASSERT(urn2.getNSS() == "0000-0000-2CEA-0000-1-0000-0000-Y");
    TEST_ASSERT(urn3.getNSS() == "6e8bc430-9c3a-11d9-9669-0800200c9a66");
  }
};

TEST_REGISTER(Urn);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
