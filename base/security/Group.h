/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_SECURITY__GROUP_H
#define _DK_SDU_MIP__BASE_SECURITY__GROUP_H

#include <base/security/User.h>
#include <base/string/String.h>
#include <base/OutOfDomain.h>
#include <base/collection/Array.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Group.

  @ingroup security
  @short Group
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/

class Group : public Object {
private:

  /** The identifier of the group. */
  unsigned long long id;
public:

  /**
    This exception is raised by the Group class.

    @short Group exception
    @ingroup exceptions
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */

  class GroupException : public Exception {
  public:

    /**
      Initializes the exception object with no message.
    */
    inline GroupException() throw() {}

    /**
      Initializes the exception object.

      @param message The message.
    */
    inline GroupException(const char* message) throw() : Exception(message) {}

    /**
      Initializes the exception object without an associated message.

      @param type The identity of the type.
    */
    inline GroupException(Type type) throw() : Exception(type) {}

    /**
      Initializes the exception object.

      @param message An NULL-terminated string (ASCII).
      @param type The identity of the type.
    */
    inline GroupException(const char* message, Type type) throw() : Exception(message, type) {}
  };

  /**
    Initializes group with id 0.
  */
  inline Group() throw() : id(0) {}

  /**
    Initializes the group by id.

    @param id The identifier of the group.
  */
  Group(unsigned long long) throw(OutOfDomain);

  /**
    Initializes the group by name.

    @param name The name of the group.
  */
  Group(const String& name) throw(GroupException);

  /**
    Initializes group as the primary group of the specified user.
  */
  Group(const User& user) throw(GroupException);

  /**
    Initializaes group from other group.
  */
  Group(const Group& copy) throw();

  /**
    Returns the id of the group.
  */
  inline unsigned long long getId() const throw() {
    return id;
  }
  
  /**
    Returns the name of the group.
  */
  String getName() const throw(GroupException);

  /**
    Returns the members of the group.
  */
  Array<String> getMembers() const throw(GroupException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
