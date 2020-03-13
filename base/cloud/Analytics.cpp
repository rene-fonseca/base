/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/cloud/Analytics.h>
#include <base/net/HTTPSRequest.h>
#include <base/string/Format.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Analytics::Event Analytics::makeResource(const String& resource, const String& title)
{
  static String type("resource");
  Event e;
  e.type = type;
  e.category = resource;
  e.label = title;
  return e;
}

Analytics::Event Analytics::makeEvent(const String& category, const String& action)
{
  static String type("event");
  Event e;
  e.type = type;
  e.category = category;
  e.action = action;
  return e;
}

Analytics::Analytics()
{
}

void Analytics::run()
{
  while (true) {
    bool flush = false;
    if (!event.wait(1000 * 1000)) {
      flush = true;
    }
    MutualExclusion::Sync _sync(mutex);
    if (!running) {
      break;
    }
    
    if (queue) {
      const auto e = queue.getFirst();
      queue.removeFirst();
      for (auto consumer : consumers) {
        if (consumer->isEnabled()) {
          consumer->send(e);
        }
      }
    }

    if (flush) {
      for (auto consumer : consumers) {
        consumer->flush();
      }
    }
  }
}

Analytics& Analytics::getAnalytics()
{
  static Analytics analytics;
  return analytics;
}

void Analytics::registerConsumer(Consumer* consumer)
{
  if (!consumer) {
    _throw NullPointer();
  }
  MutualExclusion::Sync _sync(mutex);
  consumers.append(consumer);
}

bool Analytics::isRunning() const noexcept
{
  return running;
}

void Analytics::start()
{
  if (!running) {
    running = true;
    Thread::start();
  }
}

void Analytics::stop()
{
  if (running) {
    running = false;
    event.signal();
    Thread::join();
    // thread = Worker();
  }
}

void Analytics::send(const Event& e)
{
  MutualExclusion::Sync _sync(mutex);
  queue.append(e);
  event.signal();
}

Analytics::~Analytics()
{
  for (auto consumer : consumers) {
    delete consumer;
  }
  consumers.removeAll();
}

Analytics::Task::Task(const Event& _e)
  : e(_e)
{
}

Analytics::Task::~Task()
{
  e.endTime = Date::getNow().getValue();
  getAnalytics().send(e);
}

class GoogleAnalytics : public Analytics::Consumer {
private:

  bool enabled = true;
  bool batch = true;
  String endpoint;
  String property;
  String clientId;
  String dh;
  
  String buffer;
  unsigned int count = 0;
  
  static constexpr unsigned int MAXIMUM_HITS = 20;
  static constexpr unsigned int MAXIMUM_PAYLOAD = 16 * 1024;
  static constexpr unsigned int MAXIMUM_HIT_PAYLOAD = 8 * 1024;

  void send(const String& payload)
  {
    try {
      HTTPSRequest request;
      if (!request.open(HTTPSRequest::METHOD_POST, endpoint)) {
        return;
      }
      request.send(payload);
    } catch (...) {
    }
  }
public:

  GoogleAnalytics(const String& _property, bool _batch = true)
    : batch(_batch), property(_property)
  {
    if (!property) {
      _throw OutOfDomain("Invalid property.");
    }
    endpoint = batch ? "https://www.google-analytics.com/batch" :
      "https://www.google-analytics.com/collect";
  }
  
  void open(const Array<StringPair>& params) override
  {
    for (const auto& param : params) {
      if (param.getFirst() == "clientId") {
        clientId = param.getSecond();
      } else if (param.getFirst() == "dh") {
        dh = param.getSecond();
      }
    }
  }

  bool isEnabled() const noexcept override
  {
    return enabled;
  }
  
  void setEnabled(bool enabled) override
  {
    this->enabled = enabled;
  }
  
  void send(const Analytics::Event& e) override
  {
    if (!enabled) {
      return;
    }
    
    String args;
    if (e.type == "resource") {
      args = "v=1&tid=%1&cid=%2&t=pageview&dh=%3&dp=%4&dt=%5\n" %
        Subst(property, clientId, dh, e.category, e.label);
    } else {
      args = "v=1&tid=%1&cid=%2&t=event&ec=%3&ea=%4&el=%5&ev=%6\n" %
        Subst(property, clientId, e.category, e.action, e.label, e.value);
    }

    if (args.getLength() > MAXIMUM_HIT_PAYLOAD) { // skip if too big
      return;
    }

    if (batch) {
      if ((buffer.getLength() + args.getLength()) > MAXIMUM_PAYLOAD) {
        flush();
      }
      buffer += args;
      ++count;
      if (count >= MAXIMUM_HITS) {
        flush();
      }
    } else {
      send(args);
    }
  }

  void flush() override
  {
    if (buffer) {
      send(buffer);
      buffer = String();
      count = 0;
    }
  }

  void close() override
  {
    flush();
  }
  
  ~GoogleAnalytics()
  {
    close();
  }
};

Analytics::Consumer* Analytics::createGoogleAnalytics(const String& property, bool batch)
{
  return new GoogleAnalytics(property, batch);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
