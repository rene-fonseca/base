/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__NET_URL_H
#define _DK_SDU_MIP__BASE__NET_URL_H

#include <base/Object.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Internet url. This class is used to break down a url into its components.

  @author René Møller Fonseca
  @version 1.0
*/

class Url : public Object {
private:

  /** The protocol. */
  String protocol;
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
    Parses the string as a url.
  */
  void parse(const String& url) throw();
public:

  /**
    Initializes an empty url.
  */
  Url() throw(MemoryException);

  /**
    Initializes url from a string representation.

    @param url The string representation of the url.
  */
  Url(const String& url) throw(MemoryException);

  /**
    Initializes url from separate string representations of relative and base url.

    @param relative The relative url.
    @param base The base url.
  */
  Url(const String& relative, const String& base) throw(MemoryException);

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
    Returns the protocol.
  */
  String getProtocol() const throw();

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
  String getPath() const throw();

  /**
    Returns the url.
  */
  String getUrl() const throw(MemoryException);

  /**
    Sets the protocol.
  */
  void setProtocol(const String& value) throw();

  /**
    Sets the user.
  */
  void setUser(const String& value) throw();

  /**
    Sets the password.
  */
  void setPassword(const String& value) throw();

  /**
    Sets the host.
  */
  void setHost(const String& value) throw();

  /**
    Sets the port.
  */
  void setPort(const String& value) throw();

  /**
    Sets the path.
  */
  void setPath(const String& value) throw();
};

/**
  Writes the specified url to the format output stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Url& value) throw(MemoryException, IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
