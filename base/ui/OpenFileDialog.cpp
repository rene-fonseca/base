/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/OpenFileDialog.h>
#include <base/concurrency/Thread.h>
#include <base/UnexpectedFailure.h>
#include <base/NotImplemented.h>
#include <base/dl/DynamicLinker.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#else // unix
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

OpenFileDialog::OpenFileDialog() throw() : flags(OpenFileDialog::MUST_EXIST), defaultFilter(0) {
}

void OpenFileDialog::setFilter(const String& description, const String& filter) throw() {
  filters[description] = filter;
}

bool OpenFileDialog::execute() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  typedef BOOL (WINAPI *FGetOpenFileNameA)(LPOPENFILENAME);
  static FGetOpenFileNameA GetOpenFileNameA = 0;
  if (!GetOpenFileNameA) { // TAG: need to be atomic
    DynamicLinker* dynamicLinker = new DynamicLinker(MESSAGE("comdlg32.dll"));
    GetOpenFileNameA = (FGetOpenFileNameA)dynamicLinker->getSymbol(MESSAGE("GetOpenFileNameA")); // TAG: fix cast
  }
  
  char filters[4096]; // TAG: fixme
  char* dest = filters;
  Map<String, String>::ReadEnumerator enu = this->filters.getReadEnumerator();
  while (enu.hasNext()) {
    const Map<String, String>::Node* node = enu.next();
    copy(dest, node->getValue()->getElements(), node->getValue()->getLength() + 1); // include terminator
    dest += node->getValue()->getLength() + 1;
    copy(dest, node->getKey()->getElements(), node->getKey()->getLength() + 1); // include terminator
    dest += node->getKey()->getLength() + 1;
  }
  *dest++ = '\0'; // final termination;
  *dest++ = '\0'; // final termination;
  
  Allocator<char>* buffer = Thread::getLocalStorage();
  assert(buffer->getSize() >= 256, UnexpectedFailure("Thread local buffer is too small", this));
  copy(buffer->getElements(), filename.getElements(), filename.getLength() + 1); // includes terminator
  
  OPENFILENAME openFile;
  clear(openFile);
  openFile.lStructSize = sizeof(openFile);
  openFile.lpstrFilter = filters;
  openFile.nFilterIndex = defaultFilter; // select custom
  
  openFile.lpstrFile = buffer->getElements();
  openFile.nMaxFile = buffer->getSize();
  openFile.lpstrInitialDir = folder.isProper() ? folder.getElements() : 0;
  openFile.lpstrTitle = title.isProper() ? title.getElements() : 0;
  openFile.Flags |= (flags & OpenFileDialog::MUST_EXIST) ? OFN_FILEMUSTEXIST : 0; // OFN_PATHMUSTEXIST
  openFile.Flags |= (flags & OpenFileDialog::ALLOW_MULTIPLE) ? OFN_ALLOWMULTISELECT : 0;
  
  BOOL result = GetOpenFileNameA(&openFile);
  if (result != 0) {
    filenames.removeAll();
    if (flags & OpenFileDialog::ALLOW_MULTIPLE) {
      const char* src = openFile.lpstrFile;
      folder = src;
      src += openFile.nFileOffset;
      while (*src) {
        String temp(src);
        src += temp.getLength() + 1;
        filenames.append(temp);
      }
    } else {
      folder = String(openFile.lpstrFile, openFile.nFileOffset);
      filename = String(openFile.lpstrFile); // preserved folder
    }
    defaultFilter = openFile.nFilterIndex;
  }
  return result != 0;
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

OpenFileDialog::~OpenFileDialog() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

// TAG: remove folder attribute if not required
