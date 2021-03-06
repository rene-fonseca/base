/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>

_COM_AZURE_DEV__BASE__DUMMY_SYMBOL

// include platform specific source files

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <base/platforms/win32/AsyncReadFileContext.cpp>
#  include <base/platforms/win32/AsyncReadStreamContext.cpp>
#  include <base/platforms/win32/AsyncWriteFileContext.cpp>
#  include <base/platforms/win32/AsyncWriteStreamContext.cpp>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
