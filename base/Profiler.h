/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/concurrency/Thread.h>
#include <base/concurrency/Process.h>
#include <base/io/FileOutputStream.h>
#include <base/TypeInfo.h>
#include <base/Timer.h>
#include <base/concurrency/SpinLock.h>
#include <base/collection/Map.h>

// TAG: check unknown threads
// TAG: add heap ids
// TAG: skip extra frame when ready
// TAG: hook exception throws
// TAG: trim module filename
// TAG: support external stack trace
// TAG: sample memory/objects

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Profiler.
 
  https://docs.google.com/document/d/1CvAClvFfyA5R-PhYUmn5OOQtYMH4h6I0nSsKchNAySU/preview
*/

class _COM_AZURE_DEV__BASE__API Profiler {
public:

  /** Event information. Volatile class subject to change. */
  class _COM_AZURE_DEV__BASE__API Event {
  public:

    Reference<ReferenceCountedObject> data; // meta info
    const char* cat = nullptr;
    const char* name = nullptr;
    // uint32 pid = 0; // process id // we output on export
    uint32 tid = 0; // thread id
    Timer::XTime ts; // timestamp
    Timer::XTime dur; // duration
    // Timer::XTime tdur = 0; // optional - thread duration
    // uint64 tts = 0; // optional thread timestamp
    uint32 sf = 0; // stack frame // uint16 would be enough
    char ph = 0; // event type
    uint8 flags = 0; // flags
    uint16 id = 0xffff; // object id - 0xffff is reserved
  };
  
  /** Single frame. */
  class Frame {
  public:

    String name;
    String category; // module
    unsigned int parent = 0; // 0 is no parent

    inline Frame()
    {
    }

    inline Frame(const String& _name, const String& _category, unsigned int _parent)
      : name(_name), category(_category), parent(_parent)
    {
    }

    inline bool operator==(const Frame& compare) const noexcept
    {
      return (parent == compare.parent) &&
        (name == compare.name) &&
        (category == compare.category);
    }
  };

  // TAG: should be hidden
  class ProfilerImpl {
  public:

    /** Block of events. */
    class Block {
    public:
      
      static constexpr unsigned int SIZE = 4096;

      Block* next = nullptr; // next block
      Block* previous = nullptr; // previous block
      Profiler::Event events[SIZE]; // preallocated buffer for events
      MemorySize size = 0; // events in the block
    };

    /** Double linked list of all events. */
    Block* blocks = nullptr;
    const unsigned int pid = Process::getProcess().getId();
    Array<StackFrame> stackFramesHash; // cached frames (hash table)
    Array<StackFrame> stackFramesUnhash; // cached frames (remaining stack traces)
    String stackPattern; // stack frame pattern
    Array<Frame> stackFrames; // all frames - index is id for frame
    Array<MemorySize> stackFramesRoots; // only frame roots
    Map<uint32, unsigned int> stackFramesLookup; // lookup for sf to first frame

    PreferredAtomicCounter numberOfEvents;
    FileOutputStream fos;
    bool useStackFrames = false; // include stack frames for events
    unsigned int minimumWaitTime = 1; // minimum time to wait to record event
    unsigned int minimumHeapSize = 4096 * 2/2; // minimum heap size to record event

    static constexpr uint32 SF_HIGH_BIT = 0x80000000U;
    
    bool open(const String& path);

    /** Add new event. */
    void addEvent(const Profiler::Event& e);

    /** Returns the stack frame for the given sf. */
    inline const StackFrame& getStackFrame(uint32 sf) const noexcept
    {
      if ((sf & SF_HIGH_BIT) == 0) {
        // BASSERT(sf < stackFramesHash.getSize());
        return stackFramesHash[sf];
      } else {
        const uint32 index = sf & ~SF_HIGH_BIT;
        // BASSERT(index < stackFramesUnhash.getSize());
        return stackFramesUnhash[index];
      }
    }

    uint32 getStackFrame(StackFrame&& stackTrace);

    unsigned int buildStackFrame(const uint32 sf);
    
    void close();
    
    /** Release events. */
    void releaseEvents();

    /** Release. */
    void release();
    
    ~ProfilerImpl();
  };
  
  static ProfilerImpl profiler;
private:

  static bool enabled;

  /** Submit event. */
  static void pushEvent(const Event& e);
  static void pushObjectCreateImpl(MemorySize size);
  static void pushObjectDestroyImpl(MemorySize size);
  static void pushExceptionImpl(const char* type);
  static void pushSignalImpl(const char* name);
  static void pushThreadStartImpl(const char* name, unsigned int parentId);
  static void pushProcessMetaImpl(ReferenceCountedObject* name);
  static void pushThreadMetaImpl(ReferenceCountedObject* name/*, unsigned int parentId*/);
public:

  /** Enables stack frames. */
  static void setUseStackFrames(bool useStackFrames) noexcept;
  /** Set the minimum time to wait to record event. */
  static void setMinimumWaitTime(unsigned int minimumWaitTime) noexcept;
  /** Sets the minimum heap size to record event. */
  static void setMinimumHeapSize(unsigned int minimumHeapSize) noexcept;
  /** Sets the stack frame pattern. */
  static void setStackPattern(const String& stackPattern) noexcept;
  
  static constexpr const char* CAT_MEMORY = "MEMORY";
  static constexpr const char* CAT_OBJECT = "OBJECT";
  static constexpr const char* CAT_IO = "IO";
  static constexpr const char* CAT_NETWORK = "NET";
  static constexpr const char* CAT_WAIT = "WAIT";
  static constexpr const char* CAT_EXCEPTION = "EXCEPTION";
  static constexpr const char* CAT_SIGNAL = "SIGNAL";
  static constexpr const char* CAT_RENDERER = "RENDERER";
  
  enum {
    EVENT_BEGIN = 'B',
    EVENT_END = 'E',
    EVENT_ASYNC_BEGIN = 'b',
    EVENT_ASYNC_END = 'e',
    EVENT_COMPLETE = 'X',
    EVENT_COUNTER = 'C',
    EVENT_OBJECT_CREATE = 'N', // set "id"
    EVENT_OBJECT_DESTROY = 'D', // set "id"
    EVENT_SAMPLE = 'P',
    EVENT_MEMORY = 'v',
    EVENT_META = 'M',
    EVENT_INSTANT = 'i'
  };

  /** Suspends profiling for the current scope. */
  class _COM_AZURE_DEV__BASE__API SuspendProfiling {
  private:
  
    static void suspendAndResume(bool resume) noexcept;
  public:
    
    inline SuspendProfiling() noexcept
    {
      suspendAndResume(false);
    }

    inline ~SuspendProfiling() noexcept
    {
      suspendAndResume(true);
    }
  };

  /** Use this to profile a specific class. */
  class _COM_AZURE_DEV__BASE__API ProfileObject {
  private:
    
    const MemorySize id = Debug::allocateUniqueId();
  public:
    
    ProfileObject() noexcept
    {
      pushObjectCreate(id | (static_cast<MemorySize>(1) << (sizeof(MemorySize) * 8 - 1)));
    }

#if 0
    // cat not used for now
    ProfileObject(const char* _cat) noexcept : cat(_cat)
    {
      pushObjectCreate(id | (static_cast<MemorySize>(1) << (sizeof(MemorySize) * 8 - 1)));
    }
#endif

    ~ProfileObject() noexcept
    {
      pushObjectDestroy(id | (static_cast<MemorySize>(1) << (sizeof(MemorySize) * 8 - 1)));
    }
  };

  /** A reference counted string. */
  class _COM_AZURE_DEV__BASE__API ReferenceString : public ReferenceCountedObject {
  public:
    
    String string;
    
    inline ReferenceString() noexcept
    {
    }

    inline ReferenceString(const String& _string) noexcept : string(_string)
    {
    }
  };
  
  /** Returns timestamp. */
  static uint64 getTimestamp() noexcept;

  /** Opens profiler. */
  static bool open(const String& path);
  
  /** Closes profiler. */
  static void close();
  
  /** Enables profiler. */
  static void start();

  /** Disables profiler. */
  static void stop() noexcept;

  /** Returns true if profiler is enabled globally. */
  static bool isEnabled() noexcept;

  /** Returns true if profiler is enabled for scope. */
  static bool isEnabledScope() noexcept;

  /** Returns the number of events. */
  static MemorySize getNumberOfEvents() noexcept;
  
  /** Initializes event with pid/tid/ts. */
  static void initEvent(Event& e) noexcept;

  /** Task. */
  class _COM_AZURE_DEV__BASE__API Task {
  private:

    static constexpr unsigned int BAD = static_cast<unsigned int>(0) - 1;

    unsigned int taskId = BAD;

    static unsigned int getTask(const char* name, const char* cat) noexcept;

    static void pushTask(unsigned int taskId) noexcept;
  public:
    
    inline Task(const char* _name, const char* _cat = nullptr) noexcept
    {
      if (enabled) { // doesnt matter if we do this for scope disabled case - we check this in destructor
        taskId = getTask(_name, _cat);
      }
    }

    inline ~Task() noexcept // we must not allow exception for profiling since it changes exception handling otherwise
    {
      if (taskId != BAD) {
        pushTask(taskId);
      }
    }
  };

  /** Wait task. */
  class _COM_AZURE_DEV__BASE__API WaitTask : public Task {
  public:

    inline WaitTask(const char* name) : Task(name, CAT_WAIT)
    {
    }
  };

  /** IO task. */
  class _COM_AZURE_DEV__BASE__API IOTask : public Task {
  public:
    
    inline IOTask(const char* name) : Task(name, CAT_IO)
    {
    }
  };

  /** HTTPS/Websocket task. */
  class _COM_AZURE_DEV__BASE__API HTTPSTask : public IOTask {
  public:
    
    inline HTTPSTask(const char* name) : IOTask(name)
    {
    }
  };

  // TAG: push error/warning
  // TAG: exception constructors
  // TAG: push sample
  // TAG: auto push if task takes a long time
  
  static inline void pushObjectCreate(MemorySize size) // TAG: include pointer value - resize needs to call destroy/create when pointer changes
  {
    if (!enabled) {
      return;
    }
    pushObjectCreateImpl(size);
  }

  static inline void pushObjectDestroy(MemorySize size)
  {
    if (!enabled) {
      return;
    }
    pushObjectDestroyImpl(size);
  }

  static inline void pushException(const char* type)
  {
    if (!enabled) {
      return;
    }
    pushExceptionImpl(type);
  }

  static inline void pushSignal(const char* name)
  {
    if (!enabled) {
      return;
    }
    pushSignalImpl(name);
  }

  static inline void pushThreadStart(const char* name, unsigned int parentId)
  {
    if (!enabled) {
      return;
    }
    pushThreadStartImpl(name, parentId);
  }

  static inline void pushProcessMeta(ReferenceCountedObject* name)
  {
    if (!enabled) {
      return;
    }
    pushProcessMetaImpl(name);
  }

  static inline void pushThreadMeta(ReferenceCountedObject* name/*, unsigned int parentId*/)
  {
    if (!enabled) {
      return;
    }
    pushThreadMetaImpl(name);
  }

  /** Release profiler resources. */
  static void release();
  
  // TAG: process and thread stats - no sync for process
  class _COM_AZURE_DEV__BASE__API Stats {
  public:
    
    uint64 readOperations = 0;
    uint64 writeOperations = 0;
    uint64 readBytes = 0;
    uint64 writtenBytes = 0;
    uint64 readNetworkBytes = 0;
    uint64 writtenNetworkBytes = 0;
    uint64 allocatedBytes = 0;
    uint64 deallocatedBytes = 0;
    uint64 waiting = 0; // total time waiting in microseconds
  };
  
  // static Stats& getProcessStats();
  // static Stats& getThreadStats();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
