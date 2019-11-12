/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/Timer.h>
#include <base/Primitives.h>
#include <base/UnsignedInteger.h>
#include <base/io/FileOutputStream.h>
#include <base/concurrency/Thread.h>
#include <base/net/HTTPSRequest.h>
#include <base/net/Url.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>
#include <base/objectmodel/JSON.h>
#include <base/io/FileDescriptor.h>

using namespace com::azure::dev::base;

class PullInterface {
public:

  virtual uint64 pullBegin() const = 0;
  virtual unsigned int pull(uint8* buffer, unsigned int size) = 0;
};

class PushToNothing {
public:

  bool pushBegin(uint64 totalSize) {
    return true;
  }

  unsigned int push(const uint8* buffer, unsigned int size) {
    return size;
  }

  void pushEnd() {
  }
};

class PushToStandardOutput : public virtual Object, public PushInterface {
public:

  PushToStandardOutput() {
  }

  bool pushBegin(uint64 totalSize) {
    return true;
  }

  unsigned int push(const uint8* buffer, unsigned int size) {
    for (unsigned int i = 0; i < size;) {
      char ch = *buffer++;
      ++i;
      if (ch == '\n') {
        fout << EOL;
        if ((i < size) && (*buffer == '\r')) {
          ++buffer; // skip
          ++i;
        }
      } else if (ch == '\r') {
        fout << EOL;
        if ((i < size) && (*buffer == '\n')) {
          ++buffer; // skip
          ++i;
        }
      } else if (ASCIITraits::isGraph(ch)) {
        fout << ch;
      } else if (ch != ' ') {
        fout << '.';
      } else {
        fout << ' ';
      }
    }
    return size;
  }

  void pushEnd() {
    fout << ENDL;
  }

  virtual ~PushToStandardOutput() {
  }
};

class PushToFile : public virtual Object, public PushInterface {
private:

  File file;
  Timer timer;
  uint64 bytesWritten = 0;
  uint64 totalSize = 0;
public:

  PushToFile(File _file)
    : file(_file)
  {
  }

  bool pushBegin(uint64 totalSize)
  {
    this->totalSize = totalSize;
    timer.start();
    return true;
  }
  
  unsigned int push(const uint8* buffer, unsigned int size)
  {
    unsigned int result = file.write(buffer, size);
    BASSERT(result == size);
    bytesWritten += size;
    if (totalSize > 0) {
      fout << "Read=" << bytesWritten
           << "  Progress=" << base::FIXED << (bytesWritten*1000/totalSize)/10.0 << '%'
           << "  Time=" << base::FIXED << timer.getLiveMicroseconds()/100000/10.0 << " s"
           << "  Rate=" << base::FIXED << setPrecision(1) << (1000000./1024 * static_cast<double>(bytesWritten)/timer.getLiveMicroseconds())
           << " kb/s          \r" << FLUSH;
    } else {
      fout << "Read=" << bytesWritten // TAG: show in kb/Mb/Gb/...
           << "  Time=" << base::FIXED << timer.getLiveMicroseconds()/100000/10.0 << " s" // use HH:MM:SS format
           << "  Rate=" << base::FIXED << setPrecision(1) << (1000000./1024 * static_cast<double>(bytesWritten)/timer.getLiveMicroseconds())
           << " kb/s          \r" << FLUSH;
    }
    // Thread::microsleep(125*1000); // TAG: temp test
    return size;
  }
  
  void pushEnd()
  {
    fout << ENDL;
    file.close();
  }
  
  virtual ~PushToFile()
  {
  }
};

class ServiceException : public Exception {
public:
  
  /**
    Initializes the exception object with no message.
  */
  inline ServiceException() noexcept {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline ServiceException(const char* message) noexcept : Exception(message) {
  }
  
  /**
    Initializes the exception with a type.
    
    @param type The type.
  */
  inline ServiceException(const Type& type) noexcept : Exception(type) {
  }
  
  /**
    Initializes the exception with a message and type.

    @param message The message.
    @param type The type.
  */
  inline ServiceException(const char* message, const Type& type) noexcept
    : Exception(message, type) {
  }
};

class SlackException : public ServiceException {
public:
  
  /**
    Initializes the exception object with no message.
  */
  inline SlackException() noexcept {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline SlackException(const char* message) noexcept : ServiceException(message) {
  }
  
  /**
    Initializes the exception with a type.
    
    @param type The type.
  */
  inline SlackException(const Type& type) noexcept : ServiceException(type) {
  }
  
  /**
    Initializes the exception with a message and type.

    @param message The message.
    @param type The type.
  */
  inline SlackException(const char* message, const Type& type) noexcept
    : ServiceException(message, type) {
  }
};

/** Slack integration. */
class Slack {
public:

  String token;
  bool verbose = true;

  Slack(const String& _token)
    : token(_token)
  {
  }

  Reference<ObjectModel::Object> get(const String& url)
  {
    String response;
    try {
      HTTPSRequest request;
      if (!request.open(HTTPSRequest::METHOD_GET, url)) {
        return nullptr;
      }

      request.setRequestHeader("Authorization", "Bearer " + token);
      // request.setRequestHeader("Content-Type", "application/json; charset=utf-8");

      request.send();
      response = request.getResponse();
      request.close();
    } catch (...) {
      return nullptr;
    }
    
    auto r = JSON().parse(response).cast<ObjectModel::Object>(); // TAG: do this via Value class
    if (!r) {
      return nullptr;
    }

    if (verbose) {
      auto data = JSON::getJSON(r, ObjectModel::DEFAULT_FORMATTING | (FileDescriptor::getStandardOutput().isANSITerminal() ? ObjectModel::FLAG_COLOR : 0));
      fout << data << ENDL;
    }
    
    auto ok = r->getBoolean("ok", false);
    if (!ok) {
      return nullptr;
    }
    
    return r;
  }
  
  Reference<ObjectModel::Object> post(const String& url, Reference<ObjectModel::Object> o)
  {
    const String body = JSON::getJSONNoFormatting(o);
    // fout << body << ENDL;
    
    String response;
    try {
      HTTPSRequest request;
      if (!request.open(HTTPSRequest::METHOD_POST, url)) {
        return nullptr;
      }

      request.setRequestHeader("Authorization", "Bearer " + token);
      request.setRequestHeader("Content-Type", "application/json; charset=utf-8");

      request.send(body);
      response = request.getResponse();
      request.close();
    } catch (...) {
      return nullptr;
    }
    
    auto r = JSON().parse(response).cast<ObjectModel::Object>(); // TAG: do this via Value class
    if (!r) {
      return nullptr;
    }

    if (verbose) {
      fout << r << ENDL;
    }
    
    auto ok = r->getBoolean("ok", false);
    if (!ok) {
      return nullptr;
    }
    
    return r;
  }
  
  String rtm_connect()
  {
    if (!token) {
      throw SlackException("No token.");
    }

    // TAG: JSON color should highlight url strings
    
    // use a bot token
    // https://api.slack.com/docs/token-types#bot
    
    const String url = "https://slack.com/api/rtm.connect";

    auto r = get(url);
    String wss;
    if (r) {
      wss = r->getString("url", "");
      fout << "WSS: " << wss << ENDL;
    }
    return wss;
  }
  
  // TAG: test link, images, ...
  String chat_postMessage(const String& channel, const String& text, const String& thread = String())
  {
    if (!token) {
      throw SlackException("No token.");
    }
    
    // TAG: check webhook API
    const String url = "https://slack.com/api/chat.postMessage";

    ObjectModel o;
    auto root = o.createObject();
    root->setValue(o.createString("channel"), o.createString(channel));
    root->setValue(o.createString("text"), o.createString(text));
    root->setValue(o.createString("as_user"), o.createBoolean(true));
    root->setValue(o.createString("username"), o.createString("From C++"));
    if (thread) {
      root->setValue(o.createString("thread_ts"), o.createString(thread));
    }

    Reference<ObjectModel::Object> r = post(url, root);
    if (r) {
      auto ts = r->getString("ts", String());
      fout << "TS: " << ts << ENDL;
      return ts;
    }
    
    return String();
  }

  String chat_update(const String& channel, const String& text, const String& thread)
  {
    if (!token) {
      throw SlackException("No token.");
    }

    const String url = "https://slack.com/api/chat.update";

    ObjectModel o;
    auto root = o.createObject();
    root->setValue(o.createString("ts"), o.createString(thread));
    root->setValue(o.createString("channel"), o.createString(channel));
    root->setValue(o.createString("text"), o.createString(text));
    root->setValue(o.createString("as_user"), o.createBoolean(true));
    root->setValue(o.createString("username"), o.createString("From C++"));
    // TAG: add support for set with String, bool, int, double, ...
    
    Reference<ObjectModel::Object> r = post(url, root);
    if (r) {
      auto ts = r->getString("ts", String());
      // fout << "TS: " << ts << ENDL;
      return ts;
    }
    
    return String();
  }

  bool channels_info(const String& channel)
  {
    if (!token) {
      throw SlackException("No token.");
    }

    const String url = "https://slack.com/api/channels.info?channel=" + channel;

#if 0
    ObjectModel o;
    auto root = o.createObject();
    root->setValue(o.createString("channel"), o.createString(channel));
#endif
    
    Reference<ObjectModel::Object> r = get(url);
    if (r) {
      auto c = r->getObject("channel");
      fout << c << ENDL;
      return true;
    }
    
    return false;
  }
  
  // TAG: https://slack.com/api/api.test
  // https://slack.com/api/channels.list
  // https://slack.com/api/chat.delete
  // https://slack.com/api/users.info
  // https://slack.com/api/users.list
  // https://api.slack.com/rtm

};

class SlackApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;

  String token;
  String channel;
  String text;
  String thread;
  bool post = false;
  bool update = false;
  bool rtm = false;
public:
  
  SlackApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[])
    : Application("slack", numberOfArguments, arguments, environment)
  {
  }
  
  bool parseArguments()
  {
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      const String argument = *enu.next();
      if (argument == "--token") {
        if (!enu.hasNext()) {
          ferr << "Expected token." << ENDL;
          return false;
        }
        token = *enu.next();
      } else if (argument == "--channel") {
        if (!enu.hasNext()) {
          ferr << "Expected channel." << ENDL;
          return false;
        }
        channel = *enu.next();
      } else if (argument == "--rtm") {
        post = false;
        update = false;
        rtm = true;
      } else if (argument == "--post") {
        if (!enu.hasNext()) {
          ferr << "Expected text." << ENDL;
          return false;
        }
        text = *enu.next();
        post = true;
        update = false;
        rtm = false;
      } else if (argument == "--update") {
        if (!enu.hasNext()) {
          ferr << "Expected text." << ENDL;
          return false;
        }
        text = *enu.next();
        post = false;
        update = true;
        rtm = false;
      } else if (argument == "--thread") {
        if (!enu.hasNext()) {
          ferr << "Expected thread ts." << ENDL;
          return false;
        }
        thread = *enu.next();
      } else {
        if (!token) {
          token = argument;
        } else {
          ferr << "Invalid argument '" << argument << "'." << ENDL;
          return false;
        }
      }
    }
    if (!token) {
      ferr << "Token not specified." << ENDL;
      return false;
    }
    return true;
  }

  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2001-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;

    if (!parseArguments()) {
      setExitCode(Application::EXIT_CODE_ERROR);
      return;
    }
    
    // TAG: get from env TOKEN, CHANNEL, ... SLACK_TOKEN, SLACK_CHANNEL, ...

    if (!token) {
      ferr << "Error: Need token." << ENDL;
      setExitCode(1);
      return;
    }
    
    Slack slack(token);
    if (post) {
      slack.chat_postMessage(channel, text, thread);
    } else if (update) {
      slack.chat_update(channel, text, thread);
    } else if (channel) {
      slack.channels_info(channel);
    } else if (rtm) {
      slack.rtm_connect();
    } else {
      ferr << "Error: Need action." << ENDL;
      setExitCode(1);
    }
  }
};

APPLICATION_STUB(SlackApplication);
