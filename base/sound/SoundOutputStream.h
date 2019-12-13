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
#include <base/OutOfDomain.h>
#include <base/NotSupported.h>
#include <base/concurrency/Event.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Sound output stream.

  @short Sound output stream.
  @see SoundInputStream
  @ingroup sound
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API SoundOutputStream : public Object {
private:

  /** Handle to the sound output device. */
  OperatingSystem::Handle handle;
  /** Event. */
  Event event;

  class _COM_AZURE_DEV__BASE__API SoundOutputStreamHelper;
public:

  /**
    Initializes the sound output device. Common sampling rates are 5000Hz,
    8000Hz, 11025Hz, 22050Hz, 44100Hz, and 48000Hz.

    @param samplingRate The sampling rate specified in samples per second.
    @param channels The number of channels (must be greater than 0).
  */
  SoundOutputStream(unsigned int samplingRate, unsigned int channels);

  /**
    Returns the number of channels (greater than 0).
  */
  unsigned int getChannels() const noexcept;

  /**
    Returns the sampling rate (samples per second).
  */
  unsigned int getRate() const noexcept;

  /**
    Returns the total number of samples since the last resume.
  */
  unsigned int getPosition() const noexcept;

  /**
    Resumes the stream.
  */
  void resume() noexcept;

  /**
    Pauses the stream.
  */
  void pause() noexcept;

  /**
    Resets the device.
  */
  void reset() noexcept;

  /**
    Wait for all the samples to be played back.
  */
  void wait() noexcept;

  /**
    Writes bytes in buffer to stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
    @return The actual number of bytes written.
  */
  unsigned int write(const void* buffer, unsigned int size) noexcept;

  /**
    Destructs the stream object.
  */
  ~SoundOutputStream() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
