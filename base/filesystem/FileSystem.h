/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_FILESYSTEM__FILE_SYSTEM_H
#define _DK_SDU_MIP__BASE_FILESYSTEM__FILE_SYSTEM_H

#include <base/Object.h>
#include <base/string/String.h>
#include <base/filesystem/FileSystemException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @defgroup filesystem File system
*/

/**
  File system.

  @ingroup filesystem
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class FileSystem : public Object {
public:

  /**
    Returns the combined path of the specified base and relative path.

    @param base The base path.
    @param relative The relative path.
  */
  static String getPath(const String& base, const String& relative) throw();

  /**
    Returns the path of the current folder.
  */
  static String getCurrentFolder() throw(FileSystemException);

  /**
    Sets the current folder.
  */
  static void setCurrentFolder(const String& path) throw(FileSystemException);

  /**
    Returns true if the file exists.
  */
  static bool fileExists(const String& path) throw(FileSystemException);

  /**
    Returns true if the folder exists.
  */
  static bool folderExists(const String& path) throw(FileSystemException);

  /**
    Removes the specified file.
  */
  static void removeFile(const String& path) throw(FileSystemException);

  /**
    Removes the specified folder. The folder must be empty.
  */
  static void removeFolder(const String& path) throw(FileSystemException);

  /**
    Makes a folder.
  */
  static void makeFolder(const String& path) throw(FileSystemException);

  /**
    Returns the path of a temporary file.
  */
//  static String getTempFileName() throw(FileException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
