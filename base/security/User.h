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
#include <base/Exception.h> // TAG: need other exception

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Group;

/**
  User.

  @ingroup security
  @short User
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class User : public Object {
private:

  unsigned long long id;
public:

  /**
    Returns the user associated with the current process.
  */
  static User getCurrentUser() throw();

  /**
    Initializes user by id.
  */
  User(unsigned long long id) throw();

  /**
    Initializaes user from other user.
  */
  User(const User& copy) throw();

  /**
    Initializes user by name.
  */
  User(const String& name) throw(Exception);

  /**
    Returns the short name of the account.
  */
  String getName() const throw(Exception);

  /**
    Returns the full name of the account.
  */
  String getFullName() const throw(Exception);

  /**
    Returns the path of the home folder.
  */
  String getHomeFolder() const throw();

  /**
    Returns true if the user is a member of the specified group.
  */
  bool isMemberOf(const Group& group) throw(Exception);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
