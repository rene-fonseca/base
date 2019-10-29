/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/collection/Array.h>
#include <base/UnitTest.h>
#include <algorithm>

_COM_AZURE_DEV__BASE__DUMMY_SYMBOL

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

_COM_AZURE_DEV__BASE__INSTANTIATE_CONTAINER_COMMON_TYPE_LIMITED(Array)

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Array) : public UnitTest {
public:

  TEST_PRIORITY(10);
  TEST_PROJECT("base/collection");

  void run() override
  {
    Array<int> ai;
    for (auto i : range(-10, 20)) {
      ai.append(i);
    }

    Array<String> as;
    as.append("abc");
    as.append("123");
    as.append("up");
    as.append("down");
    // TAG: need UTF-8 comparison for String

    TEST_ASSERT(std::find(as.begin(), as.end(), "up") != as.end());
    TEST_ASSERT((std::find(as.begin(), as.end(), "up") - as.begin()) == 2);
    TEST_ASSERT(std::find(as.begin(), as.end(), "left") == as.end());

    std::sort(as.begin(), as.end());
    TEST_ASSERT((std::find(as.begin(), as.end(), "123") - as.begin()) == 0);
    TEST_ASSERT((std::find(as.begin(), as.end(), "up") - as.begin()) == 3);
  }
};

TEST_REGISTER(Array);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
