/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2003-2006 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__VERSION_H
#define _DK_SDU_MIP__BASE__VERSION_H

#include <base/DynamicObject.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class returns information concerning some component (default is this
  framework).
  
  @short Component information
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Version : public DynamicObject {
private:

  /** Banner. */
  static const char banner[];
public:

  /**
    Returns the major version of the framework.
  */
  virtual unsigned int getMajorVersion() const throw();

  /**
    Returns the minor version of the framework. By convention equal and
    unequal numbers indicate a final and a debug release, respectively.
  */
  virtual unsigned int getMinorVersion() const throw();

  /**
    Returns the micro version of the framework.
  */
  virtual unsigned int getMicroVersion() const throw();

  /**
    Returns the release of the framework as a string. The format of the string
    is unspecified.
  */
  virtual String getRelease() const throw();

  /**
    Returns the version of the framework as a string. The recommended format
    is 'major.minor.micro suffix' where suffix could be anything.
  */
  virtual String getVersion() const throw();

  /**
    Returns a short banner (multiple lines) describing the framework.
  */
  virtual String getBanner() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
