/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/sound/SoundOutputStream.h>
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
class SoundOutputStream::SoundOutputStreamHelper {
public:

  void CALLBACK callback(HWAVEOUT handle, UINT message, DWORD instance, DWORD param1, DWORD param2) {
    // signal
  }
};
#endif // win32

SoundOutputStream::SoundOutputStream(unsigned int samplingRate, unsigned channels) throw(OutOfDomain, NotSupported) {
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

  unsigned int result = waveOutOpen(&handle, WAVE_MAPPER, &format, &SoundOutputStreamHelper::callback, 0, CALLBACK_FUNCTION);
  assert(result == MMSYSERR_NOERROR, NotSupported());
#else
  handle = ::open("/dev/audio", O_WRONLY);
  assert(handle != -1, NotSupported());

  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__LINUX)
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    AUDIO_INITINFO(&info);
    info.play.pause = 1;
    info.play.rate = samplingRate;
    info.play.channels = channels;
    info.play.precision = 16;
    info.play.encoding = AUDIO_ENCODING_LINEAR; // PCM
    assert(::ioctl(handle, AUDIO_SETINFO, &info) == 0, UnexpectedFailure());

    assert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure());
    assert(
      (info.play.rate == samplingRate) &&
      (info.play.channels == channels) &&
      (info.play.precision == 16) &&
      (info.play.encoding == AUDIO_ENCODING_LINEAR),
      NotSupported()
    );

    assert(::ioctl(handle, I_FLUSH, FLUSHW) == 0, UnexpectedFailure()); // should never fail
  #endif // os
#endif // flavour
}

unsigned int SoundOutputStream::getChannels() const throw() {
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
    return info.play.channels;
  #endif // os
#endif // flavour
}

unsigned int SoundOutputStream::getRate() const throw() {
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
    return info.play.sample_rate;
  #endif // os
#endif // flavour
}

unsigned int SoundOutputStream::getPosition() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  MMTIME time;
  clear(time);
  time.wType = TIME_SAMPLES;
  ::waveOutGetPosition(handle, &time, sizeof(time));
  assert(time.wType == TIME_SAMPLES, UnexpectedFailure());
  return time.u.sample;
#else
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__LINUX)
    return 0;
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    assert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure()); // should never fail
    return info.play.samples;
  #endif // os
#endif // flavour
}

void SoundOutputStream::resume() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
#else
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__LINUX)
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    assert(::ioctl(handle, I_FLUSH, FLUSHW) == 0, UnexpectedFailure()); // should never fail
  #endif // os
#endif // flavour
}

void SoundOutputStream::reset() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  ::waveOutReset(handle);
#else
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__LINUX)
    assert(::ioctl(handle, SNDCTL_DSP_RESET, 0) == 0), UnexpectedFailure()); // should never fail
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    assert(::ioctl(handle, I_FLUSH, FLUSHW) == 0, UnexpectedFailure()); // should never fail
  #endif // os
#endif // flavour
}

void SoundOutputStream::wait() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  WAVEHDR header;
  clear(header);
  header.lpData = 0;
  header.dwBufferLength = 0;

  unsigned int result = ::waveOutPrepareHeader(handle, &header, sizeof(header));
  assert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  result = ::waveOutWrite(handle, &header, sizeof(header));
  assert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  do {
  } while ((header.dwFlags & WHDR_DONE) == 0);

  result = ::waveOutUnprepareHeader(handle, &header, sizeof(header));
  assert(result == MMSYSERR_NOERROR, UnexpectedFailure());
#else
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__LINUX)
    assert(::ioctl(handle, SNDCTL_DSP_SYNC, 0) == 0), UnexpectedFailure());
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    #error NOTIMPLEMENTED
  #endif // os
#endif // flavour
}

unsigned int SoundOutputStream::write(const void* buffer, unsigned int size) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  WAVEHDR header;
  clear(header);
  header.lpData = pointer_cast<LPSTR>(buffer);
  header.dwBufferLength = size;

  unsigned int result = ::waveOutPrepareHeader(handle, &header, sizeof(header));
  assert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  result = ::waveOutWrite(handle, &header, sizeof(header));
  assert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  do {
    event.wait();
    event.reset();
  } while ((header.dwFlags & WHDR_DONE) == 0);

  result = ::waveOutUnprepareHeader(handle, &header, sizeof(header));
  assert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  return header.dwBytesRecorded;
#else
  unsigned int bytesWritten = 0;
  while (bytesWritten < size) {
    int result;
    do {
      result = ::write(handle, buffer, (size <= SSIZE_MAX) ? size : SSIZE_MAX);
      if (result < 0) { // has an error occured
        assert(errno == EINTR, IOException());
      }
    } while (result < 0);
    bytesWritten += result;
  }
  return bytesWritten;
#endif // flavour
}

SoundOutputStream::~SoundOutputStream() throw() {
  reset();
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  ::waveOutClose(handle);
#else
  ::close(handle);
#endif // flavour
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
