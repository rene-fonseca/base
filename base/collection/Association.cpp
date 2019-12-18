/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/collection/Association.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__DUMMY_SYMBOL

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Association) : public UnitTest {
public:

  TEST_PRIORITY(30);
  TEST_PROJECT("base/collection");

  void run() override
  {
    Association<String, String> p1("abc", "123");
    auto p2 = p1;
    TEST_ASSERT(p1 == p2);
    TEST_ASSERT(!(p1 != p2));

    Association<String, String> p3("abc", "1234");
    TEST_ASSERT(p1 == p3); // only key is compared
    TEST_ASSERT(!(p1 != p3)); // only key is compared
    TEST_ASSERT(!(p1 < p3)); // only key is compared

    Association<String, String> p4("abcd", "1234");
    TEST_ASSERT(!(p1 == p4)); // only key is compared
    TEST_ASSERT(p1 != p4); // only key is compared
    TEST_ASSERT(p1 < p4); // only key is compared
  }
};

TEST_REGISTER(Association);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
