/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/net/Url.h>
#include <base/net/InetAddress.h>
#include <base/string/ASCIITraits.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class UrlImpl {
public:

  enum Encode {
    NEVER, // character is not required to be encoded
    RELAXED, // character must be encoded - however, decoding accepts unencoded character
    ALWAYS // character must always be encoded
  };

  typedef Encode (*Encoding)(char);

  static inline Encode defaultEncoding(char ch) throw() {
    switch (ch) {
    case 0x00: case 0x01: case 0x02: case 0x03:
    case 0x04: case 0x05: case 0x06: case 0x07:
    case 0x08: case 0x09: case 0x0a: case 0x0b:
    case 0x0c: case 0x0d: case 0x0e: case 0x0f:
    case 0x10: case 0x11: case 0x12: case 0x13:
    case 0x14: case 0x15: case 0x16: case 0x17:
    case 0x18: case 0x19: case 0x1a: case 0x1b:
    case 0x1c: case 0x1d: case 0x1e: case 0x1f:
    case ' ':
    case '\\':
    case 0x7f:
      return ALWAYS;
    case '<':
    case '>':
    case '"':
    case '#':
    case '%':
    case '{':
    case '}':
    case '|':
    case '^':
    case '~':
    case '`':
      return RELAXED;
    case '[': // RFC 2732
    case ']': // RFC 2732
    default:
      return NEVER;
    }
  }

  static inline Encode userEncoding(char ch) throw() {
    return ((ch == ':') || (ch == '@') || (ch == '/')) ? ALWAYS : defaultEncoding(ch);
  }

  static inline Encode passwordEncoding(char ch) throw() {
    return ((ch == ':') || (ch == '@') || (ch == '/')) ? ALWAYS : defaultEncoding(ch);
  }

  static inline String encode(
    const String& str,
    Encoding encoding = defaultEncoding)
    throw(Url::UrlException, MemoryException) {
    
    String temp(str.getLength());
    const String::ReadIterator end = str.getEndReadIterator();
    for (String::ReadIterator i = str.getBeginReadIterator(); i < end; ++i) {
      char ch = *i;
      bassert(
        ASCIITraits::isASCII(ch),
        Url::UrlException("Invalid character", Type::getType<UrlImpl>())
      );
      if (encoding(ch) != NEVER) {
        temp += '%';
        temp += ASCIITraits::valueToDigit(ch >> 4);
        temp += ASCIITraits::valueToDigit(ch & 0x0f);
      } else {
        temp += ch;
      }
    }
    return temp;
  }

  static String decode(
    const String& str,
    Encoding encoding,
    bool strict) throw(Url::UrlException, MemoryException) {
    String temp(str.getLength());
    const String::ReadIterator end = str.getEndReadIterator();
    for (String::ReadIterator i = str.getBeginReadIterator(); i < end; ++i) {
      char ch = *i;
      if (ch == '%') {
        bassert(
          end - i >= 2,
          Url::UrlException("Invalid encoding", Type::getType<UrlImpl>())
        ); // need two digits
        char high = *++i;
        char low = *++i;
        bassert(
          ASCIITraits::isHexDigit(high) && ASCIITraits::isHexDigit(low),
          Url::UrlException("Invalid encoding", Type::getType<UrlImpl>())
        );
        ch = (ASCIITraits::digitToValue(high) << 4) + ASCIITraits::digitToValue(low); // replace with decoded char
      } else {
        Encode encode = encoding(ch);
        bassert(
          strict ? (encode == NEVER) : (encode <= RELAXED),
          Url::UrlException("Part contains unencoded character", Type::getType<UrlImpl>())
        );
      }
      temp += ch;
    }
    return temp;
  }
};



Url::Url() throw(MemoryException) {
}

Url::Url(const String& url, bool strict) throw(UrlException, MemoryException) {
  parse(url, strict);
}

Url::Url(
  const String& baseUrl,
  const String& relativeUrl,
  bool strict) throw(UrlException, MemoryException) {
  Url url(relativeUrl);
  if (url.isRelative()) {
    parse(baseUrl + relativeUrl, strict);
  } else {
    *this = url;
  }
}

Url::Url(const Url& copy) throw() :
  scheme(copy.scheme),
  user(copy.user),
  password(copy.password),
  host(copy.host),
  port(copy.port),
  path(copy.path) {
}

Url& Url::operator=(const Url& eq) throw() {
  scheme = eq.scheme;
  user = eq.user;
  password = eq.password;
  host = eq.host;
  port = eq.port;
  path = eq.path;
  return *this;
}

bool Url::isRelative() const throw() {
  return !path.getLength() || path.startsWith(Literal("/"));
}

String Url::validateScheme(
  const String& value) throw(UrlException, MemoryException) {
  String temp(value.getLength());
  String::ReadIterator end = value.getEndReadIterator();

  for (String::ReadIterator i = value.getBeginReadIterator(); i < end; ++i) {
    char ch = ASCIITraits::toLower(*i);
    if (ASCIITraits::isLower(ch)) {
    } else if (ASCIITraits::isDigit(ch)) {
    } else if (ch == '+') {
    } else if (ch == '.') {
    } else if (ch == '-') {
    } else {
      throw UrlException("Invalid scheme", Type::getType<Url>());
    }
    temp += ch;
  }
  return temp;
}

String Url::validateUser(const String& str) throw(UrlException) {
  const String::ReadIterator end = str.getEndReadIterator();
  for (String::ReadIterator i = str.getBeginReadIterator(); i < end; ++i) {
    char ch = *i;
    bassert(
      ASCIITraits::isASCII(ch),
      UrlException("Invalid character", Type::getType<Url>())
    );
  }
  return str;
}

String Url::validatePassword(const String& str) throw(UrlException) {
  const String::ReadIterator end = str.getEndReadIterator();
  for (String::ReadIterator i = str.getBeginReadIterator(); i < end; ++i) {
    char ch = *i;
    bassert(
      ASCIITraits::isASCII(ch),
      UrlException("Invalid character", Type::getType<Url>())
    );
  }
  return str;
}

bool Url::isHost(
  String::ReadIterator i, const String::ReadIterator& end) throw() {
  if (i >= end) {
    return false;
  }
  
  if (*i == '[') { // is IPv6 address
    ++i; // skip [
    const String::ReadIterator j = i;
    while ((i != end) && (*i != ']')) {
      ++i;
    }
    if ((i != end) && (*i != ']')) {
      return false;
    }
    ++i; // skip ]
    return (i == end) && InetAddress::isIPv6(j, end);
  } else if (ASCIITraits::isDigit(*i)) { // is IP address
    // "123.123.123.123" or "123.123.123" or "123.123" or "123"
    for (unsigned int digitGroup = 0; digitGroup < 4; ++digitGroup) {
      if (ASCIITraits::isDigit(*i)) { // first digit
        ++i;
        if ((i < end) && ASCIITraits::isDigit(*i)) { // second digit
          ++i;
          if ((i < end) && ASCIITraits::isDigit(*i)) { // third digit
            ++i;
          }
        }
      } else {
        return false; // invalid digit group
      }

      if (i == end) { // have we read all chars of address
        return true;
      }

      if (*i++ != '.') { // check for digit group separator
        return false;
      }
    }
    return false;
  } else if (ASCIITraits::isAlpha(*i)) { // is fully qualified domain name
    while (true) { // see RFC 1034
      // check label
      if ((i < end) && ASCIITraits::isAlpha(*i++)) { // check first char
        while ((i < end) && (ASCIITraits::isAlphaNum(*i) || (*i == '-'))) {++i;}
        if (*--i == '-') { // char last char
          return false;
        }
        ++i; // skip last char
      } else {
        return false; // invalid label
      }

      if (i == end) { // have we read all chars of domain name
        return true;
      }

      if (*i++ != '.') { // check for label separator
        return false;
      }
    }
  } else { // not a host
    return false;
  }
}

bool Url::isPort(String::ReadIterator i, const String::ReadIterator& end) throw() {
  if (i >= end) {
    return false;
  }

  for (unsigned int port = 0; (i < end) && ASCIITraits::isDigit(*i); ++i) {
    port = (*i - '0') + 10 * port;
    if (port >= (1 << 16)) { // max 16 bit
      return false;
    }
  }
  if (i != end) { // have we read all chars
    return false;
  }
  return true;
}

void Url::parse(const String& url, bool strict) throw(UrlException, MemoryException) {
  // example url: http://username:password@server:80/folder/

  int index = 0; // current position in the url
  int end = url.getLength();

  while ((index < end) && (url[index] == ' ')) { // ignore prefix spaces
    ++index;
  }

  while ((end > index) && (url[end - 1] == ' ')) { // ignore suffix spaces
    --end;
  }

  // read scheme if present
  {
    int colonIndex = url.indexOf("://", index);
    if (colonIndex >= 0) {
      setScheme(UrlImpl::decode(url.substring(index, colonIndex), UrlImpl::defaultEncoding, strict));
      index = colonIndex + 3;
    }
  }

  int slashIndex = url.indexOf('/', index); // find beginning of path

  // read user, and password if present
  {
    int atIndex = url.indexOf('@', index);
    if ((atIndex >= 0) && ((slashIndex < 0) || (atIndex < slashIndex))) {
      int colonIndex = url.indexOf(':', index);
      if ((colonIndex >= 0) && (colonIndex < atIndex)) {
        setUser(UrlImpl::decode(url.substring(index, colonIndex), UrlImpl::userEncoding, strict));
        setPassword(UrlImpl::decode(url.substring(colonIndex + 1, atIndex), UrlImpl::passwordEncoding, strict));
      } else {
        setUser(UrlImpl::decode(url.substring(index, atIndex), UrlImpl::userEncoding, strict));
      }
      index = atIndex + 1;
    }
  }

  // read host, and port if present
  {
    int endIndex = (slashIndex < 0) ? end : slashIndex;
    int colonIndex = url.indexOf(':', index);
    int endOfHost = ((colonIndex >= 0) && (colonIndex < endIndex)) ? colonIndex : endIndex;
    setHost(UrlImpl::decode(url.substring(index, endOfHost), UrlImpl::defaultEncoding, strict));
    if ((colonIndex >= 0) && (colonIndex < endIndex)) {
      setPort(UrlImpl::decode(url.substring(colonIndex + 1, endIndex), UrlImpl::defaultEncoding, strict));
    }
  }

  // read path
  if (slashIndex >= 0) {
    setPath(UrlImpl::decode(url.substring(slashIndex + 1, end), UrlImpl::defaultEncoding, strict)); // standard says throw away slash
  } else {
    setPath(String()); // no path
  }
}

String Url::getScheme() const throw() {
  return scheme;
}

String Url::getUser() const throw() {
  return user;
}

String Url::getPassword() const throw() {
  return password;
}

String Url::getHost() const throw() {
  return host;
}

String Url::getPort() const throw() {
  return port;
}

String Url::getPath() const throw(MemoryException) {
  return path;
}

String Url::getUrl() const throw(MemoryException) {
  String result;

  if (!scheme.isEmpty()) {
    result += scheme; // no need to encode
    result += Literal("://"); // scheme separator + indicate common Internet scheme syntax
  }

  if (!user.isEmpty() || !password.isEmpty()) { // has user or password - allows blank user
    result += UrlImpl::encode(user, UrlImpl::userEncoding);
    if (!password.isEmpty()) { // has password
      result += ':'; // indicate password
      result += UrlImpl::encode(password, UrlImpl::passwordEncoding);
    }
    result += '@'; // indicate user/password part
  }

  if (!host.isEmpty()) {
    result += host; // no need to encode

    if (!port.isEmpty()) {
      result += ':'; // indicate port
      result += port; // no need to encode
    }
  }

  if (!path.isEmpty()) {
    result += '/'; // indicate relative path
    result += UrlImpl::encode(path);
  }

  return result;
}

void Url::setScheme(const String& value) throw(UrlException, MemoryException) {
  scheme = validateScheme(value);
}

void Url::setUser(const String& value) throw(UrlException, MemoryException) {
  user = validateUser(value);
}

void Url::setPassword(const String& value) throw(UrlException, MemoryException) {
  password = validatePassword(value);
}

void Url::setHost(const String& value) throw(UrlException) {
  bassert(
    isHost(value.getBeginReadIterator(), value.getEndReadIterator()),
    UrlException("Invalid host part", this)
  );
  host = value;
}

void Url::setPort(const String& value) throw(UrlException) {
  bassert(
    isPort(value.getBeginReadIterator(), value.getEndReadIterator()),
    UrlException("Invalid port part", this)
  );
  port = value;
}

void Url::setPath(const String& value) throw(UrlException, MemoryException) {
  path = value;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Url& value) throw(MemoryException, IOException) {
  return stream << value.getUrl();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
