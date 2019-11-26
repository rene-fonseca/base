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
#include <base/io/FileDescriptor.h>

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

const char* JSON_EXAMPLE_BAD_STRING2 = u8"{\n"
"  \"hello\": \"This is a test\\u0021\",\n"
"  \"count\": 123456,\n"
"  \"house\": {\"area\": 90, \"floors\": 2, \"description\": \"Great place.\"},\n"
"  \"list\": [false, 997, true, \"\xc6#Æ#Ø#Å#\", \"JSON is nice and simple\"]\n"
"}\n";

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
      if (d == 0) {
        throw Exception();
      }
    }
    timer.stop();
    fout << "LOOP 1: " << timer << ENDL;

    count = 1000000;
    timer.start();
    while (count--) {
      double d = -1; // PrimitiveTraits<double>::MAXIMUM; // TAG: need infinity and NaN
      bool result = Posix::toDouble(text.getValue(), text.getValue() + text.getLength(), d);
      if (d == 0) {
        throw Exception();
      }
    }
    timer.stop();
    fout << "LOOP 2: " << timer << ENDL;
  }

  JSONApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[])
    : Application("JSON", numberOfArguments, arguments, environment)
  {
  }
  
  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2019 by Rene Moeller Fonseca" << EOL
         << ENDL;

    testPerformance();

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
    std::vector<String> animals = {"penguin", "giraf", "horse", "lion", "eagle"};
    std::vector<bool> bs = { false, true, true, false };
    *a3 = bs;
    std::vector<int> is = { 7, -1, 25, 900 };
    *a3 = is;
    std::vector<double> ds = { 0.1, -10, 99.9 };
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
    fout << JSON::getJSON(o, ObjectModel::DEFAULT_FORMATTING | (FileDescriptor::getStandardOutput().isANSITerminal() ? ObjectModel::FLAG_COLOR : 0)) << ENDL;

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
