/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/string/String.h>
#include <base/string/InvalidFormat.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception is thrown by the Uri class on violations of the RFC 2396
  standard.

  @short URL exception.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class UriException : public InvalidFormat {
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
  Uri uri("http://www.mip.sdu.dk")
  Uri uri("ftp://ftp.gnu.org")
  </pre>

  @short Uniform Resource Identifier (URI)
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Uri : public Object {
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
  static String validateScheme(const String& value) throw(UriException, MemoryException);

  /**
    Validates the specified user and returns it.
  */
  static String validateUser(const String& str) throw(UriException);

  /**
    Validates the specified password and returns it.
  */
  static String validatePassword(const String& str) throw(UriException);

  /**
    Returns true if the string is a valid host specification.
  */
  static bool isHost(String::ReadIterator i, const String::ReadIterator& end) throw();

  /**
    Returns true if the string is a valid port.
  */
  static bool isPort(String::ReadIterator i, const String::ReadIterator& end) throw();

  /**
    Parses the string as a uri.
  */
  void parse(const String& uri, bool strict) throw(UriException, MemoryException);
public:

  /**
    Initializes an empty uri.
  */
  Uri() throw(MemoryException);

  /**
    Initializes uri from a string representation.

    @param uri The string representation of the uri.
    @param strict Specifies that unsafe characters are required to be encoded. Default is true.
  */
  Uri(const String& uri, bool strict = true) throw(UriException, MemoryException);

  /**
    Initializes uri from separate string representations of relative and base uri.

    @param relative The relative uri.
    @param base The base uri.
    @param strict Specifies that unsafe characters are required to be encoded. Default is true.
  */
  Uri(const String& relative, const String& base, bool strict = true) throw(UriException, MemoryException);

  /**
    Initializes uri from other uri.
  */
  Uri(const Uri& copy) throw();

  /**
    Assignment of uri by uri.
  */
  Uri& operator=(const Uri& eq) throw();

  /**
    Returns true if the uri is relative.
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
    Returns the search part of the identifier.
  */
  String getSearch() const throw();

  /**
    Returns the entire identifier.
  */
  String getUri() const throw(MemoryException);

  /**
    Sets the scheme (a.k.a. protocol). The scheme is composed of letters
    ('a'-'z'), digits ('0'-'9'), and the characters plus ('+'), period ('.'),
    and hyphen ('-'). Upper case letters are also allowed but are converted
    into lower case. The exception URLException is thrown if the set of allowed
    characters is violated.
  */
  virtual void setScheme(const String& value) throw(UriException, MemoryException);

  /**
    Sets the user.
  */
  void setUser(const String& value) throw(UriException, MemoryException);

  /**
    Sets the password.
  */
  void setPassword(const String& value) throw(UriException, MemoryException);

  /**
    Sets the host.
  */
  void setHost(const String& value) throw(UriException);

  /**
    Sets the port.
  */
  void setPort(const String& value) throw(UriException);

  /**
    Sets the path.
  */
  virtual void setPath(const String& value) throw(UriException, MemoryException);
};

/**
  Writes the specified uri to the format output stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Uri& value) throw(MemoryException, IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE