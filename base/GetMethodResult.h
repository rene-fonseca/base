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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  @short Result type of a method.
  @version 1.0
*/

template<class CLASS,
         class R, class A, class B, class C, class E, class F>
class GetMethodResult {
public:
};

template<class CLASS,
         class R>
class GetMethodResult<R (CLASS::*)()> {
public:

  typedef R Result;
};

template<class CLASS,
         class R, class A>
class GetMethodResult<R (CLASS::*)(A)> {
public:

  typedef R Result;
};

template<class CLASS,
         class R, class A, class B>
class GetMethodResult<R (CLASS::*)(A, B)> {
public:

  typedef R Result;
};

template<class CLASS,
         class R, class A, class B, class C>
class GetMethodResult<R (CLASS::*)(A, B, C)> {
public:

  typedef R Result;
};

template<class CLASS,
         class R, class A, class B, class C, class D>
class GetMethodResult<R (CLASS::*)(A, B, C, D)> {
public:

  typedef R Result;
};

template<class CLASS,
         class R, class A, class B, class C, class D, class E>
class GetMethodResult<R (CLASS::*)(A, B, C, D, E)> {
public:

  typedef R Result;
};

template<class CLASS,
         class R, class A, class B, class C, class D, class E, class F>
class GetMethodResult<R (CLASS::*)(A, B, C, D, E, F)> {
public:

  typedef R Result;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
