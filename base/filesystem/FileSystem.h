/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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
#include <base/io/File.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @defgroup filesystem File system
*/

/**
  File system.
  
  @short File system
  @ingroup filesystem
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class FileSystem : public Object {
private:

  /** Counter used for generating temporary file names. */
  static unsigned int counter;
public:

  /** The temporary folder. */
  enum TemporaryFolder {
    /**
      Uses the temporary folder specified by the 'TMP' environment variable. If
      the variable doesn't exist the TEMP variable is used. If neither are
      defined this falls back on MACHINE_NONPERSISTENT.
    */
    USER_SPECIFIED,
    /** Specifies the temporary folder intended for non-persistent temporary files. */
    MACHINE_NONPERSISTENT,
    /** Specifies the temporary folder intended for persistent temporary files. */
    MACHINE_PERSISTENT
  };
  
  /** The fields of a temporary file. */
  enum TemporaryFileNameField {
    /** Add the prefix '~'. */
    PREFIX = 1,
    /** Add the user name. */
    USER = 2,
    /** Add the formal name of application. */
    APPLICATION = 4,
    /** Add the process id. */
    PROCESS = 8,
    /** Add the host name. */
    HOST = 16,
    /** Add internal counter (an increment). */
    COUNTER = 32,
    /** Add the current time. */
    TIME = 64,
    /** Add a random number. */
    RANDOM = 128,
    /** Add the suffix '.tmp'. */
    SUFFIX = 256
  };

  /** Temporary file options. */
  enum TemporaryFileOption {
    /** Sets the access control of the file such that only the owner of the process has access to it. */
    STRICT_ACCESS = 1,
    /** The content of the file is overwritten when the file is closed (this implies REMOVE_ON_CLOSE). */
    OVERWRITE_ON_CLOSE = 2,
    /** The file is removed when closed. */
    REMOVE_ON_CLOSE = 4,
    /** The file is opened in exclusive mode. */
    EXCLUSIVE = 8,
    /** The file is opened in secure mode. */
    SECURE = STRICT_ACCESS | OVERWRITE_ON_CLOSE | REMOVE_ON_CLOSE
  };
  
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
    Returns the path to the folder intended for temporary files.

    @param folder Specifies the folder to be used. The default is USER_SPECIFIED.
  */
  static String getTempFolder(TemporaryFolder folder = USER_SPECIFIED) throw();
  
  /**
    Generates a temporary file name. The returned name should be used in
    conjuction with a folder path returned by getTempFolder or other folder
    path. Every field is separated by a '-' except for the PREFIX and SUFFIX.
    The order of the fields are fixed and given by the order of the fields
    within the enumeration TemporaryFileNameField.
    
    @param fields Specifies the fields to include in the name. The default is
    PREFIX|APPLICATION|USER|SUFFIX.
  */
  static String getTempFileName(unsigned int fields = PREFIX|APPLICATION|USER|SUFFIX) throw();
  
  /**
    Returns a temporary file.
    
    @param options The file options. The default is SECURE.
  */
  static File getTempFile(unsigned int options = SECURE) throw(IOException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
