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

#include <base/SingletonException.h>
#include <base/string/String.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  @short Linker module.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API LinkerModule : public Object {
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
  static const LinkerModule* getLinkerModule() noexcept;

  /**
    Initializes module.

    The version must match the regular expression /[1-9]*[0-9](.[1-9]*[0-9]){1,2}/.

    @param identity Unique and fully qualified name (e.g. "{17B00396-00E1-4C1C-BCA0-89EAD65FA71B}/GIP/Modules/Flip").
    @param name A nice name (e.g. "Flip").
    @param version Version (e.g. "1.12.1").
  */
  LinkerModule(const String& identity, const String& name, const String& version);

  /**
    Returns the identity of the module.
  */
  String getIdentity() const noexcept;

  /**
    Returns the name of the module.
  */
  String getName() const noexcept;

  /**
    Returns the version of the module.
  */
  String getVersion() const noexcept;

  /**
    Destroy the module.
  */
  virtual ~LinkerModule();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
