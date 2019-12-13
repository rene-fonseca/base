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
  Open file dialog.
  
  @short Open file dialog.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OpenFileDialog : public Dialog {
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
  OpenFileDialog() noexcept;

  /**
    Returns the flags.
  */
  inline unsigned int getFlags() const throw() {
    return flags;
  }

  /**
    Sets the flags.
  */
  inline void setFlags(unsigned int flags) noexcept {
    this->flags = flags;
  }
  
  /**
    Returns the title of the dialog window.
  */
  inline const String& getTitle() const throw() {
    return title;
  }

  /**
    Sets the title of the dialog window.
  */
  inline void setTitle(const String& title) noexcept {
    this->title = title;
  }

  /**
    Returns the file name.
  */
  inline const String& getFilename() const throw() {
    return filename;
  }
  
  /**
    Sets the current file name.
  */
  inline void setFilename(const String& filename) noexcept {
    this->filename = filename;
  }
  
  /**
    Returns the file names.
  */
  inline const Array<String>& getFilenames() const throw() {
    return filenames;
  }
  
  /**
    Returns the initial folder.
  */
  inline const String& getFolder() const throw() {
    return folder;
  }
  
  /**
    Sets the initial folder.
  */
  inline void setFolder(const String& folder) noexcept {
    this->folder = folder;
  }
  
  /**
    Sets the filter.

    @param description The filter description.
    @param filter The filter.
  */
  void setFilter(const String& description, const String& filter) noexcept;
  
  /**
    Returns the filters.
  */
  inline const Map<String, String>& getFilters() const throw() {
    return filters;
  }
  
  /**
    Sets the filters.
  */
  inline void setFilters(const Map<String, String>& filters) noexcept {
    this->filters = filters;
  }
  
  /**
    Executes the dialog.
  */
  bool execute() throw(UserInterfaceException);

  ~OpenFileDialog() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
