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
#include <base/collection/HashTable.h>
#include <base/StackFrame.h>
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
  pushProcessMeta(new ReferenceString(Process::getProcess().getName()));
  pushThreadMeta(new ReferenceString("main")); // TAG: get thread name
}

void Profiler::stop()
{
  enabled = false;
}

void Profiler::initEvent(Event& e)
{
  e.pid = pid;
  if (auto tlc = Thread::getLocalContext()) {
    e.tid = tlc->simpleId;
  } else { // TAG: fix missing id
    e.tid = 0;
    // e.tid = reinterpret_cast<MemorySize>(Thread::getIdentifier());
  }
  e.ts = Timer::getNow();
}

namespace {

  Array<StackFrame> stackFrames;
  const bool initialize = (stackFrames.setSize(65521+0*4723), true);
}

void Profiler::pushEvent(const Event& e)
{
  if (!INLINE_ASSERT(enabled)) {
    return;
  }

  // TAG: make unique id for stack trace
  StackFrame stackTrace = StackFrame::getStack();
  // stackTrace.dump();
  uint32 hash = stackTrace.getHash();
  // fout << hash << ENDL;
  hash %= 65521; // 4723;
  fout << hash << ENDL;
  
  static unsigned int frames = 0;
  static unsigned int conflicts = 0;
  if (stackFrames[hash].isEmpty()) {
    ++frames;
    stackFrames[hash] = stackTrace;
  } else {
    if (stackTrace != stackFrames[hash]) {
      ++conflicts;
    }
  }
  fout << "STACK: " << frames << " / " << conflicts << ENDL;

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
  
  if (useJSON) {
    // "traceEvents": [
    String text("[");
    fos.write(reinterpret_cast<const uint8*>(text.native()), text.getLength(), false);
  }
  bool first = true;
  while (current) {
    for (MemorySize i = 0; i < current->size; ++i) {
      const Event& e = current->events[i];
      if (useJSON) {
        
        // put network/IO in separate track - requires unique ids?

        // TAG: if meta then store thread/process info
        item->setValue(o.createString("tid"), o.createInteger(e.tid));
        char temp[2];
        temp[0] = e.ph;
        temp[1] = '\0';
        item->setValue(o.createString("ph"), o.createString(temp));
        item->setValue(o.createString("name"), o.createString(e.name));
        item->setValue(o.createString("cat"), o.createString(e.cat));
        if (e.ph == EVENT_META) {
          // continue;
          
          if (auto r = e.data.cast<ReferenceString>()) {
            auto args = o.createObject();
            item->setValue(o.createString("args"), args);
            // item->setValue(o.createString("name"), o.createString("process_name"));
            args->setValue(o.createString("name"), o.createString(r->string));
          }
        } else if (e.ph == EVENT_INSTANT) {
          if (auto r = e.data.cast<ReferenceString>()) {
            item->setValue(o.createString("name"), o.createString(r->string));
          }
        } else {
          item->setValue(o.createString("ts"), o.createInteger(e.ts));
          item->setValue(o.createString("dur"), o.createInteger(e.dur));
          item->setValue(o.createString("tdur"), o.createInteger(e.tdur));
          item->setValue(o.createString("tts"), o.createInteger(e.tts));
          // "sf" or "stack": ["0x1", "0x2"] // for stack frame
          // get from stack frame lot
        }
/*
        {
          'category': 'modulename',
          'name': 'symbol',
          'parent': 1 // if available
        }
*/
        
        // "s" for instant event scope global, process, thread (default)
        
        if (!first) {
          fos.write(reinterpret_cast<const uint8*>(","), 1, false);
        }
        first = false;
        const String text = JSON::getJSON(item);
        item->removeKey(o.createString("args"));
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
/*
    "displayTimeUnit": "ns", // "ms" or "ns"
    "systemTraceEvents": "SystemTraceData", // Linux ftrace data or Windows ETW trace data. This data must start with # tracer: and adhere to the Linux ftrace format or adhere to Windows ETW format
    "powerTraceAsString": "", // BattOr power data
    "otherData": {
      "version": "Application 1.0"
    },
    stackFrames: {
      "5": { name: "main", category: "app" },
      "7": { parent: "5", name: "SomeFunction", category: "app" },
      "9": { parent: "5", name: "SomeFunction", category: "app" }
    },
    "samples": [...]
*/
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
