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

#include <base/security/User.h>
#include <base/string/String.h>
#include <base/collection/Array.h>
#include <base/mem/ReferenceCountedAllocator.h>
#include <base/collection/Hash.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class Group;

/**
  A user trustee.
  
  @short User trustee.
  @ingroup security
  @version 1.2
*/

class _COM_AZURE_DEV__BASE__API User : public Object {
  friend class Hash<User>;
  friend class Trustee;
private:
  
  static constexpr unsigned long INVALID = PrimitiveTraits<unsigned long>::MAXIMUM;
  /** Identifier of the user represented as an integral if possible. */
  unsigned long integralId = INVALID;
  /** Opaque identifier of the user. */
  Reference<ReferenceCountedAllocator<uint8> > id;
public:
  
  /**
    This exception is raised by the User class to indicate a non-existent user.

    @short User exception
    @ingroup exceptions security
    @version 1.1
  */

  class _COM_AZURE_DEV__BASE__API UserException : public Exception {
  public:

    /**
      Initializes the exception object with no message.
    */
    inline UserException() noexcept {
    }

    /**
      Initializes the exception object.

      @param message The message.
    */
    inline UserException(const char* message) noexcept : Exception(message) {
    }

    /**
      Initializes the exception object without an associated message.

      @param type The identity of the type.
    */
    inline UserException(const Type& type) noexcept : Exception(type) {
    }

    /**
      Initializes the exception object.

      @param message An NULL-terminated string (ASCII).
      @param type The identity of the type.
    */
    inline UserException(const char* message, const Type& type) noexcept
      : Exception(message, type) {
    }
    
    _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
  };
  
  /**
    Returns the user associated with the current process.
  */
  static User getCurrentUser();
  
  /**
    Initializes user as invalid.
  */
  inline User() noexcept {
  }

  /**
    Initializes user by id. Raises OutOfDomain if the specified id is not
    supported by the platform.
  */  
  User(unsigned long _id);
  
  /**
    Initializes user by id. Raises OutOfDomain if the specified id is not
    supported by the platform.
  */
  User(const void* id);
  
  /**
    Initializes user from other user.
  */
  User(const User& copy) noexcept;
  
  /**
    Assignment of user by user.
  */
  User& operator=(const User& assign) noexcept;
  
  /**
    Initializes user by name.
  */
  User(const String& name);
  
  /**
    Returns true if the users are identical. The method returns true if both
    users are invalid.
  */
  bool operator==(const User& compare) const noexcept;
  
  /**
    Returns false if the users are not identical. The method returns false if
    both users are invalid.
  */
  inline bool operator!=(const User& compare) const noexcept
  {
    return !(*this == compare);
  }
  
  /**
    Returns true if the user is initialized. This does not mean that the user
    exists.
  */
  inline bool isValid() const noexcept
  {
    return integralId != INVALID;
  }
  
  /**
    Returns the id of the user.
  */
  inline const void* getId() const noexcept
  {
    return id.isValid() ? id->getElements() : nullptr;
  }

  /**
    Returns the integral id of the user.
  */
  inline unsigned long getIntegralId() const noexcept
  {
    return integralId;
  }
  
  /**
    Returns the short name of the account.

    @param fallback Specifies, when true, that a string representation of the
    id should be returned if the name cannot be resolved. Fall-back is enabled
    by default.
  */
  String getName(bool fallback = true) const;
  
  /**
    Returns the full name of the account.
  */
  String getFullName() const;
  
  /**
    Returns the path of the home folder.
  */
  String getHomeFolder() const;
  
  /**
    Returns true if the user is an administrator/root.
  */
  bool isAdmin() const;
  
  /**
    Returns true if the user is a member of the specified group.

    @return false if the group doesn't exist.
  */
  bool isMemberOf(const Group& group);

  /**
    Returns the groups to which the user belongs.
  */
  Array<String> getGroups();
};

/**
  Writes the user id to the format output stream.
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const User& value);

template<>
class _COM_AZURE_DEV__BASE__API Hash<User> {
public:
  
  unsigned long operator()(const User& value) noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
