/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/collection/PriorityQueue.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__DUMMY_SYMBOL

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

_COM_AZURE_DEV__BASE__INSTANTIATE_CONTAINER_COMMON_TYPE_LIMITED(PriorityQueue)

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(PriorityQueue) : public UnitTest {
public:

  TEST_PRIORITY(10);
  TEST_PROJECT("base/collection");

  void run() override
  {
    PriorityQueue<String> q1{};
    PriorityQueue<String> q2{{10, "1"}};
    PriorityQueue<String> q3{{5, "2"}, {10, "1"}, {7, "3"}};
    TEST_ASSERT(q3.getSize() == 3);
    TEST_ASSERT(q3.pop() == "1");
    TEST_ASSERT(q3.getSize() == 2);
    TEST_ASSERT(q3.pop() == "3");
    TEST_ASSERT(q3.getSize() == 1);

#if 0
    for (auto value : q3) {
      fout << value << ENDL;
    }
#endif
    
    q3.removeAll();
    TEST_ASSERT(q3.isEmpty());
  }
};

TEST_REGISTER(PriorityQueue);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
