/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__VERSION_H
#define _DK_SDU_MIP__BASE__VERSION_H

#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class returns information concerning the framework.

  @short Framework information.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Version : public Object {
private:

  /** Banner. */
  static const char banner[];
public:

  /**
    Returns the major version of the framework.
  */
  unsigned int getMajorVersion() const throw();

  /**
    Returns the minor version of the framework.
  */
  unsigned int getMinorVersion() const throw();

  /**
    Returns the micro version of the framework.
  */
  unsigned int getMicroVersion() const throw();

  /**
    Returns the release of the framework as a string. The format of the string
    is unspecified.
  */
  String getRelease() const throw();

  /**
    Returns the version of the framework as a string. The format of the string
    is 'major.minor.micro suffix' where suffix could be anything.
  */
  String getVersion() const throw();

  /**
    Returns a short banner (multiple lines) describing the framework.
  */
  String getBanner() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
