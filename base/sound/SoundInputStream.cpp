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

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
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

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
class SoundInputStream::SoundInputStreamHelper {
public:

  void CALLBACK callback(HWAVEIN handle, UINT message, DWORD instance, DWORD param1, DWORD param2) {
    // signal
  }
};
#endif // win32

SoundInputStream::SoundInputStream(unsigned int samplingRate, unsigned channels) throw(OutOfDomain, NotSupported) {
  assert(channels > 0, OutOfDomain());
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  WAVEFORMATEX format;
  clear(format);
  format.wFormatTag = WAVE_FORMAT_PCM;
  format.nChannels = channels;
  format.nSamplesPerSec = sampleRate;
  format.wBitsPerSample = 16;
  format.nBlockAlign = channels * format.wBitsPerSample/8;
  format.nAvgBytesPerSec = sampleRate * format.nBlockAlign;

  unsigned int result = ::waveInOpen(&handle, WAVE_MAPPER, &format, &SoundInputStreamHelper::callback, 0, CALLBACK_FUNCTION);
  assert(result == MMSYSERR_NOERROR, NotSupported());
#else
  handle = ::open("/dev/audio", O_RDONLY);
  assert(handle != -1, NotSupported());

  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__LINUX)
    int format = AFMT_S16_NE;
    assert(::ioctl(handle, SNDCTL_DSP_SETFMT, &format) == 0), UnexpectedFailure());
    assert(format == AFMT_S16_NE, NotSupported());

    unsigned int desiredChannels = channels;
    assert(::ioctl(handle, SNDCTL_DSP_CHANNELS, &desiredChannels) == 0), UnexpectedFailure());
    assert(desiredChannels == channels, NotSupported());

    unsigned int desiredRate = samplingRate;
    assert(::ioctl(handle, SNDCTL_DSP_SPEED, &desiredRate) == 0), UnexpectedFailure());
    assert(desiredRate == samplingRate, NotSupported());

  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    AUDIO_INITINFO(&info);
    info.record.pause = 1;
    info.record.rate = samplingRate;
    info.record.channels = channels;
    info.record.precision = 16;
    info.record.encoding = AUDIO_ENCODING_LINEAR; // PCM
    assert(::ioctl(handle, AUDIO_SETINFO, &info) == 0, UnexpectedFailure());

    assert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure()); // is this required
    assert(
      (info.record.rate == samplingRate) &&
      (info.record.channels == channels) &&
      (info.record.precision == 16) &&
      (info.record.encoding == AUDIO_ENCODING_LINEAR),
      NotSupported()
    );

    assert(::ioctl(handle, I_FLUSH, FLUSHR) == 0, UnexpectedFailure()); // should never fail
  #endif // os
#endif // flavour
}

unsigned int SoundInputStream::available() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  return 0;
#else
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__LINUX)
    return 0;
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    unsigned int available;
    assert(::ioctl(handle, I_NREAD, &available) == 0, UnexpectedFailure()); // should not fail
    return available & ~(channels * 2 - 1); // align to sample frame (channels * bitsPerSample/8)
  #endif // os
#endif // flavour
}

unsigned int SoundInputStream::getChannels() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  return 0;
#else
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__LINUX)
    int channels;
    assert(::ioctl(handle, SOUND_PCM_READ_CHANNELS, &channels) == 0), UnexpectedFailure());
    return channels;
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    assert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure()); // should never fail
    return info.record.channels;
  #endif // os
#endif // flavour
}

unsigned int SoundInputStream::getRate() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  return 0;
#else
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__LINUX)
    int rate;
    assert(::ioctl(handle, SOUND_PCM_READ_RATE, &rate) == 0), UnexpectedFailure());
    return rate;
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    assert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure()); // should never fail
    return info.record.sample_rate;
  #endif // os
#endif // flavour
}

unsigned int SoundInputStream::getPosition() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  MMTIME time;
  clear(time);
  time.wType = TIME_SAMPLES;
  ::waveInGetPosition(handle, &time, sizeof(time));
  assert(time.wType == TIME_SAMPLES, UnexpectedFailure());
  return time.u.sample;
#else
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__LINUX)
    return 0;
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    assert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure()); // should never fail
    return info.record.samples;
  #endif // os
#endif // flavour
}

void SoundInputStream::resume() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  ::waveInStart(handle);
#else
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__LINUX)
    reset();
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    AUDIO_INITINFO(&info);
    info.record.pause = 0;
    assert(::ioctl(handle, AUDIO_SETINFO, &info) == 0, UnexpectedFailure());
    reset();
  #endif // os
#endif // flavour
}

void SoundInputStream::pause() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  ::waveInStop(handle);
#else
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__LINUX)
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    AUDIO_INITINFO(&info);
    info.record.pause = 1;
    assert(::ioctl(handle, AUDIO_SETINFO, &info) == 0, UnexpectedFailure());
    assert(::ioctl(handle, I_FLUSH, FLUSHR) == 0, UnexpectedFailure()); // should never fail
  #endif // os
#endif // flavour
}

void SoundInputStream::reset() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  ::waveInReset(handle);
#else
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__LINUX)
    assert(::ioctl(handle, SNDCTL_DSP_RESET, 0) == 0), UnexpectedFailure());
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    AUDIO_INITINFO(&info);
    info.record.pause = 1;
    assert(::ioctl(handle, AUDIO_SETINFO, &info) == 0, UnexpectedFailure());
    assert(::ioctl(handle, I_FLUSH, FLUSHR) == 0, UnexpectedFailure()); // should not fail
  #endif // os
#endif // flavour
}

unsigned int SoundInputStream::read(void* buffer, unsigned int size) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  WAVEHDR header;
  clear(header);
  header.lpData = pointer_cast<LPSTR>(buffer);
  header.dwBufferLength = size;

  unsigned int result = ::waveInPrepareHeader(handle, &header, sizeof(header));
  assert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  result = ::waveInAddBuffer(handle, &header, sizeof(header));
  assert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  // FIXME
  do {
  } while ((header.dwFlags & WHDR_DONE) == 0);

  result = ::waveInUnprepareHeader(handle, &header, sizeof(header));
  assert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  return header.dwBytesRecorded;
#else
  unsigned int bytesRead = 0;
  while (bytesRead < size) {
    int result;
    do {
      result = ::read(handle, buffer, (size <= SSIZE_MAX) ? size : SSIZE_MAX);
      if (result < 0) { // has an error occured
        assert(errno == EINTR, IOException());
      }
    } while (result < 0);
    bytesRead += result;
  }
  return bytesRead;
#endif // flavour
}

SoundInputStream::~SoundInputStream() throw() {
  reset();
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  ::waveInClose(handle);
#else
  ::close(handle);
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
