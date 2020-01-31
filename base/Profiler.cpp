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
#include <base/concurrency/ThreadLocalContext.h>
#include <base/dl/DynamicLinker.h>
#include <base/filesystem/FileSystem.h>
#include <base/Application.h>
#include <base/SystemInformation.h>
#include <base/objectmodel/ObjectModel.h>
#include <base/ResourceHandle.h>
#include <base/UnitTest.h>

#if 0
#define _COM_AZURE_DEV__BASE__ALLOW_OPTIMIZATION
#if defined(_COM_AZURE_DEV__BASE__ALLOW_OPTIMIZATION)
#  pragma optimize("", on)
#endif
#endif

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
#include <sys/resource.h>
#endif

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS)
#  include <malloc.h>
#endif

// ATTENTION: we do not want to record heap/IO for Profile implementation
// TAG: compact stack trace like utf8

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// Using this to avoid include of Profiler
ComputeTask::ComputeTask(const char* name) noexcept
{
  if (Profiler::isEnabledDirect()) { // doesnt matter if we do this for scope disabled case - we check this in destructor
    taskId = Profiler::Task::getTask(name, Profiler::CAT_COMPUTE);
  }
}

ComputeTask::~ComputeTask() noexcept
{
  if (taskId != static_cast<unsigned int>(-1)) {
    Profiler::Task::pushTask(taskId);
  }
}

_COM_AZURE_DEV__BASE__GLOBAL_PRINT();

namespace profiler {

  SpinLock lock;

  PreferredAtomicCounter memoryUsed;
  PreferredAtomicCounter objects;

  inline void writeString(FileOutputStream& fos, const char* text)
  {
    fos.write(reinterpret_cast<const uint8*>(text), (unsigned int)getNullTerminatedLength(text), false);
  }

  inline void writeString(FileOutputStream& fos, const String& text)
  {
    fos.write(reinterpret_cast<const uint8*>(text.native()), (unsigned int)text.getLength(), false);
  }
}

using namespace profiler;

bool Profiler::ProfilerImpl::open(const String& path)
{
  fos = FileOutputStream(path);
  return true;
}

void Profiler::ProfilerImpl::addEvent(const Profiler::Event& e)
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

unsigned int Profiler::ProfilerImpl::buildStackFrame(const uint32 sf)
{
  if (auto value = stackFramesLookup.find(sf)) { // found stack
    return *value;
  }

  const StackFrame& frame = getStackFrame(sf);
  auto size = frame.getSize();
  if (!INLINE_ASSERT(size > 0)) {
    return 0; // bad
  }

  if (sf & SF_HIGH_BIT) { // look through unhashed frames
    const uint32 index = sf & ~SF_HIGH_BIT;
    for (MemoryDiff i = static_cast<int32>(index) - 1; i >= 0; --i) {
      if (stackFramesUnhash[i] == frame) {
        if (auto value = stackFramesLookup.find(static_cast<unsigned int>(i | SF_HIGH_BIT))) { // found stack
          stackFramesLookup.add(sf, *value); // remember this sf
          return *value;
        }
        // BASSERT(!"Unexpected flow.");
        break;
      }
    }
  }
  
  unsigned int parent = 0; // no parent
  auto trace = frame.getTrace();

  String previousName;
  String path;
  void* previousImageAddress = nullptr;

  for (MemoryDiff i = size - 1; i >= 0; --i) { // reverse trace!
    if (auto ip = trace[i]) {
      auto symbol = DynamicLinker::getSymbolInfo(ip);
      if (!symbol) {
        continue;
      }
      const String demangled = TypeInfo::demangleName(symbol.name.native());
      if (!demangled) {
        continue;
      }
      if (demangled == previousName) {
        continue;
      }
      previousName = demangled;
      if (demangled.startsWith("base::Profiler::")) { // we do not want to include Profiler methods in stack
        continue;
      }
      
      if (!symbol.imageAddress || (symbol.imageAddress != previousImageAddress)) {
        previousImageAddress = symbol.imageAddress;
        path = FileSystem::getComponent(DynamicLinker::getImagePath(ip), FileSystem::NAME); // reduce since FILENAME takes up more space
      }

      if (stackPattern) { // could really be list of allowed modules "name;name2;name3"
        if (!Parser::doesMatchPattern(stackPattern, path)) {
          continue;
        }
      }

      const SymbolAndParent current(symbol.address, parent);
      if (auto found = stackFramesBySymbol.find(current)) {
        parent = *found + 1; // index  to parent
        // fout << "REUSE FRAME " << parent << " => " << it->parent << " " << it->name << ENDL;
      } else {
        if (stackFrames.getSize() == stackFrames.getCapacity()) {
          stackFrames.ensureCapacity(stackFrames.getSize() * 2);
        }
        stackFrames.append(Frame(demangled, path, parent));
        // auto previous = parent;
        parent = static_cast<unsigned int>(stackFrames.getSize()); // next frame uses this as parent
        stackFramesBySymbol.add(current, parent - 1); // index of frame

        // TAG: rebalance tree until done on
        if (stackFramesBySymbol.getSize() > 4) {
          ++rebalanceCount;
          if (rebalanceCount > stackFramesBySymbol.getSize() / 4) {
            stackFramesBySymbol.rebalance();
            rebalanceCount = 0;
          }
        }
        // fout << "NEW FRAME " << parent << " => " << previous << " " << demangled << ENDL;
      }
    }
  }

  if (parent) {
    stackFramesLookup.add(sf, parent);
  }

  return parent;
}

void Profiler::ProfilerImpl::releaseEvents()
{
  while (blocks) {
    auto b = blocks;
    blocks = blocks->next;
    b->~Block();
    free(b);
  }
}

void Profiler::ProfilerImpl::release()
{
  stackFramesHash.ensureCapacity(0);
  stackFramesHash.setSize(0);
  stackFramesUnhash.ensureCapacity(0);
  stackFramesUnhash.setSize(0);
  stackFrames.ensureCapacity(0);
  stackFrames.setSize(0);
  
#if 0
  if (false) {
    // TAG: measure balancing
    
    auto root = buildObjectModel(stackFramesBySymbol.getTree().getRoot());
    if (root) {
      FileOutputStream fos("symbols.json");
      writeString(fos, JSON::getJSON(root));
    }
  }
#endif
  
  stackFramesBySymbol.removeAll();
  stackFramesLookup.removeAll();
  releaseEvents();
}

uint32 Profiler::ProfilerImpl::getStackFrame(const ConstSpan<const void*>& stackTrace)
{
  constexpr unsigned int FRAMES = 65521; // 4723
  const uint32 hash = StackFrame::getHash(stackTrace) % FRAMES;

  SpinLock::Sync _sync(lock);

  if (stackFramesHash.getSize() != FRAMES) { // we could resize if too many conflicts but would require calc of hashes
    stackFramesHash.setSize(FRAMES);
    stackFramesUnhash.ensureCapacity(1024);
  }

  auto& hashItem = stackFramesHash[hash];
  if (hashItem.isEmpty()) { // unused slot
    hashItem = stackTrace; // allocation
    return hash;
  } else {
    if (hashItem == stackTrace) { // hit
      return hash;
    } else {
      auto size = stackFramesUnhash.getSize();
      if (size >= stackFramesUnhash.getCapacity()) {
        stackFramesUnhash.ensureCapacity(stackFramesUnhash.getSize() * 2);
      }
      if (size >= SF_HIGH_BIT) {
        return SF_HIGH_BIT | (SF_HIGH_BIT - 1); // no more room
      }
      stackFramesUnhash.append(stackTrace);
      return static_cast<uint32>(size | SF_HIGH_BIT); // differentiate from hash id
    }
  }
}

Profiler::ProfilerImpl::ProfilerImpl()
{
}

Profiler::ProfilerImpl::~ProfilerImpl()
{
  enabled = false;
  
  close();
  release();
}

bool Profiler::isEnabled() noexcept
{
  return enabled;
}

void Profiler::setUseStackFrames(bool _useStackFrames) noexcept
{
  profiler.useStackFrames = _useStackFrames;
}

void Profiler::setMinimumWaitTime(unsigned int _minimumWaitTime) noexcept
{
  profiler.minimumWaitTime = _minimumWaitTime;
}

void Profiler::setMinimumHeapSize(unsigned int _minimumHeapSize) noexcept
{
  profiler.minimumHeapSize = _minimumHeapSize;
}

void Profiler::setCaptureIO(unsigned int maximumSize) noexcept
{
  profiler.captureIOSize = maximumSize;
}

void Profiler::setStackPattern(const String& _stackPattern) noexcept
{
  profiler.stackPattern = _stackPattern;
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
  return profiler.numberOfEvents;
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

unsigned int Profiler::CaptureIO::setCaptureIO(unsigned int size) noexcept
{
  if (auto tlc = Thread::getLocalContext()) {
    auto& profiling = tlc->profiling;
    const unsigned int captureIO = profiling.captureIO;
    profiling.captureIO = size;
    return captureIO;
  }
  return 0;
}

void Profiler::release()
{
  if (enabled) {
    close();
  }
  SpinLock::Sync _sync(lock);
  profiler.release();
}

uint64 Profiler::getTimestamp() noexcept
{
  return Timer::getNow();
}

bool Profiler::open(const String& path)
{
  static bool initialized = false;
  if (initialized) {
    return false;
  }
  profiler.open(path);
  enabled = true;
  return true;
}

void Profiler::start()
{
  enabled = true;
  pushProcessMeta(new ReferenceString(Process::getProcess().getName()));
 
  if (auto tlc = Thread::getLocalContext()) {
    if (tlc->simpleId != 1) { // current thread is not main thread
      Event e;
      initEvent(e);
      e.tid = 1;
      e.ph = EVENT_META;
      e.name = "thread_name";
      e.cat = "THREAD"; // "__metadata"
      e.data = new ReferenceString("main");
      pushEvent(e);
    }
  }
  
  auto name = Thread::getThreadName();
  pushThreadMeta(new ReferenceString(name));
}

void Profiler::stop() noexcept
{
  enabled = false;
}

void Profiler::initEvent(Event& e) noexcept
{
  // e.pid = pid; // written on export
  if (auto tlc = Thread::getLocalContext()) {
    e.tid = tlc->simpleId;
  } else {
    e.tid = 0; // e.g. before TLC has been constructed
  }

  if (profiler.useStackFrames) {
    SuspendProfiling suspendProfiling; // no thanks to recursion
    // skip initEvent() -> internal caller -> ?

    // avoid allocation since stack trace is likely to be in hash table
    const void* stackTrace[ProfilerImpl::MAXIMUM_STACK_TRACE];
    const unsigned int size = StackFrame::getStack(stackTrace, getArraySize(stackTrace), 2);
    e.sf = profiler.getStackFrame(ConstSpan<const void*>(stackTrace, size));
  }

  const uint64 now = Timer::getNow();
  e.ts = Timer::toXTimeUS(now);
  e.dur = Timer::toXTimeUS(now - Timer::toTimeUS(e.ts)); // any rounding error
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

#if 0
Profiler::Event* Profiler::Task::getEvent() noexcept
{
  return nullptr;
}
#endif

void Profiler::Task::setTaskResourceHandle(const ResourceHandle& handle) noexcept
{
  if (taskId == BAD) {
    return;
  }
  if (!enabled) {
    return;
  }
  auto tlc = Thread::getLocalContext();
  Event& e = tlc->profiling.events.getElements()[taskId];
  if (e.cat == CAT_CREATE_RESOURCE) {
    // we must copy to avoid resource not lingering
    Reference<ReferenceResource> data = new ReferenceResource(handle.getResourceId());
    if (handle.getDescription()) {
      data->description = handle.getDescription();
    }
    e.data = data;
  }
}

void Profiler::Task::setTaskSecurityLevel(unsigned int securityLevel) noexcept
{
  if (taskId == BAD) {
    return;
  }
  if (!enabled) {
    return;
  }
  auto tlc = Thread::getLocalContext();
  Event& e = tlc->profiling.events.getElements()[taskId];
  if (e.cat == CAT_SECURITY) {
    e.data = new ReferenceValue(securityLevel);
  }
}

void Profiler::Task::setTaskWaitId(const char* id) noexcept
{
  if (taskId == BAD) {
    return;
  }
  if (!enabled) {
    return;
  }
  if (!id) {
    return;
  }
  auto tlc = Thread::getLocalContext();
  Event& e = tlc->profiling.events.getElements()[taskId];
  if (e.cat == CAT_WAIT) {
    e.data = new ReferenceString(id);
  }
}

void Profiler::Task::setTaskWaitHandle(const ResourceHandle& handle) noexcept
{
  if (taskId == BAD) {
    return;
  }
  if (!enabled) {
    return;
  }
  auto tlc = Thread::getLocalContext();
  Event& e = tlc->profiling.events.getElements()[taskId];
  if (e.cat == CAT_WAIT) {
    // we must copy to avoid resource not lingering
    e.data = new ReferenceResource(handle.getResourceId()/*, handle.getCreatedById()*/);
  }
}

void Profiler::Task::setTaskBytesRead(const uint8* buffer, unsigned int bytesRead) noexcept
{
  if (taskId == BAD) {
    return;
  }
  if (!enabled) {
    return;
  }
  auto tlc = Thread::getLocalContext();
  Event& e = tlc->profiling.events.getElements()[taskId];
  if (e.cat == CAT_IO_READ) {
    if (buffer && (tlc->profiling.captureIO > 0)) {
      String bytes(reinterpret_cast<const char*>(buffer), minimum(bytesRead, tlc->profiling.captureIO));
      e.data = new ReferenceIO(bytesRead, bytes);
    } else {
      e.data = new ReferenceIO(bytesRead);
    }
    tlc->bytesRead += bytesRead;
  }
}

void Profiler::Task::setTaskBytesWritten(const uint8* buffer, unsigned int bytesWritten) noexcept
{
  if (taskId == BAD) {
    return;
  }
  if (!enabled) {
    return;
  }
  auto tlc = Thread::getLocalContext();
  Event& e = tlc->profiling.events.getElements()[taskId];
  if (e.cat == CAT_IO_WRITE) {
    if (buffer && (tlc->profiling.captureIO > 0)) {
      String bytes(reinterpret_cast<const char*>(buffer), minimum(bytesWritten, tlc->profiling.captureIO));
      e.data = new ReferenceIO(bytesWritten, bytes);
    } else {
      e.data = new ReferenceIO(bytesWritten);
    }
    tlc->bytesRead += bytesWritten;
  }
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
  if (!tlc) {
    return;
  }
  BASSERT(taskId < tlc->profiling.tasks);
  BASSERT(tlc->profiling.tasks > 0);
  --(tlc->profiling.tasks); // pop stack
  if (tlc->profiling.suspended != 0) {
    return;
  }

  // use EVENT_COMPLETE to combine BEGIN and END events
  const uint64 now = Timer::getNow();
  Event& e = tlc->profiling.events.getElements()[taskId];
  e.ph = EVENT_COMPLETE;
  const uint64 start = Timer::toTimeUS(e.ts) + Timer::toTimeUS(e.dur);
  auto dur = now - start;
  if (e.cat == CAT_WAIT) {
    tlc->waiting += dur;
  }
  if (dur < profiler.minimumWaitTime) {
    return;
  }
  e.dur = Timer::toXTimeUS(dur);
  pushEvent(e);
}

void Profiler::pushEvent(const Event& e)
{
  if (!INLINE_ASSERT(enabled)) {
    return;
  }
  
  ++profiler.numberOfEvents;
  profiler.addEvent(e);
}

void Profiler::pushObjectCreateImpl(MemorySize id, MemorySize size)
{
  memoryUsed += size;
  ++objects;

  if (size > 0) {
    if (size < profiler.minimumHeapSize) {
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
  e.id = id; // remember id
  pushEvent(e);
  
  static MemorySize sample = 0;
  sample += size;
  sample += 128;
  if (sample >= 4096) {
    sample = 0;
    pushCountersImpl();
  }
  
}

void Profiler::pushObjectCreateImpl(MemorySize id)
{
  pushObjectCreateImpl(id, 0);
}

void Profiler::pushObjectDestroyImpl(MemorySize id, MemorySize size)
{
  // profiler might have been enabled after objects have been created
  // BASSERT(objects > 0);
  // BASSERT(memoryUsed >= size);
  memoryUsed -= size;
  --objects;

  if (size > 0) {
    if (size < profiler.minimumHeapSize) {
      return;
    }
  }
  if (!isEnabledScope()) {
    return;
  }
  Event e;
  initEvent(e);
  e.ph = EVENT_OBJECT_DESTROY;
  e.cat = CAT_MEMORY;
  e.id = id; // remember id
  pushEvent(e);
}

void Profiler::pushObjectDestroyImpl(MemorySize id)
{
  pushObjectDestroyImpl(id, 0);
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

void Profiler::pushExceptionImpl(const Exception& exception, const Type* type)
{
  if (!isEnabledScope()) {
    return;
  }
  const char* name = (type ? *type : exception.getThisType()).getLocalName();

  Event e;
  initEvent(e);
  e.ph = EVENT_INSTANT;
  if (name) {
    String description = TypeInfo::demangleName(name);
    if (const char* message = exception.getMessage()) {
      description += " with message '";
      description += message;
      description += "'";
    }
    e.data = new ReferenceString(description);
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

void Profiler::pushInstantImpl(const char* name, const char* cat)
{
  if (!isEnabledScope()) {
    return;
  }
  Event e;
  initEvent(e);
  e.ph = EVENT_INSTANT;
  e.name = name;
  e.cat = cat;
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
  e.cat = "PROCESS"; // "__metadata"
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
  e.cat = "THREAD"; // "__metadata"
  e.data = name;
  pushEvent(e);
}

Profiler::ReferenceCounters::ReferenceCounters() noexcept
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  this->memoryUsed = profiler::memoryUsed; // usage.ru_maxrss; // usage.ru_ixrss + usage.ru_idrss + usage.ru_isrss;
  this->objects = profiler::objects;

  // TAG: temp test
  struct rusage usage;
  int status = getrusage(RUSAGE_SELF, &usage);
  if (status == 0) {
    this->processingTime = static_cast<uint64>(usage.ru_utime.tv_sec + usage.ru_stime.tv_sec) * 1000000 + static_cast<uint64>(usage.ru_utime.tv_usec + usage.ru_stime.tv_usec); // Process::getCurrentProcess().getTimes();
  } else {
    this->processingTime = 0;
  }
  
  // this->io = usage.ru_msgrcv + usage.ru_msgsnd;
  // this->operations = usage.ru_inblock + usage.ru_oublock;
#endif

  resources = ResourceHandle::getResources();
}

void Profiler::pushCountersImpl()
{
  if (!isEnabledScope()) {
    return;
  }
  Event e;
  initEvent(e);
  e.ph = EVENT_COUNTER;
  e.name = "UpdateCounters"; // what values are allowed
  e.cat = "PROCESS"; // "__metadata"
  e.data = new ReferenceCounters();
  pushEvent(e);
}

void Profiler::ProfilerImpl::close()
{
  constexpr bool useJSON = true;
  enabled = false;

  if (!fos.isOpen()) {
    return;
  }
    
  Block* current = blocks;
  if (current) {
    while (current->next) {
      current = current->next;
    }
  }

  ObjectModel o;
  
  if (useJSON) {
    writeString(fos, "{\n");
    writeString(fos, "\"traceEvents\": [\n");
  }

  auto PID = o.createString("pid");
  auto TID = o.createString("tid");
  auto PH = o.createString("ph");
  auto NAME = o.createString("name");
  auto CAT = o.createString("cat");
  auto TS = o.createString("ts");
  auto DUR = o.createString("dur");
  auto TTS = o.createString("tts");
  auto TDUR = o.createString("tdur");
  auto SF = o.createString("sf");
  auto ARGS = o.createString("args");
  auto DATA = o.createString("data");
  auto PARENT = o.createString("parent");
  auto CATEGORY = o.createString("category");
  auto ID = o.createString("id");
  auto CNAME = o.createString("cname");

  auto WAITING_FOR = o.createString("waiting for");
  auto BYTES_READ = o.createString("read");
  auto BYTES_WRITTEN = o.createString("written");
  auto BUFFER = o.createString("buffer");
  auto SEVERITY = o.createString("severity");
  auto THREAD = o.createString("thread");
  auto RESOURCE = o.createString("resource");
  auto DESCRIPTION = o.createString("description");

  auto PH_B = o.createString("B");
  auto PH_E = o.createString("E");
  auto PH_b = o.createString("b");
  auto PH_e = o.createString("e");
  auto PH_X = o.createString("X");
  auto PH_C = o.createString("C");
  auto PH_N = o.createString("N");
  auto PH_D = o.createString("D");
  auto PH_P = o.createString("P");
  auto PH_v = o.createString("v");
  auto PH_M = o.createString("M");
  auto PH_i = o.createString("i");

/* cnames:
  thread_state_uninterruptible
  thread_state_iowait
  thread_state_running
  thread_state_runnable
  thread_state_sleeping
  thread_state_unknown
  background_memory_dump
  light_memory_dump
  detailed_memory_dump
  vsync_highlight_color
  generic_work
  good
  bad
  terrible
  black
  grey
  white
  yellow
  olive
  rail_response
  rail_animation
  rail_idle
  rail_load
  startup
  heap_dump_stack_frame
  heap_dump_object_type
  heap_dump_child_node_arrow
  cq_build_running
  cq_build_passed
  cq_build_failed
  cq_build_abandoned
  cq_build_attempt_runnig
  cq_build_attempt_passed
  cq_build_attempt_failed
*/
  
  auto _pid = o.createInteger(pid);
  auto _tid = o.createInteger(0);

  auto P_EXCEPTION = static_cast<const void*>(CAT_EXCEPTION);
  auto P_WAIT = static_cast<const void*>(CAT_WAIT);
  auto P_SIGNAL = static_cast<const void*>(CAT_SIGNAL);

  bool first = true;
  while (current) {
    for (MemorySize i = 0; i < current->size; ++i) {
      const Event& e = current->events[i];
      if (useJSON) {

        if (e.tid != _tid->value) {
          _tid = o.createInteger(e.tid);
        }
        
        auto item = o.createObject();
        item->setValue(PID, _pid);
        item->setValue(TID, _tid);
        item->setValue(TS, o.createInteger(Timer::toTimeUS(e.ts))); // required
        if (e.name) {
          item->setValue(NAME, o.createString(e.name));
        }
        if (e.cat) {
          item->setValue(CAT, o.createString(e.cat));
        }
        
        auto ph = PH_B;
        switch (e.ph) {
        case EVENT_BEGIN:
          ph = PH_B;
          break;
        case EVENT_END:
          ph = PH_E;
          break;
        case EVENT_ASYNC_BEGIN:
          ph = PH_b;
          break;
        case EVENT_ASYNC_END:
          ph = PH_e;
          break;
        case EVENT_COMPLETE:
          ph = PH_X;
          break;
        case EVENT_COUNTER:
          ph = PH_C;
          break;
        case EVENT_OBJECT_CREATE:
          ph = PH_N;
          break;
        case EVENT_OBJECT_DESTROY:
          ph = PH_D;
          break;
        case EVENT_SAMPLE:
          ph = PH_P;
          break;
        case EVENT_MEMORY:
          ph = PH_v;
          break;
        case EVENT_META:
          ph = PH_M;
          break;
        case EVENT_INSTANT:
          ph = PH_i;
          break;
        default:
          BASSERT(!"Unsupported PH");
        }
        item->setValue(PH, ph);

        switch (e.ph) {
        case EVENT_COMPLETE:
          item->setValue(DUR, o.createInteger(Timer::toTimeUS(e.dur)));
          
#if 0
          if (static_cast<const void*>(e.cat) == P_WAIT) {
            item->setValue(CNAME, o.createString("bad"));
          }
#endif
          // item->setValue(TDUR, o.createInteger(e.tdur));
          // item->setValue(TTS, o.createInteger(e.tts));

          if (e.cat == CAT_WAIT) {
            if (auto r = e.data.cast<ReferenceResource>()) {
              auto args = o.createObject();
              item->setValue(ARGS, args);
              if (r->resourceId) {
                args->setValue(RESOURCE, o.createInteger(r->resourceId)); // Trace: would be nice if we could link IDs in visualization to jump automatically between related objects and highlight all instances
              }
            } else if (auto r = e.data.cast<ReferenceString>()) {
              auto args = o.createObject();
              item->setValue(ARGS, args);
              if (r->string) {
                args->setValue(WAITING_FOR, r->string);
              }
            }
          } else if (e.cat == CAT_CREATE_RESOURCE) {
            if (auto r = e.data.cast<ReferenceResource>()) {
              auto args = o.createObject();
              item->setValue(ARGS, args);
              if (r->resourceId) {
                args->setValue(RESOURCE, o.createInteger(r->resourceId));
              }
              if (r->description) {
                args->setValue(DESCRIPTION, r->description);
              }
#if 0
              if (r->createdById) {
                // TAG: lookup thread name
                args->setValue(THREAD, o.createInteger(r->createdById));
              }
#endif
            }
          } else if (e.cat == CAT_IO_READ) {
            if (auto r = e.data.cast<ReferenceIO>()) {
              auto args = o.createObject();
              item->setValue(ARGS, args);
              args->setValue(BYTES_READ, o.createInteger(r->size));
              if (r->bytes) {
                args->setValue(BUFFER, r->bytes);
              }
            }
          } else if (e.cat == CAT_IO_WRITE) {
            if (auto r = e.data.cast<ReferenceIO>()) {
              auto args = o.createObject();
              item->setValue(ARGS, args);
              args->setValue(BYTES_WRITTEN, o.createInteger(r->size));
              if (r->bytes) {
                args->setValue(BUFFER, r->bytes);
              }
            }
          } else if (e.cat == CAT_SECURITY) {
            if (auto r = e.data.cast<ReferenceValue>()) {
              auto args = o.createObject();
              item->setValue(ARGS, args);
              args->setValue(SEVERITY, o.createInteger(r->value)); // TAG: convert to string
            }
          }
          break;
        case EVENT_META:
          if (auto r = e.data.cast<ReferenceString>()) {
            auto args = o.createObject();
            item->setValue(ARGS, args);
            args->setValue(NAME, o.createString(r->string));
          }
          // item->setValue(CAT, o.createString("__metadata"));
          /*
            "name":"num_cpus","args":{"number":8}
            "name":"process_sort_index","args":{"sort_index":-5}
            "name":"process_uptime_seconds","args":{"uptime":INT}
            "name":"thread_sort_index","args":{"sort_index":-1}
          */
          break;
        case EVENT_OBJECT_CREATE:
        case EVENT_OBJECT_DESTROY:
          if (e.id) {
            item->setValue(ID, o.createInteger(e.id));
          }
          break;
        case EVENT_INSTANT:
          if (auto r = e.data.cast<ReferenceString>()) {
            item->setValue(NAME, o.createString(r->string));
          }
#if 0
          if (static_cast<const void*>(e.cat) == P_SIGNAL) {
            item->setValue(CNAME, o.createString("good"));
          } else if (static_cast<const void*>(e.cat) == P_EXCEPTION) {
            item->setValue(CNAME, o.createString("bad"));
          }
#endif
          break;
        case EVENT_COUNTER:
          if (auto r = e.data.cast<ReferenceCounters>()) {
            item->setValue("s", "g");
            auto args = o.createObject();
            item->setValue(ARGS, args);
            auto data = o.createObject();
            args->setValue(DATA, data);

            // only supported counters:
            data->setValue("jsHeapSizeUsed", format() << r->memoryUsed);
            data->setValue("documents", format() << r->objects);
            data->setValue("nodes", format() << r->resources);
            data->setValue("jsEventListeners", format() << r->processingTime);

            // need support for more counters:
            // data->setValue("processingTime", format() << r->processingTime);
            // data->setValue("io", format() << r->io);
            // data->setValue("operations", format() << r->operations);
          }
          break;
        default:
          ; // what data do we need
        }

        if (useStackFrames && e.sf) { // TAG: is 0 a valid sf id?
          // "sf" or "stack": ["0x1", "0x2"] // for stack frame
          const unsigned int id = buildStackFrame(e.sf); // TAG: crashes
          item->setValue(SF, o.createString(format() << id));
        }

        // "s" for instant event scope global, process, thread (default)
        if (!first) {
          writeString(fos, ",\n");
        }
        first = false;

        writeString(fos, JSON::getJSON(item)); // single event
      } else {
        fos.write(reinterpret_cast<const uint8*>(&e), sizeof(e), false);
      }
    }
    current = current->previous;
  }
  
  if (useJSON) {
    writeString(fos, "\n");
    writeString(fos, "]\n");
    
    if (useJSON) {
      if (auto app = Application::getApplication()) {
        auto otherData = o.createObject();
        otherData->setValue(o.createString("version"), o.createString(app->getFormalName()));
        writeString(fos, ",\n\"otherData\": ");
        writeString(fos, JSON::getJSON(otherData));
        writeString(fos, "\n"); // terminate otherData
      }

      if (true) {
        auto metadata = o.createObject();
        metadata->setValue(o.createString("clock-domain"), o.createString("MONOTONIC"));
        metadata->setValue(o.createString("highres-ticks"), o.createBoolean(true));

        if (auto os = SystemInformation::getOS()) {
          metadata->setValue(o.createString("os-name"), o.createString(os));
        }
        // metadata->setValue(o.createString("physical-memory"), o.createInteger(...::getMemory()/1024/1024));

        // "clock-domain":"LINUX_CLOCK_MONOTONIC"
        // "command_line":STR // do NOT include due to possibility of tokens/passwd
        // "cpu-brand":STR,"cpu-family":INT,"cpu-model":INT,"cpu-stepping":INT
        // "network-type":"WiFi"
        // "os-arch":"x86_64"
        // "os-version":""
        // "product-version":"BASE 0.9"
        // metadata->setValue(o.createString("num-cpus"), o.createInteger(0));
        // metadata->setValue(o.createString("revision"), o.createString("GITID"));

        writeString(fos, ",\n\"metadata\": ");
        writeString(fos, JSON::getJSON(metadata));
        writeString(fos, "\n"); // terminate metadata
      }
    }

    if (useStackFrames) {
      writeString(fos, ",\n\"stackFrames\": ");

      auto frames = o.createObject();
      for (MemorySize id = 0; id < stackFrames.getSize(); ++id) {
        const auto& f = stackFrames[id];
        auto frame = o.createObject();
        if (f.parent != 0) {
          frame->setValue(PARENT, o.createString(format() << f.parent));
        }
        if (f.name) {
          frame->setValue(NAME, o.createString(f.name));
        }
        if (f.category) {
          frame->setValue(CATEGORY, o.createString(f.category));
        }
        frames->setValue(o.createString(format() << (id + 1)), frame); // 1-indexed
        // writeString(fos, JSON::getJSON(frame));
      }

      writeString(fos, JSON::getJSON(frames));
      writeString(fos, "\n"); // terminate frames
    }

    writeString(fos, "}\n"); // terminate root object
    
    // TAG: trace viewer - extension - would like redirect for lookup url spec in JSON
    // https://cs.chromium.org/search/?sq=package:chromium&type=cs&q=Thread::entry()

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

  fos = FileOutputStream();

  release();
}

void Profiler::close()
{
  stop();
  SpinLock::Sync _sync(lock);
  profiler.close();
}

#if 0 && defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Profiler) : public UnitTest {
public:

  TEST_PRIORITY(1);
  TEST_PROJECT("base");
  TEST_IMPACT(CRITICAL);
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    Profiler::open("profiler.json");
    Profiler::setUseStackFrames(true);
    Profiler::start();
    // doit
    Profiler::close();
  }
};

TEST_REGISTER(Profiler);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
