/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mem/Reference.h>
#include <base/string/FormatOutputStream.h>
#include <base/io/FileDescriptorOutputStream.h>
#include <base/mem/Allocator.h>
#include <base/mem/ReferenceCountedAllocator.h>
#include <base/Application.h>

using namespace dk::sdu::mip::base;

/*
  When debugging this program the states of the objects should be visible.
*/

class Base : public virtual ReferenceCountedObject {
private:

  int state;
public:

  Base() throw() : state(0) {
  }
};

class Child : public Base {
private:

  int childState;
public:

  Child() throw() : childState(0) {
  }
};

class OtherChild : public Child {
private:

  int otherState;
public:

  OtherChild() throw() : otherState(0) {
  }
};



class ReferenceCountingApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  ReferenceCountingApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("referenceCounting"), numberOfArguments, arguments, environment) {
  }

  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ")
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    
    fout << MESSAGE("Initializing reference counted object pointers") << ENDL;
    Reference<Base> base; // ok
    Reference<Child> child; // ok
    Reference<OtherChild> otherChild; // ok

    fout << MESSAGE("Checking whether base is valid (expecting false): ")
         << base.isValid() << EOL;

    const Reference<Child> constChild; // ok

    fout << MESSAGE("Checking whether base is multi referenced (expecting false): ") << base.isMultiReferenced() << EOL;

    base.copyOnWrite();



    fout << MESSAGE("Initializing Allocator") << ENDL;
    Allocator<int> a1;

    fout << MESSAGE("Initializing ReferenceCountedAllocator") << ENDL;
    ReferenceCountedAllocator<int> a2;

    fout << MESSAGE("Initializing ReferenceCountedAllocator (on heap)") << ENDL;
    ReferenceCountedAllocator<int>* a3 = new ReferenceCountedAllocator<int>();
    delete a3;

    fout << MESSAGE("Explicit initialization of automation pointer") << ENDL;
    Reference<ReferenceCountedAllocator<int> > a4 =
      new ReferenceCountedAllocator<int>();

    fout << MESSAGE("Assignment of automation pointer") << ENDL;
    a4 = new ReferenceCountedAllocator<int>();
/*
  Reference<AAA> aaa1 = new AAA(); // test exclicit
initialization
//  Reference<BBB> aaa2 = new AAA(); // test exclicit
initialization - should not compile

  // test assignment
//  const AAA* a = new AAA();
  aaa = new AAA(); // test assignment - ok
//  aaa = a; // should not compile
  aaa = new BBB(); // test assignment and polymophism - ok
  bbb = new CCC(); // test assignment and polymophism - ok
  ccc = new CCC(); // test assignment and polymophism - ok
//  cbbb = new BBB(); // should not compile

  aaa = bbb; // ok
  bbb = bbb; // self assignment
//  ccc = bbb; // should not compile

  // test copy construction
  Reference<AAA> cpy1(aaa); // ok
  Reference<BBB> cpy2(ccc); // poly - ok
  Reference<CCC> cpy3(ccc); // ok
//  Reference<BBB> cpy4(aaa); // should not compile

  // test type cast
  AAA* val1;
  BBB* val2;
  BBB* val3;
  const BBB* val4;

  val1 = aaa.getValue(); // ok
  val2 = ccc.getValue(); // ok;
//  val2 = aaa.getValue(); // should not compile
  val3 = ccc.getValue(); // ok
  val4 = ccc.getValue(); // ok

  // test getValue()
  val1 = aaa.getValue(); // ok
//  val2 = aaa.getValue(); // should not compile
  val3 = ccc.getValue(); // ok

  // test for ambiguous references
  val1->references = 0;


  // const object section
  const AAA* c1 = new AAA();
  const BBB* c2 = new BBB();
  const CCC* c3 = new CCC();
*/
  }
};

STUB(ReferenceCountingApplication);
