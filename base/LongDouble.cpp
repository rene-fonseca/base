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
#include <cfloat>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

long double LongDouble::getMinimum() noexcept
{
  return LDBL_MIN;
}

long double LongDouble::getMaximum() noexcept
{
  return LDBL_MAX;
}

long double LongDouble::getEpsilon() noexcept
{
  return LDBL_EPSILON;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
