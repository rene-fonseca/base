/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_SECURITY__USER_H
#define _DK_SDU_MIP__BASE_SECURITY__USER_H

#include <base/security/User.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Group;

/**
  User.

  @short User
  @ingroup security
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class User : public Object {
private:

  /** The identifier of the user. */
  unsigned long long id;
public:

  /**
    This exception is raised by the User class to indicate a non-existent user.

    @short User exception
    @ingroup exceptions security
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.1
  */

  class UserException : public Exception {
  public:

    /**
      Initializes the exception object with no message.
    */
    inline UserException() throw() {}

    /**
      Initializes the exception object.

      @param message The message.
    */
    inline UserException(const char* message) throw() : Exception(message) {}

    /**
      Initializes the exception object without an associated message.

      @param type The identity of the type.
    */
    inline UserException(Type type) throw() : Exception(type) {}

    /**
      Initializes the exception object.

      @param message An NULL-terminated string (ASCII).
      @param type The identity of the type.
    */
    inline UserException(const char* message, Type type) throw() : Exception(message, type) {}
  };

  /**
    Returns the user associated with the current process.
  */
  static User getCurrentUser() throw();

  /**
    Initializes user with id 0.
  */
  inline User() throw() : id(0) {}

  /**
    Initializes user by id. Raises 
  */
  User(unsigned long long id) throw(OutOfDomain);

  /**
    Initializes user from other user.
  */
  User(const User& copy) throw();

  /**
    Initializes user by name.
  */
  User(const String& name) throw(UserException);

  /**
    Returns the id of the user.
  */
  inline unsigned long long getId() const throw() {
    return id;
  }

  /**
    Returns the short name of the account.
  */
  String getName() const throw(UserException);

  /**
    Returns the full name of the account.
  */
  String getFullName() const throw(UserException);

  /**
    Returns the path of the home folder.
  */
  String getHomeFolder() const throw(UserException);

  /**
    Returns true if the user is a member of the specified group.

    @return false if the group doesn't exist.
  */
  bool isMemberOf(const Group& group) throw(UserException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
