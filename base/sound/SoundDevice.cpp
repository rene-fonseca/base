/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/sound/SoundInputStream.h>
#include <base/UnexpectedFailure.h>
#include <base/io/IOException.h>
#include <base/sound/SoundDevice.h>
#include <base/concurrency/ExclusiveSynchronize.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  define NO_STRICT
  // I don't get it: in STRICT mode handles are of type int but in NO_STRICT
  // mode the handles are of size void*. This is a problem on 64 bit platforms
  // where int and void* may be of different sizes.
#  include <windows.h>
#elif (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__UNIX)
#  include <sys/types.h> // open
#  include <sys/stat.h> // open
#  include <fcntl.h> // open
#  include <unistd.h> // read
#  include <errno.h> // errno
#  include <limits.h> // SSIZE_MAX

#  if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
#    include <sys/ioctl.h> // ioctl
#    include <sys/soundcard.h> // ioctl
#  elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
#    include <stropts.h> // ioctl
#    include <sys/conf.h> // ioctl
#    include <sys/audio.h>
#    include <sys/audioio.h>
#  endif // os
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

SoundDevice::SoundDevice() noexcept
  : access(0)
{
}

void SoundDevice::reacquireAccess(unsigned int access)
{
  // guarded externally
  if (this->access != access) {
    this->access = access;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__UNIX)
  #if ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX) || (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS))
    deviceDescriptor.close(); // must close before reopening
    if (access != 0) { // only open if required
      int flags = 0;
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
      bassert(handle != OperatingSystem::INVALID_HANDLE, NotSupported());
      deviceDescriptor.setHandle(handle);
    }
  #endif // os
#endif // flavor
  }
}

void SoundDevice::acquireReadAccess()
{
  ExclusiveSynchronize<ReadWriteLock> _guard(guard);
  if (!isReadable()) {
    reacquireAccess(access | READ);
  }
}

void SoundDevice::relinquishReadAccess()
{
  ExclusiveSynchronize<ReadWriteLock> _guard(guard);
  if (isReadable()) {
    reacquireAccess(access & ~READ);
  }
}

void SoundDevice::acquireWriteAccess()
{
  ExclusiveSynchronize<ReadWriteLock> _guard(guard);
  if (!isWriteable()) {
    reacquireAccess(access | WRITE);
  }
}

void SoundDevice::relinquishWriteAccess()
{
  ExclusiveSynchronize<ReadWriteLock> _guard(guard);
  if (isWriteable()) {
    reacquireAccess(access & ~WRITE);
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
