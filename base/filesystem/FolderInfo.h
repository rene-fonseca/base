/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_FILESYSTEM__FOLDER_INFO_H
#define _DK_SDU_MIP__BASE_FILESYSTEM__FOLDER_INFO_H

#include <base/Object.h>
#include <base/filesystem/FileSystemException.h>
#include <base/collection/Array.h>
#include <base/string/String.h>
#include <base/Date.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Folder information.

  @author René Møller Fonseca
  @version 1.0
*/

class FolderInfo : public Object {
private:

  /** The path of the folder. */
  String path;
  /** The last modification time. */
  Date modification;
  /** The last access time. */
  Date access;
  /** The last change time. */
  Date change;
public:

  /**
    Initializes a folder.

    @param path the path of the folder.
  */
  FolderInfo(const String& path) throw(FileSystemException);

  /**
    Initializes folder from other folder.
  */
  FolderInfo(const FolderInfo& copy) throw() : path(copy.path), modification(copy.modification), access(copy.access), change(copy.change) {}

  /**
    Returns a folder object for the parent folder.
  */
  FolderInfo getParent() const throw(FileSystemException);

  /**
    Returns the names of the entries (files and subfolders) of this folder.
  */
  Array<String> getEntries() const throw(FileSystemException);

  /**
    Returns the last modification time of the folder.
  */
  Date getLastModification() const throw();

  /**
    Returns the last access time of the folder.
  */
  Date getLastAccess() const throw();

  /**
    Returns the last chnage time of the folder.
  */
  Date getLastChange() const throw();

  /**
    Returns the path of the folder.
  */
  String getPath() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
