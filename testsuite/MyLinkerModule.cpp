/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

// could be set in CMakeLists.txt
#if !defined(_COM_AZURE_DEV__BASE__SHARED_LIBRARY)
#  define _COM_AZURE_DEV__BASE__SHARED_LIBRARY
#endif
#if !defined(_COM_AZURE_DEV__MY_LINKER_MODULE__SHARED_LIBRARY_BUILD)
#  define _COM_AZURE_DEV__MY_LINKER_MODULE__SHARED_LIBRARY_BUILD
#endif

#include "MyLinkerModule.h"
#include <base/string/FormatOutputStream.h>
#include <base/concurrency/Thread.h>

#if defined(_COM_AZURE_DEV__BASE__SHARED_LIBRARY_BUILD) // catch CMake config mistake
#error _COM_AZURE_DEV__BASE__SHARED_LIBRARY_BUILD is enabled for test
#endif

using namespace com::azure::dev::base;

MyLinkerModule::MyLinkerModule(
  const String& identity,
  const String& name,
  const String& version) throw()
  : LinkerModule(identity, name, version) {
  fout << "Initializing MyLinkerModule..." << ENDL;
}

void MyLinkerModule::myFunction() throw() {
  for (unsigned int count = 0; count < 16; ++count) {
    fout << "Written by MyLinkerModule: " << count << ENDL;
    Thread::sleep(1);
  }
}
