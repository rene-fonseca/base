/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/collection/Map.h>
#include <base/collection/Moveable.h>
#include <base/UnitTest.h>
#include <string>

_COM_AZURE_DEV__BASE__DUMMY_SYMBOL

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

template _COM_AZURE_DEV__BASE__API class Map<bool, bool>;
template _COM_AZURE_DEV__BASE__API class Map<short, short>;
template _COM_AZURE_DEV__BASE__API class Map<unsigned short, unsigned short>;
template _COM_AZURE_DEV__BASE__API class Map<int, int>;
template _COM_AZURE_DEV__BASE__API class Map<unsigned int, unsigned int>;
template _COM_AZURE_DEV__BASE__API class Map<long, long>;
template _COM_AZURE_DEV__BASE__API class Map<unsigned long, unsigned long>;
#if (_COM_AZURE_DEV__BASE__COMPILER != _COM_AZURE_DEV__BASE__COMPILER_GCC)
template _COM_AZURE_DEV__BASE__API class Map<int64, int64>;
template _COM_AZURE_DEV__BASE__API class Map<uint64, uint64>;
#endif
template _COM_AZURE_DEV__BASE__API class Map<char, char>;
template _COM_AZURE_DEV__BASE__API class Map<unsigned char, unsigned char>;
template _COM_AZURE_DEV__BASE__API class Map<wchar, wchar>;
template _COM_AZURE_DEV__BASE__API class Map<void*, void*>;
template _COM_AZURE_DEV__BASE__API class Map<float, float>;
template _COM_AZURE_DEV__BASE__API class Map<double, double>;
template _COM_AZURE_DEV__BASE__API class Map<long double, long double>;

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Map) : public UnitTest {
public:

  TEST_PRIORITY(10);
  TEST_PROJECT("base/collection");

  void run() override
  {
    TEST_ASSERT((std::is_default_constructible<Map<String, String> >()));
    TEST_ASSERT((std::is_copy_constructible<Map<String, String> >()));
    TEST_ASSERT((std::is_copy_assignable<Map<String, String> >()));
    TEST_ASSERT((std::is_move_constructible<Map<String, String> >()));
    TEST_ASSERT((std::is_move_assignable<Map<String, String> >()));

#if 0
    Map<NonDefaultConstructible, NonDefaultConstructible> map1;
    map1.add(NonDefaultConstructible(nullptr), NonDefaultConstructible(nullptr));

    Map<NonMoveable, NonMoveable> map2;
    map2.add(NonMoveable(), NonMoveable());
#endif

    Map<String, String> map1;
    map1.add("key4", "value4");
    map1.add("key3", "value3");
    map1.add("key1", "value1");
    map1.add("key2", "value2");
    map1.add("key5", "value5");
    TEST_ASSERT(map1);
    TEST_ASSERT(map1.getSize() == 5);

    TEST_ASSERT(map1.getValue("key3") == "value3");
    TEST_ASSERT(map1[String("key3")] == "value3");
    map1[String("key3")] = "other";
    TEST_ASSERT(map1[String("key3")] == "other");

#if 0
    for (auto v : map1) {
    }
#endif

    TEST_ASSERT(map1.hasKey("key2"));
    TEST_ASSERT(map1.find("key2"));
    map1.remove("key2");
    TEST_ASSERT(!map1.hasKey("key2"));
    TEST_ASSERT(!map1.find("key2"));
    TEST_ASSERT(map1.getSize() == 4);
    
    TEST_EXCEPTION(map1.remove("key6"), InvalidKey);

    map1.removeAll();
    TEST_ASSERT(!map1);
  }
};

TEST_REGISTER(Map);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
