/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/Format.h>
#include <base/net/HTTPSRequest.h>
#include <base/objectmodel/JSON.h>
#include <base/Module.h>

using namespace com::azure::dev::base;

class ModulesApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;

  bool colorize = false;
  String format = "text";
  String url;
  String token;
public:

  ModulesApplication()
    : Application("modules")
  {
    colorize = fout.isANSITerminal();
  }

  /** Returns name and value for string with = separator. */
  static StringPair getNameValue(const String& text, char separator = '=')
  {
    auto index = text.indexOf(separator);
    if (index >= 0) {
      return StringPair(text.substring(0, index), text.substring(index + 1));
    } else {
      return StringPair(text);
    }
  }

  /** Returns name and value for string with = separator. */
  static StringPair getNameValue(const String& text, const String& _default, char separator = '=')
  {
    auto index = text.indexOf(separator);
    if (index >= 0) {
      return StringPair(text.substring(0, index), text.substring(index + 1));
    } else {
      return StringPair(text, _default);
    }
  }

  bool validateValues(const String& value, const std::initializer_list<String>& accepted)
  {
    for (auto a : accepted) {
      if (value == a) {
        return true;
      }
    }
    return false;
  }

  bool parseArguments()
  {
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      String argument = enu.next();
      String value;
      if (argument.startsWith("--")) {
        StringPair option = getNameValue(argument);
        argument = option.getFirst();
        value = option.getSecond();
      }
      
      if (argument == "--color") {
        // validateValues(value, {"always", "auto", "never"}, "auto");
        colorize = true;
      } else if (argument == "--nocolor") {
        colorize = false;
      } else if (argument == "--format") {
#if 0
        if (!value) {
          Application::error("Expected value for --format.", EXIT_CODE_ERROR);
          return false;
        }
        // validateValues(value, {"text", "json", "xml","csv"}, "text"); // text is default
#endif

        if (!enu.hasNext()) {
          Application::error("Expected format [text,json,xml,csv].", EXIT_CODE_ERROR);
          return false;
        }
        format = enu.next();
      } else if (argument == "--token") {
        if (!enu.hasNext()) {
          Application::error("Expected token.", EXIT_CODE_ERROR);
          return false;
        }
        token = enu.next();
      } else if (argument == "--submit") {
        if (!enu.hasNext()) {
          Application::error("Expected URL.", EXIT_CODE_ERROR);
          return false;
        }
        url = enu.next();
      } else {
        Application::error("Invalid argument.", EXIT_CODE_ERROR);
        return false;
      }
    }
    return true;
  }

  /** Submits JSON to given url. */
  bool submit(const String& url, const Reference<ObjectModel::Value>& o, const String& token = String())
  {
    if (!HTTPSRequest::isSupported()) {
      Application::error("Runtime does not supported HTTPS.", EXIT_CODE_ERROR);
      return false;
    }

    const String body = JSON::getJSONNoFormatting(o);

    String response;
    HTTPSRequest request;
    try {
      if (request.open(HTTPSRequest::METHOD_GET, url)) {
        if (token) {
          request.setRequestHeader("Authorization", "Bearer " + token);
        }
        request.setRequestHeader("Content-Type", "application/json; charset=utf-8");

        request.send(body);
        response = request.getResponse();
        request.close();
      }
      // TAG: detect approved/rejected/other
      if (request.getStatus() != 400) {
      }
      fout << "DEBUG: status=" << request.getStatus() << ENDL;
      return true;
    } catch (...) {
      Application::error(Format::subst("Failed to submit request to '%1'.", url));
    }
    return false;
  }

  void main()
  {
    if (!parseArguments()) {
      return;
    }

    auto& manager = ModuleManager::getManager();

    Reference<ObjectModel::Value> om;

    if (format == "text") {
      if (!manager.traverseModules(fout, "*", true)) {
        setExitCode(1);
        return;
      }
    } else if ((format == "json") || (format == "xml")) {
      auto report = manager.getModules();
      if (!report) {
        setExitCode(1);
        return;
      }

      if (format == "json") {
        ObjectModel::NiceFormat niceFormat;
        niceFormat.setNiceFlags(ObjectModel::DEFAULT_FORMATTING | (colorize ? ObjectModel::FLAG_COLOR : 0));
        niceFormat << report << FLUSH;
        fout << niceFormat.getString() << ENDL;
      } else {
        DOMImplementation impl;
        String xml = impl.saveDocumentToMemory(ObjectModel::getXML(report, "modules"), true);
        fout << xml << ENDL;
      }
      om = report;
    } else if (format == "csv") {
      auto report = manager.getModulesCSV();
      if (!report) {
        setExitCode(1);
        return;
      }
      fout << report;
    } else {
      // ("%1", "value") // TAG: can we support substitution easily
      Application::error(Format::subst("Unsupported format '%1'.", format));
      return;
    }

    if (url) {
      if (!om) {
        om = manager.getModules();
      }
      if (!om) {
        setExitCode(1);
        return;
      }
      if (!submit(url, om, token)) {
        setExitCode(1);
      }
    }
  }
};

APPLICATION_STUB(ModulesApplication);
