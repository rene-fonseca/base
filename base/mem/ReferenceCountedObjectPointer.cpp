/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "ReferenceCountedObjectPointer.h"
/*
class AAA : public ReferenceCountedObject {
public:
  char references;
};

class BBB : public AAA {
};

class CCC : public BBB {
};

int main() {
  // test initialization
  ReferenceCountedObjectPointer<AAA> aaa; // ok
  ReferenceCountedObjectPointer<BBB> bbb; // ok
  ReferenceCountedObjectPointer<CCC> ccc; // ok
  const ReferenceCountedObjectPointer<BBB> cbbb; // ok

  ReferenceCountedObjectPointer<AAA> aaa1 = new AAA(); // test exclicit initialization
//  ReferenceCountedObjectPointer<BBB> aaa2 = new AAA(); // test exclicit initialization - should not compile

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

  // test initialization
  ReferenceCountedObjectPointer<const AAA> aa; // ok
  ReferenceCountedObjectPointer<const BBB> bb; // ok
  ReferenceCountedObjectPointer<const CCC> cc; // ok
  const ReferenceCountedObjectPointer<const BBB> cbb; // ok

  ReferenceCountedObjectPointer<const AAA> aa1 = new BBB(); // test exclicit initialization
//  ReferenceCountedObjectPointer<const BBB> aa2 = new AAA(); // should not compile

  // test assignment
  aa = c1; // test assignment - ok
  aa = c2; // test assignment and polymophism - ok
  bb = c3; // test assignment and polymophism - ok
  cc = c3; // test assignment and polymophism - ok
//  cbb = c2; // should not compile

  aa = bb; // ok
  bb = bb; // self assignment
//  cc = bb; // should not compile


  // test copy construction
  ReferenceCountedObjectPointer<const AAA> cp1(aa); // ok
  ReferenceCountedObjectPointer<const BBB> cp2(cc); // poly - ok
  ReferenceCountedObjectPointer<const CCC> cp3(cc); // ok
//  ReferenceCountedObjectPointer<const BBB> cp4(aa); // should not compile


  // test: from mutable to const auto pointer
  ReferenceCountedObjectPointer<AAA> m1; // check copy constructor of general
  ReferenceCountedObjectPointer<const AAA> m2(m1); // check copy constructor of general
  ReferenceCountedObjectPointer<const AAA> m3 = m1; // check copy constructor of general
  m3 = m1;
//  m1 = m3; // should not compile

  aa = aaa;
  bb = ccc;

  // test: from const to mutable auto pointer
//  ReferenceCountedObjectPointer<AAA> m3(aa); // check copy constructor of general

  return 0;
}
*/
