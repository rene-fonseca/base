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

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short True if two types are the same.
  @version 1.0
*/

template<class A, class B>
class IsSame {
public:
  
  enum {
    VALUE = false
  };
};

template<class A>
class IsSame<A, A> {
public:

  enum {
    VALUE = true;
  };
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
