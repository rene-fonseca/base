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
#include <base/objectmodel/JSON.h>
#include <base/io/FileDescriptor.h>
#include <base/io/FileOutputStream.h>
#include <base/string/Format.h>
#include <base/net/HTTPSRequest.h>
#include <base/io/File.h>

using namespace com::azure::dev::base;

/*
inline String operator%(const String& text, const std::initializer_list<String>& args)
{
  return Format::subst(text, args);
}
*/

class CodeSearchApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  CodeSearchApplication()
    : Application("codesearch")
  {
  }

  /** Save SVG. */
  void saveSVG(const String& path, const String& svg)
  {
    FileOutputStream fos(path);
    fos.write(svg.getBytes(), svg.getLength());
  }
  
  /** Gets search result from Azure DevOps. */
  String connect(const String& org, const String& project, const String& expression)
  {
    // C++: Would like to do use initializer list on right hand size of %
    // String url = "https://almsearch.dev.azure.com/%1/%2/_apis/search/codeSearchResults?api-version=5.1-preview.1"_s % {org, project};
    String url = Format::subst("https://almsearch.dev.azure.com/%1/%2/_apis/search/codeSearchResults?api-version=5.1-preview.1",
      {org, project});
    
    ObjectModel o;
    auto root = o.createObject();
    root->setValue("searchText", expression);
    root->setValue(o.createString("$skip"), o.createInteger(0));
    root->setValue(o.createString("$top"), o.createInteger(2));
    auto orderBy = o.createObject();
    orderBy->setValue("field", "filename");
    orderBy->setValue("sortOrder", "ASC");
    root->setValue(o.createString("$orderBy"), orderBy);

    String body = JSON::getJSONNoFormatting(root);
    // fout << body << ENDL;

    try {
      HTTPSRequest request;
      if (!request.open(HTTPSRequest::METHOD_POST, url)) {
        error("Failed to connect to Azure DevOps.", EXIT_CODE_ERROR);
        return String();
      }

      request.setRequestHeader("Content-Type", "application/json");
      request.send(body);

      if (true /*request.getStatus() != 200*/) {
        /*
        fout << "Status: " << request.getStatus() << ENDL;
        fout << "Status text: " << request.getStatusText() << ENDL;
        fout << "Content length: " << request.getContentLength() << ENDL;
        fout << "Content length: " << request.getResponseHeader("Content-Length") << ENDL;
        fout << "Content type: " << request.getResponseHeader("Content-Type") << ENDL;
        
        const String header = request.getResponseHeader();
        fout << escape(header) << ENDL;
        */
      }
      
      return request.getResponse();
    } catch (...) {
      error("Failed to do code search on Azure DevOps.", EXIT_CODE_ERROR);
    }
    return String();
  }

#if 0
static inline FormatOutputStream& operator<<(FormatOutputStream& stream, const R<ObjectModel::Value>& value)
{
  return operator<<(stream, Reference<ObjectModel::Value>(const_cast<ObjectModel::Value*>(&value)));
}
#endif

  /** Process search results. */
  String search(Reference<ObjectModel::Value> root, const String& label = String())
  {
    if (!root) {
      fout << "No items." << ENDL;
      return String();
    }
    
    if (auto object = root.cast<ObjectModel::Object>()) {
      auto count = object->getPath("/count", true);
      // fout << "count=" << count << ENDL;
      // TAG: add support for SVG with count only
      // TAG: use label + count - green if 0 otherwise red
    }
    String xml = R"""(<svg width="100.0%" height="20.0" xmlns="http://www.w3.org/2000/svg">
  <style>
    .back { fill:black; fill-opacity="0.3" }
    .front { fill:white }
  </style>
  <linearGradient id="a" x2="100%" y2="100%">
    <stop offset="0.0" stop-opacity="0.50" stop-color="#000" />
    <stop offset="1.0" stop-opacity="0.25" stop-color="#000" />
  </linearGradient>
  <clipPath id="c">
    <rect width="100.0%" height="100.0%" rx="10.0" />
  </clipPath>
  <g clip-path="url(#c)">
    <rect width="100.0%" height="25" fill="#55d055" x="0" y="0"/>
    <rect width="100.0%" height="0.0%" fill="#555555" x="0" y="0"/>
    <rect width="100.0%" height="100.0%" fill="url(#a)"/>
  </g>
  <g id="label" text-anchor="middle" font-family="DejaVu Sans,Verdana,Geneva,sans-serif" font-size="14px">
  </g>
  <g id="result" text-anchor="left" font-family="DejaVu Sans,Verdana,Geneva,sans-serif" font-size="14px">
  </g>
</svg>)""";

    DOMImplementation dom;
    Document d = dom.createDocumentFromString(xml);
    // fout << dom.saveDocumentToMemory(d) << ENDL;
    
    Document::PrefixNamespace ns{Pair<String, String>("svg", "http://www.w3.org/2000/svg")};
    Node svg = d.selectSingleNode("//svg:svg", ns);
    if (!INLINE_ASSERT(svg)) {
      return String();
    }
    Node glabel = d.selectSingleNode("//svg:g[@id='label']", ns);
    Node gresult = d.selectSingleNode("//svg:g[@id='result']", ns);
    if (!INLINE_ASSERT(gresult)) {
      return String();
    }

    unsigned int y = 20;
    if (label && glabel) {
      Element t = d.createElement("text");
      t.setAttribute("class", "back");
      t.setAttribute("x", "50.0%");
      t.setAttribute("y", format() << (y + 1));
      t.appendChild(d.createText(label));
      glabel.appendChild(t);

      t = d.createElement("text");
      t.setAttribute("class", "front");
      t.setAttribute("x", "50.0%");
      t.setAttribute("y", format() << y);
      t.appendChild(d.createText(label));
      glabel.appendChild(t);
      y += 20;
    }
    
    unsigned int count = 0;
    if (auto object = root.cast<ObjectModel::Object>()) {
      if (auto results = object->getArray("/results")) {
        for (auto _result : results->values) {
          if (auto result = _result.cast<ObjectModel::Object>()) {
            String path = result->getString("/path", String());
            if (path.startsWith("/")) { // TAG: add startsWith(char ch)
              path = path.substring(1);
            }
            // fout << "path=" << path << ENDL;

            Element t = d.createElement("text");
            t.setAttribute("class", "back");
            t.setAttribute("x", format() << 11);
            t.setAttribute("y", format() << (y + 1));
            t.appendChild(d.createText(path));
            gresult.appendChild(t);

            t = d.createElement("text");
            t.setAttribute("class", "front");
            t.setAttribute("x", format() << 10);
            t.setAttribute("y", format() << y);
            t.appendChild(d.createText(path));
            gresult.appendChild(t);
            
            y += 20;
            ++count;
          }
        }
      }
    }

    if (!count) {
      String empty = "Empty";
      Element t = d.createElement("text");
      t.setAttribute("class", "back");
      t.setAttribute("x", format() << 11);
      t.setAttribute("y", format() << (y + 1));
      t.appendChild(d.createText(empty));
      gresult.appendChild(t);

      t = d.createElement("text");
      t.setAttribute("class", "front");
      t.setAttribute("x", format() << 10);
      t.setAttribute("y", format() << y);
      t.appendChild(d.createText(empty));
      gresult.appendChild(t);
      
      y += 20;
    }
    
    Element e(svg);
    e.setAttribute("height", format() << (y - 10));

    return dom.saveDocumentToMemory(d);
  }
  
  enum Command {
    COMMAND_SEARCH
  };

  Command command = COMMAND_SEARCH;
  String path;
  String expression;
  String label = "Search Result";
  // TAG: cache search result - remember expression with sha256
  String org;
  String project;
  String destPath;
  
  bool parseArguments()
  {
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      const String argument = enu.next();
      if (argument == "--search") {
        path = enu.next();
        command = COMMAND_SEARCH;
      } else if (argument == "--label") {
        if (!enu.hasNext()) {
          error("Expected label.");
          return false;
        }
        label = enu.next();
      } else if (argument == "--org") {
        if (!enu.hasNext()) {
          error("Expected organization.");
          return false;
        }
        org = enu.next();
      } else if (argument == "--project") {
        if (!enu.hasNext()) {
          error("Expected project.");
          return false;
        }
        project = enu.next();
      } else {
        if (!expression) {
          expression = argument;
        } else if (destPath) {
          destPath = argument;
        } else {
          error("Invalid argument.");
          return false;
        }
      }
      // TAG: need project path also
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
      setExitCode(1);
      return;
    }

    switch (command) {
    case COMMAND_SEARCH:
      {
        Reference<ObjectModel::Value> root;
        if (expression) {
          String result = connect(org, project, expression);
          if (!result) {
            error("Failed to get search result from Azure DevOps.", EXIT_CODE_ERROR);
            return;
          }
          root = JSON::parse(result);
        } else {
          root = JSON::parseFile(path);
        }
        String xml = search(root, label);
        
        if (destPath) {
          saveSVG(destPath, xml);
        } else {
          fout << xml << ENDL;
        }
      }
      return;
    default:
      ;
    }
  }
};

APPLICATION_STUB(CodeSearchApplication);
