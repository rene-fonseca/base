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
  Internet url.

  @author René Møller Fonseca
  @version 1.0
*/

class Url : public Object {
private:

  String<> protocol;
  String<> user;
  String<> password;
  String<> host;
  String<> path;
  String<> port;
protected:

  void parse(String<> url) throw();
public:

  Url() throw(MemoryException);
  Url(String<> url) throw(MemoryException);
  Url(String<> url, String<> root) throw(MemoryException);
  Url(const Url& copy) throw();
  Url& operator=(const Url& eq) throw();

  bool isRelative() const throw();

  String<> getProtocol() const throw();
  String<> getUser() const throw();
  String<> getPassword() const throw();
  String<> getHost() const throw();
  String<> getPort() const throw();
  String<> getPath() const throw();

  String<> getUrl() const throw(MemoryException);

  void setProtocol(String<> value) throw();
  void setUser(String<> value) throw();
  void setPassword(String<> value) throw();
  void setHost(String<> value) throw();
  void setPort(String<> value) throw();
  void setPath(String<> value) throw();
};

FormatOutputStream& operator<<(FormatOutputStream& stream, const Url& value) throw(MemoryException, IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
