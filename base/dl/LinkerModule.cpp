/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/dl/LinkerModule.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

LinkerModule* LinkerModule::linkerModule = 0; // initialize linker module as uninitialized

LinkerModule::LinkerModule(const String& i, const String& n, const String& v) throw(SingletonException) :
  identity(i),
  name(n),
  version(v) {
  static unsigned int singleton = 0;
  assert(singleton == 0, SingletonException("LinkerModule has been instantiated"));
  ++singleton;
  linkerModule = this;
}

String LinkerModule::getIdentity() const throw() {
  return identity;
}

String LinkerModule::getName() const throw() {
  return name;
}

String LinkerModule::getVersion() const throw() {
  return version;
}

LinkerModule::~LinkerModule() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
