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

#include <base/Unspecified.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short Result type of a function.
  @version 1.0
*/

template<class R, class A, class B, class C, class E, class F>
class GetFunctionResult {
public:
};

template<class R>
class GetFunctionResult<R (*)()> {
public:

  typedef R Result;
};

template<class R, class A>
class GetFunctionResult<R (*)(A)> {
public:

  typedef R Result;
};

template<class R, class A, class B>
class GetFunctionResult<R (*)(A, B)> {
public:

  typedef R Result;
};

template<class R, class A, class B, class C>
class GetFunctionResult<R (*)(A, B, C)> {
public:

  typedef R Result;
};

template<class R, class A, class B, class C, class D>
class GetFunctionResult<R (*)(A, B, C, D)> {
public:

  typedef R Result;
};

template<class R, class A, class B, class C, class D, class E>
class GetFunctionResult<R (*)(A, B, C, D, E)> {
public:

  typedef R Result;
};

template<class R, class A, class B, class C, class D, class E, class F>
class GetFunctionResult<R (*)(A, B, C, D, E, F)> {
public:

  typedef R Result;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
