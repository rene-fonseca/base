/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__INET_INTERFACE_H
#define _DK_SDU_MIP__BASE_NET__INET_INTERFACE_H

#include <base/Object.h>
#include <base/string/String.h>
#include <base/collection/List.h>
#include <base/net/NetworkException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class represents an Internet Protocol network interface.

  @author René Møller Fonseca
  @version 1.0
*/

class InetInterface : public Object {
private:

  /** Specifies the index of the network interface. */
  const unsigned int index;
  /** Specifies the name of the network interface. */
  const String<> name;
public:

  /**
    Returns all of the Internet Protocol network interfaces of the host.
  */
  static List<InetInterface> getInetInterfaces() throw(NetworkException);

  /**
    Initializes the network interface .

    @param index The index of the network interface.
    @param name The name of the network interface.
  */
  InetInterface(unsigned int index, const char* name);

  /**
    Copy constructor.
  */
  InetInterface(const InetInterface& copy) throw();

  /**
    Returns the index of the network interface.
  */
  unsigned int getIndex() const throw();

  /**
    Returns the name of the network interface.
  */
  String<> getName() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
