/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/SaveFileDialog.h>
#include <base/concurrency/Thread.h>
#include <base/UnexpectedFailure.h>
#include <base/NotImplemented.h>
#include <base/dl/DynamicLinker.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#else // unix
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

SaveFileDialog::SaveFileDialog() throw() : flags(SaveFileDialog::ASK_TO_OVERWRITE), defaultFilter(0) {
}

void SaveFileDialog::setFilter(const String& description, const String& filter) throw() {
  defaultFilter = 0;
  filters[description] = filter;
}

void SaveFileDialog::setFilters(const Map<String, String>& filters) throw() {
  defaultFilter = 0;
  this->filters = filters;
}

bool SaveFileDialog::execute() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  typedef BOOL (WINAPI *FGetSaveFileNameA)(LPOPENFILENAME);
  static FGetSaveFileNameA GetSaveFileNameA = 0;
  if (!GetSaveFileNameA) { // TAG: need to be atomic
    DynamicLinker* dynamicLinker = new DynamicLinker(MESSAGE("comdlg32.dll"));
    GetSaveFileNameA = (FGetSaveFileNameA)dynamicLinker->getSymbol(MESSAGE("GetSaveFileNameA")); // TAG: fix cast
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
  
  OPENFILENAME saveFile;
  clear(saveFile);
  saveFile.lStructSize = sizeof(saveFile);
  saveFile.lpstrFilter = filters;
  saveFile.nFilterIndex = defaultFilter; // select custom
  
  saveFile.lpstrFile = buffer->getElements();
  saveFile.nMaxFile = buffer->getSize();
  saveFile.lpstrInitialDir = folder.isProper() ? folder.getElements() : 0;
  saveFile.lpstrTitle = title.isProper() ? title.getElements() : 0;
  saveFile.Flags |= (flags & SaveFileDialog::ASK_TO_OVERWRITE) ? OFN_OVERWRITEPROMPT : 0;
  saveFile.Flags |= (flags & SaveFileDialog::ASK_TO_CREATE) ? OFN_CREATEPROMPT : 0;
  
  BOOL result = GetSaveFileNameA(&saveFile);
  if (result != 0) {
    folder = String(saveFile.lpstrFile, saveFile.nFileOffset);
    filename = String(saveFile.lpstrFile); // preserved folder
    defaultFilter = saveFile.nFilterIndex;
  }
  return result != 0;
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

SaveFileDialog::~SaveFileDialog() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

// TAG: remove folder attribute if not required
