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
#include <base/ui/Dialog.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Message dialog.
  
  @short Message dialog.
  @ingroup ui
  @version 1.0
*/

class MessageDialog : public Dialog {
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
  Category category;
  /** Specifies the title of the dialog. */
  String title;
  /** Specifies the message. */
  String message;
  /** Answer. */
  Answer answer;
public:
  
  /**
    Initializes the message dialog.
  */
  MessageDialog() throw();

  /**
    Initializes the message dialog.
  */
  MessageDialog(const String& title, const String& message, Category category) throw();
  
  /**
    Returns the message category.
  */
  inline Category getCategory() const throw() {
    return category;
  }

  /**
    Sets the flags.
  */
  inline void setCategory(Category category) throw() {
    this->category =category ;
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
    Returns the message.
  */
  inline String getMessage() const throw() {
    return message;
  }
  
  /**
    Sets the current folder.
  */
  inline void setMessage(const String& message) throw() {
    this->message = message;
  }
  
  /**
    Executes the dialog.
  */
  bool execute() throw(UserInterfaceException);

  /**
    Returns the answer.
  */
  inline Answer getAnswer() throw() {
    return answer;
  }

  ~MessageDialog() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
