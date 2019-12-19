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

double Double::getMinimum() noexcept
{
  return DBL_MIN;
}

double Double::getMaximum() noexcept
{
  return DBL_MAX;
}

double Double::getEpsilon() noexcept
{
  return DBL_EPSILON;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
