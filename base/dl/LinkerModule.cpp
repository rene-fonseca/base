/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/dl/LinkerModule.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

LinkerModule* LinkerModule::linkerModule = nullptr; // initialize linker module as uninitialized

const LinkerModule* LinkerModule::getLinkerModule() noexcept
{
  return linkerModule;
}

LinkerModule::LinkerModule(const String& i, const String& n, const String& v) throw(SingletonException)
  : identity(i),
    name(n),
    version(v)
{
  static unsigned int singleton = 0;
  if (singleton != 0) {
    throw SingletonException("LinkerModule has been instantiated", this);
  }
  ++singleton;
  linkerModule = this;
}

String LinkerModule::getIdentity() const noexcept
{
  return identity;
}

String LinkerModule::getName() const noexcept
{
  return name;
}

String LinkerModule::getVersion() const noexcept
{
  return version;
}

LinkerModule::~LinkerModule()
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
