/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_SOUND__SOUND_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_SOUND__SOUND_OUTPUT_STREAM_H

#include <base/Object.h>
#include <base/OperatingSystem.h>
#include <base/OutOfDomain.h>
#include <base/NotSupported.h>
#include <base/concurrency/Event.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Sound output stream.

  @see SoundInputStream.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class SoundOutputStream : public Object {
private:

  /** Handle to the sound output device. */
  OperatingSystem::Handle handle;
  Event event;

  class SoundOutputStreamHelper;
public:

  /**
    Initializes the sound output device. Common sampling rates are 5000Hz,
    8000Hz, 11025Hz, 22050Hz, 44100Hz, and 48000Hz.

    @param samplingRate The sampling rate specified in samples per second.
    @param channels The number of channels (must be greater than 0).
  */
  SoundOutputStream(unsigned int samplingRate, unsigned int channels) throw(OutOfDomain, NotSupported);

  /**
    Returns the number of channels (greater than 0).
  */
  unsigned int getChannels() const throw();

  /**
    Returns the sampling rate (samples per second).
  */
  unsigned int getRate() const throw();

  /**
    Returns the total number of samples since the last resume.
  */
  unsigned int getPosition() const throw();

  /**
    Resumes the stream.
  */
  void resume() throw();

  /**
    Pauses the stream.
  */
  void pause() throw();

  /**
    Resets the device.
  */
  void reset() throw();

  /**
    Wait for all the samples to be played back.
  */
  void wait() throw();

  /**
    Writes bytes in buffer to stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
    @return The actual number of bytes written.
  */
  unsigned int write(const void* buffer, unsigned int size) throw();

  /**
    Destructs the stream object.
  */
  ~SoundOutputStream() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
