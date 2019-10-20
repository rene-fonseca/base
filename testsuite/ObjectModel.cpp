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
#include <base/objectmodel/ObjectModel.h>
#include <base/Functor.h>
#include <base/TypeInfo.h>

using namespace com::azure::dev::base;

class ObjectModelApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  ObjectModelApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("ObjectModel", numberOfArguments, arguments, environment) {
  }
  
  void main() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2019 by Rene Moeller Fonseca" << EOL
         << ENDL;
    
    fout << "Initializing ObjectModel" << ENDL;

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
    fout << ((age == 25) ? MESSAGE("age has expected value.") : MESSAGE("age doesn't have expected value.")) << ENDL;

    Reference<ObjectModel::Value> hasCat = (*o)[objectModel.createString("hasCat")];
    fout << ((hasCat == true) ? MESSAGE("hasCat has expected value.") : MESSAGE("hasCat doesn't have expected value.")) << ENDL;
    
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
    auto a4 = objectModel.createArray({ "Hello","World","from","Base","Framework" });
    (*o2)["words"] = a4;
    a4->setAt(2, nullptr);
    (*o)["meta"] = o2;

    auto a5 = a3->getAs<String>();
    fout << ((a5 == animals) ? MESSAGE("animals has expected value.") : MESSAGE("animals doesn't have expected value.")) << ENDL;

    fout << "Get firstName by path: " << o->getPath("firstName") << ENDL;
    fout << "Get lastName by path: " << o->getPath("lastName") << ENDL;
    fout << "Get meta by path: " << o->getPath("meta") << ENDL;
    fout << "Get description by path: " << o->getString("meta/description", "") << ENDL;
    fout << "Get age by path: " << o->getInteger("age", 0) << ENDL;
    fout << "Get non-existing height by path: " << o->getInteger("height", -1) << ENDL;
    fout << "Get array item by path: " << o->getString("animals[1]", "UNKNOWN") << ENDL;

    fout << ENDL;

    fout << o << ENDL;

    fout << ENDL;
    ObjectModel::NiceFormat niceFormat;
    niceFormat << o << FLUSH;
    fout << niceFormat.getString() << ENDL;
  }
};

APPLICATION_STUB(ObjectModelApplication);
