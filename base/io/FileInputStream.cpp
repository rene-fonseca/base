/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/FileInputStream.h>
#include <base/io/EndOfFile.h>
#include <base/Base.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileInputStream::FileInputStream(const String& path, bool exclusive) throw(FileNotFound) :
  file(path, File::READ, exclusive ? File::EXCLUSIVE : 0), end(false) {
}

unsigned int FileInputStream::available() const throw(FileException) {
  long long size = file.getSize();
  long long position = file.getPosition();
  if (position >= size) {
    return 0;
  }
  return minimum<long long>(size - position, UINT_MAX);
}

void FileInputStream::close() throw(FileException) {
  file.close();
}

unsigned int FileInputStream::skip(unsigned int count) throw(FileException) {
  file.setPosition(count, File::CURRENT); // may skip past end of file
  return count;
}

void FileInputStream::wait() const throw(FileException) {
  // data is always available?
}

bool FileInputStream::wait(unsigned int timeout) const throw(FileException) {
  // data is always available?
  return true;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
