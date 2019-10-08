/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/SelectFolderDialog.h>
#include <base/concurrency/Thread.h>
#include <base/UnexpectedFailure.h>
#include <base/NotImplemented.h>
#include <base/dl/DynamicLinker.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <base/platforms/win32/Helpers.h>
#  include <windows.h>
#  include <objbase.h>
#  include <shlobj.h>
#else // unix
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

SelectFolderDialog::SelectFolderDialog() throw() {
}

bool SelectFolderDialog::execute() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  typedef LPITEMIDLIST (WINAPI *FSHBrowseForFolderW)(LPBROWSEINFO);
  static FSHBrowseForFolderW SHBrowseForFolderW = nullptr;
  if (!SHBrowseForFolderW) { // TAG: need to be atomic
    DynamicLinker* dynamicLinker = new DynamicLinker(MESSAGE("shell32.dll"));
    SHBrowseForFolderW = (FSHBrowseForFolderW)dynamicLinker->getSymbol(MESSAGE("SHBrowseForFolderW"));
  }
  
  HRESULT r = ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
  bassert(r == S_OK, UserInterfaceException(this));
  
  wchar buffer[MAX_PATH];
  BROWSEINFO browseInfo;
  clear(browseInfo);
  browseInfo.pszDisplayName = buffer;
  browseInfo.pidlRoot = 0; // start folder
  OSString _title(title);
  browseInfo.lpszTitle = _title;
  browseInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_BROWSEFORCOMPUTER | BIF_USENEWUI | BIF_SHAREABLE; // TAG: fixme
  
  LPITEMIDLIST result = SHBrowseForFolderW(&browseInfo);
  if (result != 0) {
    // BOOL SHGetPathFromIDList(LPCITEMIDLIST pidl, LPTSTR pszPath);
    folder = toUTF8(buffer);
    LPMALLOC malloc;
    /*HRESULT r =*/ ::CoGetMalloc(1, &malloc);
    malloc->Free(result);
  }
  return result != 0;
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

SelectFolderDialog::~SelectFolderDialog() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
