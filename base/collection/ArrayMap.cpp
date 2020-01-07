/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/collection/ArrayMap.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__DUMMY_SYMBOL

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(ArrayMap) : public UnitTest {
public:

  TEST_PRIORITY(30);
  TEST_PROJECT("base/collection");

  void run() override
  {
    TEST_ASSERT((std::is_default_constructible<ArrayMap<String, String> >()));
    TEST_ASSERT((std::is_copy_constructible<ArrayMap<String, String> >()));
    TEST_ASSERT((std::is_copy_assignable<ArrayMap<String, String> >()));
    TEST_ASSERT((std::is_move_constructible<ArrayMap<String, String> >()));
    TEST_ASSERT((std::is_move_assignable<ArrayMap<String, String> >()));

    ArrayMap<String, String> m1{};
    ArrayMap<String, String> m2{ Association<String, String>("1", "a") };
    ArrayMap<String, String> m3{
      Association<String, String>("1", "a"), Association<String, String>("2", "b")
    };
    TEST_ASSERT(m3.hasKey("2"));

    ArrayMap<String, String> c1;
    c1.add("key4", "value4");
    c1.add("key3", "value3");
    c1.add("key1", "value1");
    c1.add("key2", "value2");
    c1.add("key5", "value5");
    TEST_ASSERT(c1);
    TEST_ASSERT(c1.getSize() == 5);

    TEST_ASSERT(c1.getValue("key3") == "value3");
#if 0
    TEST_ASSERT(c1[String("key3")] == "value3");
    c1[String("key3")] = "other";
    TEST_ASSERT(c1[String("key3")] == "other");

    MemorySize count = 0;
    for (const auto& v : c1) {
      ++count;
      // fout << v << ENDL;
    }
    TEST_ASSERT(count == c1.getSize());
#endif

    TEST_ASSERT(c1.hasKey("key2"));
    TEST_ASSERT(c1.find("key2"));
    // c1.remove("key2");
    // TEST_ASSERT(!c1.hasKey("key2"));
    // TEST_ASSERT(!c1.find("key2"));
    // TEST_ASSERT(c1.getSize() == 4);

    // TEST_EXCEPTION(c1.remove("key6"), InvalidKey);

    c1.removeAll();
    TEST_ASSERT(!c1);

    ArrayMap<String, int> ht1 = {};
    ArrayMap<String, int> ht2 = { {"123", 3} };
    TEST_ASSERT(ht2.getSize() == 1);
    ArrayMap<String, int> ht3 = { {"123", 3}, {"abc", 12} };
    TEST_ASSERT(ht3.getSize() == 2);
  }
};

TEST_REGISTER(ArrayMap);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
