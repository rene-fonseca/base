/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__NET_URL_H
#define _DK_SDU_MIP__BASE__NET_URL_H

#include <base/Object.h>
#include <base/string/String.h>
#include <base/string/InvalidFormat.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception is thrown by the Url class on violations of the RFC 1738
  standard.

  @short URL exception.
  @author René Møller Fonseca
  @version 1.0
*/

class UrlException : public InvalidFormat {
public:

  /**
    Initializes the exception object with no message.
  */
  inline UrlException() {}

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline UrlException(const char* message) : InvalidFormat(message) {}
};



/**
  Implementation of Uniform Resource Locator (URL) as specified by RFC 1738.
  A compact string representation for a resource available via the Internet.
  This class is used to break down a url into its components. The components
  of the url are stored internally in decoded format.

  <pre>
  Url url("http://www.mip.sdu.dk")
  Url url("ftp://ftp.gnu.org")
  </pre>

  @short Uniform Resource Locator (URL)
  @author René Møller Fonseca
  @version 1.0
*/

class Url : public Object {
private:

  /** The protocol. */
  String scheme;
  /** The user. */
  String user;
  /** The password. */
  String password;
  /** The host. */
  String host;
  /** The port. */
  String port;
  /** The path. */
  String path;
protected:

  /**
    Validates the specified scheme and returns a slightly converted scheme to be used.
  */
  static String validateScheme(const String& value) throw(UrlException, MemoryException);

  /**
    Validates the specified user and returns it.
  */
  static String validateUser(const String& str) throw(UrlException);

  /**
    Validates the specified password and returns it.
  */
  static String validatePassword(const String& str) throw(UrlException);

  /**
    Returns true if the string is a valid host specification.
  */
  static bool isHost(String::ReadIterator i, const String::ReadIterator& end) throw();

  /**
    Returns true if the string is a valid port.
  */
  static bool isPort(String::ReadIterator i, const String::ReadIterator& end) throw();

  /**
    Parses the string as a url.
  */
  void parse(const String& url, bool strict) throw(UrlException, MemoryException);
public:

  /**
    Initializes an empty url.
  */
  Url() throw(MemoryException);

  /**
    Initializes url from a string representation.

    @param url The string representation of the url.
    @param strict Specifies that unsafe characters are required to be encoded. Default is true.
  */
  Url(const String& url, bool strict = true) throw(UrlException, MemoryException);

  /**
    Initializes url from separate string representations of relative and base url.

    @param relative The relative url.
    @param base The base url.
    @param strict Specifies that unsafe characters are required to be encoded. Default is true.
  */
  Url(const String& relative, const String& base, bool strict = true) throw(UrlException, MemoryException);

  /**
    Initializes url from other url.
  */
  Url(const Url& copy) throw();

  /**
    Assignment of url by url.
  */
  Url& operator=(const Url& eq) throw();

  /**
    Returns true if the url is relative.
  */
  bool isRelative() const throw();

  /**
    Returns the scheme (a.k.a. protocol).
  */
  String getScheme() const throw();

  /**
    Returns the user.
  */
  String getUser() const throw();

  /**
    Returns the password.
  */
  String getPassword() const throw();

  /**
    Returns the host.
  */
  String getHost() const throw();

  /**
    Returns the port.
  */
  String getPort() const throw();

  /**
    Returns the path.
  */
  virtual String getPath() const throw(MemoryException);

  /**
    Returns the url.
  */
  String getUrl() const throw(MemoryException);

  /**
    Sets the scheme (a.k.a. protocol). The scheme is composed of letters
    ('a'-'z'), digits ('0'-'9'), and the characters plus ('+'), period ('.'),
    and hyphen ('-'). Upper case letters are also allowed but are converted
    into lower case. The exception URLException is thrown if the set of allowed
    characters is violated.
  */
  virtual void setScheme(const String& value) throw(UrlException, MemoryException);

  /**
    Sets the user.
  */
  void setUser(const String& value) throw(UrlException, MemoryException);

  /**
    Sets the password.
  */
  void setPassword(const String& value) throw(UrlException, MemoryException);

  /**
    Sets the host.
  */
  void setHost(const String& value) throw(UrlException);

  /**
    Sets the port.
  */
  void setPort(const String& value) throw(UrlException);

  /**
    Sets the path.
  */
  virtual void setPath(const String& value) throw(UrlException, MemoryException);
};

/**
  Writes the specified url to the format output stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Url& value) throw(MemoryException, IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
