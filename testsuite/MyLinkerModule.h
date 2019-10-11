/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/dl/LinkerModule.h>
#include <base/string/FormatOutputStream.h>

using namespace com::azure::dev::base;

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#if defined(_COM_AZURE_DEV__MY_LINKER_MODULE__SHARED_LIBRARY_BUILD)
#  define _COM_AZURE_DEV__MY_LINKER_MODULE__API __declspec(dllexport)
#elif defined(_COM_AZURE_DEV__MY_LINKER_MODULE__SHARED_LIBRARY)
#  define _COM_AZURE_DEV__MY_LINKER_MODULE__API __declspec(dllimport)
#else
#  define _COM_AZURE_DEV__MY_LINKER_MODULE__API
#endif
#else
#  define _COM_AZURE_DEV__MY_LINKER_MODULE__API
#endif

class _COM_AZURE_DEV__MY_LINKER_MODULE__API MyLinkerModule : public LinkerModule {
public:

  MyLinkerModule(
    const String& identity,
    const String& name,
    const String& version) throw();

  virtual void myFunction() throw();
};
