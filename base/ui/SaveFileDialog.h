/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_UI__SAVE_FILE_DIALOG_H
#define _DK_SDU_MIP__BASE_UI__SAVE_FILE_DIALOG_H

#include <base/string/String.h>
#include <base/collection/Array.h>
#include <base/collection/Map.h>
#include <base/ui/Dialog.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Save file dialog.
  
  @short Save file dialog.
  @ingroup ui
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class SaveFileDialog : public Dialog {
private:

  /** Flags. */
  unsigned int flags;
  /** Specifies the title of the dialog. */
  String title;
  /** Specifies the current file name. */
  String filename;
  /** Specifies the initial folder. */
  String folder;
  /** Filters. */
  Map<String, String> filters;
  /** The default filter. */
  unsigned int defaultFilter;
public:

  enum Flag {
    ASK_TO_CREATE = 1, /**< Ask to create the file if it doesn't exist. */
    ASK_TO_OVERWRITE = 2 /**< Ask to overwrite the file if the does exist. */
  };
  
  /**
    Initializes the save file dialog window.
  */
  SaveFileDialog() throw();

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
    Returns the file name.
  */
  inline String getFilename() const throw() {
    return filename;
  }
  
  /**
    Sets the current file name.
  */
  inline void setFilename(const String& filename) throw() {
    this->filename = filename;
  }
  
  /**
    Returns the initial folder.
  */
  inline String getFolder() const throw() {
    return folder;
  }
  
  /**
    Sets the initial folder.
  */
  inline void setFolder(const String& folder) throw() {
    this->folder = folder;
  }

  /**
    Returns the filters.
  */
  inline Map<String, String> getFilters() const throw() {
    return filters;
  }

  /**
    Sets the filter.

    @param description The filter description.
    @param filter The filter.
  */
  void setFilter(const String& description, const String& filter) throw();
  
  /**
    Sets the filters.
  */
  void setFilters(const Map<String, String>& filters) throw();
  
  /**
    Executes the dialog.
  */
  bool execute() throw(UserInterfaceException);

  ~SaveFileDialog() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
