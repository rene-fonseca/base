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
  Implementation of Uniform Resource Locator (URL) as specified by RFC 1738 and
  2732. A compact string representation for a resource available via the
  Internet. This class is used to break down a url into its components. The
  components of the url are stored internally in decoded format.
  
  @code
  Url url("http://www.google.com")
  Url url("ftp://ftp.gnu.org")
  @endcode

  @short Uniform Resource Locator (URL)
  @ingroup net
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Url : public Object {
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
public:

  /** Returns true if character is reserved per RFC3986. */
  static bool isReserved(ucs4 ch);

  /** Returns true if character is unreserved per RFC3986. */
  static bool isUnreserved(ucs4 ch);

  /** Returns the encoded URI component. */
  static String encodeURIComponent(const String& component);
  
  /** Returns the decoded URI component. */
  static String decodeURIComponent(const String& component);

  /**
    This exception is raised by the Url class on violations of the RFC 1738
    standard.

    @short URL exception
    @ingroup exceptions
    @version 1.1
  */

  class UrlException : public InvalidFormat {
  public:
    
    /**
      Initializes the exception object with no message.
    */
    inline UrlException() noexcept
    {
    }

    /**
      Initializes the exception object.
      
      @param message The message.
    */
    inline UrlException(const char* message) noexcept
      : InvalidFormat(message)
    {
    }

    /**
      Initializes the exception object without an associated message.
      
      @param type The identity of the type.
    */
    inline UrlException(const Type& type) noexcept
      : InvalidFormat(type)
    {
    }

    /**
      Initializes the exception object.
      
      @param message An NULL-terminated string (ASCII).
      @param type The identity of the type.
    */
    inline UrlException(const char* message, const Type& type) noexcept
      : InvalidFormat(message, type)
    {
    }
  };
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
    Parses the string as a url.
  */
  void parse(const String& url, bool strict);
public:

  /**
    Initializes an empty url.
  */
  Url();

  /**
    Initializes url from a string representation.

    @param url The string representation of the url.
    @param strict Specifies that unsafe characters are required to be encoded. Default is true.
  */
  Url(const String& url, bool strict = true);

  /**
    Initializes url from separate string representations of relative and base url.

    @param relative The relative url.
    @param base The base url.
    @param strict Specifies that unsafe characters are required to be encoded. Default is true.
  */
  Url(const String& relative, const String& base, bool strict = true);

  /**
    Initializes url from other url.
  */
  Url(const Url& copy) noexcept;

  /**
    Assignment of url by url.
  */
  Url& operator=(const Url& assign) noexcept;

  /**
    Returns true if the url is relative.
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
    Returns the url.
  */
  String getUrl() const;

  /**
    Sets the scheme (a.k.a. protocol). The scheme is composed of letters
    ('a'-'z'), digits ('0'-'9'), and the characters plus ('+'), period ('.'),
    and hyphen ('-'). Upper case letters are also allowed but are converted
    into lower case. The exception URLException is raised if the set of allowed
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

  /**
    Destroys the url.
  */
  inline virtual ~Url() noexcept
  {
  }
};

/**
  Writes the specified url to the format output stream.
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Url& value);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
