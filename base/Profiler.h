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

class ResourceHandle;

/**
  Profiler.
 
  https://docs.google.com/document/d/1CvAClvFfyA5R-PhYUmn5OOQtYMH4h6I0nSsKchNAySU/preview
*/

class _COM_AZURE_DEV__BASE__API Profiler {
public:

  /** Event information. Volatile class subject to change. */
  class _COM_AZURE_DEV__BASE__API Event {
  public:

    AnyReference data; // meta info
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

    /** Construct event. */
    inline Event() noexcept
    {
      ts.exponent = 0;
      ts.mantissa = 0;
      dur.exponent = 0;
      dur.mantissa = 0;
    }
  };

  /** Single stack frame. */
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

    static constexpr unsigned int MAXIMUM_STACK_TRACE = 64;

    class SymbolAndParent {
    public:

      const void* symbol = nullptr;
      unsigned int parent = 0;

      inline SymbolAndParent(const void* _symbol, unsigned int _parent) noexcept
        : symbol(_symbol), parent(_parent)
      {
      }

      inline bool operator<(const SymbolAndParent& value) const noexcept
      {
        return (symbol < value.symbol) ||
          ((symbol == value.symbol) && (parent < value.parent));
      }
      
      // C++: operators could be auto generated when undefined - operator!=() => !operation==(), ...
      inline bool operator==(const SymbolAndParent& value) const noexcept
      {
        return (symbol == value.symbol) && (parent == value.parent);
      }
    };

    /** Double linked list of all events. */
    Block* blocks = nullptr;
    const MemorySize pid = Process::getProcess().getId();
    Array<StackFrame> stackFramesHash; // cached frames (hash table)
    Array<StackFrame> stackFramesUnhash; // cached frames (remaining stack traces)
    String stackPattern; // stack frame pattern
    Array<Frame> stackFrames; // all frames - index is ID of frame
    Map<SymbolAndParent, unsigned int> stackFramesBySymbol;
    MemorySize rebalanceCount = 0;
    Map<uint32, unsigned int> stackFramesLookup; // lookup for sf to first frame

    PreferredAtomicCounter numberOfEvents;
    FileOutputStream fos;
    /** Include stack frames for events. */
    bool useStackFrames = false;
    /** The minimum time to wait to record event. */
    unsigned int minimumWaitTime = 1;
    /** The minimum heap size to record event. */
    unsigned int minimumHeapSize = 4096 * 2 / 2;
    /** The global IO capture limit. */
    unsigned int captureIOSize = 0;

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
  static void pushExceptionImpl(const Exception& exception, const Type* type);
  static void pushSignalImpl(const char* name);
  static void pushThreadStartImpl(const char* name, unsigned int parentId);
  static void pushProcessMetaImpl(ReferenceCountedObject* name);
  static void pushThreadMetaImpl(ReferenceCountedObject* name/*, unsigned int parentId*/);
  static void pushCountersImpl();
public:

  /** Returns true if profiler is enabled globally (use isEnabled() instead). */
  static bool isEnabledDirect() noexcept
  {
    return enabled;
  }

  /** Enables stack frames. */
  static void setUseStackFrames(bool useStackFrames) noexcept;
  /** Set the minimum time to wait to record event. */
  static void setMinimumWaitTime(unsigned int minimumWaitTime) noexcept;
  /** Sets the minimum heap size to record event. */
  static void setMinimumHeapSize(unsigned int minimumHeapSize) noexcept;
  /** Sets the whether to record IO bytes. */
  static void setCaptureIO(unsigned int maximumSize) noexcept;
  /** Sets the stack frame pattern. */
  static void setStackPattern(const String& stackPattern) noexcept;

#if 0
  enum {
    CAT_MEMORY = 1,
    CAT_OBJECT = 2,
    CAT_IO = 3,
    CAT_IO_FLUSH = 4,
    CAT_IO_READ = 5,
    CAT_IO_WRITE = 6,
    CAT_NETWORK = 7,
    CAT_RESOURCE = 8,
    CAT_WAIT = 9,
    CAT_EXCEPTION = 10,
    CAT_SIGNAL = 11,
    CAT_RENDERER = 12
  };
  // use macro to encode/decode in char*
#endif

  // ATTENTION: cannot be shared across shared library boundaries - use enum instead
  static constexpr const char* CAT_MEMORY = "MEMORY";
  static constexpr const char* CAT_OBJECT = "OBJECT";
  static constexpr const char* CAT_CREATE_RESOURCE = "RESOURCE";
  static constexpr const char* CAT_IO = "IO";
  static constexpr const char* CAT_IO_FLUSH = "IOFLUSH";
  static constexpr const char* CAT_IO_READ = "IOREAD";
  static constexpr const char* CAT_IO_WRITE = "IOWRITE";
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

  /** Enables IO Capture for the current scope. ATTENTION: Your profiling data can contain secrets!. */
  class _COM_AZURE_DEV__BASE__API CaptureIO {
  private:

    /** Updates the IO capture for the thread and returns the previous capture limit. */
    static unsigned int setCaptureIO(unsigned int size) noexcept;
    unsigned int size = 0;
  public:

    inline CaptureIO(unsigned int _size) noexcept
    {
      size = setCaptureIO(_size);
    }

    inline ~CaptureIO() noexcept
    {
      setCaptureIO(size);
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

  /** A reference counted value. */
  class _COM_AZURE_DEV__BASE__API ReferenceValue : public ReferenceCountedObject {
  public:

    unsigned int value = 0;

    inline ReferenceValue() noexcept
    {
    }

    inline ReferenceValue(unsigned int _value) noexcept : value(_value)
    {
    }
  };

  /** Reference counted IO context. */
  class _COM_AZURE_DEV__BASE__API ReferenceIO : public ReferenceCountedObject {
  public:

    unsigned int size = 0;
    String bytes;

    inline ReferenceIO() noexcept
    {
    }

    inline ReferenceIO(unsigned int _size) noexcept
      : size(_size)
    {
    }

    inline ReferenceIO(unsigned int _size, const String& _bytes) noexcept
      : size(_size), bytes(_bytes)
    {
    }
  };

  /** Reference counted resource context. */
  class _COM_AZURE_DEV__BASE__API ReferenceResource : public ReferenceCountedObject {
  public:

    unsigned int resourceId = 0;
    // unsigned int createdById = 0;

    inline ReferenceResource() noexcept
    {
    }

    inline ReferenceResource(unsigned int _resourceId) noexcept
      : resourceId(_resourceId)
    {
    }

    /*
    inline ReferenceResource(unsigned int _resourceId, unsigned int _createdById) noexcept
      : resourceId(_resourceId), createdById(_createdById)
    {
    }
    */
  };

  /** Reference counted performance counters. */
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

    /** The ID of the task. */
    unsigned int taskId = BAD;

    /** Allocates new task. */
    static unsigned int getTask(const char* name, const char* cat) noexcept;

    /** Pushes task. */
    static void pushTask(unsigned int taskId) noexcept;
  protected:

    /** Sets the resource handle for the object. */
    void setTaskResourceHandle(const ResourceHandle& handle) noexcept;

    /** Sets the name of the object. */
    void setTaskWaitId(const char* id) noexcept;
    
    /** Sets the resource handle for the object. */
    void setTaskWaitHandle(const ResourceHandle& handle) noexcept;    

    /** Sets bytes read for task. */
    void setTaskBytesRead(const uint8* buffer, unsigned int bytesRead) noexcept;

    /** Sets bytes written for task. */
    void setTaskBytesWritten(const uint8* buffer, unsigned int bytesWritten) noexcept;
  public:
    
    /** Task start. */
    inline Task(const char* _name, const char* _cat = nullptr) noexcept
    {
      if (enabled) { // doesnt matter if we do this for scope disabled case - we check this in destructor
        taskId = getTask(_name, _cat);
      }
    }

    /** Task complete. */
    inline ~Task() noexcept // we must not allow exception for profiling since it changes execution flow
    {
      if (taskId != BAD) {
        pushTask(taskId);
      }
    }
  };

  /** Wait task. */
  class _COM_AZURE_DEV__BASE__API WaitTask : public Task {
  public:

    /** Wait task start. */
    inline WaitTask(const char* name, const char* id = nullptr) noexcept
      : Task(name, CAT_WAIT)
    {
      if (id) {
        setTaskWaitId(id);
      }
    }

    /** Wait task start. */
    inline WaitTask(const char* name, const ResourceHandle* handle) noexcept
      : Task(name, CAT_WAIT)
    {
      if (handle) {
        setTaskWaitHandle(*handle);
      }
    }
  };

  /** Resource create/acquisition task. */
  class _COM_AZURE_DEV__BASE__API ResourceCreateTask : public Task {
  public:
    
    /** Resource create task start. */
    inline ResourceCreateTask(const char* name, const char* cat = CAT_CREATE_RESOURCE) noexcept
      : Task(name, cat)
    {
    }

    /** Sets the resource handle. */
    inline void setHandle(const ResourceHandle& handle)
    {
      setTaskResourceHandle(handle);
    }
  };

  /** IO task. */
  class _COM_AZURE_DEV__BASE__API IOTask : public Task {
  public:
    
    /** IO task start. */
    inline IOTask(const char* name, const char* cat = CAT_IO) noexcept 
      : Task(name, cat)
    {
    }
  };

  /** IO flush task. */
  class _COM_AZURE_DEV__BASE__API IOFlushTask : public IOTask {
  public:

    /** IO flush task start. */
    inline IOFlushTask(const char* name, const char* cat = CAT_IO_FLUSH) noexcept
      : IOTask(name, cat)
    {
    }
  };

  /** IO read task. */
  class _COM_AZURE_DEV__BASE__API IOReadTask : public IOTask {
  private:

    /** Read buffer. */
    const uint8* buffer = nullptr;
    /** Total bytes read for task. */
    unsigned int bytesRead = 0;
  public:

    /** IO read task start. */
    inline IOReadTask(const char* name, const uint8* _buffer = nullptr) noexcept
      : IOTask(name, CAT_IO_READ), buffer(_buffer)
    {
    }

    /** Sets the buffer.  */
    inline void setBuffer(const uint8* _buffer) noexcept
    {
      BASSERT(!buffer || (_buffer == buffer));
      buffer = _buffer;
    }

    /** Update bytes read. */
    inline void onBytesRead(unsigned int _bytesRead) noexcept
    {
      bytesRead += _bytesRead;
    }

    /** Returns the bytes read. */
    inline unsigned int getBytesRead() const noexcept
    {
      return bytesRead;
    }

    /** Task complete. */
    inline ~IOReadTask() noexcept
    {
      setTaskBytesRead(nullptr, bytesRead);
    }
  };

  /** IO write task. */
  class _COM_AZURE_DEV__BASE__API IOWriteTask : public IOTask {
  private:

    /** Write buffer. */
    const uint8* buffer = nullptr;
    /** Total bytes written for task. */
    unsigned int bytesWritten = 0;
  public:

    /** IO write task start. */
    inline IOWriteTask(const char* name, const uint8* _buffer = nullptr) noexcept
      : IOTask(name, CAT_IO_WRITE), buffer(_buffer)
    {
    }

    /** Sets the buffer.  */
    inline void setBuffer(const uint8* _buffer) noexcept
    {
      BASSERT(!buffer || (_buffer == buffer));
      buffer = _buffer;
    }

    /** Update bytes written. */
    inline void onBytesWritten(unsigned int _bytesWritten) noexcept
    {
      bytesWritten += _bytesWritten;
    }

    /** Returns the bytes written. */
    inline unsigned int getBytesWritten() const noexcept
    {
      return bytesWritten;
    }

    /** Task complete. */
    inline ~IOWriteTask() noexcept
    {
      setTaskBytesWritten(buffer, bytesWritten);
    }
  };

  /** HTTPS/Websocket task. */
  class _COM_AZURE_DEV__BASE__API HTTPSTask : public IOTask {
  public:
    
    /** HTTPS task start. */
    inline HTTPSTask(const char* name) noexcept
      : IOTask(name)
    {
    }
  };
  
  /** Push object creation. */
  static inline void pushObjectCreate(MemorySize id, MemorySize size)
  {
    if (!enabled) {
      return;
    }
    pushObjectCreateImpl(id, size);
  }

  /** Push object creation. */
  static inline void pushObjectCreate(MemorySize id)
  {
    if (!enabled) {
      return;
    }
    pushObjectCreateImpl(id);
  }

  /** Push object destruction. */
  static inline void pushObjectDestroy(MemorySize id, MemorySize size)
  {
    if (!enabled) {
      return;
    }
    pushObjectDestroyImpl(id, size);
  }

  /** Push object destruction. */
  static inline void pushObjectDestroy(MemorySize id)
  {
    if (!enabled) {
      return;
    }
    pushObjectDestroyImpl(id);
  }

  /** Push exception. */
  static inline void pushException(const char* type)
  {
    if (!enabled) {
      return;
    }
    pushExceptionImpl(type);
  }

  /** Push exception. */
  static inline void pushException(const Exception& exception, const Type* type = nullptr)
  {
    if (!enabled) {
      return;
    }
    pushExceptionImpl(exception, type);
  }

  /** Push signal. */
  static inline void pushSignal(const char* name)
  {
    if (!enabled) {
      return;
    }
    pushSignalImpl(name);
  }

  /** Push thread start. */
  static inline void pushThreadStart(const char* name, unsigned int parentId)
  {
    if (!enabled) {
      return;
    }
    pushThreadStartImpl(name, parentId);
  }

  /** Push process info. */
  static inline void pushProcessMeta(ReferenceCountedObject* name)
  {
    if (!enabled) {
      return;
    }
    pushProcessMetaImpl(name);
  }

  /** Push thread info. */
  static inline void pushThreadMeta(ReferenceCountedObject* name/*, unsigned int parentId*/)
  {
    if (!enabled) {
      return;
    }
    pushThreadMetaImpl(name);
  }

  /** Push counters. */
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

/** Write symbol and parent. */
inline FormatOutputStream& operator<<(FormatOutputStream& stream, const Profiler::ProfilerImpl::SymbolAndParent& sp)
{
  return stream << sp.symbol << "/" << sp.parent;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
