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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Select folder dialog.
  
  @short Select folder dialog.
  @ingroup ui
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API SelectFolderDialog : public Dialog {
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
  SelectFolderDialog() throw();

  /**
    Returns the flags.
  */
  inline unsigned int getFlags() const throw() {
    return flags;
  }

  /**
    Sets the flags.
  */
  inline void setFlags(unsigned int flags) throw() {
    this->flags = flags;
  }
  
  /**
    Returns the title of the dialog window.
  */
  inline String getTitle() const throw() {
    return title;
  }

  /**
    Sets the title of the dialog window.
  */
  inline void setTitle(const String& title) throw() {
    this->title = title;
  }

  /**
    Returns the folder.
  */
  inline String getFolder() const throw() {
    return folder;
  }
  
  /**
    Sets the current folder.
  */
  inline void setFolder(const String& folder) throw() {
    this->folder = folder;
  }  
  
  /**
    Executes the dialog.
  */
  bool execute() throw(UserInterfaceException);

  ~SelectFolderDialog() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
