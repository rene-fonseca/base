/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#define _COM_AZURE_DEV__MY_LINKER_MODULE__SHARED_LIBRARY
#if !defined(_COM_AZURE_DEV__BASE__SHARED_LIBRARY) // avoid compiler warning for static build
#  define _COM_AZURE_DEV__BASE__SHARED_LIBRARY
#endif

#include "MyLinkerModule.h"
#include <base/dl/LinkerManager.h>
#include <base/string/FormatOutputStream.h> // debug only
#include <base/mem/DynamicMemory.h>

using namespace com::azure::dev::base;

void moduleEntry() {
  Trace::message("Module entry function");
  LinkerManager::getManager()->registrate(new MyLinkerModule("https://dev.azure.com/base/MyLinkerModule", "MyLinkerModule", "0.1"));
}

void moduleCleanUp() {
  Trace::message("Module clean-up function");
  delete LinkerManager::getManager()->deregistrate();
}
