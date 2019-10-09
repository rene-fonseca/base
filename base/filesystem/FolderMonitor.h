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

#include <base/Object.h>
#include <base/string/String.h>
#include <base/filesystem/FileSystemException.h>
#include <base/OperatingSystem.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Folder monitor.
  
  @short Folder monitor.
  @ingroup filesystem
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API FolderMonitor : public Object {
private:

  OperatingSystem::Handle handle = 0;
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
