/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/LongDouble.h>
#include <float.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

const long double LongDouble::MINIMUM = LDBL_MIN;
const long double LongDouble::MAXIMUM = LDBL_MAX;
const long double LongDouble::EPSILON = LDBL_EPSILON;
const long double LongDouble::INFINITY = 1/0.l;
const long double LongDouble::MINUS_INFINITY = -1/0.l;
const long double LongDouble::QUIET_NAN = 0/0.l;

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
