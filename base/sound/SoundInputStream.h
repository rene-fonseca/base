/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_SOUND__SOUND_INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_SOUND__SOUND_INPUT_STREAM_H

#include <base/Object.h>
#include <base/OperatingSystem.h>
#include <base/OutOfDomain.h>
#include <base/NotSupported.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Sound input stream.

  @see SoundOutputStream.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class SoundInputStream : public Object {
private:

  /** Handle to the sound input device. */
  OperatingSystem::Handle handle;

  class SoundInputStreamHelper;
public:

  /**
    Initializes the sound input device. Common sampling rates are 5000Hz,
    8000Hz, 11025Hz, 22050Hz, 44100Hz, and 48000Hz.

    @param samplingRate The sampling rate specified in samples per second.
    @param channels The number of channels (must be greater than 0).
  */
  SoundInputStream(unsigned int samplingRate, unsigned int channels) throw(OutOfDomain, NotSupported);

  /**
    Returns the number of available samples.
  */
  unsigned int available() const throw();

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
    Starts the sampling.
  */
  void resume() throw();

  /**
    Stops the sampling.
  */
  void pause() throw();

  /**
    Resets the device.
  */
  void reset() throw();

  /**
    Fills the specified buffer with bytes from the stream. In blocking mode the
    method does not return until all bytes have been read. In non-blocking mode
    the total bytes read may be any number below or equal to the requested
    number of bytes. Attempting to read beyond the end of a stream in blocking
    mode results in an EndOfFile exception.

    @param buffer The buffer to receive the bytes.
    @param size The size of the buffer.
    @return The actual number of bytes read from the stream.
  */
  unsigned int read(void* buffer, unsigned int size) throw();

  /**
    Destructs the stream object.
  */
  ~SoundInputStream() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
