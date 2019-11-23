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
#include <base/Performance.h>

// TAG: add heap ids
// TAG: support external stack trace
// TAG: sample memory/objects
// TAG: push error/warning
// TAG: exception constructors
// push UI frame begin/end - count each frame
// push UI user interaction

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
    Timer::XTime ts; // timestamp
    Timer::XTime dur; // duration
#if 0 // thread timing is optional
    Timer::XTime tts; // timestamp
    Timer::XTime tdur; // duration
#endif
    uint32 sf = 0; // stack frame // uint16 is normally enough
    // uint32 pid = 0; // process id // we output on export
    uint16 tid = 0; // thread id // uint16 is normally enough
    uint16 id = 0xffff; // object id - 0xffff is reserved
    uint8 flags = 0; // flags
    char ph = 0; // event type
  };

  /** Single stack frame. */
  class Frame {
  public:

    const void* symbol = nullptr; // allows fast comparison
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

    inline Frame(void* _symbol, const String& _name, const String& _category, unsigned int _parent)
      : symbol(_symbol), name(_name), category(_category), parent(_parent)
    {
    }

    inline bool operator<(const Frame& compare) const noexcept
    {
      if (parent < compare.parent) {
        return true;
      } else if (parent > compare.parent) {
        return false;
      }
      auto n = name.compareTo(compare.name);
      if (n < 0) {
        return true;
      } else if (n > 0) {
        return false;
      }
      // TAG: compare reference pointer value
      auto c = category.compareTo(compare.category); // likely to be equal
      if (c < 0) {
        return true;
      }
      return false;
    }

    inline bool operator==(const Frame& compare) const noexcept
    {
      return symbol == compare.symbol; // allows fast comparison
#if 0
      return (parent == compare.parent) &&
        (name == compare.name) &&
        (category == compare.category);
#endif
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

    static constexpr unsigned int MAXIMUM_STACK_TRACE = 64;

    /** Double linked list of all events. */
    Block* blocks = nullptr;
    const unsigned int pid = Process::getProcess().getId();
    Array<StackFrame> stackFramesHash; // cached frames (hash table)
    Array<StackFrame> stackFramesUnhash; // cached frames (remaining stack traces)
    String stackPattern; // stack frame pattern
    Array<Frame> stackFrames; // all frames - index is id for frame
    Array<MemorySize> stackFramesByParent[MAXIMUM_STACK_TRACE]; // lookup by parent
    Performance::Counter stackFramesCounters[MAXIMUM_STACK_TRACE]; // lookup by parent
    Map<uint32, unsigned int> stackFramesLookup; // lookup for sf to first frame

    PreferredAtomicCounter numberOfEvents;
    FileOutputStream fos;
    bool useStackFrames = false; // include stack frames for events
    unsigned int minimumWaitTime = 1; // minimum time to wait to record event
    unsigned int minimumHeapSize = 4096 * 2/2; // minimum heap size to record event

    static constexpr uint32 SF_HIGH_BIT = 0x80000000U; // differentiates between hashed and unhashed buffers
    
    ProfilerImpl();

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

    /** Registers the stack trace. */
    uint32 getStackFrame(const ConstSpan<const void*>& stackTrace);

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
  static void pushObjectCreateImpl(MemorySize id, MemorySize size);
  static void pushObjectCreateImpl(MemorySize id);
  static void pushObjectDestroyImpl(MemorySize id, MemorySize size);
  static void pushObjectDestroyImpl(MemorySize id);
  static void pushExceptionImpl(const char* type);
  static void pushSignalImpl(const char* name);
  static void pushThreadStartImpl(const char* name, unsigned int parentId);
  static void pushProcessMetaImpl(ReferenceCountedObject* name);
  static void pushThreadMetaImpl(ReferenceCountedObject* name/*, unsigned int parentId*/);
  static void pushCountersImpl();
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
      pushObjectCreate(id, 0);
    }

#if 0
    // cat not used for now
    ProfileObject(const char* _cat) noexcept : cat(_cat)
    {
      pushObjectCreate(id, 0);
    }
#endif

    ~ProfileObject() noexcept
    {
      pushObjectDestroy(id, 0);
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

  /** A reference counted string. */
  class _COM_AZURE_DEV__BASE__API ReferenceCounters : public ReferenceCountedObject {
  public:
    
    MemorySize memoryUsed = 0; // memory in use
    MemorySize objects = 0; // number of objects
    uint64 processingTime = 0; // processing time
    uint64 io = 0; // IO transmitted
    uint64 operations = 0; // number of IO operations

    ReferenceCounters() noexcept;
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
  
  static inline void pushObjectCreate(MemorySize id, MemorySize size)
  {
    if (!enabled) {
      return;
    }
    pushObjectCreateImpl(id, size);
  }

  static inline void pushObjectCreate(MemorySize id)
  {
    if (!enabled) {
      return;
    }
    pushObjectCreateImpl(id);
  }

  static inline void pushObjectDestroy(MemorySize id, MemorySize size)
  {
    if (!enabled) {
      return;
    }
    pushObjectDestroyImpl(id, size);
  }

  static inline void pushObjectDestroy(MemorySize id)
  {
    if (!enabled) {
      return;
    }
    pushObjectDestroyImpl(id);
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

  static inline void pushCounters()
  {
    if (!enabled) {
      return;
    }
    pushCountersImpl();
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
