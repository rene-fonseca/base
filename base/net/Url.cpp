/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/net/Url.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Url::Url() throw(MemoryException) {
}

Url::Url(const String& url) throw(MemoryException) {
  parse(url);
}

Url::Url(const String& baseUrl, const String& relativeUrl) throw(MemoryException) {
  Url url(relativeUrl);
  if (url.isRelative()) {
    parse(baseUrl + relativeUrl);
  } else {
    *this = url;
  }
}

Url::Url(const Url& copy) throw() :
  protocol(copy.protocol),
  user(copy.user),
  password(copy.password),
  host(copy.host),
  port(copy.port),
  path(copy.path) {
}

Url& Url::operator=(const Url& eq) throw() {
  protocol = eq.protocol;
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

void Url::parse(const String& url) throw() {
  // example url: http://fonseca:password@www.mip.sdu.dk:80/~fonseca/base/

  int index = 0; // current position in the url
  int last = url.getLength() - 1;

  while ((index < last) && (url[index] == ' ')) { // ignore prefix spaces
    ++index;
  }

  while ((last > index) && (url[index] == ' ')) { // ignore suffix spaces
    --last;
  }

  // read protocol if present
  {
    int colonIndex = url.indexOf("://", index);
    if (colonIndex >= 0) {
      protocol = url.substring(index, colonIndex - 1);
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
        user = url.substring(index, colonIndex - 1);
        password = url.substring(colonIndex + 1, atIndex - 1);
      } else {
        user = url.substring(index, atIndex - 1);
      }
      index = atIndex + 1;
    }
  }

  // read host, and port if present
  {
    int colonIndex = url.indexOf(':', index);
    if (slashIndex < 0) {
      slashIndex = last;
    }
    if ((colonIndex >= 0) && (colonIndex < slashIndex)) {
      host = url.substring(index, colonIndex - 1);
      port = url.substring(colonIndex + 1, slashIndex - 1);
    } else {
      host = url.substring(index, slashIndex - 1);
    }
  }

  // read path
  path = url.substring(slashIndex, last); // should we keep the slash - I keep it for now
}

String Url::getProtocol() const throw() {
  return protocol;
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

String Url::getPath() const throw() {
  return path;
}

String Url::getUrl() const throw(MemoryException) {
  String result;

  if (protocol.getLength()) {
    result.append(protocol);
    result.append("://");
  }

  if (user.getLength()) {
    result.append(user);
    if (password.getLength()) {
      result.append(":");
      result.append(password);
    }
    result.append("@");
  }

  if (host.getLength()) {
    result.append(host);

    if (port.getLength()) {
      result.append(":");
      result.append(port);
    }
  }

  result.append(path);

  return result;
}

void Url::setProtocol(const String& value) throw() {
  protocol = value;
}

void Url::setUser(const String& value) throw() {
  user = value;
}

void Url::setPassword(const String& value) throw() {
  password = value;
}

void Url::setHost(const String& value) throw() {
  host = value;
}

void Url::setPort(const String& value) throw() {
  port = value;
}

void Url::setPath(const String& value) throw() {
  path = value;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Url& value) throw(MemoryException, IOException) {
  return stream << value.getUrl();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
