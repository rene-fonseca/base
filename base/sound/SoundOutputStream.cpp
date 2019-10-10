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
#include <base/sound/SoundOutputStream.h>
#include <base/UnexpectedFailure.h>
#include <base/io/IOException.h>
#include <base/string/FormatOutputStream.h>
#include <base/sound/SoundDevice.h>
#include <base/concurrency/SharedSynchronize.h>

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

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
class SoundOutputStream::SoundOutputStreamHelper {
public:

  void CALLBACK callback(HWAVEOUT handle, UINT message, DWORD instance, DWORD param1, DWORD param2) {
    // signal
  }
};
#endif // win32

SoundOutputStream::SoundOutputStream(unsigned int samplingRate, unsigned int channels) throw(OutOfDomain, NotSupported) {
  bassert(channels > 0, OutOfDomain());
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  WAVEFORMATEX format;
  clear(format);
  format.wFormatTag = WAVE_FORMAT_PCM;
  format.nChannels = channels;
  format.nSamplesPerSec = samplingRate;
  format.wBitsPerSample = 16;
  format.nBlockAlign = channels * format.wBitsPerSample/8;
  format.nAvgBytesPerSec = samplingRate * format.nBlockAlign;

  unsigned int result = waveOutOpen((HWAVEOUT*)&handle, WAVE_MAPPER, &format, (DWORD_PTR)event.getHandle(), 0, CALLBACK_EVENT);
  bassert(result == MMSYSERR_NOERROR, NotSupported());
  event.reset();
#else
  SoundDevice::soundDevice.acquireWriteAccess();
  SharedSynchronize<ReadWriteLock> sharedSynchronization(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getWriteHandle();

  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
    int format = AFMT_S16_LE;
    bassert(::ioctl(handle, SNDCTL_DSP_SETFMT, &format) == 0, UnexpectedFailure());
    bassert(format == AFMT_S16_LE, NotSupported());

    unsigned int desiredChannels = channels;
    bassert(::ioctl(handle, SNDCTL_DSP_CHANNELS, &desiredChannels) == 0, UnexpectedFailure());
    bassert(desiredChannels == channels, NotSupported());

    unsigned int desiredRate = samplingRate;
    bassert(::ioctl(handle, SNDCTL_DSP_SPEED, &desiredRate) == 0, UnexpectedFailure());
    bassert(desiredRate == samplingRate, NotSupported());

  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
    audio_info_t info;
    AUDIO_INITINFO(&info);
    info.play.pause = 1;
    info.play.sample_rate = samplingRate;
    info.play.channels = channels;
    info.play.precision = 16;
    info.play.encoding = AUDIO_ENCODING_LINEAR; // PCM
    bassert(::ioctl(handle, AUDIO_SETINFO, &info) == 0, UnexpectedFailure());

    bassert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure());
    bassert(
      (info.play.sample_rate == samplingRate) &&
      (info.play.channels == channels) &&
      (info.play.precision == 16) &&
      (info.play.encoding == AUDIO_ENCODING_LINEAR),
      NotSupported()
    );

    bassert(::ioctl(handle, I_FLUSH, FLUSHW) == 0, UnexpectedFailure()); // should never fail
  #endif // os
#endif // flavor
}

unsigned int SoundOutputStream::getChannels() const throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return 0;
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronization(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getWriteHandle();
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
    int channels = 0;
    bassert(::ioctl(handle, SOUND_PCM_READ_CHANNELS, &channels) == 0, UnexpectedFailure());
    return channels;
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
    audio_info_t info;
    bassert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure()); // should never fail
    return info.play.channels;
  #else
    return 0;
  #endif // os
#endif // flavor
}

unsigned int SoundOutputStream::getRate() const throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return 0;
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronization(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getWriteHandle();
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
    int rate = 0;
    bassert(::ioctl(handle, SOUND_PCM_READ_RATE, &rate) == 0, UnexpectedFailure());
    return rate;
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
    audio_info_t info;
    bassert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure()); // should never fail
    return info.play.sample_rate;
  #else
    return 0;
  #endif // os
#endif // flavor
}

unsigned int SoundOutputStream::getPosition() const throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  MMTIME time;
  clear(time);
  time.wType = TIME_SAMPLES;
  ::waveOutGetPosition((HWAVEOUT)handle, &time, sizeof(time));
  bassert(time.wType == TIME_SAMPLES, UnexpectedFailure());
  return time.u.sample;
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronization(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getWriteHandle();
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
    return 0;
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
    audio_info_t info;
    bassert(::ioctl(handle, AUDIO_GETINFO, &info) == 0, UnexpectedFailure()); // should never fail
    return info.play.samples;
  #else
    return 0;
  #endif // os
#endif // flavor
}

void SoundOutputStream::resume() throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  event.reset();
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronization(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getWriteHandle();
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
    bassert(::ioctl(handle, I_FLUSH, FLUSHW) == 0, UnexpectedFailure()); // should never fail
  #endif // os
#endif // flavor
}

void SoundOutputStream::pause() throw() {
  // FIXME
}

void SoundOutputStream::reset() throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::waveOutReset((HWAVEOUT)handle);
  event.reset();
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronization(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getWriteHandle();
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
    bassert(::ioctl(handle, SNDCTL_DSP_RESET, 0) == 0, UnexpectedFailure()); // should never fail
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
    bassert(::ioctl(handle, I_FLUSH, FLUSHW) == 0, UnexpectedFailure()); // should never fail
  #endif // os
#endif // flavor
}

void SoundOutputStream::wait() throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  WAVEHDR header;
  clear(header);
  header.lpData = 0;
  header.dwBufferLength = 0;

  unsigned int result = ::waveOutPrepareHeader((HWAVEOUT)handle, &header, sizeof(header));
  bassert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  result = ::waveOutWrite((HWAVEOUT)handle, &header, sizeof(header));
  bassert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  do {
    event.wait();
    event.reset();
  } while ((header.dwFlags & WHDR_DONE) == 0);

  result = ::waveOutUnprepareHeader((HWAVEOUT)handle, &header, sizeof(header));
  bassert(result == MMSYSERR_NOERROR, UnexpectedFailure());
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronization(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getWriteHandle();
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
    bassert(::ioctl(handle, SNDCTL_DSP_SYNC, 0) == 0, UnexpectedFailure());
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
  #endif // os
#endif // flavor
}

unsigned int SoundOutputStream::write(const void* buffer, unsigned int size) throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  WAVEHDR header;
  clear(header);
  header.lpData = Cast::pointer<char*>(const_cast<void*>(buffer)); // do not change buffer content
  header.dwBufferLength = size;

  unsigned int result = ::waveOutPrepareHeader((HWAVEOUT)handle, &header, sizeof(header));
  bassert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  result = ::waveOutWrite((HWAVEOUT)handle, &header, sizeof(header));
  bassert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  do {
    event.wait();
    event.reset();
  } while ((header.dwFlags & WHDR_DONE) == 0);

  result = ::waveOutUnprepareHeader((HWAVEOUT)handle, &header, sizeof(header));
  bassert(result == MMSYSERR_NOERROR, UnexpectedFailure());

  return header.dwBytesRecorded;
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronization(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getWriteHandle();
  unsigned int bytesWritten = 0;
  while (bytesWritten < size) {
    int result = 0;
    do {
      result = ::write(handle, buffer, ((size_t)size <= SSIZE_MAX) ? size : SSIZE_MAX);
      if (result < 0) { // has an error occured
        bassert(errno == EINTR, IOException());
      }
    } while (result < 0);
    bytesWritten += result;
  }
  return bytesWritten;
#endif // flavor
}

SoundOutputStream::~SoundOutputStream() throw() {
  reset();
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::waveOutClose((HWAVEOUT)handle);
#else
  SoundDevice::soundDevice.relinquishWriteAccess();
#endif // flavor
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
