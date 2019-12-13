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

#include <base/string/String.h>
#include <base/collection/Array.h>
#include <base/collection/Map.h>
#include <base/ui/Dialog.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Select folder dialog.
  
  @short Select folder dialog.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API SelectFolderDialog : public Dialog {
private:

  /** Flags. */
  unsigned int flags = 0;
  /** Specifies the title of the dialog. */
  String title;
  /** Specifies the current folder. */
  String folder;
public:
  
  /**
    Initializes the save file dialog window.
  */
  SelectFolderDialog() noexcept;

  /**
    Returns the flags.
  */
  inline unsigned int getFlags() const noexcept
  {
    return flags;
  }

  /**
    Sets the flags.
  */
  inline void setFlags(unsigned int flags) noexcept
  {
    this->flags = flags;
  }
  
  /**
    Returns the title of the dialog window.
  */
  inline const String& getTitle() const noexcept
  {
    return title;
  }

  /**
    Sets the title of the dialog window.
  */
  inline void setTitle(const String& title) noexcept
  {
    this->title = title;
  }

  /**
    Returns the folder.
  */
  inline const String& getFolder() const noexcept
  {
    return folder;
  }
  
  /**
    Sets the current folder.
  */
  inline void setFolder(const String& folder) noexcept
  {
    this->folder = folder;
  }  
  
  /**
    Executes the dialog.
  */
  bool execute();

  ~SelectFolderDialog() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
