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
  A group trustee.
  
  @short Group trustee.
  @ingroup security
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/

class Group : public Object {
private:

  /** Opaque identifier of the group. */
  void* id;
public:

  /** The valid of an invalid identifier. */
  static const void* const INVALID = static_cast<char*>(0) - 1;

  /**
    This exception is raised by the Group class.

    @short Group exception
    @ingroup exceptions
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.1
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
    Initializes group as invalid.
  */
  inline Group() throw() : id(static_cast<char*>(0) - 1) {}
  
  /**
    Initializes the group by id.

    @param id The identifier of the group.
  */
  Group(const void* id) throw(OutOfDomain);
  
  /**
    Initializes group from other group.
  */
  Group(const Group& copy) throw();
  
  /**
    Assignment of group by group.
  */
  Group& operator=(const Group& eq) throw();
  
  /**
    Initializes the group by name.

    @param name The name of the group.
  */
  Group(const String& name) throw(GroupException);
  
  /**
    Returns true if the groups are identical. The method returns true if both
    users are invalid.
  */
  bool operator==(const Group& eq) throw();
  
  /**
    Returns false if the groups are not identical. The method returns false if
    both groups are invalid.
  */
  inline bool operator!=(const Group& eq) throw() {
    return !(*this == eq);
  }

  /**
    Initializes group as the primary group of the specified user.
  */
  Group(const User& user) throw(GroupException);
  
  /**
    Returns true if the group is initialized. This does not mean that the group
    exists.
  */
  inline bool isInitialized() const throw() {
    return id != INVALID;
  }

  /**
    Returns the id of the group.
  */
  inline const void* getId() const throw() {
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

  /**
    Destroys the group object.
  */
  ~Group() throw();
};

/**
  Writes the group id to the format output stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Group& value) throw(IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
