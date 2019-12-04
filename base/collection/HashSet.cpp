/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/collection/HashSet.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__DUMMY_SYMBOL

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE


#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(HashSet) : public UnitTest {
public:

  TEST_PRIORITY(10);
  TEST_PROJECT("base/collection");

  void run() override
  {
    HashSet<int> si;
    for (auto i : range(-10, 20)) {
      si.add(i);
    }
    // fout << si << ENDL;
    TEST_ASSERT(si.hasKey(-10));
    TEST_ASSERT(!si.hasKey(20));

    HashSet<String> ss;
    ss.add("abc");
    ss.add("123");
    ss.add("up");
    ss.add("down");
    // fout << ss << ENDL;

#if 0
    MemorySize count = 0;
    for (const auto& v : ss) {
      ++count;
      // fout << v << ENDL;
    }
    TEST_ASSERT(count == ss.getSize());
#endif
    
    TEST_ASSERT(ss.getSize() == 4);
    TEST_ASSERT(ss.hasKey("up"));
    TEST_ASSERT(!ss.hasKey("UP"));
    TEST_ASSERT(ss);

    // TAG: add iterators
#if 0
    for (const auto& value : ss) {
    }
#endif
    
#if 0
    MemorySize count = 0;
    for (const auto& value : ss) {
      const String _value = static_cast<const String&>(value);
      ++count;
    }
    TEST_ASSERT(count == 4);
#endif
    
#if 0
    TEST_ASSERT(std::find(ss.begin(), ss.end(), "up") != ss.end());
    TEST_ASSERT(*std::find(ss.begin(), ss.end(), "up") == "up");
    TEST_ASSERT(std::find(ss.begin(), ss.end(), "left") == ss.end());
#endif
  }
};

TEST_REGISTER(HashSet);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
