/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_FILESYSTEM__FOLDER_MONITOR_H
#define _DK_SDU_MIP__BASE_FILESYSTEM__FOLDER_MONITOR_H

#include <base/Object.h>
#include <base/string/String.h>
#include <base/filesystem/FileSystemException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Folder monitor.
  
  @short Folder monitor.
  @ingroup filesystem
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class FolderMonitor : public Object {
public:
  
  /**
    Initializes folder monitor.
    
    @param path Specifies the folder to monitor.
  */
  FolderMonitor(const String& path) throw(ResourceException);  

  /**
    Returns true if the an event has occured.
  */
  bool isSignaled() const throw();

  /**
    Waits for an event.
  */
  void wait() const throw();

  /**
    Waits for an event.
    
    @param milliseconds The timeout period.
  */
  bool wait(unsigned int milliseconds) const throw();
  
  /**
    Destroys the monitor.
  */
  ~FolderMonitor() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
