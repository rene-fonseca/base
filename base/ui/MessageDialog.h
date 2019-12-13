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
#include <base/ui/Dialog.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Message dialog.
  
  @short Message dialog.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API MessageDialog : public Dialog {
public:

  /** The message type. */
  enum Category {
    WARNING, /**< A warning. */
    QUESTION, /**< A question. */
    ERROR, /**< An error message. */
    EXCLAMATION, /**< An important informative message. */
    INFORMATION /**< A normal informative message. */
  };

  /** The answer. */
  enum Answer {
    ABORT, /**< Abort. */
    CANCEL, /**< Cancel. */
    CONTINUE, /**< Continue. */
    IGNORE, /**< Ignore. */
    NO, /**< No. */
    OK, /**< Ok. */
    RETRY, /**< Retry. */
    YES /**< Yes. */
  };
private:

  /** Type. */
  Category category = ERROR;
  /** Specifies the title of the dialog. */
  String title;
  /** Specifies the message. */
  String message;
  /** Answer. */
  Answer answer = ABORT;
public:
  
  /**
    Initializes the message dialog.
  */
  MessageDialog() noexcept;

  /**
    Initializes the message dialog.
  */
  MessageDialog(const String& title, const String& message, Category category) noexcept;
  
  /**
    Returns the message category.
  */
  inline Category getCategory() const noexcept {
    return category;
  }

  /**
    Sets the flags.
  */
  inline void setCategory(Category category) noexcept {
    this->category =category ;
  }
  
  /**
    Returns the title of the dialog window.
  */
  inline const String& getTitle() const noexcept {
    return title;
  }

  /**
    Sets the title of the dialog window.
  */
  inline void setTitle(const String& title) noexcept {
    this->title = title;
  }

  /**
    Returns the message.
  */
  inline const String& getMessage() const noexcept {
    return message;
  }
  
  /**
    Sets the current folder.
  */
  inline void setMessage(const String& message) noexcept {
    this->message = message;
  }
  
  /**
    Executes the dialog.
  */
  bool execute() throw(UserInterfaceException);

  /**
    Returns the answer.
  */
  inline Answer getAnswer() noexcept {
    return answer;
  }

  ~MessageDialog() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
