/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include "MyLinkerModule.h"
#include <base/dl/LinkerManager.h>
#include <base/string/FormatOutputStream.h> // debug only
#include <base/mem/DynamicMemory.h>

using namespace com::azure::dev::base;

void moduleEntry() throw() {
  Trace::message("Module entry function");
  LinkerManager::getManager()->registrate(new MyLinkerModule("https://www.fonseca.dk/base/MyLinkerModule", "MyLinkerModule", "0.1"));
}

void moduleCleanUp() throw() {
  Trace::message("Module clean-up function");
  delete LinkerManager::getManager()->deregistrate();
}

/*
extern "C" void _init() {
  moduleEntry();
}

extern "C" void _fini() {
  moduleCleanUp();
}
*/
