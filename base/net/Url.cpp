/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/net/Url.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class UrlImpl {
public:

  typedef bool (*Encoding)(String::Character);

  static inline bool defaultEncoding(String::Character ch) throw() {
    if (ch <= 0x1f) {
      return true;
    }
    switch (ch) {
    case ' ':
    case '<':
    case '>':
    case '"':
    case '#':
    case '%':
    case '{':
    case '}':
    case '|':
    case '\\':
    case '^':
    case '~':
    case '[':
    case ']':
    case '`':
    case 0x7f:
      return true;
    }
    return false;
  }

  static inline bool userEncoding(String::Character ch) throw() {
    return defaultEncoding(ch) || (ch == ':') || (ch == '@') || (ch == '/');
  }

  static inline bool passwordEncoding(String::Character ch) throw() {
    return defaultEncoding(ch) || (ch == ':') || (ch == '@') || (ch == '/');
  }

  static inline String encode(const String& str, Encoding encoding = defaultEncoding) throw(UrlException, MemoryException) {
    static char digits[] = "0123456789abcdef";
    String temp(str.getLength());
    const String::ReadIterator end = str.getEndReadIterator();
    for (String::ReadIterator i = str.getBeginReadIterator(); i < end;) {
      String::Character ch = *i++;
      assert(String::Traits::isASCII(ch), UrlException("Invalid character"));
      if (encoding(ch)) {
        temp += '%';
        temp += digits[ch >> 4];
        temp += digits[ch & 0x0f];
      } else {
        temp += ch;
      }
    }
    return temp;
  }

  static String decode(const String& str, Encoding encoding = defaultEncoding) throw(UrlException, MemoryException) {
    String temp(str.getLength());
    const String::ReadIterator end = str.getEndReadIterator();
    for (String::ReadIterator i = str.getBeginReadIterator(); i < end;) {
      String::Character ch = *i++;
      if (ch == '%') {
        assert(end - i >= 2, UrlException("Invalid encoding")); // need two digits
        String::Character highDigit = *i++;
        String::Character lowDigit = *i++;
        assert(String::Traits::isDigit(highDigit) && String::Traits::isDigit(lowDigit), UrlException("Invalid encoding")); // need two digits
        ch = static_cast<unsigned int>(highDigit - '0') << 4 + static_cast<unsigned int>(lowDigit - '0'); // replace with decoded char
      } else {
        assert(encoding(ch) == false, UrlException("Part contains unencoded character"));
      }
      temp += ch;
    }
    return temp;
  }

//  static String validateEncoded(const String& str, Encoding encoding = defaultEncoding) throw(UrlException, MemoryException) {
//    const String::ReadIterator end = str.getEndReadIterator();
//    for (String::ReadIterator i = str.getBeginReadIterator(); i < end;) {
//      String::Character ch = *i++;
//      assert(!encoding(ch), UrlException("Part contains unencoded character"));
//      if (ch == '%') {
//        assert(end - i >= 2, UrlException("Invalid encoding")); // need two digits
//        String::Character highDigit = *i++;
//        String::Character lowDigit = *i++;
//        assert(String::Traits::isDigit(highDigit) && String::Traits::isDigit(lowDigit), UrlException("Invalid encoding")); // need two digits
//        ch = static_cast<unsigned int>(highDigit - '0') << 4 + static_cast<unsigned int>(lowDigit - '0'); // replace with decoded char
//      }
//    }
//    return str;
//  }
};



//class FTPUrl : public Object {
//private:
//
//  static const String scheme;
//  static const unsigned short DEFAULT_PORT = 21;
//  //static const String DEFAULT_USER;
//
//  String path;
//  char type;
//public:
//
//  String getScheme() const throw() {return scheme;}
//  unsigned short getDefaultPort() const throw() {return DEFAULT_PORT;}
//
//  String getPath() const throw() {
//    return path;
//  }
//
//  char getType() const throw() {
//    return type;
//  }
//
//  void setPath(const String& path) const throw(UrlException) {
//    int semicolonIndex = path.indexOf(';');
//    if (semicolonIndex >= 0) {
//      assert(path.indexOf("type=", semicolonIndex + 1) == semicolonIndex + 1, UrlException());
//      int index = semicolonIndex + 5;
//    }
//    this->path = path;
//  }
//};
//
//const String FTPUrl::scheme = "ftp";
////const String FTPUrl::DEFAULT_USER = "anonymous";



class HTTPUrl : public Object {
private:

  static const String scheme;
  static const unsigned short DEFAULT_PORT = 80;
public:

  String getScheme() const throw();
  unsigned short getDefaultPort() const throw() {return DEFAULT_PORT;}

  bool isPath(const String& path) const throw() {
  }
};

const String HTTPUrl::scheme = "http";


Url::Url() throw(MemoryException) {
}

Url::Url(const String& url) throw(UrlException, MemoryException) {
  parse(url);
}

Url::Url(const String& baseUrl, const String& relativeUrl) throw(UrlException, MemoryException) {
  Url url(relativeUrl);
  if (url.isRelative()) {
    parse(baseUrl + relativeUrl);
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
  return !path.getLength() || path.startsWith("/");
}

String Url::validateScheme(const String& value) throw(UrlException, MemoryException) {
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
      throw UrlException("Invalid scheme");
    }
    temp += ch;
  }
  return temp;
}

String Url::validateUser(const String& str) throw(UrlException) {
  const String::ReadIterator end = str.getEndReadIterator();
  for (String::ReadIterator i = str.getBeginReadIterator(); i < end;) {
    String::Character ch = *i++;
    assert(String::Traits::isASCII(ch), UrlException("Invalid character"));
  }
  return str;
}

String Url::validatePassword(const String& str) throw(UrlException) {
  const String::ReadIterator end = str.getEndReadIterator();
  for (String::ReadIterator i = str.getBeginReadIterator(); i < end;) {
    String::Character ch = *i++;
    assert(String::Traits::isASCII(ch), UrlException("Invalid character"));
  }
  return str;
}

bool Url::isHost(String::ReadIterator i, const String::ReadIterator& end) throw() {
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

bool Url::isPort(String::ReadIterator i, const String::ReadIterator& end) throw() {
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

void Url::parse(const String& url) throw(UrlException, MemoryException) {
  // example url: http://fonseca:password@www.mip.sdu.dk:80/~fonseca/base/

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
      setScheme(UrlImpl::decode(url.substring(index, colonIndex)));
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
        setUser(UrlImpl::decode(url.substring(index, colonIndex), UrlImpl::userEncoding));
        setPassword(UrlImpl::decode(url.substring(colonIndex + 1, atIndex), UrlImpl::passwordEncoding));
      } else {
        setUser(UrlImpl::decode(url.substring(index, atIndex), UrlImpl::userEncoding));
      }
      index = atIndex + 1;
    }
  }

  // read host, and port if present
  {
    int endIndex = (slashIndex < 0) ? end : slashIndex;
    int colonIndex = url.indexOf(':', index);
    int endOfHost = ((colonIndex >= 0) && (colonIndex < endIndex)) ? colonIndex : endIndex;
    setHost(UrlImpl::decode(url.substring(index, endOfHost)));
    if ((colonIndex >= 0) && (colonIndex < endIndex)) {
      setPort(UrlImpl::decode(url.substring(colonIndex + 1, endIndex)));
    }
  }

  // read path
  if (slashIndex >= 0) {
    setPath(UrlImpl::decode(url.substring(slashIndex + 1, end))); // standard says throw away slash
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
    result += "://"; // scheme separator + indicate common Internet scheme syntax
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
  assert(
    isHost(value.getBeginReadIterator(), value.getEndReadIterator()),
    UrlException("Invalid host part")
  );
  host = value;
}

void Url::setPort(const String& value) throw(UrlException) {
  assert(
    isPort(value.getBeginReadIterator(), value.getEndReadIterator()),
    UrlException("Invalid port part")
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
