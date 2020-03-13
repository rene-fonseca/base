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

#include <base/string/String.h>
#include <base/collection/Array.h>
#include <base/collection/List.h>
#include <base/collection/Pair.h>
#include <base/Date.h>
#include <base/concurrency/Event.h>
#include <base/concurrency/MutualExclusion.h>
#include <base/concurrency/Thread.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Analytics.
*/

class _COM_AZURE_DEV__BASE__API Analytics : public Thread  {
public:
  
  /** Event. */
  class Event {
  public:
    
    uint64 startTime = Date::getNow().getValue();
    uint64 endTime = 0;
    
    String type; // 'event' or 'resource'

    // event
    String category; // resource id
    String action;
    String label; // resource title
    String value;

    /** Sets the category. */
    inline Event& setCategory(const String& category) noexcept
    {
      this->category = category;
      return *this;
    }

    /** Sets the action. */
    inline Event& setAction(const String& action) noexcept
    {
      this->action = action;
      return *this;
    }

    /** Sets the label. */
    inline Event& setLabel(const String& label) noexcept
    {
      this->label = label;
      return *this;
    }

    /** Sets the value. */
    inline Event& setValue(const String& value) noexcept
    {
      this->value = value;
      return *this;
    }
  };

  /** Analytics consumer. */
  class Consumer : public DynamicObject { // TAG: maybe Provider is a better name
  public:

    /** Opens consumer. */
    virtual void open(const Array<StringPair>& params = {}) = 0;

    /** Returns true if enabled. */
    virtual bool isEnabled() const = 0;

    /** Sets enabled. */
    virtual void setEnabled(bool enabled) = 0;

    /** Sends event to consumer. */
    virtual void send(const Event& e) = 0;

    /** Flushes consumer. */
    virtual void flush() = 0;

    /** Closes consumer. */
    virtual void close() = 0;
  };
private:
  
  Array<Consumer*> consumers;
  List<Event> queue;
  MutualExclusion mutex;
  base::Event event;
  bool running = false;
  String userId;
  String clientId;

  /** Initializes analytics instance. */
  Analytics();
  
  void run() override;
public:

  /** Returns Google Analytics consumer. */
  static Consumer* createGoogleAnalytics(const String& property, bool batch = true);

  /** Returns resource event. */
  static Event makeResource(const String& resource, const String& title);
  
  /** Returns normal event. */
  static Event makeEvent(const String& category, const String& action);
  
  /** Returns the analytics instance. */
  static Analytics& getAnalytics();

  /** Returns anonymous user id. */
  inline const String& getUserId() const noexcept
  {
    return userId;
  }
  
  /** Sets anonymous user id. */
  inline void setUserId(const String& userId) noexcept
  {
    this->userId = userId;
  }

  /** Returns anonymous user id. */
  inline const String& getClientId() const noexcept
  {
    return clientId;
  }

  /** Sets anonymous client id. */
  inline void setClientId(const String& clientId) noexcept
  {
    this->clientId = clientId;
  }

  /** Registers analytics consumer. */
  void registerConsumer(Consumer* consumer);
  
  /** Returns true if running. */
  bool isRunning() const noexcept;
  
  /** Startss the analytics. */
  void start();

  /** Stops the analytics. */
  void stop();

  /** Send the given event. */
  void send(const Event& e);

  /** Destroys analytics. */
  ~Analytics();

  /** Task. */
  class Task {
  private:
    
    Event e;
  public:
    
    /** Start task. */
    Task(const Event& e);

    /** End of task. */
    ~Task();
  };
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
