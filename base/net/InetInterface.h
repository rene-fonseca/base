/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
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

  @ingroup net
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class InetInterface : public virtual Object {
private:

  /** Specifies the index of the network interface. */
  const unsigned int index;
  /** Specifies the name of the network interface. */
  const String name;
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
  String getName() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
