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
#include <base/objectmodel/JSON.h>
#include <base/concurrency/SpinLock.h>
#include <base/UnitTest.h>

// TAG: we should be able to push multi process events to common dest - use merge tool?

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace {

  SpinLock lock;
  const unsigned int pid = Process::getProcess().getId();

  class Block {
  public:
    
    Block* next = nullptr;
    Block* previous = nullptr;
    Profiler::Event events[4096];
    MemorySize size = 0;
  };

  Block* blocks = nullptr;

  void addEvent(const Profiler::Event& e)
  {
    SpinLock::Sync _sync(lock);
    if (!blocks || (blocks->size >= 4096)) {
      Block* b = reinterpret_cast<Block*>(malloc(sizeof(Block))); // do NOT record memory alloc as event
      b->next = blocks;
      b->previous = nullptr;
      b->size = 0;
      if (blocks) {
        blocks->previous = b;
      }
      blocks = b;
    }
    blocks->events[blocks->size++] = e;
  }

  void releaseEvents()
  {
    while (blocks) {
      auto b = blocks;
      blocks = blocks->next;
      free(b);
    }
  }
}

void Profiler::release()
{
  if (isEnabled()) {
    close();
  }
  SpinLock::Sync _sync(lock);
  releaseEvents();
}

uint64 Profiler::getTimestamp()
{
  return Timer::getNow();
}

bool Profiler::open(const String& path, bool _useJSON)
{
  static bool initialized = false;
  if (initialized) {
    return false;
  }
  
  fos = FileOutputStream(path);
  useJSON = _useJSON;
  enabled = true;
  return true;
}

void Profiler::start()
{
  enabled = true;
}

void Profiler::stop()
{
  enabled = false;
}

void Profiler::initEvent(Event& e)
{
  clear(e);
  e.pid = pid;
  e.tid = reinterpret_cast<MemorySize>(Thread::getIdentifier());
  e.ts = Timer::getNow();
}

void Profiler::pushEvent(const Event& e)
{
  if (!INLINE_ASSERT(enabled)) {
    return;
  }
  ++numberOfEvents;
  addEvent(e);
}

void Profiler::close()
{
  // ATTENTION: we do not want to record heap/IO for Profile implementation
  stop();

  SpinLock::Sync _sync(lock);

  Block* current = blocks;
  if (current) {
    while (current->next) {
      current = current->next;
    }
  }
  
  ObjectModel o;
  auto item = o.createObject();
  auto tid = o.createInteger(0);
  auto ts = o.createInteger(0);
  auto ph = o.createString("");
  auto cat = o.createString("");
  auto name = o.createString("");
  auto dur = o.createInteger(0);
  auto tdur = o.createInteger(0);
  auto tts = o.createInteger(0);
  if (useJSON) {
    item->setValue(o.createString("pid"), o.createInteger(pid));
    item->setValue(o.createString("tid"), tid);
    item->setValue(o.createString("ts"), ts);
    item->setValue(o.createString("ph"), ph);
    item->setValue(o.createString("cat"), cat);
    item->setValue(o.createString("name"), name);
    // item->setValue(o.createString("args"), args);
    item->setValue(o.createString("dur"), dur);
    // item->setValue(o.createString("tdur"), tdur);
    // item->setValue(o.createString("tts"), tts);
  }
  
  if (useJSON) {
    String text("[");
    fos.write(reinterpret_cast<const uint8*>(text.native()), text.getLength(), false);
  }
  bool first = true;
  while (current) {
    for (MemorySize i = 0; i < current->size; ++i) {
      const Event& e = current->events[i];
      if (useJSON) {
        if (!first) {
          fos.write(reinterpret_cast<const uint8*>(","), 1, false);
        }
        first = false;

        tid->value = e.tid;
        ts->value = e.ts;
        char temp[2];
        temp[0] = e.ph;
        temp[1] = '\0';
        ph->value = temp;
        cat->value = e.cat;
        name->value = e.name;
        dur->value = e.dur;
        tdur->value = e.tdur;
        tts->value = e.tts;

        const String text = JSON::getJSON(item);
        // fout << text << ENDL;
        fos.write(reinterpret_cast<const uint8*>(text.native()), text.getLength(), false);
        fos.write(reinterpret_cast<const uint8*>("\n"), 1, false);
      } else {
        fos.write(reinterpret_cast<const uint8*>(&e), sizeof(e), false);
      }
    }
    current = current->previous;
  }
  if (useJSON) {
    String text("]\n");
    fos.write(reinterpret_cast<const uint8*>(text.native()), text.getLength(), false);
  }

  releaseEvents();
  fos = FileOutputStream();
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Profiler) : public UnitTest {
public:

  TEST_PRIORITY(1);
  TEST_PROJECT("base");
  TEST_IMPACT(CRITICAL);
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    Profiler::open("profiler.json", true);
    Profiler::start();
  }
};

TEST_REGISTER(Profiler);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
