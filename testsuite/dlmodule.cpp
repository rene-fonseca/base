/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include "MyLinkerModule.h"
#include <base/dl/LinkerManager.h>
#include <base/string/FormatOutputStream.h> // debug only
#include <base/mem/DynamicMemory.h>

using namespace base;

void moduleEntry() throw() {
  Trace::message("Module entry function");
  LinkerManager::getManager()->registrate(new MyLinkerModule("mip.sdu.dk/~fonseca/base/MyLinkerModule", "MyLinkerModule", "0.1"));
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
