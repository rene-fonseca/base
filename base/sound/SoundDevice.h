/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_SOUND__SOUND_DEVICE_H
#define _DK_SDU_MIP__BASE_SOUND__SOUND_DEVICE_H

#include <base/Object.h>
#include <base/OperatingSystem.h>
#include <base/NotSupported.h>
#include <base/concurrency/ReadWriteLock.h>
#include <base/io/FileDescriptor.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @defgroup sound Sound
*/

/**
  Sound device (singleton).
  
  @short Sound device.
  @see SoundInputStream SoundOutputStream.
  @ingroup sound
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class SoundDevice : public virtual Object {
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
  unsigned int access;
  /** Synchronization object. */
  ReadWriteLock guard;
  
  /**
    Acquires the desired access to the device.
  */
  void reacquireAccess(unsigned int access) throw(NotSupported);
public:

  /**
    Initializes the sound device.
  */
  SoundDevice() throw();

  /**
    Returns true if the device is readable.
  */
  inline bool isReadable() const throw() {
    return (access & READ) != 0;
  }

  /**
    Returns true if the device is writeable.
  */
  inline bool isWriteable() const throw() {
    return (access & WRITE) != 0;
  }

  /**
    Acquires read access to the device.
  */
  void acquireReadAccess() throw(IOException);

  /**
    Releases the read access to the device.
  */
  void relinquishReadAccess() throw(IOException);

  /**
    Acquires write access to the device.
  */
  void acquireWriteAccess() throw(IOException);

  /**
    Releases the write access to the device.
  */
  void relinquishWriteAccess() throw(IOException);

  /**
    Returns the handle associated with read access from device (may be identical
    to the write handle).
  */
  inline OperatingSystem::Handle getReadHandle() const throw() {
    return deviceDescriptor.getHandle();
  }

  /**
    Returns the handle associated with write access to device (may be identical
    to the read handle).
  */
  inline OperatingSystem::Handle getWriteHandle() const throw() {
    return deviceDescriptor.getHandle();
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
