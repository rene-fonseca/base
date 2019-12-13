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

#include <base/Object.h>
#include <base/string/String.h>
#include <base/string/InvalidFormat.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This exception is thrown by the Uri class on violations of the RFC 2396
  standard.

  @short URL exception.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API UriException : public InvalidFormat {
public:

  /**
    Initializes the exception object with no message.
  */
  inline UriException() {}

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline UriException(const char* message) : InvalidFormat(message) {}
};



/**
  Implementation of Uniform Resource Identifier (URI) as specified by RFC 2396.
  A Uniform Resource Identifier (URI) is a compact string for identifying an
  abstract or physical resource.

  <pre>
  Uri uri("http://www.google.com")
  Uri uri("ftp://ftp.gnu.org")
  </pre>

  @short Uniform Resource Identifier (URI)
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Uri : public Object {
private:

  /** The protocol part. */
  String scheme;
  /** The user part. */
  String user;
  /** The password part. */
  String password;
  /** The host part. */
  String host;
  /** The port part. */
  String port;
  /** The path part. */
  String path;
  /** The search part. */
  String search;
protected:

  /**
    Validates the specified scheme and returns a slightly converted scheme to be used.
  */
  static String validateScheme(const String& value);

  /**
    Validates the specified user and returns it.
  */
  static String validateUser(const String& str);

  /**
    Validates the specified password and returns it.
  */
  static String validatePassword(const String& str);

  /**
    Returns true if the string is a valid host specification.
  */
  static bool isHost(String::ReadIterator i, const String::ReadIterator& end) noexcept;

  /**
    Returns true if the string is a valid port.
  */
  static bool isPort(String::ReadIterator i, const String::ReadIterator& end) noexcept;

  /**
    Parses the string as a uri.
  */
  void parse(const String& uri, bool strict);
public:

  /**
    Initializes an empty uri.
  */
  Uri();

  /**
    Initializes uri from a string representation.

    @param uri The string representation of the uri.
    @param strict Specifies that unsafe characters are required to be encoded. Default is true.
  */
  Uri(const String& uri, bool strict = true);

  /**
    Initializes uri from separate string representations of relative and base uri.

    @param relative The relative uri.
    @param base The base uri.
    @param strict Specifies that unsafe characters are required to be encoded. Default is true.
  */
  Uri(const String& relative, const String& base, bool strict = true);

  /**
    Initializes uri from other uri.
  */
  Uri(const Uri& copy) noexcept;

  /**
    Assignment of uri by uri.
  */
  Uri& operator=(const Uri& assign) noexcept;

  /**
    Returns true if the uri is relative.
  */
  bool isRelative() const noexcept;

  /**
    Returns the scheme (a.k.a. protocol).
  */
  String getScheme() const noexcept;

  /**
    Returns the user.
  */
  String getUser() const noexcept;

  /**
    Returns the password.
  */
  String getPassword() const noexcept;

  /**
    Returns the host.
  */
  String getHost() const noexcept;

  /**
    Returns the port.
  */
  String getPort() const noexcept;

  /**
    Returns the path.
  */
  virtual String getPath() const;

  /**
    Returns the search part of the identifier.
  */
  String getSearch() const noexcept;

  /**
    Returns the entire identifier.
  */
  String getUri() const;

  /**
    Sets the scheme (a.k.a. protocol). The scheme is composed of letters
    ('a'-'z'), digits ('0'-'9'), and the characters plus ('+'), period ('.'),
    and hyphen ('-'). Upper case letters are also allowed but are converted
    into lower case. The exception URLException is thrown if the set of allowed
    characters is violated.
  */
  virtual void setScheme(const String& value);

  /**
    Sets the user.
  */
  void setUser(const String& value);

  /**
    Sets the password.
  */
  void setPassword(const String& value);

  /**
    Sets the host.
  */
  void setHost(const String& value);

  /**
    Sets the port.
  */
  void setPort(const String& value);

  /**
    Sets the path.
  */
  virtual void setPath(const String& value);
};

/**
  Writes the specified uri to the format output stream.
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Uri& value);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
