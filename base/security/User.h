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
#include <base/collection/Array.h>
#include <base/mem/ReferenceCountedAllocator.h>
#include <base/collection/Hash.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Group;

/**
  A user trustee.
  
  @short User trustee.
  @ingroup security
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.2
*/

class User : public Object {
  friend class Hash<User>;
  friend class Trustee;
private:
  
  /** Identifier of the user represented as an integral if possible. */
  unsigned long integralId;
  /** Opaque identifier of the user. */
  Reference<ReferenceCountedAllocator<uint8> > id;
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
    inline UserException() throw() {
    }

    /**
      Initializes the exception object.

      @param message The message.
    */
    inline UserException(const char* message) throw() : Exception(message) {
    }

    /**
      Initializes the exception object without an associated message.

      @param type The identity of the type.
    */
    inline UserException(Type type) throw() : Exception(type) {
    }

    /**
      Initializes the exception object.

      @param message An NULL-terminated string (ASCII).
      @param type The identity of the type.
    */
    inline UserException(const char* message, Type type) throw()
      : Exception(message, type) {
    }
  };
  
  /**
    Returns the user associated with the current process.
  */
  static User getCurrentUser() throw(UserException);
  
  /**
    Initializes user as invalid.
  */
  inline User() throw() : integralId(getMaximum(integralId)) {
  }

  /**
    Initializes user by id. Raises OutOfDomain if the specified id is not
    supported by the platform.
  */  
  User(unsigned long _id) throw(OutOfDomain);
  
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
  bool operator==(const User& eq) const throw();
  
  /**
    Returns false if the users are not identical. The method returns false if
    both users are invalid.
  */
  inline bool operator!=(const User& eq) const throw() {
    return !(*this == eq);
  }
  
  /**
    Returns true if the user is initialized. This does not mean that the user
    exists.
  */
  inline bool isValid() const throw() {
    return integralId != getMaximum(integralId);
  }
  
  /**
    Returns the id of the user.
  */
  inline const void* getId() const throw() {
    return id.isValid() ? id->getElements() : 0;
  }

  /**
    Returns the integral id of the user.
  */
  inline unsigned long getIntegralId() const throw() {
    return integralId;
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
    Returns the groups to which the user belongs.
  */
  Array<String> getGroups() throw(UserException);
};

/**
  Writes the user id to the format output stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const User& value) throw(IOException);

template<>
class Hash<User> {
public:
  
  unsigned long operator()(const User& value) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
