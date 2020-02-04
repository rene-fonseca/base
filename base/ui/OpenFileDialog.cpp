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
#include <base/dl/DynamicLinker.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <base/platforms/win32/Helpers.h>
#  include <windows.h>
#else // unix
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

OpenFileDialog::OpenFileDialog() noexcept
  : flags(OpenFileDialog::MUST_EXIST), defaultFilter(0)
{
}

void OpenFileDialog::setFilter(const String& description, const String& filter) noexcept
{
  filters.add(description, filter);
}

namespace {

  /** Copies the null terminated sequence/string. */
  template<class TYPE>
  inline TYPE* copyNullTerminated(TYPE* restrict dest, const TYPE* restrict src)
  {
    BASSERT(dest && src);
    while (*dest) {
      *dest++ = *src++;
    }
    return dest;
  }
}

bool OpenFileDialog::execute()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  typedef BOOL (WINAPI *FGetOpenFileNameW)(LPOPENFILENAME);
  static FGetOpenFileNameW GetOpenFileNameW = nullptr;
  if (!GetOpenFileNameW) { // TAG: need to be atomic
    DynamicLinker* dynamicLinker = new DynamicLinker("comdlg32.dll");
    GetOpenFileNameW = (FGetOpenFileNameW)dynamicLinker->getSymbol(
      Literal("GetOpenFileNameW")
    ); // TAG: fix cast
  }
  
  String temp;
  Map<String, String>::ReadEnumerator enu = this->filters.getReadEnumerator();
  while (enu.hasNext()) {
    const Map<String, String>::Node& node = enu.next();
    // terminator not allowed in value and key
    if (node.getValue().indexOf('\0') >= 0) {
      _throw UserInterfaceException();
    }
    if (node.getKey().indexOf('\0') >= 0) {
      _throw UserInterfaceException();
    }
    temp += node.getValue();
    temp += '\0';
    temp += node.getKey();
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
  
  OPENFILENAME openFile;
  clear(openFile);
  openFile.lStructSize = sizeof(openFile);
  openFile.lpstrFilter = filters;
  openFile.nFilterIndex = defaultFilter; // select custom
  
  openFile.lpstrFile = static_cast<wchar*>(buffer);
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
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif // flavor
}

OpenFileDialog::~OpenFileDialog() noexcept {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE

// TAG: remove folder attribute if not required
