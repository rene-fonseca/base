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

#include <base/Resource.h>
#include <base/string/String.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Folder monitor.
  
  @short Folder monitor.
  @ingroup filesystem
  @version 1.1
*/

class _COM_AZURE_DEV__BASE__API FolderMonitor : public Resource {
public:
  
  /**
    Initializes folder monitor.
    
    @param path Specifies the folder to monitor.
  */
  FolderMonitor(const String& path);

  /**
    Returns true if the an event has occured.
  */
  bool isSignaled() const;

  /**
    Waits for an event.
  */
  void wait() const;

  /**
    Waits for an event.
    
    @param milliseconds The timeout period.
  */
  bool wait(unsigned int milliseconds) const;
  
  /**
    Destroys the monitor.
  */
  ~FolderMonitor() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
