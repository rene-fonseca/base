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

// TAG: add push object to JSON writer

class _COM_AZURE_DEV__BASE__API Profiler {
public:
  
  // bool open(const String& path);
  // TAG: store data in temporary file - for MappedFile
  
  static constexpr const char* CAT_MEMORY = "MEMORY";
  static constexpr const char* CAT_OBJECT = "OBJECT";
  static constexpr const char* CAT_IO = "IO";
  static constexpr const char* CAT_NETWORK = "NET";

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
  
  static bool enabled;
  static MemorySize numberOfEvents;
  static unsigned int flags;
  static FileOutputStream fos;

  static uint64 getTimestamp();
  
  static void pushEvent(const Event& e);

  static bool open(const String& path);
  
  /** Enables profiler. */
  static void start();

  /** Disables profiler. */
  static void stop();
  
  static uint64 getNumberOfEvents()
  {
    return numberOfEvents;
  }
  
  class Task {
  private:
    
    Event e; // store in TLC
  public:
    
    inline Task(const char* name) noexcept
    {
      if (enabled) {
        // beginTask();
        clear(e);
        e.pid = Process::getProcess().getId();
        e.tid = reinterpret_cast<MemorySize>(Thread::getIdentifier());
        e.ts = getTimestamp();
        e.ph = EVENT_BEGIN;
        e.name = name;
        e.dur = 0;
        pushEvent(e);
      }
    }

    inline ~Task() noexcept
    {
      if (enabled) {
        e.ts = getTimestamp();
        e.ph = EVENT_END;
        pushEvent(e);
      }
    }
    
    // TAG: connect on demand - from command line - add show events streaming
  };
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
