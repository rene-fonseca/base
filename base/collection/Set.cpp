/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/collection/Set.h>
#include <string>

_DK_SDU_MIP__BASE__DUMMY_SYMBOL

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// instantiate common types
// TAG: defined common types for containers: bool, char, uint8, wchar, short, unsigned short, int, unsigned int, int64, uint64, std::string, std::wstring, float, double, long double

template _DK_SDU_MIP__BASE__API class Set<int>;
template _DK_SDU_MIP__BASE__API class Set<unsigned int>;

template _DK_SDU_MIP__BASE__API class Set<char>;
template _DK_SDU_MIP__BASE__API class Set<wchar>;
template _DK_SDU_MIP__BASE__API class Set<void*>;

template _DK_SDU_MIP__BASE__API class Set<float>;
template _DK_SDU_MIP__BASE__API class Set<double>;
template _DK_SDU_MIP__BASE__API class Set<long double>;

template _DK_SDU_MIP__BASE__API class Set<std::string>;
template _DK_SDU_MIP__BASE__API class Set<std::wstring>;

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
