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

#include <base/DynamicObject.h>
#include <base/string/String.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class returns information concerning some component (default is this
  framework).
  
  @short Component information
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Version : public DynamicObject {
private:

  /** Banner. */
  static const char banner[];
public:

  /**
    Returns the major version of the framework.
  */
  virtual unsigned int getMajorVersion() const noexcept;

  /**
    Returns the minor version of the framework. By convention equal and
    unequal numbers indicate a final and a debug release, respectively.
  */
  virtual unsigned int getMinorVersion() const noexcept;

  /**
    Returns the micro version of the framework.
  */
  virtual unsigned int getMicroVersion() const noexcept;

  /**
    Returns the revision of the framework.
  */
  virtual unsigned int getRevision() const noexcept;

  /**
    Returns the release of the framework as a string. The format of the string
    is unspecified.
  */
  virtual String getRelease() const noexcept;

  /**
    Returns the version of the framework as a string. The recommended format
    is 'major.minor.micro suffix' where suffix could be anything.
  */
  virtual String getVersion() const noexcept;

  /**
    Returns the short commit id.
  */
  virtual String getCommit() const noexcept;

  /*
    Returns the build date in seconds since epoch.
  */
  virtual int64 getBuildDate() const noexcept;

  /**
    Returns a short banner (multiple lines) describing the framework.
  */
  virtual String getBanner() const noexcept;

  /** Returns the size of DynamicObject. */
  static MemorySize getDynamicObjectSize() noexcept;

  /** Returns true if _COM_AZURE_DEV__BASE__DEBUG was defined for the Base Framework. _COM_AZURE_DEV__BASE__DEBUG influences class state and must be set identical for dependent projects. */
  static bool getDefine_COM_AZURE_DEV__BASE__DEBUG() noexcept;

  /** Returns true if DEBUG, _DEBUG, or _COM_AZURE_DEV__BASE__DEBUG were defined for the Base Framework. */
  static bool getDefine_DEBUG() noexcept;

  /** Returns true if the project is built with the correct defines as the Base Framework. Raises exception on conflict. */
  static inline bool isBuildCompatible()
  {
#if defined(_COM_AZURE_DEV__BASE__DEBUG)
    const bool define_COM_AZURE_DEV__BASE__DEBUG = true;
#else
    const bool define_COM_AZURE_DEV__BASE__DEBUG = false;
#endif
    
#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
    const bool define_DEBUG = true;
#else
    const bool define_DEBUG = false;
#endif
    
    if (getDefine_COM_AZURE_DEV__BASE__DEBUG() != define_COM_AZURE_DEV__BASE__DEBUG) {
      throw Exception("_COM_AZURE_DEV__BASE__DEBUG symbol mismatch detected for Base Framework.");
    }
    if (getDefine_DEBUG() != define_DEBUG) {
      throw Exception("DEBUG, _DEBUG, or _COM_AZURE_DEV__BASE__DEBUG symbol mismatch detected for Base Framework.");
    }
    if (getDynamicObjectSize() != sizeof(DynamicObject)) {
      throw Exception("DynamicObject mismatch detected for Base Framework.");
    }

    return true;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
