/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/sound/SoundOutputStream.h>
#include <base/UnexpectedFailure.h>
#include <base/io/IOException.h>
#include <base/string/FormatOutputStream.h>
#include <base/sound/SoundDevice.h>
#include <base/concurrency/SharedSynchronize.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  define NO_STRICT
  // I don't get it: in STRICT mode handles are of type int but in NO_STRICT
  // mode the handles are of size void*. This is a problem on 64 bit platforms
  // where int and void* may be of different sizes.
#  include <windows.h>
#elif (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__UNIX)
#  include <sys/types.h> // open
#  include <sys/stat.h> // open
#  include <fcntl.h> // open
#  include <unistd.h> // read
#  include <errno.h> // errno
#  include <limits.h> // SSIZE_MAX

#  if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
#    include <sys/ioctl.h> // ioctl
#    include <sys/soundcard.h> // ioctl
#  elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
#    include <stropts.h> // ioctl
#    include <sys/conf.h> // ioctl
#    include <sys/audio.h>
#    include <sys/audioio.h>
#  endif // os
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
class SoundOutputStream::SoundOutputStreamHelper {
public:

  void CALLBACK callback(HWAVEOUT handle, UINT message, DWORD instance, DWORD param1, DWORD param2) {
    // signal
  }
};
#endif // win32

SoundOutputStream::SoundOutputStream(unsigned int samplingRate, unsigned int channels) throw(OutOfDomain, NotSupported) {
  assert(channels > 0, OutOfDomain());
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  WAVEFORMATEX format;
  clear(format);
  format.wFormatTag = WAVE_FORMAT_PCM;
  format.nChannels = channels;
  format.nSamplesPerSec = samplingRate;
  format.wBitsPerSample = 16;
  format.nBlockAlign = channels * format.wBitsPerSample/8;
  format.nAvgBytesPerSec = samplingRate * format.nBlockAlign;

  unsigned int result = waveOutOpen((HWAVEOUT*)&handle, WAVE_MAPPER, &format, (DWORD)event.getHandle(), 0, CALLBACK_EVENT);
  assert(result == MMSYSERR_NOERROR, NotSupported());
  event.reset();
#else
  SoundDevice::soundDevice.acquireWriteAccess();
  SharedSynchronize<SoundDevice> sharedSynchronization(SoundDevice::soundDevice);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getWriteHandle();

  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    int format = AFMT_S16_LE;
    assert(::ioctl(handle, SNDCTL_DSP_SETFMT, &format) == 0, UnexpectedFailure());
    assert(format == AFMT_S16_LE, NotSupported());

    unsigned int desiredChannels = channels;
    assert(::ioctl(handle, SNDCTL_DSP_CHANNELS, &desiredChannels) == 0, UnexpectedFailure());
    assert(desiredChannels == channels, NotSupported());

    unsigned int desiredRate = samplingRate;
    assert(::ioctl(handle, SNDCTL_DSP_SPEED, &desiredRate) == 0, UnexpectedFailure());
    assert(desiredRate == samplingRate, NotSupported());

  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    AUDIO_INITINFO(&info);
    info.play.pause = 1;
    info.play.sample_rate = samplingRate;
    info.play.channels = channels;
    info.play.precision = 16;
    info.play.encoding = AUDIO_ENCODING_LINEAR; // PCM
    assert(::ioctl(handle, AUDIO_SETINFO, &info) == 0, UnexpectedFailure());

    assert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure());
    assert(
      (info.play.sample_rate == samplingRate) &&
      (info.play.channels == channels) &&
      (info.play.precision == 16) &&
      (info.play.encoding == AUDIO_ENCODING_LINEAR),
      NotSupported()
    );

    assert(::ioctl(handle, I_FLUSH, FLUSHW) == 0, UnexpectedFailure()); // should never fail
  #endif // os
#endif // flavor
}

unsigned int SoundOutputStream::getChannels() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return 0;
#else
  SharedSynchronize<SoundDevice> sharedSynchronization(SoundDevice::soundDevice);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getWriteHandle();
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    int channels;
    assert(::ioctl(handle, SOUND_PCM_READ_CHANNELS, &channels) == 0, UnexpectedFailure());
    return channels;
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    assert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure()); // should never fail
    return info.play.channels;
  #endif // os
#endif // flavor
}

unsigned int SoundOutputStream::getRate() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return 0;
#else
  SharedSynchronize<SoundDevice> sharedSynchronization(SoundDevice::soundDevice);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getWriteHandle();
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    int rate;
    assert(::ioctl(handle, SOUND_PCM_READ_RATE, &rate) == 0, UnexpectedFailure());
    return rate;
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    assert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure()); // should never fail
    return info.play.sample_rate;
  #endif // os
#endif // flavor
}

unsigned int SoundOutputStream::getPosition() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  MMTIME time;
  clear(time);
  time.wType = TIME_SAMPLES;
  ::waveOutGetPosition((HWAVEOUT)handle, &time, sizeof(time));
  assert(time.wType == TIME_SAMPLES, UnexpectedFailure());
  return time.u.sample;
#else
  SharedSynchronize<SoundDevice> sharedSynchronization(SoundDevice::soundDevice);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getWriteHandle();
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    return 0;
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    assert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure()); // should never fail
    return info.play.samples;
  #endif // os
#endif // flavor
}

void SoundOutputStream::resume() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  event.reset();
#else
  SharedSynchronize<SoundDevice> sharedSynchronization(SoundDevice::soundDevice);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getWriteHandle();
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    assert(::ioctl(handle, I_FLUSH, FLUSHW) == 0, UnexpectedFailure()); // should never fail
  #endif // os
#endif // flavor
}

void SoundOutputStream::pause() throw() {
  // FIXME
}

void SoundOutputStream::reset() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::waveOutReset((HWAVEOUT)handle);
  event.reset();
#else
  SharedSynchronize<SoundDevice> sharedSynchronization(SoundDevice::soundDevice);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getWriteHandle();
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    assert(::ioctl(handle, SNDCTL_DSP_RESET, 0) == 0, UnexpectedFailure()); // should never fail
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    assert(::ioctl(handle, I_FLUSH, FLUSHW) == 0, UnexpectedFailure()); // should never fail
  #endif // os
#endif // flavor
}

void SoundOutputStream::wait() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  WAVEHDR header;
  clear(header);
  header.lpData = 0;
  header.dwBufferLength = 0;

  unsigned int result = ::waveOutPrepareHeader((HWAVEOUT)handle, &header, sizeof(header));
  assert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  result = ::waveOutWrite((HWAVEOUT)handle, &header, sizeof(header));
  assert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  do {
    event.wait();
    event.reset();
  } while ((header.dwFlags & WHDR_DONE) == 0);

  result = ::waveOutUnprepareHeader((HWAVEOUT)handle, &header, sizeof(header));
  assert(result == MMSYSERR_NOERROR, UnexpectedFailure());
#else
  SharedSynchronize<SoundDevice> sharedSynchronization(SoundDevice::soundDevice);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getWriteHandle();
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    assert(::ioctl(handle, SNDCTL_DSP_SYNC, 0) == 0, UnexpectedFailure());
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
  #endif // os
#endif // flavor
}

unsigned int SoundOutputStream::write(const void* buffer, unsigned int size) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  WAVEHDR header;
  clear(header);
  header.lpData = Cast::pointer<char*>(const_cast<void*>(buffer)); // do not change buffer content
  header.dwBufferLength = size;

  unsigned int result = ::waveOutPrepareHeader((HWAVEOUT)handle, &header, sizeof(header));
  assert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  result = ::waveOutWrite((HWAVEOUT)handle, &header, sizeof(header));
  assert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  do {
    event.wait();
    event.reset();
  } while ((header.dwFlags & WHDR_DONE) == 0);

  result = ::waveOutUnprepareHeader((HWAVEOUT)handle, &header, sizeof(header));
  assert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  return header.dwBytesRecorded;
#else
  SharedSynchronize<SoundDevice> sharedSynchronization(SoundDevice::soundDevice);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getWriteHandle();
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
#endif // flavor
}

SoundOutputStream::~SoundOutputStream() throw() {
  reset();
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::waveOutClose((HWAVEOUT)handle);
#else
  SoundDevice::soundDevice.relinquishWriteAccess();
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
