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
#include <base/objectmodel/YAML.h>
#include <base/Timer.h>
#include <base/Random.h>
#include <base/Guid.h>
#include <base/filesystem/FolderInfo.h>
#include <base/io/File.h>

using namespace com::azure::dev::base;

// TAG: add support for ObjectModel::Comment // and /**/ style - see https://code.visualstudio.com/docs/languages/json

const char* JSON_EXAMPLE1 = "{"
"  \"hello\": \"This is a test.\","
"  \"count\": 123456,"
"  \"house\": {\"area\": 90, \"floors\": 2, \"description\": \"Great place.\"},"
"  \"list\": [false, 997, true, \"JSON is nice and simple\"]"
"}";

const char* JSON_EXAMPLE2 = "{\n"
"  \"hello\": \"This is a test.\",\n"
"  \"count\": 1234BAD56,\n"
"  \"house\": {\"area\": 90, \"floors\": 2, \"description\": \"Great place.\"},\n"
"  \"list\": [false, 997, true, \"JSON is nice and simple\"]\n"
"}\n";

const char* JSON_EXAMPLE3 = "{\n"
"  \"hello\": \"This is a test.\",\n"
"  \"count\": 123456,\n"
"  \"house\": {\"area\": 90, \"floors\": 2, \"description\": \"Great place.\"},\n"
"  \"list\": [false 997, true, \"JSON is nice and simple\"]\n"
"}\n";

const char* JSON_EXAMPLE_BAD_STRING = "{\n"
"  \"hello\": \"This is a test.\",\n"
"  \"count\": 123456,\n"
"  \"house\": {\"area\": 90, \"floors\": 2, \"description\": \"Great place.\"},\n"
"  \"list\": [false, 997, true, \"JSON is \rnice and simple\"]\n"
"}\n";

const char* JSON_EXAMPLE_BAD_STRING2 = reinterpret_cast<const char*>(u8"{\n"
"  \"hello\": \"This is a test\\u0021\",\n"
"  \"count\": 123456,\n"
"  \"house\": {\"area\": 90, \"floors\": 2, \"description\": \"Great place.\"},\n"
"  \"list\": [false, 997, true, \"\xc6#Æ#Ø#Å#\", \"JSON is nice and simple\"]\n"
"}\n");

// TAG: can we change a setting and auto pop upon return for FormatOutputStream - e.g. change a flag and similar

class JSONApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  void testPerformance()
  {
    // const Literal text = MESSAGE("12345.98765e12");
    const Literal text = MESSAGE("12345.98765e3");
    Posix posix;
    unsigned int count = 1000000;
    Timer timer;
    timer.start();
    while (count--) {
      double d = -1; // PrimitiveTraits<double>::MAXIMUM; // TAG: need infinity and NaN
      bool result = posix.getSeries(text.getValue(), text.getValue() + text.getLength(), d);
      if (!result) {
        _throw Exception();
      }
    }
    timer.stop();
    fout << "LOOP 1: " << timer << ENDL;

    count = 1000000;
    timer.start();
    while (count--) {
      double d = -1; // PrimitiveTraits<double>::MAXIMUM; // TAG: need infinity and NaN
      bool result = Posix::toDouble(text.getValue(), text.getValue() + text.getLength(), d);
      if (!result) {
        _throw Exception();
      }
    }
    timer.stop();
    fout << "LOOP 2: " << timer << ENDL;
  }

  JSONApplication()
    : Application("JSON")
  {
  }

  void performance(const String& path)
  {
    Timer timer;
    Reference<ObjectModel::Value> object = JSON::parseFile(path);
    fout << "Reading JSON to DOM: " << timer << ENDL;
    String compact = JSON::getJSONNoFormatting(object);
    fout << "Convert DOM to compact JSON: " << timer << ENDL;
    String nice = JSON::getJSON(object);
    fout << "Convert DOM to nice JSON: " << timer << ENDL;
  }

  void test(const String& path)
  {
    FolderInfo folder(path);
    Array<String> entries = folder.getEntries();
    // fout << entries << ENDL;

    for (const auto filename : entries) {
      const String p = path + "/" + filename;
      bool valid = false;
      if (filename.indexOf("_EXCLUDE") >= 0) {
        continue;
      }
      bool pass = filename.startsWith("pass");
      bool fail = filename.startsWith("fail");
      if (!pass && !fail) {
        continue;
      }
      try {
        Reference<ObjectModel::Value> object = JSON::parseFile(p);
        valid = true;
      } catch (Exception&) {
        // fout << e << ENDL;
      } catch (...) {
      }
      fout << "Reading: " << filename << " "
           << ((valid == pass) ? "PASSED" : "FAILED") << ENDL;
    }
  }

  void roundtrip(const String& path)
  {
    FolderInfo folder(path);
    Array<String> entries = folder.getEntries();
    // fout << entries << ENDL;

    for (const auto filename : entries) {
      const String p = path + "/" + filename;
      bool match = false;
      if (!filename.startsWith("roundtrip")) {
        continue;
      }
      if (filename.indexOf("_EXCLUDE") >= 0) {
        continue;
      }
      try {
        PrimitiveStackArray<uint8> buffer(0);
        {
          File file(p, File::READ, 0);
          buffer.resize(file.getSize());
          file.read(buffer, buffer.size(), false);
        }
        String input(
          reinterpret_cast<const char*>(static_cast<const uint8*>(buffer)),
          buffer.size()
        );
        Reference<ObjectModel::Value> object = JSON::parse(input);
        String compact = JSON::getJSONNoFormatting(object);
        // object = JSON::parse(compact);
        // String compact2 = JSON::getJSONNoFormatting(object);
        match = compact == input;
        if (!match) {
          fout << ">>> " << input << ENDL;
          fout << "<<< " << compact << ENDL;
        }
      } catch (Exception&) {
        // fout << e << ENDL;
      } catch (...) {
      }
      fout << "Reading: " << filename << " "
           << (match ? "PASSED" : "FAILED") << ENDL;
    }
  }

  void filter(const String& path, const String& xpath)
  {
    Reference<ObjectModel::Value> root = JSON::parseFile(path);
    if (xpath) {
      if (auto object = root.cast<ObjectModel::Object>()) {
        root = object->getPath(xpath, true);
      }
    }
    if (!root) {
      fout << "No items." << ENDL;
      return;
    }
    // TAG: add support for finding item/search by value
    // TAG: add support for * patterns / regex
    
    String nice = JSON::getJSON(root);
    fout << JSON::getJSON(
      root,
      ObjectModel::DEFAULT_FORMATTING | (fout.isANSITerminal() ? ObjectModel::FLAG_COLOR : 0)
    ) << ENDL;
  }
  
  void stats(const String& path, const String& xpath)
  {
    // TAG: dump stat support - count items and types
    Reference<ObjectModel::Value> root = JSON::parseFile(path);
    if (xpath) {
      if (auto object = root.cast<ObjectModel::Object>()) {
        root = object->getPath(xpath, true);
      }
    }
    if (!root) {
      fout << "No items." << ENDL;
      return;
    }
    auto stats = ObjectModel::getStats(root);
    fout << "objects: " << stats.numberOfObjects << EOL
         << "arrays: " << stats.numberOfArrays << EOL
         << "booleans: " << stats.numberOfBools << EOL
         << "integers: " << stats.numberOfInts << EOL
         << "floats: " << stats.numberOfFloats << EOL
         << "strings: " << stats.numberOfStrings << EOL
         << "nulls: " << stats.numberOfNulls << ENDL;
  }

  enum Command {
    COMMAND_PERFORMANCE,
    COMMAND_TEST,
    COMMAND_ROUNDTRIP,
    COMMAND_FILTER,
    COMMAND_STATS,
    COMMAND_OTHER
  };

  Command command = COMMAND_OTHER;
  String path;
  String xpath;

  bool parseArguments()
  {
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      const String argument = enu.next();
      if (argument == "--performance") {
        command = COMMAND_PERFORMANCE;
      } else if (argument == "--test") {
        command = COMMAND_TEST;
      } else if (argument == "--roundtrip") {
        command = COMMAND_ROUNDTRIP;
      } else if (argument == "--filter") {
        command = COMMAND_FILTER;
      } else if (argument == "--stats") {
        command = COMMAND_STATS;
      } else {
        if (!path) {
          path = argument;
        } else if (!xpath) {
          xpath = argument;
        }
      }
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
    case COMMAND_PERFORMANCE:
      performance(path);
      return;
    case COMMAND_TEST:
      test(path);
      return;
    case COMMAND_ROUNDTRIP:
      roundtrip(path);
      return;
    case COMMAND_FILTER:
      filter(path, xpath);
      return;
    case COMMAND_STATS:
      stats(path, xpath);
      return;
    default:
      ;
    }

    // testPerformance();

#if 0
    for (unsigned int i = 0; i < 10; ++i) {
      fout << ZEROPAD << HEX << Random::random<uint32>() << " " << EOL;
    }
    for (unsigned int i = 0; i < 10; ++i) {
      fout << ZEROPAD << HEX << Guid::createGuid() << " " << EOL;
    }
    fout << DEC << ENDL;
#endif

    Reference<ObjectModel::Value> example1 = JSON().parse(JSON_EXAMPLE1);
    // fout << "Example1:" << EOL << example1 << EOL << ENDL;
    fout << "Example1:" << EOL << JSON::getJSONNoFormatting(example1) << ENDL;

#if (!defined(_COM_AZURE_DEV__BASE__NO_EXCEPTIONS)) && \
    (_COM_AZURE_DEV__BASE__ARCH != _COM_AZURE_DEV__BASE__SPARC64) && \
    (_COM_AZURE_DEV__BASE__ARCH != _COM_AZURE_DEV__BASE__POWERPC64) && \
    (_COM_AZURE_DEV__BASE__ARCH != _COM_AZURE_DEV__BASE__S390X)
    try {
      Reference<ObjectModel::Value> example2 = JSON().parse(JSON_EXAMPLE2);
    } catch (JSONException& e) {
      fout << "Example2: '" << e.getMessage() << "' at line " << e.getPosition() << ENDL;
    }

    try {
      Reference<ObjectModel::Value> example3 = JSON().parse(JSON_EXAMPLE3);
    } catch (JSONException& e) {
      fout << "Example3: '" << e.getMessage() << "' at line " << e.getPosition() << ENDL;
    } catch (ParseException& e) {
      fout << "Example bad string: " << e.getMessage() << ENDL;
    }

    try {
      Reference<ObjectModel::Value> example4 = JSON().parse(JSON_EXAMPLE_BAD_STRING);
    } catch (JSONException& e) {
      fout << "Example bad string: '" << e.getMessage() << "' at line " << e.getPosition() << ENDL;
    } catch (ParseException& e) {
      fout << "Example bad string: " << e.getMessage() << ENDL;
    }

    try {
      // TAG: FIXME exception for GCC
      // Reference<ObjectModel::Value> example5 = JSON().parse(JSON_EXAMPLE_BAD_STRING2);
    } catch (JSONException& e) {
      fout << "Example bad string2: '" << e.getMessage() << "' at line " << e.getPosition() << ENDL;
    } catch (ParseException& e) {
      fout << "Example bad string: '" << e.getMessage() << "'" << ENDL;
    }
#endif

    ObjectModel objectModel;
    auto o = objectModel.createObject();
    // TAG add support for *o[key] = value;
    o->setValue(objectModel.createString("firstName"), objectModel.createString("My first name"));
    o->setValue(objectModel.createString("lastName"), objectModel.createString("My last name"));
    o->setValue(objectModel.createString("age"), objectModel.createInteger(25));
    o->setValue(objectModel.createString("hasDog"), objectModel.createBoolean(false));
    o->setValue(objectModel.createString("hasCat"), objectModel.createBoolean(false));
    (*o)[objectModel.createString("hasCat")] = objectModel.createBoolean(true);
    (*o)["hasCat"] = true;

    Reference<ObjectModel::Value> age = (*o)[objectModel.createString("age")];

    Reference<ObjectModel::Value> hasCat = (*o)[objectModel.createString("hasCat")];
    
    auto a1 = objectModel.createArray(); // TAG: initializer list
    for (unsigned int i = 0; i < 10; ++i) {
      a1->append(objectModel.createInteger(i * 3));
    }
    o->setValue(objectModel.createString("ints"), a1);

    auto a2 = objectModel.createArray();
    for (unsigned int i = 0; i < 5; ++i) {
      a2->append(objectModel.createFloat(i * 1.23));
    }
    o->setValue(objectModel.createString("floats"), a2);

    auto a3 = objectModel.createArray();
    Array<String> animals = {"penguin", "giraf", "horse", "lion", "eagle"};
    Array<bool> bs = { false, true, true, false };
    *a3 = bs;
    Array<int> is = { 7, -1, 25, 900 };
    *a3 = is;
    Array<double> ds = { 0.1, -10, 99.9 };
    *a3 = ds;
    *a3 = animals;
    o->setValue(objectModel.createString("animals"), a3);

    auto o2 = objectModel.createObject();
    (*o2)["description"] = "This is a long description.";
    (*o2)["color"] = 0xfff0f0;
    (*o2)["emptyArray"] = objectModel.createArray();
    (*o2)["emptyObject"] = objectModel.createObject();
    (*o2)["dummy"] = objectModel.createVoid();
    (*o2)["shortArray"] = objectModel.createArray({1,2,3});
    auto a4 = objectModel.createArray({"Hello", "World", "from", "Base", "Framework"});
    (*o2)["words"] = a4;
    a4->setAt(2, nullptr);
    (*o)["meta"] = o2;

    const String text = JSON::getJSON(o);
    fout << text << ENDL;

    fout << ">>>" << ENDL;
    fout << JSON::getJSON(o, ObjectModel::DEFAULT_FORMATTING | (fout.isANSITerminal() ? ObjectModel::FLAG_COLOR : 0)) << ENDL;

    fout << ENDL;
   
    Reference<ObjectModel::Value> o3 = JSON().parse(text);
    fout << o3 << ENDL;

#if 0
    const String yaml = YAML::getYAML(o);
    fout << EOL
         << yaml << ENDL;
#endif
  }
};

APPLICATION_STUB(JSONApplication);
