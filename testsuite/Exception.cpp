/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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

using namespace base;

class Hello {
public:

  int myFunction(long long) {return 0;}
};

class World {};

class foo {
public:

template<class A, class B>
class bar {
public:

void test() {}

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

template foo::bar<int, MyMemberType>;
template foo::bar<int, MyFunctionType>;

int test(MyFunctionType, MyFunctionType, MyFunctionType* const) {
  return 0;
}


class ExceptionApplication : public Application {
public:

  ExceptionApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw() :
    Application(MESSAGE("Exception"), numberOfArguments, arguments, environment) {
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
      } catch(const Exception& e) {
        fout << MESSAGE("Handled exception") << ENDL;
      }
    }
  }

  void testDemangling(const StringLiteral& mangled, const String& expected) throw() {
    String demangled = TypeInfo::demangleName(mangled);
    if (demangled == expected) {
      fout << MESSAGE("Demangling ") << mangled << MESSAGE(": ") << demangled << MESSAGE(" <OK>") << ENDL;
    } else {
      fout << MESSAGE("Demangling ") << mangled << MESSAGE(": ") << demangled << MESSAGE(" <ERROR>: ") << expected << ENDL;
    }
  }

  void testDemangling() throw() {
    foo::bar<int, unsigned int>::myFunction<long long>(foo(), foo::bar<int, unsigned int>(), Hello(), World(), World(), Hello(), 0);

    testDemangling(MESSAGE("N3gip7ConvertINS_10ArrayImageIfEENS1_INS_8RGBPixelIhEEEENS_10RGBToFloatEEE"),
                          MESSAGE("gip::Convert<gip::ArrayImage<float>, gip::ArrayImage<gip::RGBPixel<unsigned char> >, gip::RGBToFloat>"));

    // member type
    testDemangling(MESSAGE("10myFunctionM5HelloFixE"), MESSAGE("myFunction(int (Hello::*)(long long))"));
    testDemangling(MESSAGE("10myFunctionM5Hellol"), MESSAGE("myFunction(long Hello::*)"));
    testDemangling(MESSAGE("N7MyClassC1E"), MESSAGE("MyClass::MyClass"));
    testDemangling(MESSAGE("7MyClassPRrK7MyClass"), MESSAGE("MyClass(MyClass restrict const&*)"));

    testDemangling(MESSAGE("rm1XS_"), MESSAGE("operator%(X, X)"));
    testDemangling(MESSAGE("plR1XS0_"), MESSAGE("operator+(X&, X&)"));
    testDemangling(MESSAGE("lsRK1XS1_"), MESSAGE("operator<<(X const&, X const&)"));
    testDemangling(MESSAGE("N3FooIA4_iE3barE"), MESSAGE("Foo<int[4]>::bar"));
    testDemangling(MESSAGE("1fIiEvi"), MESSAGE("void f<int>(int)"));

    testDemangling(MESSAGE("5firstI3DuoEvS0_"), MESSAGE("void first<Duo>(Duo)"));
    testDemangling(MESSAGE("5firstI3DuoEvT_"), MESSAGE("void first<Duo>(Duo)"));
    testDemangling(MESSAGE("3fooIiFidEiEvv"), MESSAGE("void foo<int, int (*)(double), int>()")); // changed from 3fooIiPFidEiEvv
    testDemangling(MESSAGE("N1N1fE"), MESSAGE("N::f"));
    testDemangling(MESSAGE("N6System5Sound4beepEv"), MESSAGE("System::Sound::beep()"));
    testDemangling(MESSAGE("N5Arena5levelE"), MESSAGE("Arena::level"));
    testDemangling(MESSAGE("N5StackIiiE5levelE"), MESSAGE("Stack<int, int>::level"));
    testDemangling(MESSAGE("1fI1XEvPVN1AIT_E1TE"), MESSAGE("void f<X>(A<X>::T volatile*)"));
    //testDemangling(MESSAGE("ngILi42EEvN1AIXplT_Li2EEE1TE"), MESSAGE("void operator-<42>(A<42+2>::T)"));
    testDemangling(MESSAGE("4makeI7FactoryiET_IT0_Ev"), MESSAGE("Factory<int> make<Factory, int>()"));
    testDemangling(MESSAGE("3foo5Hello5WorldS0_S_"), MESSAGE("foo(Hello, World, World, Hello)"));
    testDemangling(MESSAGE("3fooPM2ABi"), MESSAGE("foo(int AB::**)"));
    testDemangling(MESSAGE("lsRSoRKSs"), MESSAGE("operator<<(std::ostream&, std::string const&)"));

    testDemangling(MESSAGE("TI7a_class"), MESSAGE("typeinfo for a_class"));
    testDemangling(MESSAGE("NSt6vectorIS_IiSaIiEESaIS1_EEC1ERKS2_"), MESSAGE("std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >::vector(std::allocator<std::vector<int, std::allocator<int> > > const&)"));
    testDemangling(MESSAGE("N1N1TIiiE2mfES0_IddE"), MESSAGE("N::T<int, int>::mf(N::T<double, double>)"));
    testDemangling(MESSAGE("3foo3bar"), MESSAGE("foo(bar)"));
    testDemangling(MESSAGE("3foov"), MESSAGE("foo()"));
    testDemangling(MESSAGE("NSt3_In4wardE"), MESSAGE("std::_In::ward"));
    testDemangling(MESSAGE("St5state"), MESSAGE("std::state"));

    fout << "Demangling of String: " << TypeInfo::getTypename<String>() << ENDL;
    fout << "Demangling of Semaphore::SemaphoreException: " << TypeInfo::getTypename<Semaphore::SemaphoreException>() << ENDL;
    fout << "Demangling of Array<int>: " << TypeInfo::getTypename<Array<int> >() << ENDL;
    fout << "Demangling of Map<String, long long>: " << TypeInfo::getTypename<Map<String, long long> >() << ENDL;
  }

  int main() throw() {
    fout << MESSAGE("Testing exception handling...") << EOL << ENDL;
    testDemangling();    
    myOuterFunction();
  }
};


int main(int argc, const char* argv[], const char* env[]) {
  ExceptionApplication application(argc, argv, env);
  
  try {
    application.main();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::getApplication()->getExitCode();
}
