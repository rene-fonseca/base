/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/net/Uri.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// global reserved chars: ; / ? : @ & = + $ , (must be encode if present in URI components)
// all global reserved chars may not be reserved for all components
// need option to escape additional characters
// escape sequences: char must be escaped if not unreserved - this includes non-printable chars and disallowed chars
// need construct from unescaped components: Uri(const String& a, const String& b, ...);
// enum {AUTHORITY = 1, PATH = 2, QUERY = 4, HIERARCHICAL = 8};
// select between server-based and registry-based authority
// Uri(const String& const, unsigned int accept = AUTHORITY|PATH|QUERY|HIERARCHICAL);
// <scheme>://<authority><path>?<query>
// scheme = alpha *(alpha | digit + - .)
// relative URI does not begin with a scheme - scheme is inherited from the base URI (section 5.2)
// authority = server | registrated_name
// authority: begins with "//" and end with "/" or "?" or end of URI
// authority: reserved chars ; : @ ? /
// server-based authority: userinfo@host:port

class UriImpl {
public:

  /** Returns true if the data character is not reserved within the URI. */
  static inline bool isUnreservedURI(char ch) throw() {
    if (ASCIITraits::isAlphaNum(ch)) {
      return true;
    }
    switch (ch) {
    case '-': case '_': case '.': case '!': case '~': case '*': case '\'': case '(': case ')':
      return true;
    }
    return false;
  }

  /** Returns true if the data character is excluded from the URI. */
  static inline bool isExcludedURI(char ch) throw() {
    if (ASCIITraits::isControl(ch)) { // 0x00-0x1f and 0x7f
      return true;
    }
    switch (ch) {
    case ' ': // SPC
    case '<': case '>': case '#': case '%': case '"': // delims
    case '{': case '}': case '|': case '\\': case '^': case '[': case ']': case '`': // unwise
      return true;
    }
    return false;
  }

  /** Returns true if the character is reserved within the authority component. */
  static inline bool isReservedAuthority(char ch) throw() {
    switch (ch) {
    case ';': case ':': case '@': case '?': case '/':
      return true;
    }
    return false;
  }

  /** Returns true if the character is reserved within the query component. */
  static inline bool isReservedPath(char ch) throw() {
    switch (ch) {
    case ';': case '/': case '?':
      return true;
    }
    return false;
  }

  /** Returns true if the character is reserved within the query component. */
  static inline bool isReservedQuery(char ch) throw() {
    switch (ch) {
    case ';': case ':': case '@': case '?': case '/': case '&': case '=': case '+': case ',': case '$':
      return true;
    }
    return false;
  }

  static inline bool isValidUserInfo(char ch) throw() {
    // userinfo: unreserved | escaped ; : & = + $ ,
    return false;
  }

  static inline bool isValidRegName(char ch) throw() {
    // unreserved | escaped $ , ; : @ & = +
    return false;
  }

  typedef enum {
    NEVER, // character is not required to be encoded
    RELAXED, // character must be encoded - however, decoding accepts unencoded character
    ALWAYS // character must always be encoded
  } Encode;

  typedef Encode (*Encoding)(String::Character);

  static inline Encode defaultEncoding(String::Character ch) throw() {
    switch (ch) {
    case 0x00: case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x06: case 0x07:
    case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e: case 0x0f:
    case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
    case 0x18: case 0x19: case 0x1a: case 0x1b: case 0x1c: case 0x1d: case 0x1e: case 0x1f:
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
    case '[':
    case ']':
    case '`':
      return RELAXED;
    default:
      return NEVER;
    }
  }

  static inline Encode userEncoding(String::Character ch) throw() {
    return ((ch == ':') || (ch == '@') || (ch == '/')) ? ALWAYS : defaultEncoding(ch);
  }

  static inline Encode passwordEncoding(String::Character ch) throw() {
    return ((ch == ':') || (ch == '@') || (ch == '/')) ? ALWAYS : defaultEncoding(ch);
  }

  static inline String encode(const String& str, Encoding encoding = defaultEncoding) throw(UriException, MemoryException) {
    String temp(str.getLength());
    const String::ReadIterator end = str.getEndReadIterator();
    for (String::ReadIterator i = str.getBeginReadIterator(); i < end;) {
      String::Character ch = *i++;
      assert(String::Traits::isASCII(ch), UriException("Invalid character"));
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

  static String decode(const String& str, Encoding encoding, bool strict) throw(UriException, MemoryException) {
    String temp(str.getLength());
    const String::ReadIterator end = str.getEndReadIterator();
    for (String::ReadIterator i = str.getBeginReadIterator(); i < end;) {
      String::Character ch = *i++;
      if (ch == '%') {
        assert(end - i >= 2, UriException("Invalid encoding")); // need two digits
        String::Character highDigit = *i++;
        String::Character lowDigit = *i++;
        assert(String::Traits::isDigit(highDigit) && String::Traits::isDigit(lowDigit), UriException("Invalid encoding")); // need two digits
        ch = static_cast<unsigned int>(highDigit - '0') << 4 + static_cast<unsigned int>(lowDigit - '0'); // replace with decoded char
      } else {
        Encode encode = encoding(ch);
        assert(strict ? (encode == NEVER) : (encode <= RELAXED), UriException("Part contains unencoded character"));
      }
      temp += ch;
    }
    return temp;
  }
};



Uri::Uri() throw(MemoryException) {
}

Uri::Uri(const String& uri, bool strict) throw(UriException, MemoryException) {
  parse(uri, strict);
}

Uri::Uri(const String& baseUri, const String& relativeUri, bool strict) throw(UriException, MemoryException) {
  Uri uri(relativeUri);
  if (uri.isRelative()) {
    parse(baseUri + relativeUri, strict);
  } else {
    *this = uri;
  }
}

Uri::Uri(const Uri& copy) throw() :
  scheme(copy.scheme),
  user(copy.user),
  password(copy.password),
  host(copy.host),
  port(copy.port),
  path(copy.path) {
}

Uri& Uri::operator=(const Uri& eq) throw() {
  scheme = eq.scheme;
  user = eq.user;
  password = eq.password;
  host = eq.host;
  port = eq.port;
  path = eq.path;
  return *this;
}

bool Uri::isRelative() const throw() {
  return !path.getLength() || path.startsWith("/");
}

String Uri::validateScheme(const String& value) throw(UriException, MemoryException) {
  String temp(value.getLength());
  String::ReadIterator end = value.getEndReadIterator();

  for (String::ReadIterator i = value.getBeginReadIterator(); i < end; ++i) {
    String::Character ch = *i;
    if ((ch >= 'a') && (ch <= 'z')) {
    } else if ((ch >= '0') && (ch <= '9')) {
    } else if (ch == '+') {
    } else if (ch == '.') {
    } else if (ch == '-') {
    } else if ((ch >= 'A') && (ch <= 'Z')) {
      ch += 'a' - 'A'; // convert to lower case (ASCII)
    } else {
      throw UriException("Invalid scheme");
    }
    temp += ch;
  }
  return temp;
}

String Uri::validateUser(const String& str) throw(UriException) {
  const String::ReadIterator end = str.getEndReadIterator();
  for (String::ReadIterator i = str.getBeginReadIterator(); i < end;) {
    String::Character ch = *i++;
    assert(String::Traits::isASCII(ch), UriException("Invalid character"));
  }
  return str;
}

String Uri::validatePassword(const String& str) throw(UriException) {
  const String::ReadIterator end = str.getEndReadIterator();
  for (String::ReadIterator i = str.getBeginReadIterator(); i < end;) {
    String::Character ch = *i++;
    assert(String::Traits::isASCII(ch), UriException("Invalid character"));
  }
  return str;
}

bool Uri::isHost(String::ReadIterator i, const String::ReadIterator& end) throw() {
  if (i >= end) {
    return false;
  }

  if (String::Traits::isDigit(*i)) { // is IP address
    // TAG: use InetAddress for IPv6 support
    // 0x1234.0x1234.0x1234.0x1234.0x1234.0x1234.0x1234.0x1234
    // 0x1234.0x1234.0x1234.0x1234.0x1234.0x1234.123.123.123.123
    // "123.123.123.123" or "123.123.123" or "123.123" or "123"
    for (unsigned int digitGroup = 0; digitGroup < 4; ++digitGroup) {
      if (String::Traits::isDigit(*i)) { // first digit
        ++i;
        if ((i < end) && String::Traits::isDigit(*i)) { // second digit
          ++i;
          if ((i < end) && String::Traits::isDigit(*i)) { // third digit
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
  } else if (String::Traits::isAlpha(*i)) { // is fully qualified domain name
    while (true) { // see RFC 1034
      // check label
      if ((i < end) && String::Traits::isAlpha(*i++)) { // check first char
        while ((i < end) && (String::Traits::isAlphaNum(*i) || (*i == '-'))) {++i;}
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

bool Uri::isPort(String::ReadIterator i, const String::ReadIterator& end) throw() {
  if (i >= end) {
    return false;
  }

  for (unsigned int port = 0; (i < end) && String::Traits::isDigit(*i); ++i) {
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

void Uri::parse(const String& uri, bool strict) throw(UriException, MemoryException) {
  // "scheme:path"
  // reserved characters: %, /, #, ?, *, !, ".", ".."
  // unsafe characters:
  // example uri: http://fonseca:password@www.mip.sdu.dk:80/~fonseca/base/
  // isAbsolute: return indexOf(':') < indexOf('/');

  int index = 0; // current position in the uri
  int end = uri.getLength();

  while ((index < end) && (uri[index] == ' ')) { // ignore prefix spaces
    ++index;
  }

  while ((end > index) && (uri[end - 1] == ' ')) { // ignore suffix spaces
    --end;
  }

  // read scheme if present
  {
    int colonIndex = uri.indexOf("://", index);
    if (colonIndex >= 0) {
      setScheme(UriImpl::decode(uri.substring(index, colonIndex), UriImpl::defaultEncoding, strict));
      index = colonIndex + 3;
    }
  }

  int slashIndex = uri.indexOf('/', index); // find beginning of path

  // read user, and password if present
  {
    int atIndex = uri.indexOf('@', index);
    if ((atIndex >= 0) && ((slashIndex < 0) || (atIndex < slashIndex))) {
      int colonIndex = uri.indexOf(':', index);
      if ((colonIndex >= 0) && (colonIndex < atIndex)) {
        setUser(UriImpl::decode(uri.substring(index, colonIndex), UriImpl::userEncoding, strict));
        setPassword(UriImpl::decode(uri.substring(colonIndex + 1, atIndex), UriImpl::passwordEncoding, strict));
      } else {
        setUser(UriImpl::decode(uri.substring(index, atIndex), UriImpl::userEncoding, strict));
      }
      index = atIndex + 1;
    }
  }

  // read host, and port if present
  {
    int endIndex = (slashIndex < 0) ? end : slashIndex;
    int colonIndex = uri.indexOf(':', index);
    int endOfHost = ((colonIndex >= 0) && (colonIndex < endIndex)) ? colonIndex : endIndex;
    setHost(UriImpl::decode(uri.substring(index, endOfHost), UriImpl::defaultEncoding, strict));
    if ((colonIndex >= 0) && (colonIndex < endIndex)) {
      setPort(UriImpl::decode(uri.substring(colonIndex + 1, endIndex), UriImpl::defaultEncoding, strict));
    }
  }

  // read path
  if (slashIndex >= 0) {
    setPath(UriImpl::decode(uri.substring(slashIndex + 1, end), UriImpl::defaultEncoding, strict)); // standard says throw away slash
  } else {
    setPath(String()); // no path
  }
}

String Uri::getScheme() const throw() {
  return scheme;
}

String Uri::getUser() const throw() {
  return user;
}

String Uri::getPassword() const throw() {
  return password;
}

String Uri::getHost() const throw() {
  return host;
}

String Uri::getPort() const throw() {
  return port;
}

String Uri::getPath() const throw(MemoryException) {
  return path;
}

String Uri::getUri() const throw(MemoryException) {
  String result;

  if (!scheme.isEmpty()) {
    result += scheme; // no need to encode
    result += "://"; // scheme separator + indicate common Internet scheme syntax
  }

  if (!user.isEmpty() || !password.isEmpty()) { // has user or password - allows blank user
    result += UriImpl::encode(user, UriImpl::userEncoding);
    if (!password.isEmpty()) { // has password
      result += ':'; // indicate password
      result += UriImpl::encode(password, UriImpl::passwordEncoding);
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
    result += UriImpl::encode(path);
  }

  return result;
}

void Uri::setScheme(const String& value) throw(UriException, MemoryException) {
  scheme = validateScheme(value);
}

void Uri::setUser(const String& value) throw(UriException, MemoryException) {
  user = validateUser(value);
}

void Uri::setPassword(const String& value) throw(UriException, MemoryException) {
  password = validatePassword(value);
}

void Uri::setHost(const String& value) throw(UriException) {
  assert(
    isHost(value.getBeginReadIterator(), value.getEndReadIterator()),
    UriException("Invalid host part")
  );
  host = value;
}

void Uri::setPort(const String& value) throw(UriException) {
  assert(
    isPort(value.getBeginReadIterator(), value.getEndReadIterator()),
    UriException("Invalid port part")
  );
  port = value;
}

void Uri::setPath(const String& value) throw(UriException, MemoryException) {
  path = value;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Uri& value) throw(MemoryException, IOException) {
  return stream << value.getUri();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
