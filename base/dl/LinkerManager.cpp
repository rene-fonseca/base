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
#include <base/dl/LinkerManager.h>
#include <base/Application.h>
#include <stdlib.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// Used by LinkerModules to locate the LinkerManager
LinkerManager* LinkerManager::linkerManager = nullptr; // TAG: need support for any number of managers?

LinkerManager* LinkerManager::getManager() noexcept
{
  if (!linkerManager) {
    ferr << "Internal error: LinkerManager has not been instantiated." << ENDL;
    exit(Application::EXIT_CODE_ERROR);
  }
  return linkerManager;
}

LinkerManager::LinkerManager()
  : registratedModule(0)
{
  bassert(linkerManager == 0, SingletonException(this));
  linkerManager = this;
}

void LinkerManager::registrate(LinkerModule* module) noexcept {
  registratedModule = module;
}

LinkerModule* LinkerManager::deregistrate() noexcept {
  return registratedModule;
}

LinkerModule* LinkerManager::load(
  const String& modulePath) {
  // acquire lock
  BASSERT(!registratedModule);
  DynamicLinker* dl = new DynamicLinker(modulePath, DynamicLinker::LAZY);
  LinkerModule* module = registratedModule;
  registratedModule = 0;
  // release lock
  modules.add(module, dl);
  return module;
}

void LinkerManager::unload(
  LinkerModule* module) {
  // acquire lock
  BASSERT(!registratedModule);
  DynamicLinker* dl = modules[module];
  registratedModule = module;
  delete dl;
  registratedModule = 0;
  // release lock
  modules.remove(module);
}

LinkerManager::ReadEnumerator LinkerManager::getModules() const noexcept {
  return modules.getReadEnumerator();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
