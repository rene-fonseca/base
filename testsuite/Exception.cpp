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
#include <base/OutOfRange.h>
#include <base/TypeInfo.h>
#include <base/concurrency/Semaphore.h>
#include <base/collection/Array.h>

using namespace com::azure::dev::base;

class Hello {
public:

  int myFunction(long long) {
    return 0;
  }
};

class World {
};

class foo {
public:

template<class A, class B>
class bar {
public:

void test() {
}

template<class TYPE>
static void myFunction(foo, bar, Hello, World, World, Hello, void*) {
}

};

};

typedef int (Hello::*MyMemberType)(long long);
typedef long Hello::*MyDataMemberType;

MyMemberType myFunction(MyMemberType) {
  return &Hello::myFunction;
}

void myFunction2(MyDataMemberType) {
}

typedef int (MyFunctionType)(int);

template class foo::bar<int, MyMemberType>;
template class foo::bar<int, MyFunctionType>;

int test(MyFunctionType, MyFunctionType, MyFunctionType* const) {
  return 0;
}


class ExceptionApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  ExceptionApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("Exception", numberOfArguments, arguments, environment) {
  }

  void myInnerFunction() throw(OutOfRange) {
    throw OutOfRange("This is a message", this);
  }

  void myOuterFunction() throw() {
//    throw 4;
    if (true) {
      myInnerFunction();
      // "unexpected" exception expected
    } else {
      try {
        myInnerFunction();
      } catch (const Exception&) {
        fout << "Handled exception" << ENDL;
      }
    }
  }

  void testDemangling(
    const Literal& mangled, const String& expected) throw() {
    String demangled = TypeInfo::demangleName(mangled.getValue());
    if (demangled == expected) {
      fout << "Demangling " << mangled << ": " << demangled << " <OK>" << ENDL;
    } else {
      fout << "Demangling " << mangled << ": " << demangled << " <ERROR>: " << expected << ENDL;
    }
  }

  void testDemangling() throw() {
    foo::bar<int, unsigned int>::myFunction<long long>(foo(), foo::bar<int, unsigned int>(), Hello(), World(), World(), Hello(), 0);

    testDemangling("N3gip7ConvertINS_10ArrayImageIfEENS1_INS_8RGBPixelIhEEEENS_10RGBToFloatEEE", "gip::Convert<gip::ArrayImage<float>, gip::ArrayImage<gip::RGBPixel<unsigned char> >, gip::RGBToFloat>");

    // member type
    testDemangling("10myFunctionM5HelloFixE", "myFunction(int (Hello::*)(long long))");
    testDemangling("10myFunctionM5Hellol", "myFunction(long Hello::*)");
    testDemangling("N7MyClassC1E", "MyClass::MyClass");
    testDemangling("7MyClassPRrK7MyClass", "MyClass(MyClass restrict const&*)");

    testDemangling("rm1XS_", "operator%(X, X)");
    testDemangling("plR1XS0_", "operator+(X&, X&)");
    testDemangling("lsRK1XS1_", "operator<<(X const&, X const&)");
    testDemangling("N3FooIA4_iE3barE", "Foo<int[4]>::bar");
    testDemangling("1fIiEvi", "void f<int>(int)");

    testDemangling("5firstI3DuoEvS0_", "void first<Duo>(Duo)");
    testDemangling("5firstI3DuoEvT_", "void first<Duo>(Duo)");
    testDemangling("3fooIiFidEiEvv", "void foo<int, int (*)(double), int>()"); // changed from 3fooIiPFidEiEvv
    testDemangling("N1N1fE", "N::f");
    testDemangling("N6System5Sound4beepEv", "System::Sound::beep()");
    testDemangling("N5Arena5levelE", "Arena::level");
    testDemangling("N5StackIiiE5levelE", "Stack<int, int>::level");
    testDemangling("1fI1XEvPVN1AIT_E1TE", "void f<X>(A<X>::T volatile*)");
    //testDemangling("ngILi42EEvN1AIXplT_Li2EEE1TE", "void operator-<42>(A<42+2>::T)");
    testDemangling("4makeI7FactoryiET_IT0_Ev", "Factory<int> make<Factory, int>()");
    testDemangling("3foo5Hello5WorldS0_S_", "foo(Hello, World, World, Hello)");
    testDemangling("3fooPM2ABi", "foo(int AB::**)");
    testDemangling("lsRSoRKSs", "operator<<(std::ostream&, std::string const&)");

    testDemangling("TI7a_class", "typeinfo for a_class");
    testDemangling("NSt6vectorIS_IiSaIiEESaIS1_EEC1ERKS2_", "std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >::vector(std::allocator<std::vector<int, std::allocator<int> > > const&)");
    testDemangling("N1N1TIiiE2mfES0_IddE", "N::T<int, int>::mf(N::T<double, double>)");
    testDemangling("3foo3bar", "foo(bar)");
    testDemangling("3foov", "foo()");
    testDemangling("NSt3_In4wardE", "std::_In::ward");
    testDemangling("St5state", "std::state");

    fout << "Demangling of String: "
         << TypeInfo::getTypename<String>() << ENDL;
    fout << "Demangling of Semaphore: "
         << TypeInfo::getTypename<Semaphore>() << ENDL;
    fout << "Demangling of Array<int>: "
         << TypeInfo::getTypename<Array<int> >() << ENDL;
    fout << "Demangling of Map<String, long long>: "
         << TypeInfo::getTypename<Map<String, long long> >() << ENDL;
  }

  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2001-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;

    testDemangling();
    myOuterFunction();
  }
};

STUB(ExceptionApplication);
