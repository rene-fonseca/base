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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/*
 Profiler::Task task("HTTPRequest");
*/

/**
  Profiler.
 
  https://docs.google.com/document/d/1CvAClvFfyA5R-PhYUmn5OOQtYMH4h6I0nSsKchNAySU/preview
*/

class _COM_AZURE_DEV__BASE__API Profiler {
private:

  static bool enabled;
  static PreferredAtomicCounter numberOfEvents;
  static bool useJSON;
  static FileOutputStream fos;
public:
  
  static constexpr const char* CAT_MEMORY = "MEMORY";
  static constexpr const char* CAT_OBJECT = "OBJECT";
  static constexpr const char* CAT_IO = "IO";
  static constexpr const char* CAT_NETWORK = "NET";
  static constexpr const char* CAT_WAIT = "WAIT";
  static constexpr const char* CAT_EXCEPTION = "EXCEPTION"; // TAG: use as error - need to filter ok exceptions
  static constexpr const char* CAT_RENDERER = "RENDERER";
  
  enum {
    EVENT_BEGIN = 'B',
    EVENT_END = 'E',
    EVENT_COMPLETE = 'X',
    EVENT_COUNTER = 'C',
    EVENT_OBJECT_CREATE = 'N',
    EVENT_OBJECT_DESTROY = 'D',
    EVENT_SAMPLE = 'P'
  };
  
  struct Event {
    uint32 pid; // process id
    uint32 tid; // thread id
    uint64 ts; // microsecond timestamp
    char ph; // event type
    const char* cat;
    const char* name;
    const char* args; // src_file, src_func
    uint64 dur; // duration in microseconds
    uint64 tdur; // optional - duration in microseconds
    uint64 tts; // optional
  };
  
  static uint64 getTimestamp();
  
  /** Submit event. */
  static void pushEvent(const Event& e);

  /** Opens profiler. */
  static bool open(const String& path, bool useJSON);
  
  /** Closes profiler. */
  static void close();
  
  /** Enables profiler. */
  static void start();

  /** Disables profiler. */
  static void stop();
  
  /** Returns true if profiler is enabled. */
  static inline bool isEnabled()
  {
    return enabled;
  }
  
  /** Returns the number of events. */
  static MemorySize getNumberOfEvents()
  {
    return numberOfEvents;
  }
  
  /** Initializes event. */
  static void initEvent(Event& e);

  /** Task. */
  class Task {
  private:
    
    Event e;
  public:
    
    inline Task(const char* name, const char* cat = nullptr) noexcept
    {
      initEvent(e);
      if (enabled) {
        e.ph = EVENT_BEGIN;
        e.name = name;
        e.cat = cat;
        e.dur = 0;
        pushEvent(e);
      }
    }

    inline ~Task() noexcept
    {
      if (enabled) {
        auto ts = getTimestamp();
        e.dur = ts - e.ts;
        e.ts = ts;
        e.ph = EVENT_END;
        pushEvent(e);
      }
    }
  };

  class IOTask : public Task {
  public:
    
    inline IOTask(const char* name) : Task(name, CAT_IO)
    {
    }
  };

  class WaitTask : public Task {
  public:
    
    inline WaitTask(const char* name) : Task(name, CAT_WAIT)
    {
    }
  };
  
  // TAG: network task?
  
  class HTTPSTask : public Task {
  public:
    
    inline HTTPSTask(const char* name) : Task(name, CAT_IO)
    {
    }
  };

  static inline void pushObjectCreate(MemorySize size)
  {
    if (!enabled) {
      return;
    }
    Event e;
    initEvent(e);
    e.ph = EVENT_OBJECT_CREATE;
    e.cat = CAT_MEMORY;
    pushEvent(e);
  }
  
  static inline void pushObjectDestroy(MemorySize size)
  {
    if (!enabled) {
      return;
    }
    Event e;
    initEvent(e);
    e.ph = EVENT_OBJECT_DESTROY;
    e.cat = CAT_MEMORY;
    pushEvent(e);
  }

  static inline void pushException(const char* type)
  {
    if (!enabled) {
      return;
    }
    Event e;
    initEvent(e);
    e.ph = EVENT_OBJECT_DESTROY; // TAG: fixme
    e.name = type;
    e.cat = CAT_EXCEPTION;
    pushEvent(e);
  }

  static inline void pushSignal(const char* name)
  {
    if (!enabled) {
      return;
    }
    Event e;
    initEvent(e);
    e.ph = EVENT_OBJECT_DESTROY; // TAG: fixme
    e.name = name;
    e.cat = CAT_WAIT;
    pushEvent(e);
  }

  static inline void pushFrame(const char* name)
  {
    if (!enabled) {
      return;
    }
    Event e;
    initEvent(e);
    e.ph = EVENT_OBJECT_DESTROY; // TAG: fixme
    e.name = name;
    e.cat = CAT_RENDERER;
    pushEvent(e);
  }
  
  /** Release profiler resources. */
  static void release();
  
  // TAG: process and thread stats - no sync for process
  class Stats {
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
