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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Analytics::Event Analytics::makeResource(const String& resource, const String& title)
{
  Event e;
  e.type = "resource";
  e.category = resource;
  e.label = title;
  return e;
}

Analytics::Event Analytics::makeEvent(const String& category, const String& action)
{
  Event e;
  e.type = "event";
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
    event.wait();
    MutualExclusion::Sync _sync(mutex);
    if (!running) {
      break;
    }
    const auto e = queue.getFirst();
    queue.removeFirst();
    for (auto consumer : consumers) {
      if (consumer->isEnabled()) {
        consumer->send(e);
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
