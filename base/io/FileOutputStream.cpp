/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/FileOutputStream.h>

#if defined(__win32__)
  #include <windows.h>
#else // __unix
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileOutputStream::FileOutputStream(const String& p, unsigned int options, unsigned int permissions) throw(FileNotFound) :
  file(p, File::WRITE, options) {
}

FileOutputStream::FileOutputStream(const FileOutputStream& copy) throw() : file(copy.file) {
}

FileOutputStream& FileOutputStream::operator=(const FileOutputStream& eq) throw() {
  if (&eq == this) {
    file = eq.file;
  }
  return *this;
}

void FileOutputStream::close() throw(IOException) {
  file.close();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
