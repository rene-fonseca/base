/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/string/FormatOutputStream.h>
#include <base/io/FileDescriptorOutputStream.h>
#include <base/mem/Allocator.h>
#include <base/mem/ReferenceCountedAllocator.h>

using namespace base;

/*
  When debugging the this program the states of the objects should be visible.
*/

class Base : public virtual ReferenceCountedObject {
private:

  int state;
public:

  Base() throw() : state(0) {}
};

class Child : public Base {
private:

  int childState;
public:

  Child() throw() : childState(0) {}
};

class OtherChild : public Child {
private:

  int otherState;
public:

  OtherChild() throw() : otherState(0) {}
};



void test() {
  fout << "Testing reference counting..." << ENDL;

  fout << "Initializing reference counted object pointers" << ENDL;
  ReferenceCountedObjectPointer<Base> base; // ok
  ReferenceCountedObjectPointer<Child> child; // ok
  ReferenceCountedObjectPointer<OtherChild> otherChild; // ok

  fout << "Checking whether base is valid (expecting false): " << base.isValid() << EOL;

  const ReferenceCountedObjectPointer<Child> constChild; // ok

  fout << "Checking whether base is multi referenced (expecting false): " << base.isMultiReferenced() << EOL;

  base.copyOnWrite();



  fout << "Initializing Allocator" << ENDL;
  Allocator<int> a1;

  fout << "Initializing ReferenceCountedAllocator" << ENDL;
  ReferenceCountedAllocator<int> a2;

  fout << "Initializing ReferenceCountedAllocator (on heap)" << ENDL;
  ReferenceCountedAllocator<int>* a3 = new ReferenceCountedAllocator<int>();
  delete a3;

  fout << "Explicit initialization of automation pointer" << ENDL;
  ReferenceCountedObjectPointer<ReferenceCountedAllocator<int> > a4 = new ReferenceCountedAllocator<int>();

  fout << "Assignment of automation pointer" << ENDL;
  a4 = new ReferenceCountedAllocator<int>();

/*


  ReferenceCountedObjectPointer<AAA> aaa1 = new AAA(); // test exclicit
initialization
//  ReferenceCountedObjectPointer<BBB> aaa2 = new AAA(); // test exclicit
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
  ReferenceCountedObjectPointer<AAA> cpy1(aaa); // ok
  ReferenceCountedObjectPointer<BBB> cpy2(ccc); // poly - ok
  ReferenceCountedObjectPointer<CCC> cpy3(ccc); // ok
//  ReferenceCountedObjectPointer<BBB> cpy4(aaa); // should not compile

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

int main() {
  try {
    test();
  } catch(Exception& e) {
    ferr << "Exception: " << e.getMessage() << ENDL;
    return 1;
  } catch(...) {
    ferr << "Unknown exception" << ENDL;
    return 1;
  }
  fout << "Completed" << ENDL;
  return 0;
}
