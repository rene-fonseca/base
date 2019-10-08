/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/SingletonException.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short Linker module.
  @version 1.0
*/

class LinkerModule : public Object {
private:

  /** The module object. */
  static LinkerModule* linkerModule;
  /** The identity of the module (unique). */
  String identity;
  /** The name of the module. */
  String name;
  /** The version of the module. */
  String version;
public:

  /**
    Returns the linker module object.
  */
  inline static const LinkerModule* getLinkerModule() throw() {
    return linkerModule;
  }

  /**
    Initializes module.

    The version must match the regular expression /[1-9]*[0-9](.[1-9]*[0-9]){1,2}/.

    @param identity Unique and fully qualified name (e.g. "mip.sdu.dk/~fonseca/GIP/Modules/Flip").
    @param name A nice name (e.g. "Flip").
    @param version Version (e.g. "1.12.1").
  */
  LinkerModule(const String& identity, const String& name, const String& version) throw(SingletonException);

  /**
    Returns the identity of the module.
  */
  String getIdentity() const throw();

  /**
    Returns the name of the module.
  */
  String getName() const throw();

  /**
    Returns the version of the module.
  */
  String getVersion() const throw();

  /**
    Destroy the module.
  */
  virtual ~LinkerModule();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
