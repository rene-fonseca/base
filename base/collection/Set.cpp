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

template class Set<int>;
template class Set<unsigned int>;

template class Set<char>;
template class Set<wchar>;
template class Set<void*>;

template class Set<float>;
template class Set<double>;
template class Set<long double>;

template class Set<std::string>;
template class Set<std::wstring>;

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
