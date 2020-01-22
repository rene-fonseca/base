/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mem/InstanceCounter.h>
#include <base/string/Format.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

void InstanceCounters::checkInstanceCounters(const Type& type, MemorySize size)
{
  const MemorySize constructions = this->constructions;
  const MemorySize destructions = this->destructions;
  if (!/*INLINE_ASSERT*/(destructions == constructions)) {
    Trace::message(
      Format::subst(
        MESSAGE("Leaked %1 object(s) of type %2. Constructed %3 and destructed %4. Minimum leaked memory is %5 bytes."),
        constructions - destructions,
        type,
        constructions,
        destructions,
        (constructions - destructions) * size
      ).native()
    );
  }
}


#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(InstanceCounter) : public UnitTest {
public:

  TEST_PRIORITY(50);
  TEST_PROJECT("base/mem");

  class MyObject : public InstanceCounter<MyObject> {
  public:
  };

  void run() override
  {
    {
      MyObject o1;
      MemorySize instances = getInstanceCount<MyObject>();
      TEST_ASSERT(instances == 1);
      MyObject o2;
      instances = getInstanceCount<MyObject>();
      TEST_ASSERT(instances == 2);
      instances = getInstanceDestructionCount<MyObject>();
      TEST_ASSERT(instances == 0);
      instances = getInstanceCountFor(o2);
      TEST_ASSERT(instances == 2);
      instances = getInstanceDestructionCountFor(o2);
      TEST_ASSERT(instances == 0);
    }
    MemorySize instances = getInstanceDestructionCount<MyObject>();
    TEST_ASSERT(instances == 2);
  }
};

TEST_REGISTER(InstanceCounter);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
