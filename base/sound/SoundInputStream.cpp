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
#include <base/sound/SoundInputStream.h>
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

#  if !defined(SSIZE_MAX)
#    define SSIZE_MAX (1024*1024)
#  endif

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
class SoundInputStream::SoundInputStreamHelper {
public:

  void CALLBACK callback(HWAVEIN handle, UINT message, DWORD instance, DWORD param1, DWORD param2) {
    // signal
  }
};
#endif // win32

SoundInputStream::SoundInputStream(unsigned int samplingRate, unsigned int channels)
{
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

  unsigned int result = ::waveInOpen((HWAVEIN*)&handle, WAVE_MAPPER, &format, (DWORD_PTR)event.getHandle(), 0, CALLBACK_EVENT);
  bassert(result == MMSYSERR_NOERROR, NotSupported("Cannot open device."));
  event.reset();
#else
  SoundDevice::soundDevice.acquireReadAccess();
  SharedSynchronize<ReadWriteLock> sharedSynchronize(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getReadHandle();

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

unsigned int SoundInputStream::available() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return 0;
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronize(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getReadHandle();
  (void)handle; // dont care about unused
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
    return 0;
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
    unsigned int available = 0;
    bassert(::ioctl(handle, I_NREAD, &available) == 0, UnexpectedFailure()); // should not fail
    return available;
//    return available & ~(channels * 2 - 1); // align to sample frame (channels * bitsPerSample/8)
  #else
    return 0;
  #endif // os
#endif // flavor
}

unsigned int SoundInputStream::getChannels() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return 0;
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronize(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getReadHandle();
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
    int channels = 0;
    binternalerror(::ioctl(handle, SOUND_PCM_READ_CHANNELS, &channels) == 0);
    return channels;
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
    audio_info_t info;
    binternalerror(::ioctl(handle, AUDIO_GETINFO, &info) == 0); // should never fail
    return info.record.channels;
  #else
    if (handle) {
    }
    return 0;
  #endif // os
#endif // flavor
}

unsigned int SoundInputStream::getRate() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return 0;
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronize(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getReadHandle();
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
    int rate = 0;
    binternalerror(::ioctl(handle, SOUND_PCM_READ_RATE, &rate) == 0);
    return rate;
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
    audio_info_t info;
    binternalerror(::ioctl(handle, AUDIO_GETINFO, &info) == 0); // should never fail
    return info.record.sample_rate;
  #else
    if (handle) {
    }
    return 0;
  #endif // os
#endif // flavor
}

unsigned int SoundInputStream::getPosition() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  MMTIME time;
  clear(time);
  time.wType = TIME_SAMPLES;
  ::waveInGetPosition((HWAVEIN)handle, &time, sizeof(time));
  if (time.wType != TIME_SAMPLES) {
    Runtime::internalError(_COM_AZURE_DEV__BASE__PRETTY_FUNCTION, nullptr);
    return 0;
  }
  return time.u.sample;
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronize(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getReadHandle();
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
    return 0;
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
    audio_info_t info;
    binternalerror(::ioctl(handle, AUDIO_GETINFO, &info) == 0); // should never fail
    return info.record.samples;
  #else
    if (handle) {
    }
    return 0;
  #endif // os
#endif // flavor
}

void SoundInputStream::resume() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::waveInStart((HWAVEIN)handle);
  event.reset();
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronize(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getReadHandle();
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
    reset();
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
    audio_info_t info;
    AUDIO_INITINFO(&info);
    info.record.pause = 0;
    binternalerror(::ioctl(handle, AUDIO_SETINFO, &info) == 0);
    reset();
  #else
    if (handle) {
    }
  #endif // os
#endif // flavor
}

void SoundInputStream::pause() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::waveInStop((HWAVEIN)handle);
  event.reset();
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronize(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getReadHandle();
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
    audio_info_t info;
    AUDIO_INITINFO(&info);
    info.record.pause = 1;
    binternalerror(::ioctl(handle, AUDIO_SETINFO, &info) == 0);
    binternalerror(::ioctl(handle, I_FLUSH, FLUSHR) == 0); // should never fail
  #else
    if (handle) {
    }
  #endif // os
#endif // flavor
}

void SoundInputStream::reset() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::waveInReset((HWAVEIN)handle);
#else
  SharedSynchronize<ReadWriteLock> sharedSynchronize(SoundDevice::soundDevice.guard);
  OperatingSystem::Handle handle = SoundDevice::soundDevice.getReadHandle();
  (void)handle; // dont care about unused
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
    binternalerror(::ioctl(handle, SNDCTL_DSP_RESET, 0) == 0);
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
    audio_info_t info;
    AUDIO_INITINFO(&info);
    info.record.pause = 1;
    binternalerror(::ioctl(handle, AUDIO_SETINFO, &info) == 0);
    binternalerror(::ioctl(handle, I_FLUSH, FLUSHR) == 0); // should not fail
  #endif // os
#endif // flavor
}

unsigned int SoundInputStream::read(void* buffer, unsigned int size)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
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
    int result = 0;
    do {
      result = (int)::read(handle, buffer, (static_cast<size_t>(size) <= SSIZE_MAX) ? size : SSIZE_MAX);
      if (result < 0) { // has an error occured
        bassert(errno == EINTR, IOException());
      }
    } while (result < 0);
    bytesRead += result;
  }
  return bytesRead;
#endif // flavor
}

SoundInputStream::~SoundInputStream() noexcept {
  reset();
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::waveInClose((HWAVEIN)handle);
#else
  SoundDevice::soundDevice.relinquishReadAccess();
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
