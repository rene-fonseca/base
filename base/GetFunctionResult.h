/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__GET_FUNCTION_RESULT_H
#define _DK_SDU_MIP__BASE__GET_FUNCTION_RESULT_H

#include <base/Unspecified.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short Result type of a function.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
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

#endif
