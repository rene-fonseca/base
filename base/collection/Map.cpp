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

_COM_AZURE_DEV__BASE__DUMMY_SYMBOL

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

template class _COM_AZURE_DEV__BASE__API Map<bool, bool>;
template class _COM_AZURE_DEV__BASE__API Map<short, short>;
template class _COM_AZURE_DEV__BASE__API Map<unsigned short, unsigned short>;
template class _COM_AZURE_DEV__BASE__API Map<int, int>;
template class _COM_AZURE_DEV__BASE__API Map<unsigned int, unsigned int>;
template class _COM_AZURE_DEV__BASE__API Map<long, long>;
template class _COM_AZURE_DEV__BASE__API Map<unsigned long, unsigned long>;
#if (_COM_AZURE_DEV__BASE__COMPILER != _COM_AZURE_DEV__BASE__COMPILER_GCC)
template class _COM_AZURE_DEV__BASE__API Map<int64, int64>;
template class _COM_AZURE_DEV__BASE__API Map<uint64, uint64>;
#endif
template class _COM_AZURE_DEV__BASE__API Map<char, char>;
template class _COM_AZURE_DEV__BASE__API Map<unsigned char, unsigned char>;
template class _COM_AZURE_DEV__BASE__API Map<wchar, wchar>;
template class _COM_AZURE_DEV__BASE__API Map<void*, void*>;
template class _COM_AZURE_DEV__BASE__API Map<float, float>;
template class _COM_AZURE_DEV__BASE__API Map<double, double>;
template class _COM_AZURE_DEV__BASE__API Map<long double, long double>;

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

    Map<String, String> m1{};
    Map<String, String> m2{Association<String, String>("1", "a")};
    Map<String, String> m3{
      Association<String, String>("1", "a"), Association<String, String>("2", "b")
    };
    TEST_ASSERT(m3.hasKey("2"));

    Map<String, int> m4 = {};
    Map<String, int> m5 = {{"123", 3}};
    TEST_ASSERT(m5.getSize() == 1);
    Map<String, int> m6 = {{"123", 3}, {"abc", 12}};
    TEST_ASSERT(m6.getSize() == 2);

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

    MemorySize count = 0;
    for (const auto& v : map1) {
      ++count;
      // fout << v << ENDL;
    }
    TEST_ASSERT(count == map1.getSize());

    count = 0;
    for (PrefixOrderIterator<ReadIteratorTraits<Map<String, String>::Tree::Node> > src(map1.getTree().getRoot()); src; ++src) {
      ++count;
      // fout << *src << ENDL;
    }
    TEST_ASSERT(count == map1.getSize());

    count = 0;
    for (InfixOrderIterator<ReadIteratorTraits<Map<String, String>::Tree::Node> > src(map1.getTree().getRoot()); src; ++src) {
      ++count;
      // fout << *src << ENDL;
    }
    TEST_ASSERT(count == map1.getSize());

    count = 0;
    for (PostfixOrderIterator<ReadIteratorTraits<Map<String, String>::Tree::Node> > src(map1.getTree().getRoot()); src; ++src) {
      ++count;
      // fout << *src << ENDL;
    }
    TEST_ASSERT(count == map1.getSize());

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
