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
#include <base/collection/Map.h>
#include <base/concurrency/ThreadLocalContext.h>
#include <base/dl/DynamicLinker.h>
#include <base/filesystem/FileSystem.h>
#include <base/UnitTest.h>

 // TAG: compact stack trace like utf8
 
_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace {

  SpinLock lock;
  const unsigned int pid = Process::getProcess().getId();
  Array<StackFrame> stackFramesHash; // cached frames (hash table)
  Array<StackFrame> stackFramesUnhash; // cached frames (remaining stack traces)

  class Frame {
  public:

    String name;
    String category; // module
    unsigned int parent = 0;

    Frame()
    {
    }

    Frame(const String& _name, const String& _category, unsigned int _parent)
      : name(_name), category(_category), parent(_parent)
    {
    }

    bool operator==(const Frame& compare) const noexcept
    {
      return (parent == compare.parent) &&
        (name == compare.name) &&
        (category == compare.category);
    }
  };

  Array<Frame> stackFrames;
  Map<uint32, unsigned int> stackFramesLookup;

  inline const StackFrame& getStackFrameImpl(uint32 sf)
  {
    if (sf & 0x80000000U) {
      const uint32 index = sf & ~0x80000000U;
      BASSERT(index < stackFramesUnhash.getSize());
      return stackFramesUnhash[index];
    } else {
      BASSERT(sf < stackFramesHash.getSize());
      return stackFramesHash[sf];
    }
  }

  class Block {
  public:
    
    static constexpr unsigned int SIZE = 4096;

    Block* next = nullptr;
    Block* previous = nullptr;
    Profiler::Event events[SIZE];
    MemorySize size = 0;
  };

  Block* blocks = nullptr;

  void addEvent(const Profiler::Event& e)
  {
    SpinLock::Sync _sync(lock);
    if (!blocks || (blocks->size >= Block::SIZE)) {
      constexpr MemorySize SIZE = sizeof(Block);
      void* heap = malloc(SIZE);
      if (!heap) {
        return;
      }
      new(heap) Block(); // do NOT record memory alloc as event
      Block* b = reinterpret_cast<Block*>(heap);
      b->next = blocks;
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
      b->~Block();
      free(b);
    }
  }
}

bool Profiler::useStackFrames = false; // include stack frames for events
unsigned int Profiler::minimumWaitTime = 5; // minimum time to wait to record event
unsigned int Profiler::minimumHeapSize = 4096 * 2; // minimum heap size to record event

bool Profiler::isEnabled() noexcept
{
  return enabled;
}

void Profiler::setUseStackFrames(bool _useStackFrames) noexcept
{
  useStackFrames = _useStackFrames;
}

void Profiler::setMinimumWaitTime(unsigned int _minimumWaitTime) noexcept
{
  minimumWaitTime = _minimumWaitTime;
}

void Profiler::setMinimumHeapSize(unsigned int _minimumHeapSize) noexcept
{
  minimumHeapSize = _minimumHeapSize;
}

bool Profiler::isEnabledScope() noexcept
{
  if (!enabled) {
    return false;
  }
  if (auto tlc = Thread::getLocalContext()) {
    return tlc->profiling.suspended == 0;
  }
  return false; // unknown thread and also happens before TLC has been constructed
}

MemorySize Profiler::getNumberOfEvents() noexcept
{
  return numberOfEvents;
}

void Profiler::SuspendProfiling::suspendAndResume(bool resume) noexcept
{
  if (auto tlc = Thread::getLocalContext()) {
    auto& profiling = tlc->profiling;
    if (resume) {
      BASSERT(profiling.suspended > 0);
      --profiling.suspended;
    } else {
      ++profiling.suspended;
    }
  }
}

void Profiler::release()
{
  if (enabled) {
    close();
  }
  SpinLock::Sync _sync(lock);
  releaseEvents();
  swapper(stackFramesHash, Array<StackFrame>());
  swapper(stackFramesUnhash, Array<StackFrame>());
  swapper(stackFrames, Array<Frame>());
  stackFramesLookup.removeAll();
}

uint64 Profiler::getTimestamp() noexcept
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

void Profiler::stop() noexcept
{
  enabled = false;
}

StackFrame Profiler::getStackFrame(uint32 sf)
{
  // we would need to copy to avoid race condition
  SpinLock::Sync _sync(lock);
  return getStackFrameImpl(sf);
}

uint32 Profiler::getStackFrame(StackFrame&& stackTrace)
{
  SpinLock::Sync _sync(lock);

  constexpr unsigned int FRAMES = 65521; // 4723
  if (stackFramesHash.getSize() != FRAMES) { // we can resize if too many conflicts
    stackFramesHash.setSize(FRAMES);
  }

  static MemorySize capacity = 1024;
  stackFramesUnhash.ensureCapacity(capacity);

  const uint32 hash = stackTrace.getHash() % FRAMES;

  static unsigned int frames = 0;
  static unsigned int hits = 0;
  static unsigned int conflicts = 0;
  if (stackFramesHash[hash].isEmpty()) { // unused slot
    ++frames;
    stackFramesHash[hash] = std::move(stackTrace);
    return hash;
  } else {
    if (stackTrace == stackFramesHash[hash]) { // hit
      ++hits;
      return hash;
    } else {
      ++conflicts;
      auto size = stackFramesUnhash.getSize();
      if (size >= capacity) { // TAG: need getCapacity()
        capacity = stackFramesUnhash.getSize() * 2;
        stackFramesUnhash.ensureCapacity(capacity);
      }
      stackFramesUnhash.append(std::move(stackTrace));
      return size | 0x80000000U; // differentiate from hash id
    }
  }
}

void Profiler::initEvent(Event& e) noexcept
{
  // e.pid = pid; // written on export
  if (auto tlc = Thread::getLocalContext()) {
    e.tid = tlc->simpleId;
  } else {
    e.tid = 0; // e.g. before TLC has been constructed
  }

  if (useStackFrames) {
    // TAG: skip extra frame when ready
    e.sf = getStackFrame(StackFrame::getStack()); // we cannot recover from memory exception
  }

  e.ts = Timer::toXTimeUS(Timer::getNow());
}

unsigned int Profiler::Task::getTask(const char* name, const char* cat) noexcept
{
  if (auto tlc = Thread::getLocalContext()) {
    auto& profiling = tlc->profiling;
    if (profiling.suspended) {
      return BAD;
    }
    auto& tasks = profiling.events;
    if (tasks.getSize() < tlc->PROFILER_TASKS) {
      SuspendProfiling suspendProfiling; // no thanks to recursion
      tasks.setSize(tlc->PROFILER_TASKS);
    }
    if (profiling.tasks >= tasks.getSize()) {
      SuspendProfiling suspendProfiling; // no thanks to recursion
      tasks.setSize(tasks.getSize() * 2);
    }

    const unsigned int id = profiling.tasks++; // push
    auto& e = tasks.getElements()[id];
    initEvent(e);
    e.name = name;
    e.cat = cat;
    return id;
  }
  return BAD; // e.g. before thread local has been constructed
}

/*
Timer::XTime operator-(Timer::XTime left, Timer::XTime right) noexcept
{
  return Timer::toXTimeUS(Timer::toTimeUS(left) - Timer::toTimeUS(right));
}
*/

void Profiler::Task::pushTask(unsigned int taskId) noexcept
{
  if (taskId == BAD) {
    return;
  }
  if (!enabled) {
    return;
  }
  auto tlc = Thread::getLocalContext();
  if (tlc) {
    BASSERT(taskId < tlc->profiling.tasks);
    BASSERT(tlc->profiling.tasks > 0);
    --(tlc->profiling.tasks); // pop stack
    if (tlc->profiling.suspended != 0) {
      return;
    }
  }

  // use EVENT_COMPLETE to combine BEGIN and END events
  const auto ts = Timer::getNow();
  Event& e = tlc->profiling.events.getElements()[taskId];
  e.ph = EVENT_COMPLETE;
  e.dur = Timer::toXTimeUS(ts - Timer::toTimeUS(e.ts));
  e.ts = Timer::toXTimeUS(ts); // after dur
  pushEvent(e);
}

void Profiler::pushEvent(const Event& e)
{
  if (!INLINE_ASSERT(enabled)) {
    return;
  }
  // TAG: FIXME - check scope?
  ++numberOfEvents;
  addEvent(e);
}

void Profiler::pushObjectCreateImpl(MemorySize size)
{
  const bool isID = size & (static_cast<MemorySize>(1) << (sizeof(MemorySize) * 8 - 1));
  if (!isID) {
    if (size < minimumHeapSize) {
      return;
    }
  }
  if (!isEnabledScope()) {
    return;
  }
  Event e;
  initEvent(e);
  e.ph = EVENT_OBJECT_CREATE;
  e.cat = CAT_MEMORY;
  if (isID) {
    // remember id
  }
  pushEvent(e);
}

void Profiler::pushObjectDestroyImpl(MemorySize size)
{
  if (size < minimumHeapSize) {
    return;
  }
  if (!isEnabledScope()) {
    return;
  }
  Event e;
  initEvent(e);
  e.ph = EVENT_OBJECT_DESTROY;
  e.cat = CAT_MEMORY;
  pushEvent(e);
}

void Profiler::pushExceptionImpl(const char* type)
{
  if (!isEnabledScope()) {
    return;
  }
  Event e;
  initEvent(e);
  e.ph = EVENT_INSTANT;
  if (type) {
    e.data = new ReferenceString(TypeInfo::demangleName(type));
  }
  e.cat = CAT_EXCEPTION;
  pushEvent(e);
}

void Profiler::pushSignalImpl(const char* name)
{
  if (!isEnabledScope()) {
    return;
  }
  Event e;
  initEvent(e);
  e.ph = EVENT_INSTANT;
  e.name = name;
  e.cat = CAT_SIGNAL;
  pushEvent(e);
}

void Profiler::pushThreadStartImpl(const char* name, unsigned int parentId)
{
  if (!isEnabledScope()) {
    return;
  }
  Event e;
  initEvent(e);
  e.ph = EVENT_INSTANT;
  e.name = name;
  e.cat = "THREAD";
  pushEvent(e);
}

void Profiler::pushProcessMetaImpl(ReferenceCountedObject* name)
{
  if (!isEnabledScope()) {
    return;
  }
  Event e;
  initEvent(e);
  e.ph = EVENT_META;
  e.name = "process_name";
  e.cat = "PROCESS";
  e.data = name;
  pushEvent(e);
}

void Profiler::pushThreadMetaImpl(ReferenceCountedObject* name/*, unsigned int parentId*/)
{
  if (!isEnabledScope()) {
    return;
  }
  Event e;
  initEvent(e);
  e.ph = EVENT_META;
  e.name = "thread_name";
  e.cat = "THREAD";
  e.data = name;
  pushEvent(e);
}

namespace {

  unsigned int buildStackFrame(const uint32 sf)
  {
    if (auto value = stackFramesLookup.find(sf)) { // found stack
      return *value;
    }

    const StackFrame& frame = getStackFrameImpl(sf);
    auto size = frame.getSize();
    if (!INLINE_ASSERT(size > 0)) {
      return 0; // bad
    }

    if (sf & 0x80000000U) { // look through unhashed frames
      const uint32 index = sf & ~0x80000000U;
      for (MemorySize i = 0; i < index; ++i) {
        if (stackFramesUnhash[i] == frame) {
          // found
          const uint32 _sf = i | 0x80000000U;
          if (auto value = stackFramesLookup.find(_sf)) { // found stack
            return *value;
          }
          BASSERT(!"Unexpected flow.");
          break;
        }
      }
    }

    MemorySize parent = 0;
    auto trace = frame.getTrace();
    for (MemoryDiff i = size - 1; i >= 0; --i) { // reverse trace!
      if (auto ip = trace[i]) {
        const void* symbol = DynamicLinker::getSymbolAddress(ip);
        if (!symbol) {
          continue;
        }
        const String name = DynamicLinker::getSymbolName(ip);
        const String demangled = TypeInfo::demangleName(name.native());
        if (!demangled) {
          continue;
        }

        String path = DynamicLinker::getImagePath(ip);
        path = FileSystem::getComponent(path, FileSystem::FILENAME);

        // look for the same frame O(n^2) complexity though // TAG: optimize?
        const Frame frame(demangled, path, parent);
        auto it = std::find(stackFrames.begin(), stackFrames.end(), frame);
        if (it != stackFrames.end()) {
          parent = static_cast<unsigned int>(it - stackFrames.begin());
        } else {
          if (stackFrames.getSize() == stackFrames.getCapacity()) {
            stackFrames.ensureCapacity(stackFrames.getSize() * 2);
          }
          parent = stackFrames.getSize();
          stackFrames.append(frame);
        }
      }
    }

    if (parent) {
      stackFramesLookup.add(sf, parent);
    }

    return parent;
  }
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
  
  if (useJSON) {
    String text("{\n\"traceEvents\": [\n");
    fos.write(reinterpret_cast<const uint8*>(text.native()), text.getLength(), false);
  }

  while (current) {
    for (MemorySize i = 0; i < current->size; ++i) {
      const Event& e = current->events[i];
      if (useJSON) {
        
        auto item = o.createObject();
        item->setValue(o.createString("pid"), o.createInteger(pid));
        item->setValue(o.createString("tid"), o.createInteger(e.tid));
        item->setValue(o.createString("ph"), o.createString(String(&e.ph, 1)));
        item->setValue(o.createString("name"), o.createString(e.name));
        item->setValue(o.createString("cat"), o.createString(e.cat));

        if (e.ph == EVENT_META) {
          if (auto r = e.data.cast<ReferenceString>()) {
            auto args = o.createObject();
            item->setValue(o.createString("args"), args);
            args->setValue(o.createString("name"), o.createString(r->string));
          }
        } else if (e.ph == EVENT_INSTANT) {
          if (auto r = e.data.cast<ReferenceString>()) {
            item->setValue(o.createString("name"), o.createString(r->string));
          }
        } else {
          item->setValue(o.createString("ts"), o.createInteger(Timer::toTimeUS(e.ts)));
          item->setValue(o.createString("dur"), o.createInteger(Timer::toTimeUS(e.dur)));
          // item->setValue(o.createString("tdur"), o.createInteger(e.tdur));
          // item->setValue(o.createString("tts"), o.createInteger(e.tts));
        }

        if (useStackFrames && e.sf) { // TAG: is 0 a valid sf id?
          // "sf" or "stack": ["0x1", "0x2"] // for stack frame
          const unsigned int id = buildStackFrame(e.sf);
          item->setValue(o.createString("sf"), o.createString(format() << id));
        }

        // "s" for instant event scope global, process, thread (default)
        
        const String text = JSON::getJSON(item); // single event
        fos.write(reinterpret_cast<const uint8*>(text.native()), text.getLength(), false);
        if (((i + 1) < current->size) || current->previous) { // not last - we could use counter also
          fos.write(reinterpret_cast<const uint8*>(","), 1, false);
        }
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

    if (useStackFrames) {
      String text1(",\n\"stackFrames\": {\n");
      fos.write(reinterpret_cast<const uint8*>(text1.native()), text1.getLength(), false);

      auto frames = o.createObject();
      for (MemorySize id = 0; id < stackFrames.getSize(); ++id) {
        const auto& f = stackFrames[id];
        auto frame = o.createObject();
        if (f.parent) {
          frame->setValue(o.createString("parent"), o.createString(format() << f.parent));
        }
        if (f.name) {
          frame->setValue(o.createString("name"), o.createString(f.name));
        }
        if (f.category) {
          frame->setValue(o.createString("category"), o.createString(f.category));
        }
fout << "!!! " << f.parent << " " << f.name << " " << f.category << ENDL;
        frames->setValue(o.createString(format() << id), frame);
      }

      const String text = JSON::getJSON(frames); // TAG: write single frame at a time instead
      fos.write(reinterpret_cast<const uint8*>(text.native()), text.getLength(), false);

      String text2("}\n}\n"); // terminate frames and root object
      fos.write(reinterpret_cast<const uint8*>(text2.native()), text2.getLength(), false);
    }

    String text3("}\n"); // terminate root object
    fos.write(reinterpret_cast<const uint8*>(text3.native()), text3.getLength(), false);

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
