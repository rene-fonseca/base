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
#include <base/dl/DynamicLinker.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <base/platforms/win32/Helpers.h>
#  include <windows.h>
#else // unix
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

SaveFileDialog::SaveFileDialog() noexcept
  : flags(SaveFileDialog::ASK_TO_OVERWRITE), defaultFilter(0)
{
}

void SaveFileDialog::setFilter(const String& description, const String& filter) noexcept
{
  defaultFilter = 0;
  filters.add(description, filter);
}

void SaveFileDialog::setFilters(const Map<String, String>& filters) noexcept
{
  defaultFilter = 0;
  this->filters = filters;
}

bool SaveFileDialog::execute()
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

  String temp;
  Map<String, String>::ReadEnumerator enu = this->filters.getReadEnumerator();
  while (enu.hasNext()) {
    const Map<String, String>::Node* node = enu.next();
    // terminator not allowed in value and key
    if (node->getValue().indexOf('\0') >= 0) {
      _throw UserInterfaceException();
    }
    if (node->getKey().indexOf('\0') >= 0) {
      _throw UserInterfaceException();
    }
    temp += node->getValue();
    temp += '\0';
    temp += node->getKey();
    temp += '\0';
  }
  temp += '\0'; // final termination
  temp += '\0'; // final termination
  const ToWCharString filters(temp);
  
  const ToWCharString _filename(filename);
  PrimitiveStackArray<wchar> buffer(maximum<MemorySize>(_filename.getLength() + 1, 4096/sizeof(wchar)));
  copy<wchar>(
    static_cast<wchar*>(buffer), _filename, _filename.getLength() + 1
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
    folder = String(saveFile.lpstrFile, saveFile.nFileOffset);
    filename = String(saveFile.lpstrFile); // preserved folder
    defaultFilter = saveFile.nFilterIndex;
  }
  return result != 0;
#else // unix
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif // flavor
}

SaveFileDialog::~SaveFileDialog() noexcept {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE

// TAG: remove folder attribute if not required
