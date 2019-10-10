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

class LinkerManager : public Object {
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
  static LinkerManager* getManager() throw();

  /**
    Invoked by module during self-registration process.
  */
  void registrate(LinkerModule* module) throw();

  /**
    Invoked by module during self-deregistration process.
  */
  LinkerModule* deregistrate() throw();

  /**
    Initializes the manager.
  */
  LinkerManager() throw(SingletonException);

  /**
    Loads the specified module.

    @param module The path of the module to be loaded into address space.
  */
  LinkerModule* load(const String& module) throw(LinkerException);

  /**
    Unloads the specified module.

    @param module The linker module to be unloaded.
  */
  void unload(LinkerModule* module) throw(InvalidKey, LinkerException);

  /**
    Returns an enumeration of all the modules of the manager.
  */
  ReadEnumerator getModules() const throw();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
