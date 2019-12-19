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

float Float::getMinimum() noexcept
{
  return FLT_MIN;
}

float Float::getMaximum() noexcept
{
  return FLT_MAX;
}

float Float::getEpsilon() noexcept
{
  return FLT_EPSILON;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
