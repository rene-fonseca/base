/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/sound/SoundInputStream.h>
#include <base/UnexpectedFailure.h>
#include <base/io/IOException.h>
#include <base/sound/SoundDevice.h>
#include <base/concurrency/ExclusiveSynchronize.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #define NO_STRICT
  // I don't get it: in STRICT mode handles are of type int but in NO_STRICT
  // mode the handles are of size void*. This is a problem on 64 bit platforms
  // where int and void* may be of different sizes.
  #include <windows.h>
#elif (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__UNIX)
  #include <sys/types.h> // open
  #include <sys/stat.h> // open
  #include <fcntl.h> // open
  #include <unistd.h> // read
  #include <errno.h> // errno
  #include <limits.h> // SSIZE_MAX

  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__LINUX)
    #include <sys/ioctl.h> // ioctl
    #include <sys/soundcard.h> // ioctl
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    #include <stropts.h> // ioctl
    #include <sys/conf.h> // ioctl
    #include <sys/audio.h>
    #include <sys/audioio.h>
  #endif // os
#endif // flavour

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

SoundDevice SoundDevice::soundDevice;

SoundDevice::SoundDevice() throw() : access(0) {
}

void SoundDevice::reacquireAccess(unsigned int access) throw(NotSupported) {
  // guarded externally
  if (this->access != access) {
    this->access = access;
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__UNIX)
  #if ((_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__LINUX) || (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS))
    deviceDescriptor.close(); // must close before reopening
    if (access != 0) { // only open if required
      int flags;
      switch (access) {
      case READ:
        flags = O_RDONLY;
        break;
      case WRITE:
        flags = O_WRONLY;
        break;
      case READ|WRITE:
        flags = O_RDWR;
        break;
      }
      OperatingSystem::Handle handle = ::open("/dev/audio", flags); // linux and solaris
      assert(handle != OperatingSystem::INVALID_HANDLE, NotSupported());
      deviceDescriptor.setHandle(handle);
    }
  #endif // os
#endif // flavour
  }
}

void SoundDevice::acquireReadAccess() throw(IOException) {
  ExclusiveSynchronize<SoundDevice> exclusiveSynchronize(*this);
  if (!isReadable()) {
    reacquireAccess(access | READ);
  }
}

void SoundDevice::relinquishReadAccess() throw(IOException) {
  ExclusiveSynchronize<SoundDevice> exclusiveSynchronize(*this);
  if (isReadable()) {
    reacquireAccess(access & ~READ);
  }
}

void SoundDevice::acquireWriteAccess() throw(IOException) {
  ExclusiveSynchronize<SoundDevice> exclusiveSynchronize(*this);
  if (!isWriteable()) {
    reacquireAccess(access | WRITE);
  }
}

void SoundDevice::relinquishWriteAccess() throw(IOException) {
  ExclusiveSynchronize<SoundDevice> exclusiveSynchronize(*this);
  if (isWriteable()) {
    reacquireAccess(access & ~WRITE);
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
