/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_DL__LINKER_MANAGER_H
#define _DK_SDU_MIP__BASE_DL__LINKER_MANAGER_H

#include <base/dl/LinkerModule.h>
#include <base/dl/DynamicLinker.h>
#include <base/string/String.h>
#include <base/collection/Map.h>
#include <base/SingletonException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class LinkerManager : public Object {
private:

  /** All the modules of the manager. */
  Map<LinkerModule*, DynamicLinker*> modules;
  /** Used temporarily during module self-registration.*/
  LinkerModule* registratedModule;

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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
