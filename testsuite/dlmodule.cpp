/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include "MyLinkerModule.h"
#include <base/dl/LinkerManager.h>
#include <base/string/FormatOutputStream.h> // debug only

using namespace base;

void moduleEntry() throw() {
  fout << "Module entry function" << ENDL; // TAG: remove from final
  LinkerManager::getManager()->registrate(new MyLinkerModule("mip.sdu.dk/~fonseca/base/MyLinkerModule", "MyLinkerModule", "0.1"));
}

void moduleCleanUp() throw() {
  fout << "Module clean-up function" << ENDL; // TAG: remove from final
  delete LinkerManager::getManager()->deregistrate();
}

extern "C" void _init() {
  moduleEntry();
}

extern "C" void _fini() {
  moduleCleanUp();
}
