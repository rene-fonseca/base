/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_FILESYSTEM__FILE_SYSTEM_H
#define _DK_SDU_MIP__BASE_FILESYSTEM__FILE_SYSTEM_H

#include <base/Object.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  File system.

  @author René Møller Fonseca
  @version 1.0
*/

class FileSystem : public Object {
public:

  /**
    Returns the path of the current folder.
  */
  static String getCurrentFolder() throw(Exception);

  /**
    Sets the current folder.
  */
  static void setCurrentFolder(const String& path) throw(Exception);

/*
  Folder getRoot() throw();
*/

  /**
    Returns true if the file exists.
  */
  static bool fileExists(const String& path) throw();

  /**
    Returns true if the folder exists.
  */
  static bool folderExists(const String& path) throw();

  /**
    Creates a temporary file.
  */
//  File tempFile() throw(FileException);

};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
