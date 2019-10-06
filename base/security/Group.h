/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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
#include <base/collection/Hash.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A group trustee.
  
  @short Group trustee.
  @ingroup security
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.2
*/

class Group : public Object {
  friend class Hash<Group>;
  friend class Trustee;
private:

  /** Identifier of the group represented as an integral if possible. */
  unsigned long integralId = 0;
  /** Opaque identifier of the group. */
  Reference<ReferenceCountedAllocator<uint8> > id;
public:
  
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
    inline GroupException() throw() {
    }

    /**
      Initializes the exception object.

      @param message The message.
    */
    inline GroupException(const char* message) throw() : Exception(message) {
    }

    /**
      Initializes the exception object without an associated message.

      @param type The identity of the type.
    */
    inline GroupException(Type type) throw() : Exception(type) {
    }

    /**
      Initializes the exception object.

      @param message An NULL-terminated string (ASCII).
      @param type The identity of the type.
    */
    inline GroupException(const char* message, Type type) throw()
      : Exception(message, type) {
    }
  };
  
  /**
    Initializes group as invalid.
  */
  inline Group() throw() : integralId(getMaximum(integralId)) {
  }
  
  /**
    Initializes the group by id.

    @param id The identifier of the group.
  */
  Group(unsigned long id) throw(OutOfDomain);

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
  bool operator==(const Group& eq) const throw();
  
  /**
    Returns false if the groups are not identical. The method returns false if
    both groups are invalid.
  */
  inline bool operator!=(const Group& eq) const throw() {
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
  inline bool isValid() const throw() {
    return integralId != getMaximum(integralId);
  }

  /**
    Returns the id of the group.
  */
  inline const void* getId() const throw() {
    return id.isValid() ? id->getElements() : 0;
  }

  /**
    Returns the integral id of the group.
  */
  inline unsigned long getIntegralId() const throw() {
    return integralId;
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

/**
  Writes the group id to the format output stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Group& value) throw(IOException);

template<>
class Hash<Group> {
public:
  
  unsigned long operator()(const Group& value) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
