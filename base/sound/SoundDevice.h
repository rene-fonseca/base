/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/OperatingSystem.h>
#include <base/NotSupported.h>
#include <base/concurrency/ReadWriteLock.h>
#include <base/io/FileDescriptor.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  @defgroup sound Sound
*/

/**
  Sound device (singleton).
  
  @short Sound device.
  @see SoundInputStream SoundOutputStream.
  @ingroup sound
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API SoundDevice : public virtual Object {
  friend class SoundInputStream;
  friend class SoundOutputStream;
private:

  /** The sound device handle. */
  static SoundDevice soundDevice;

  /** Access flags. */
  enum Access {
    READ = 1, /**< Read access to device. */
    WRITE = 2 /**< Write access to device. */
  };

  /** Handle to the sound device. */
  FileDescriptor deviceDescriptor;
  /** The current access to the device.*/
  unsigned int access = 0;
  /** Synchronization object. */
  ReadWriteLock guard;
  
  /**
    Acquires the desired access to the device.
  */
  void reacquireAccess(unsigned int access);
public:

  /**
    Initializes the sound device.
  */
  SoundDevice() noexcept;

  /**
    Returns true if the device is readable.
  */
  inline bool isReadable() const noexcept
  {
    return (access & READ) != 0;
  }

  /**
    Returns true if the device is writeable.
  */
  inline bool isWriteable() const noexcept
  {
    return (access & WRITE) != 0;
  }

  /**
    Acquires read access to the device.
  */
  void acquireReadAccess();

  /**
    Releases the read access to the device.
  */
  void relinquishReadAccess();

  /**
    Acquires write access to the device.
  */
  void acquireWriteAccess();

  /**
    Releases the write access to the device.
  */
  void relinquishWriteAccess();

  /**
    Returns the handle associated with read access from device (may be identical
    to the write handle).
  */
  inline OperatingSystem::Handle getReadHandle() const noexcept
  {
    return deviceDescriptor.getHandle();
  }

  /**
    Returns the handle associated with write access to device (may be identical
    to the read handle).
  */
  inline OperatingSystem::Handle getWriteHandle() const noexcept
  {
    return deviceDescriptor.getHandle();
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
