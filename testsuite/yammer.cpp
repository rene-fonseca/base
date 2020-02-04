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
#include <base/LongInteger.h>
#include <base/io/FileOutputStream.h>
#include <base/concurrency/Thread.h>
#include <base/net/HTTPSRequest.h>
#include <base/net/Url.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>
#include <base/string/Format.h>
#include <base/objectmodel/JSON.h>

// Get token: https://developer.yammer.com/docs/test-token

// https://developer.yammer.com/docs/api-requests

using namespace com::azure::dev::base;

class PullInterface {
public:

  virtual uint64 pullBegin() const = 0;
  virtual MemorySize pull(uint8* buffer, MemorySize size) = 0;
};

class PushToNothing {
public:

  bool pushBegin(uint64 totalSize)
  {
    return true;
  }

  MemorySize push(const uint8* buffer, MemorySize size)
  {
    return size;
  }

  void pushEnd()
  {
  }
};

class PushToStandardOutput : public virtual Object, public PushInterface {
public:

  PushToStandardOutput() {
  }

  bool pushBegin(uint64 totalSize) {
    return true;
  }

  MemorySize push(const uint8* buffer, MemorySize size)
  {
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
  
  MemorySize push(const uint8* buffer, MemorySize size)
  {
    unsigned int result = file.write(buffer, size);
    BASSERT(result == size);
    bytesWritten += size;
    if (totalSize > 0) {
      fout << "Read=" << bytesWritten
           << "  Progress=" << base::FIXED << (bytesWritten*1000/totalSize)/10.0 << '%'
           << "  Time=" << base::FIXED << timer.getLiveMicroseconds()/100000/10.0 << " s"
           << "  Rate=" << base::FIXED << setPrecision(1)
           << (1000000./1024 * static_cast<double>(bytesWritten)/timer.getLiveMicroseconds())
           << " kb/s          \r" << FLUSH;
    } else {
      fout << "Read=" << bytesWritten // TAG: show in kb/Mb/Gb/...
           << "  Time=" << base::FIXED << timer.getLiveMicroseconds()/100000/10.0 << " s" // use HH:MM:SS format
           << "  Rate=" << base::FIXED << setPrecision(1)
           << (1000000./1024 * static_cast<double>(bytesWritten)/timer.getLiveMicroseconds())
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
  inline ServiceException() noexcept
  {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline ServiceException(const char* message) noexcept
    : Exception(message)
  {
  }
  
  /**
    Initializes the exception with a type.
    
    @param type The type.
  */
  inline ServiceException(const Type& type) noexcept
    : Exception(type)
  {
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

class YammerException : public ServiceException {
public:
  
  /**
    Initializes the exception object with no message.
  */
  inline YammerException() noexcept
  {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline YammerException(const char* message) noexcept
    : ServiceException(message)
  {
  }
  
  /**
    Initializes the exception with a type.
    
    @param type The type.
  */
  inline YammerException(const Type& type) noexcept
    : ServiceException(type)
  {
  }
  
  /**
    Initializes the exception with a message and type.

    @param message The message.
    @param type The type.
  */
  inline YammerException(const char* message, const Type& type) noexcept
    : ServiceException(message, type)
  {
  }
};

/** Yammer integration. */
class Yammer {
public:

  String token;
  bool verbose = true;

  Yammer(const String& _token)
    : token(_token)
  {
  }

  Reference<ObjectModel::Object> deleteMethod(const String& url)
  {
    String response;
    try {
      HTTPSRequest request;
      if (!request.open(HTTPSRequest::METHOD_DELETE, url)) {
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
      fout << r << ENDL;
    }
    
    auto ok = r->getBoolean("ok", false);
    if (!ok) {
      return nullptr;
    }
    
    return r;
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
      ferr << "Error: " << "Request failed." << ENDL;
      return nullptr;
    }
    
    auto stat = r->getString("response/stat", "");
    if (stat) {
      ferr << "Error: " << r->getString("response/message", "Request failed.") << ENDL;
      return nullptr;
    }
    
    return r;
  }
  
  Reference<ObjectModel::Object> post(const String& url, Reference<ObjectModel::Object> o)
  {
    const String body = JSON::getJSON(o, false);
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
      ferr << "Error: " << "Request failed." << ENDL;
      return nullptr;
    }

    auto stat = r->getString("response/stat", "");
    if (stat) {
      ferr << "Error: " << r->getString("response/message", "Request failed.") << ENDL;
      return nullptr;
    }
    
    return r;
  }
  
  Reference<ObjectModel::Object> getMessages()
  {
    if (!token) {
      _throw YammerException("No token.");
    }
    
    const String url = "https://www.yammer.com/api/v1/messages.json";

    Reference<ObjectModel::Object> r = get(url);
    if (r) {
      fout << r->toString(true) << ENDL;
    }
    return r;
  }

  String postMessage(const String& _group, const String& text)
  {
    if (!token) {
      _throw YammerException("No token.");
    }
    
    const String url = "https://www.yammer.com/api/v1/messages.json";

    // TAG: need to get group by name
    const int64 group = LongInteger::parse(_group); // TAG: doesnt fail LongInteger - also need to check end reached
    
    ObjectModel o;
    auto root = o.createObject();
    root->setValue(o.createString("body"), o.createString(text));
    // root->setValue(o.createString("network_id"), o.createInteger(network_id));
    root->setValue(o.createString("group_id"), o.createInteger(group));
    // root->setValue(o.createString("direct_to_user_ids"), o.createArray());
    int64 repliedToId = 0;
    if (repliedToId) {
      root->setValue(o.createString("replied_to_id"), o.createInteger(repliedToId));
    }

    // TAG: use bot identity?
    
    Reference<ObjectModel::Object> r = post(url, root);
    if (r) {
      fout << "REPLY: " << r << ENDL;
    }
    
    return String();
  }


  bool getThreadInfo(const String& threadId)
  {
    if (!token) {
      _throw YammerException("No token.");
    }

    const String url = format() << "get/threads/" << threadId << ".json";
    
    Reference<ObjectModel::Object> r = get(url);
    if (r) {
      fout << *r << ENDL;
      return true;
    }
    
    return false;
  }
  
  bool removeMessage(const unsigned int messageId)
  {
    if (!token) {
      _throw YammerException("No token.");
    }

    const String url = format() << "https://www.yammer.com/api/v1/messages/" << messageId;
    
    Reference<ObjectModel::Object> r = deleteMethod(url);
    if (r) {
      fout << "REPLY: " << *r << ENDL;
    }
    
    return false;
  }
};

class YammerApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;

  String token;
  String group;
  String threadId;
  String text;
  bool post = false;
public:
  
  YammerApplication()
    : Application("yammer")
  {
  }
  
  bool parseArguments()
  {
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      const String argument = enu.next();
      if (argument == "--token") {
        if (!enu.hasNext()) {
          ferr << "Expected token." << ENDL;
          return false;
        }
        token = enu.next();
      } else if (argument == "--group") {
        if (!enu.hasNext()) {
          ferr << "Expected group." << ENDL;
          return false;
        }
        group = enu.next();
      } else if (argument == "--threadId") {
        if (!enu.hasNext()) {
          ferr << "Expected channel." << ENDL;
          return false;
        }
        threadId = enu.next();
      } else if (argument == "--post") {
        if (!enu.hasNext()) {
          ferr << "Expected text." << ENDL;
          return false;
        }
        text = enu.next();
        post = true;
      } else {
        if (!token) {
          token = argument;
        } else {
          ferr << Format::subst("Invalid argument '%1'.", argument) << ENDL;
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
         << ENDL;

    if (!parseArguments()) {
      setExitCode(Application::EXIT_CODE_ERROR);
      return;
    }
    
    if (!token) {
      ferr << "Error: Need token." << ENDL;
      setExitCode(1);
      return;
    }
    
    Yammer yammer(token);
    if (post) {
      yammer.postMessage(group, text);
    } else if (threadId) {
      yammer.getThreadInfo(threadId);
    } else {
      yammer.getMessages();
      // ferr << "Error: Need action." << ENDL;
      // setExitCode(1);
    }
  }
};

APPLICATION_STUB(YammerApplication);
