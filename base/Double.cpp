/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Double.h>
#include <float.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

const double Double::MINIMUM = DBL_MIN;
const double Double::MAXIMUM = DBL_MAX;
const double Double::EPSILON = DBL_EPSILON;
const double Double::INFINITY = 1/0.f;
const double Double::MINUS_INFINITY = -1/0.f;
const double Double::QUIET_NAN = 0/0.f;

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
