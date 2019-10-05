/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>
#include <base/string/String.h>
#include <base/Functor.h>

using namespace dk::sdu::mip::base;

class MyFunction {
public:

  int operator()(int a, long b) const throw() {
    return a + b;
  }
};

template<unsigned int>
class FunctionN {
public:
};

template<>
class FunctionN<0> {
public:
  
  template<class R>
  class Function {
  public:
    
    typedef R (F)();
  };
};

template<>
class FunctionN<1> {
public:
  
  template<class R, class A1>
  class Function {
  public:
    
    typedef R (F)(A1);
  };
};

template<>
class FunctionN<2> {
public:
  
  template<class R, class A1, class A2>
  class Function {
  public:
    
    typedef R (F)(A1, A2);
  };
};

template<>
class FunctionN<3> {
public:
  
  template<class R, class A1, class A2, class A3>
  class Function {
  public:
    
    typedef R (F)(A1, A2, A3);
  };
};

template<unsigned int>
class InvokerN {
public:
};

#if 0
template<unsigned int>
class InvokerN {
public:
};
template<>
class InvokerN<0> {
public:

  template<class R>
  class 
  static Result invoke() /*throw(...)*/ {
    return function();
  }
};

template<>
class InvokerN<1> {
public:
  
  static Result invoke(A1 a1) /*throw(...)*/ {
    return function(a1);
  }
};
  
template<>
class InvokerN<2> {
public:
  
  static Result invoke(A1 a1, A2 a2) /*throw(...)*/ {
    return function(a1, a2);
  }
};

template<>
class InvokerN<3> {
public:
    
  static Result invoke(A1 a1, A2 a2, A3 a3) /*throw(...)*/ {
    return function(a1, a2, a3);
  }
};

template<>
class InvokerN<4> {
public:
  
  static Result invoke(A1 a1, A2 a2, A3 a3, A4 a4) /*throw(...)*/ {
    return function(a1, a2, a3, a4);
  }
};

template<>
class InvokerN<5> {
public:
  
  static Result invoke(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) /*throw(...)*/ {
    return function(a1, a2, a3, a4, a5);
  }
};

template<>
class InvokerN<6> {
public:
  
  static Result invoke(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) /*throw(...)*/ {
    return function(a1, a2, a3, a4, a5, a6);
  }
};
#endif

template<class FUNCTION, class PROTOTYPE>
class Invoker {
private:
  
  FUNCTION function;
public:
  
  typedef typename PROTOTYPE::R Result;
  typedef typename PROTOTYPE::A1 A1;
  typedef typename PROTOTYPE::A2 A2;
  typedef typename PROTOTYPE::A3 A3;
  typedef typename PROTOTYPE::A4 A4;
  typedef typename PROTOTYPE::A5 A5;
  typedef typename PROTOTYPE::A6 A6;
  
  enum {
    NUMBER_OF_ARGUMENTS = PROTOTYPE::NUMBER_OF_ARGUMENTS
  };
};



typedef int (FX)(int);
typedef void (FY)(void);
typedef void (FZ)(void);

int FFX(int) {
  return 0;
}

void FFY(void) {
}

void FFZ(void) {
}

class MyClass {
public:

  ~MyClass() throw() {
  }
};

class MyVirtualClass {
public:
  
  virtual ~MyVirtualClass() throw() {
  }
};

class MyOtherClass {
private:
  
  int value;
};



template<class R, class A, class B, class C, class D, class E, class F>
inline FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Prototype<R, A, B, C, D, E, F>& prototype) throw(IOException) {
  typedef Prototype<R, A, B, C, D, E, F> P;
  switch (P::NUMBER_OF_ARGUMENTS) {
  case 0:
    stream << Type::getType<typename P::Result>() << SP << "(*)" << '('
           << ')';
    break;
  case 1:
    stream << Type::getType<typename P::Result>() << SP << "(*)" << '('
           << Type::getType<typename P::A1>()
           << ')';
    break;
  case 2:
    typedef typename P::Result (QWERTY)(typename P::A1, typename P::A2);
    stream << Type::getType<QWERTY>();
//     stream << Type::getType<typename P::Result>() << FLUSH << SP << "(*)" << '('
//            << Type::getType<typename P::A1>() << ',' << SP<< FLUSH 
//            << Type::getType<typename P::A2>()<< FLUSH 
//            << ')';
    break;
  case 3:
    stream << Type::getType<typename P::Result>() << SP << "(*)" << '('
           << Type::getType<typename P::A1>() << ',' << SP
           << Type::getType<typename P::A2>() << ',' << SP
           << Type::getType<typename P::A3>()
           << ')';
    break;
  case 4:
    stream << Type::getType<typename P::Result>() << SP << "(*)" << '('
           << Type::getType<typename P::A1>() << ',' << SP
           << Type::getType<typename P::A2>() << ',' << SP
           << Type::getType<typename P::A3>() << ',' << SP
           << Type::getType<typename P::A4>()
           << ')';
    break;
  case 5:
    stream << Type::getType<typename P::Result>() << SP << "(*)" << '('
           << Type::getType<typename P::A1>() << ',' << SP
           << Type::getType<typename P::A2>() << ',' << SP
           << Type::getType<typename P::A3>() << ',' << SP
           << Type::getType<typename P::A4>() << ',' << SP
           << Type::getType<typename P::A5>()
           << ')';
    break;
  case 6:
    stream << Type::getType<typename P::Result>() << SP << "(*)" << '('
           << Type::getType<typename P::A1>() << ',' << SP
           << Type::getType<typename P::A2>() << ',' << SP
           << Type::getType<typename P::A3>() << ',' << SP
           << Type::getType<typename P::A4>() << ',' << SP
           << Type::getType<typename P::A5>() << ',' << SP
           << Type::getType<typename P::A6>()
           << ')';
    break;
  }
  return stream;
}

template<class TYPE>
class T {
public:

  typedef TYPE Type;
};

class Function {
public:
  
  template<class R>
  static inline T<R> getResult(R (*)()) throw() {
    return T<R>();
  }
  
  template<class R>
  static inline Prototype<R> getPrototype(R (*)()) throw() {
    return Prototype<R>();
  }
  
  template<class R, class A>
  static inline T<R> getResult(R (*)(A)) throw() {
    return T<R>();
  }

  template<class R, class A>
  static inline Prototype<R, A> getPrototype(R (*)(A)) throw() {
    return Prototype<R, A>();
  }
  
  template<class R, class A, class B>
  static inline T<R> getResult(R (*)(A, B)) throw() {
    return T<R>();
  }

  template<class R, class A, class B>
  static inline Prototype<R, A, B> getPrototype(R (*)(A, B)) throw() {
    return Prototype<R, A, B>();
  }
  
  template<class R, class A, class B, class C>
  static inline T<R> getResult(R (*)(A, B, C)) throw() {
    return T<R>();
  }
  
  template<class R, class A, class B, class C>
  static inline Prototype<R, A, B, C> getPrototype(R (*)(A, B, C)) throw() {
    return Prototype<R, A, B, C>();
  }
  
  template<class R, class A, class B, class C, class D>
  static inline T<R> getResult(R (*)(A, B, C, D)) throw() {
    return T<R>();
  }
  
  template<class R, class A, class B, class C, class D>
  static inline Prototype<R, A, B, C, D> getPrototype(R (*)(A, B, C, D)) throw() {
    return Prototype<R, A, B, C, D>();
  }
  
  template<class R, class A, class B, class C, class D, class E>
  static inline T<R> getResult(R (*)(A, B, C, D, E)) throw() {
    return T<R>();
  }

  template<class R, class A, class B, class C, class D, class E>
  static inline Prototype<R, A, B, C, D, E> getPrototype(R (*)(A, B, C, D, E)) throw() {
    return Prototype<R, A, B, C, D, E>();
  }
  
  template<class R, class A, class B, class C, class D, class E, class F>
  static inline T<R> getResult(R (*)(A, B, C, D, E, F)) throw() {
    return T<R>();
  }
  
  template<class R, class A, class B, class C, class D, class E, class F>
  static inline Prototype<R, A, B, C, D, E, F> getPrototype(R (*)(A, B, C, D, E, F)) throw() {
    return Prototype<R, A, B, C, D, E, F>();
  }
};

template<class CLASS>
class Method {
public:

  template<class R>
  static inline T<R> getResult(R (CLASS::*)() const) throw() {
    return T<R>();
  }
  
  template<class R>
  static inline T<R> getResult(R (CLASS::*)()) throw() {
    return T<R>();
  }
  
  template<class R, class A>
  static inline T<R> getResult(R (CLASS::*)(A) const) throw() {
    return T<R>();
  }
  
  template<class R, class A>
  static inline T<R> getResult(R (CLASS::*)(A)) throw() {
    return T<R>();
  }
  
  template<class R, class A, class B>
  static inline T<R> getResult(R (CLASS::*)(A, B) const) throw() {
    return T<R>();
  }
  
  template<class R, class A, class B>
  static inline T<R> getResult(R (CLASS::*)(A, B)) throw() {
    return T<R>();
  }
  
  template<class R, class A, class B, class C>
  static inline T<R> getResult(R (CLASS::*)(A, B, C) const) throw() {
    return T<R>();
  }
  
  template<class R, class A, class B, class C>
  static inline T<R> getResult(R (CLASS::*)(A, B, C)) throw() {
    return T<R>();
  }
  
  template<class R, class A, class B, class C, class D>
  static inline T<R> getResult(R (CLASS::*)(A, B, C, D) const) throw() {
    return T<R>();
  }
  
  template<class R, class A, class B, class C, class D>
  static inline T<R> getResult(R (CLASS::*)(A, B, C, D)) throw() {
    return T<R>();
  }

  template<class R, class A, class B, class C, class D, class E>
  static inline T<R> getResult(R (CLASS::*)(A, B, C, D, E) const) throw() {
    return T<R>();
  }
  
  template<class R, class A, class B, class C, class D, class E>
  static inline T<R> getResult(R (CLASS::*)(A, B, C, D, E)) throw() {
    return T<R>();
  }
  
  template<class R, class A, class B, class C, class D, class E, class F>
  static inline T<R> getResult(R (CLASS::*)(A, B, C, D, E, F) const) throw() {
    return T<R>();
  }
  
  template<class R, class A, class B, class C, class D, class E, class F>
  static inline T<R> getResult(R (CLASS::*)(A, B, C, D, E, F)) throw() {
    return T<R>();
  }
};

void* f(char, int, long) {
  return 0;
}

long long g(long long, int) {
  return 0;
}

class XXX {
public:
  
  int* operator()() throw() {
    return 0;
  };
};



class FunctionApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:
  
  FunctionApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("Function", numberOfArguments, arguments, environment) {
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "http://www.mip.sdu.dk/~fonseca/base" << EOL
         << "Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>" << EOL
         << ENDL;

    Function::getResult(f);
    Function::getResult(g);
    MyFunction* f = 0;
    Method<XXX>::getResult(&XXX::operator());
    Method<MyFunction>::getResult(&MyFunction::operator());
    
    fout << literal("Howdy").getLength() << " "
         << literal("Howdy") << " "
         << literal("").getLength() << ENDL;
    
    fout << IsSame<int, int>::VALUE << ENDL;
    fout << IsSame<int, long>::VALUE << ENDL;
    fout << IsSame<char*, int>::VALUE << ENDL;
    fout << IsSame<char*, void*>::VALUE << ENDL;
    fout << IsSame<void, void*>::VALUE << ENDL;
    
    fout << (TypeOf<void*>() == TypeOf<int>()) << ENDL;
    fout << (TypeOf<int>() == TypeOf<long int>()) << ENDL;
    fout << (TypeOf<FX>() == TypeOf<FY>()) << ENDL;
    fout << (TypeOf<FY>() == TypeOf<FZ>()) << ENDL;

    fout << (type(FFX) == type(FFY)) << ENDL;
    fout << (type(FFX) != type(FFY)) << ENDL;
    fout << (type(FFY) != type(FFZ)) << ENDL;
    
    fout << "IsStateless<MyClass>::VALUE: " << IsStateless<MyClass>::VALUE << ENDL;
    fout << "IsStateless<MyVirtualClass>::VALUE: " << IsStateless<MyVirtualClass>::VALUE << ENDL;
    fout << "IsStateless<MyOtherClass>::VALUE: " << IsStateless<MyOtherClass>::VALUE << ENDL;
    
    //Function<int (int, int)> function(MyFunction);
    
    int a = 1;
    int b = 2;
    //int result = function(a, b);
    typedef int (QWERTY)(char, int);
    
//     fout << Type::getType<QWERTY>() << ENDL;
//     fout << Function::getPrototype(g) << ENDL;
  }
};

STUB(FunctionApplication);
