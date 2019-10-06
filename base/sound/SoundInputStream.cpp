/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/sound/SoundInputStream.h>
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
class SoundInputStream::SoundInputStreamHelper {
public:

  void CALLBACK callback(HWAVEIN handle, UINT message, DWORD instance, DWORD param1, DWORD param2) {
    // signal
  }
};
#endif // win32

SoundInputStream::SoundInputStream(unsigned int samplingRate, unsigned int channels) throw(OutOfDomain, NotSupported) {
  bassert(channels > 0, OutOfDomain());
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  WAVEFORMATEX format;
  clear(format);
  format.wFormatTag = WAVE_FORMAT_PCM;
  format.nChannels = channels;
  format.nSamplesPerSec = samplingRate;
  format.wBitsPerSample = 16;
  format.nBlockAlign = channels * format.wBitsPerSample/8;
  format.nAvgBytesPerSec = samplingRate * format.nBlockAlign;

  unsigned int result = ::waveInOpen((HWAVEIN*)&handle, WAVE_MAPPER, &format, (DWORD)event.getHandle(), 0, CALLBACK_EVENT);
  bassert(result == MMSYSERR_NOERROR, NotSupported("Cannot open device"));
  event.reset();
#else
  SoundDevice::soundDevice.acquireReadAccess();
  SharedSynchronize<ReadWriteLock> sharedSynchronize(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getReadHandle();

  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    int format = AFMT_S16_LE;
    bassert(::ioctl(handle, SNDCTL_DSP_SETFMT, &format) == 0, UnexpectedFailure());
    bassert(format == AFMT_S16_LE, NotSupported());

    unsigned int desiredChannels = channels;
    bassert(::ioctl(handle, SNDCTL_DSP_CHANNELS, &desiredChannels) == 0, UnexpectedFailure());
    bassert(desiredChannels == channels, NotSupported());

    unsigned int desiredRate = samplingRate;
    bassert(::ioctl(handle, SNDCTL_DSP_SPEED, &desiredRate) == 0, UnexpectedFailure());
    bassert(desiredRate == samplingRate, NotSupported());

  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    AUDIO_INITINFO(&info);
    info.record.pause = 1;
    info.record.sample_rate = samplingRate;
    info.record.channels = channels;
    info.record.precision = 16;
    info.record.encoding = AUDIO_ENCODING_LINEAR; // PCM
    bassert(::ioctl(handle, AUDIO_SETINFO, &info) == 0, UnexpectedFailure());

    bassert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure()); // is this required
    bassert(
      (info.record.sample_rate == samplingRate) &&
      (info.record.channels == channels) &&
      (info.record.precision == 16) &&
      (info.record.encoding == AUDIO_ENCODING_LINEAR),
      NotSupported()
    );

    bassert(::ioctl(handle, I_FLUSH, FLUSHR) == 0, UnexpectedFailure()); // should never fail
  #endif // os
#endif // flavor
}

unsigned int SoundInputStream::available() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return 0;
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronize(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getReadHandle();
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    return 0;
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    unsigned int available;
    bassert(::ioctl(handle, I_NREAD, &available) == 0, UnexpectedFailure()); // should not fail
    return available;
//    return available & ~(channels * 2 - 1); // align to sample frame (channels * bitsPerSample/8)
  #endif // os
#endif // flavor
}

unsigned int SoundInputStream::getChannels() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return 0;
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronize(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getReadHandle();
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    int channels;
    bassert(::ioctl(handle, SOUND_PCM_READ_CHANNELS, &channels) == 0, UnexpectedFailure());
    return channels;
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    bassert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure()); // should never fail
    return info.record.channels;
  #endif // os
#endif // flavor
}

unsigned int SoundInputStream::getRate() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return 0;
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronize(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getReadHandle();
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    int rate;
    bassert(::ioctl(handle, SOUND_PCM_READ_RATE, &rate) == 0, UnexpectedFailure());
    return rate;
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    bassert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure()); // should never fail
    return info.record.sample_rate;
  #endif // os
#endif // flavor
}

unsigned int SoundInputStream::getPosition() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  MMTIME time;
  clear(time);
  time.wType = TIME_SAMPLES;
  ::waveInGetPosition((HWAVEIN)handle, &time, sizeof(time));
  bassert(time.wType == TIME_SAMPLES, UnexpectedFailure());
  return time.u.sample;
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronize(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getReadHandle();
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    return 0;
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    bassert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure()); // should never fail
    return info.record.samples;
  #endif // os
#endif // flavor
}

void SoundInputStream::resume() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::waveInStart((HWAVEIN)handle);
  event.reset();
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronize(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getReadHandle();
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    reset();
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    AUDIO_INITINFO(&info);
    info.record.pause = 0;
    bassert(::ioctl(handle, AUDIO_SETINFO, &info) == 0, UnexpectedFailure());
    reset();
  #endif // os
#endif // flavor
}

void SoundInputStream::pause() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::waveInStop((HWAVEIN)handle);
  event.reset();
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronize(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getReadHandle();
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    AUDIO_INITINFO(&info);
    info.record.pause = 1;
    bassert(::ioctl(handle, AUDIO_SETINFO, &info) == 0, UnexpectedFailure());
    bassert(::ioctl(handle, I_FLUSH, FLUSHR) == 0, UnexpectedFailure()); // should never fail
  #endif // os
#endif // flavor
}

void SoundInputStream::reset() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::waveInReset((HWAVEIN)handle);
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronize(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getReadHandle();
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    bassert(::ioctl(handle, SNDCTL_DSP_RESET, 0) == 0, UnexpectedFailure());
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    audio_info_t info;
    AUDIO_INITINFO(&info);
    info.record.pause = 1;
    bassert(::ioctl(handle, AUDIO_SETINFO, &info) == 0, UnexpectedFailure());
    bassert(::ioctl(handle, I_FLUSH, FLUSHR) == 0, UnexpectedFailure()); // should not fail
  #endif // os
#endif // flavor
}

unsigned int SoundInputStream::read(void* buffer, unsigned int size) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  WAVEHDR header;
  clear(header);
  header.lpData = Cast::pointer<LPSTR>(buffer);
  header.dwBufferLength = size;

  unsigned int result = ::waveInPrepareHeader((HWAVEIN)handle, &header, sizeof(header));
  bassert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  result = ::waveInAddBuffer((HWAVEIN)handle, &header, sizeof(header));
  bassert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  do {
    event.wait();
    event.reset();
  } while ((header.dwFlags & WHDR_DONE) == 0);

  result = ::waveInUnprepareHeader((HWAVEIN)handle, &header, sizeof(header));
  bassert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  return header.dwBytesRecorded;
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronize(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getReadHandle();
  unsigned int bytesRead = 0;
  while (bytesRead < size) {
    int result;
    do {
      result = ::read(handle, buffer, (size <= SSIZE_MAX) ? size : SSIZE_MAX);
      if (result < 0) { // has an error occured
        bassert(errno == EINTR, IOException());
      }
    } while (result < 0);
    bytesRead += result;
  }
  return bytesRead;
#endif // flavor
}

SoundInputStream::~SoundInputStream() throw() {
  reset();
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::waveInClose((HWAVEIN)handle);
#else
  SoundDevice::soundDevice.relinquishReadAccess();
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
