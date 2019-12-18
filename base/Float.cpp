/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Float.h>
#include <float.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

const float Float::MINIMUM = FLT_MIN;
const float Float::MAXIMUM = FLT_MAX;
const float Float::EPSILON = FLT_EPSILON;
const float Float::INFINITY = 1.f/0.f;
const float Float::MINUS_INFINITY = -1.f/0.f;
const float Float::QUIET_NAN = 0.f/0.f;

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
