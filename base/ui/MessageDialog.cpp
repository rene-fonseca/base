/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/MessageDialog.h>
#include <base/concurrency/Thread.h>
#include <base/UnexpectedFailure.h>
#include <base/NotImplemented.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  define WINVER 0x0500
#  include <windows.h>
#  undef IGNORE
#else // unix
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

MessageDialog::MessageDialog() throw() : category(MessageDialog::INFORMATION), answer(MessageDialog::CANCEL) {
}

MessageDialog::MessageDialog(const String& _title, const String& _message, Category _category) throw()
  : title(_title), message(_message), category(_category), answer(MessageDialog::CANCEL) {
}

bool MessageDialog::execute() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  static const unsigned int NATIVE_TYPES[] = {
    MB_OK | MB_ICONWARNING, // WARNING
    MB_YESNO | MB_ICONQUESTION, // QUESTION
    MB_OK | MB_ICONERROR, // ERROR
    MB_OK | MB_ICONEXCLAMATION, // EXCLAMATION
    MB_OK | MB_ICONINFORMATION, // INFORMATION
  };
  int result = ::MessageBox(0, message.getElements(), title.getElements(), MB_TOPMOST | MB_SETFOREGROUND | NATIVE_TYPES[category]);
  switch (result) {
  case IDABORT:
    answer = MessageDialog::ABORT;
    break;
  case IDCANCEL:
    answer = MessageDialog::CANCEL;
    break;
  case IDCONTINUE:
    answer = MessageDialog::CONTINUE;
    break;
  case IDIGNORE:
    answer = MessageDialog::IGNORE;
    break;
  case IDNO:
    answer = MessageDialog::NO;
    break;
  case IDOK:
    answer = MessageDialog::OK;
    break;
  case IDRETRY:
    answer = MessageDialog::RETRY;
    break;
  case IDTRYAGAIN:
    answer = MessageDialog::RETRY;
    break;
  case IDYES:
    answer = MessageDialog::YES;
    break;
  default:
    throw UserInterfaceException(this);
  }
  return answer == MessageDialog::CANCEL;
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

MessageDialog::~MessageDialog() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
