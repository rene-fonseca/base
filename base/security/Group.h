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
#include <base/Exception.h> // TAG: need other exception
#include <base/collection/Array.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Group.

  @ingroup security
  @short Group
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Group : public Object {
private:

  unsigned long long id;
public:

  /**
    Initializes the group by id.

    @param id The identifier of the group.
  */
  Group(unsigned long long) throw();

  /**
    Initializes the group by name.

    @param name The name of the group.
  */
  Group(const String& name) throw(Exception);

  /**
    Initializes group as the primary group of the specified user.
  */
  Group(const User& user) throw(Exception);

  /**
    Initializaes group from other group.
  */
  Group(const Group& copy) throw();

  /**
    Returns the name of the group.
  */
  String getName() const throw(Exception);

  /**
    Returns the members of the group.
  */
  Array<String> getMembers() const throw(Exception);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
