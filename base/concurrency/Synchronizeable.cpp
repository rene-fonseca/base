/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Synchronizeable.h"
#include "Synchronize.h"

template<class LOCK = DefaultLock>
class test : public Synchronizeable<LOCK> {
public:
  test() {};
  void doit();
};

template<class LOCK>
void test<LOCK>::doit() {
//  Synchronize<MTLEVEL> _sync(*this);
  SynchronizeShared();
  if (true)
  {
    SynchronizeRelease();
    SynchronizeExclusively();
  }
}

test<> testmt;
test<Unsafe> testst;

int main() {
  testmt.doit();
  testst.doit();
  return 0;
}
