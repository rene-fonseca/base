/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

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

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <base/platforms/win32/Helpers.h>
#  include <windows.h>
#else // unix
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

SaveFileDialog::SaveFileDialog() throw()
  : flags(SaveFileDialog::ASK_TO_OVERWRITE), defaultFilter(0)
{
}

void SaveFileDialog::setFilter(const String& description, const String& filter) throw()
{
  defaultFilter = 0;
  filters.add(description, filter);
}

void SaveFileDialog::setFilters(const Map<String, String>& filters) throw()
{
  defaultFilter = 0;
  this->filters = filters;
}

bool SaveFileDialog::execute() throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  typedef BOOL (WINAPI *FGetSaveFileNameW)(LPOPENFILENAME);
  static FGetSaveFileNameW GetSaveFileNameW = 0;
  if (!GetSaveFileNameW) { // TAG: need to be atomic
    DynamicLinker* dynamicLinker = new DynamicLinker("comdlg32.dll");
    GetSaveFileNameW = (FGetSaveFileNameW)dynamicLinker->getSymbol(
      Literal("GetSaveFileNameW")
    ); // TAG: fix cast
  }

  wchar filters[4096]; // TAG: fixme
  wchar* dest = filters;
  Map<String, String>::ReadEnumerator enu = this->filters.getReadEnumerator();
  while (enu.hasNext()) {
    const Map<String, String>::Node* node = enu.next();
    const std::wstring value(toWide(node->getValue()));
    const std::wstring key(toWide(node->getKey()));
    copy(
      dest,
      value.c_str(),
      value.size() + 1
    ); // include terminator
    dest += value.size() + 1;
    copy(
      dest,
      key.c_str(),
      key.size() + 1
    ); // include terminator
    dest += key.size() + 1;
  }
  *dest++ = L'\0'; // final termination;
  *dest++ = L'\0'; // final termination;
  
  PrimitiveStackArray<wchar> buffer(4096);
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
  
  OPENFILENAME saveFile;
  clear(saveFile);
  saveFile.lStructSize = sizeof(saveFile);
  saveFile.lpstrFilter = filters;
  saveFile.nFilterIndex = defaultFilter; // select custom
  
  saveFile.lpstrFile = buffer;
  saveFile.nMaxFile = buffer.size();
  OSString _folder(folder);
  saveFile.lpstrInitialDir = !_folder.empty() ? _folder : 0;
  OSString _title(title);
  saveFile.lpstrTitle = !_title.empty() ? _title : 0;
  saveFile.Flags |= (flags & SaveFileDialog::ASK_TO_OVERWRITE) ? OFN_OVERWRITEPROMPT : 0;
  saveFile.Flags |= (flags & SaveFileDialog::ASK_TO_CREATE) ? OFN_CREATEPROMPT : 0;
  
  BOOL result = GetSaveFileNameW(&saveFile);
  if (result != 0) {
    folder = toUTF8(saveFile.lpstrFile, saveFile.nFileOffset);
    filename = toUTF8(saveFile.lpstrFile); // preserved folder
    defaultFilter = saveFile.nFilterIndex;
  }
  return result != 0;
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

SaveFileDialog::~SaveFileDialog() throw() {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE

// TAG: remove folder attribute if not required
