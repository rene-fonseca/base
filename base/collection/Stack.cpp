/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/collection/Stack.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__DUMMY_SYMBOL

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

_COM_AZURE_DEV__BASE__INSTANTIATE_CONTAINER_COMMON_TYPE(Stack)

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Stack) : public UnitTest {
public:

  TEST_PRIORITY(10);

  void run() override
  {
    Stack<int> si;
    for (auto i : range(-10, 20)) {
      si.push(i);
    }
    TEST_ASSERT(si.peek() == 19);
    TEST_ASSERT(si.peek(1) == 18);

    auto size = si.getSize();
    TEST_ASSERT(si.pop() == 19);
    TEST_ASSERT((size - 1) == si.getSize());
    si.pop(5);
    TEST_ASSERT((size - 6) == si.getSize());

    /*
    for (auto value : si) {
      fout << value << ENDL;
    }
    */
    
    /* // GCC issue
    TEST_ASSERT(std::find(si.begin(), si.end(), 10) != si.end());
    TEST_ASSERT(std::find(si.begin(), si.end(), 19) == si.end());
    */
    
    si.removeAll();
    TEST_ASSERT(si.isEmpty());
  }
};

TEST_REGISTER(Stack);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
