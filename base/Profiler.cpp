/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Profiler.h>
#include <base/Timer.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

bool Profiler::enabled = 0;
MemorySize Profiler::numberOfEvents = 0; // TAG: atomic
unsigned int Profiler::flags = 0;
FileOutputStream Profiler::fos;

uint64 Profiler::getTimestamp()
{
  return Timer::getNow();
}

bool Profiler::open(const String& path)
{
  if (!enabled) {
    fos = FileOutputStream(path);
    enabled = true;
    return true;
  }
  return false;
}

void Profiler::start()
{
  enabled = true;
}

void Profiler::stop()
{
  enabled = false;
}

void Profiler::pushEvent(const Event& e)
{
  if (!INLINE_ASSERT(enabled)) {
    return;
  }
  ++numberOfEvents;
  fos.write(reinterpret_cast<const uint8*>(&e), sizeof(e), false);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
