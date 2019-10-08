/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

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
#  include <base/platforms/win32/Helpers.h>
#  include <windows.h>
#else // unix
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

OpenFileDialog::OpenFileDialog() throw()
  : flags(OpenFileDialog::MUST_EXIST), defaultFilter(0) {
}

void OpenFileDialog::setFilter(
  const String& description, const String& filter) throw() {
  filters[description] = filter;
}

bool OpenFileDialog::execute() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  typedef BOOL (WINAPI *FGetOpenFileNameW)(LPOPENFILENAME);
  static FGetOpenFileNameW GetOpenFileNameW = nullptr;
  if (!GetOpenFileNameW) { // TAG: need to be atomic
    DynamicLinker* dynamicLinker = new DynamicLinker("comdlg32.dll");
    GetOpenFileNameW = (FGetOpenFileNameW)dynamicLinker->getSymbol(
      Literal("GetOpenFileNameW")
    ); // TAG: fix cast
  }
  
  wchar filters[4096]; // TAG: fixme
  wchar* dest = filters;
  Map<String, String>::ReadEnumerator enu = this->filters.getReadEnumerator();
  while (enu.hasNext()) {
    const Map<String, String>::Node* node = enu.next();
    const std::wstring value = toWide(node->getValue());
    const std::wstring key = toWide(node->getKey());
    copy(dest, value.c_str(), value.size() + 1); // include terminator
    dest += value.size() + 1;
    copy(dest, key.c_str(), key.size() + 1); // include terminator
    dest += key.size() + 1;
  }
  *dest++ = L'\0'; // final termination;
  *dest++ = L'\0'; // final termination;
  
  PrimitiveArray<wchar> buffer(4096);
  bassert(
    buffer.size() >= 256,
    UnexpectedFailure("Thread local buffer is too small", this)
  );
  const std::wstring _filename(toWide(filename));
  copy(
    static_cast<wchar*>(buffer),
    _filename.c_str(),
    _filename.size() + 1
  ); // includes terminator
  
  OPENFILENAME openFile;
  clear(openFile);
  openFile.lStructSize = sizeof(openFile);
  openFile.lpstrFilter = filters;
  openFile.nFilterIndex = defaultFilter; // select custom
  
  openFile.lpstrFile = (wchar*)buffer;
  openFile.nMaxFile = buffer.size();
  OSString _folder(folder);
  openFile.lpstrInitialDir = !_folder.empty() ? _folder : nullptr;
  OSString _title(title);
  openFile.lpstrTitle = !_title.empty() ? _title : nullptr;
  openFile.Flags |= (flags & OpenFileDialog::MUST_EXIST) ? OFN_FILEMUSTEXIST : 0; // OFN_PATHMUSTEXIST
  openFile.Flags |= (flags & OpenFileDialog::ALLOW_MULTIPLE) ? OFN_ALLOWMULTISELECT : 0;
  
  BOOL result = GetOpenFileNameW(&openFile);
  if (result != 0) {
    filenames.removeAll();
    if (flags & OpenFileDialog::ALLOW_MULTIPLE) {
      const wchar* src = openFile.lpstrFile;
      folder = toUTF8(src);
      src += openFile.nFileOffset;
      while (*src) {
        String temp(src);
        src += temp.getLength() + 1;
        filenames.append(temp);
      }
    } else {
      folder = String(toUTF8(openFile.lpstrFile, openFile.nFileOffset));
      filename = String(toUTF8(openFile.lpstrFile)); // preserved folder
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
