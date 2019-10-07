/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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
  Open file dialog.
  
  @short Open file dialog.
  @ingroup ui
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OpenFileDialog : public Dialog {
private:

  /** Flags. */
  unsigned int flags = 0;
  /** Specifies the title of the dialog. */
  String title;
  /** Specifies the current file name. */
  String filename;
  /** Specifies the current file names. */
  Array<String> filenames;
  /** Specifies the initial folder. */
  String folder;
  /** Filters. */
  Map<String, String> filters;
  /** The default filter. */
  unsigned int defaultFilter = 0;
public:

  enum Flag {
    ALLOW_MULTIPLE = 1, /**< Specifies that multi files may be selected. */
    MUST_EXIST = 2 /**< Specifies that the selected file(s) must exist. */
  };
  
  /**
    Initializes the open file dialog window.
  */
  OpenFileDialog() throw();

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
    Returns the file names.
  */
  inline Array<String> getFilenames() const throw() {
    return filenames;
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
    Sets the filter.

    @param description The filter description.
    @param filter The filter.
  */
  void setFilter(const String& description, const String& filter) throw();
  
  /**
    Returns the filters.
  */
  inline Map<String, String> getFilters() const throw() {
    return filters;
  }
  
  /**
    Sets the filters.
  */
  inline void setFilters(const Map<String, String>& filters) throw() {
    this->filters = filters;
  }
  
  /**
    Executes the dialog.
  */
  bool execute() throw(UserInterfaceException);

  ~OpenFileDialog() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
