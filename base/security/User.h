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
  @version 1.1
*/

class User : public Object {
private:
  
  /** Opaque identifier of the user. */
  void* id;
public:
  
  /** The valid of an invalid identifier. */
  static const void* const INVALID = static_cast<char*>(0) - 1;

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
  static User getCurrentUser() throw(UserException);
  
  /**
    Initializes user as invalid.
  */
  inline User() throw() : id(static_cast<char*>(0) - 1) {}
  
  /**
    Initializes user by id. Raises OutOfDomain if the specified id is not
    supported by the platform.
  */
  User(const void* id) throw(OutOfDomain);
  
  /**
    Initializes user from other user.
  */
  User(const User& copy) throw();
  
  /**
    Assignment of user by user.
  */
  User& operator=(const User& eq) throw();
  
  /**
    Initializes user by name.
  */
  User(const String& name) throw(UserException);
  
  /**
    Returns true if the users are identical. The method returns true if both
    users are invalid.
  */
  bool operator==(const User& eq) throw();
  
  /**
    Returns false if the users are not identical. The method returns false if
    both users are invalid.
  */
  inline bool operator!=(const User& eq) throw() {
    return !(*this == eq);
  }
  
  /**
    Returns true if the user is initialized. This does not mean that the user
    exists.
  */
  inline bool isValid() const throw() {
    return id != INVALID;
  }
  
  /**
    Returns the id of the user.
  */
  inline const void* getId() const throw() {
    return id;
  }
  
  /**
    Returns the short name of the account.

    @param fallback Specifies, when true, that a string representation of the
    id should be returned if the name cannot be resolved. Fall-back is enabled
    by default.
  */
  String getName(bool fallback = true) const throw(UserException);
  
  /**
    Returns the full name of the account.
  */
  String getFullName() const throw(UserException);
  
  /**
    Returns the path of the home folder.
  */
  String getHomeFolder() const throw(UserException);
  
  /**
    Returns true if the user is an administrator/root.
  */
  bool isAdmin() const throw(UserException);
  
  /**
    Returns true if the user is a member of the specified group.

    @return false if the group doesn't exist.
  */
  bool isMemberOf(const Group& group) throw(UserException);
  
  /**
    Destroys the user object.
  */
  ~User() throw();
};

/**
  Writes the user id to the format output stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const User& value) throw(IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
