/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/dl/LinkerModule.h>
#include <base/dl/DynamicLinker.h>
#include <base/string/String.h>
#include <base/collection/Map.h>
#include <base/SingletonException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class _COM_AZURE_DEV__BASE__API LinkerManager : public Object {
private:

  /** All the modules of the manager. */
  Map<LinkerModule*, DynamicLinker*> modules;
  /** Used temporarily during module self-registration.*/
  LinkerModule* registratedModule = nullptr;

  /** Read enumerator of modules. */
  typedef Map<LinkerModule*, DynamicLinker*>::ReadEnumerator ReadEnumerator;

  static LinkerManager* linkerManager;
public:

  /**
    Returns the linker manager of the application.
  */
  static LinkerManager* getManager() noexcept;

  /**
    Invoked by module during self-registration process.
  */
  void registrate(LinkerModule* module) noexcept;

  /**
    Invoked by module during self-deregistration process.
  */
  LinkerModule* deregistrate() noexcept;

  /**
    Initializes the manager.
  */
  LinkerManager();

  /**
    Loads the specified module.

    @param module The path of the module to be loaded into address space.
  */
  LinkerModule* load(const String& module);

  /**
    Unloads the specified module.

    @param module The linker module to be unloaded.
  */
  void unload(LinkerModule* module);

  /**
    Returns an enumeration of all the modules of the manager.
  */
  ReadEnumerator getModules() const noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
