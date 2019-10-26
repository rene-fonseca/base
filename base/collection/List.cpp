/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/collection/List.h>
#include <base/UnitTest.h>
#include <algorithm>

_COM_AZURE_DEV__BASE__DUMMY_SYMBOL

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

_COM_AZURE_DEV__BASE__INSTANTIATE_CONTAINER_COMMON_TYPE_LIMITED(List)

#if 0 // example for avoiding friend declarations for swapper
template<class TYPE>
class SupportsSwapper {
public:

  static constexpr bool SUPPORTS_SWAPPER = false;
};

template<class TYPE>
class SupportsSwapper<Reference<TYPE> > {
public:

  static constexpr bool SUPPORTS_SWAPPER = true;
};

template<class TYPE, bool>
class SwapperForward {
public:

  static inline void swapper(TYPE& a, TYPE& b)
  {
    TYPE temp(std::move(a));
    a = std::move(b);
    b = std::move(temp);
  }
};

template<class TYPE>
class SwapperForward<TYPE, true> {
public:

  static inline void swapper(TYPE& a, TYPE& b)
  {
    TYPE::swapper(a, b); // forward to class swapper
  }
};

template<class TYPE>
inline void swapper2(TYPE& a, TYPE& b)
{
  SwapperForward<TYPE, SupportsSwapper<TYPE>::SUPPORTS_SWAPPER>::swapper(a, b);
}
#endif



#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(List) : public UnitTest {
public:

  TEST_PRIORITY(10);

  void run() override
  {
    List<int> li;
    for (auto i : range(-10, 20)) {
      li.append(i);
    }
    fout << li << ENDL;

    List<String> ls;
    ls.append("abc");
    ls.append("123");
    ls.append("up");
    ls.append("down");
    fout << ls << ENDL;

    for (const auto& value : ls) {
    }

    for (auto& value : List<String>(ls)) {
      value.append('=');
      const String _value = static_cast<const String&>(value);
    }

    MemorySize count = 0;
    for (auto& value : static_cast<const List<String>&>(ls)) {
      const String _value = static_cast<const String&>(value);
      ++count;
    }
    TEST_ASSERT(count == 4);
    
    TEST_ASSERT(std::find(ls.begin(), ls.end(), "up") != ls.end());
    TEST_ASSERT(*std::find(ls.begin(), ls.end(), "up") == "up");
    TEST_ASSERT(std::find(ls.begin(), ls.end(), "left") == ls.end());

    // TAG: test: add support for printing anything relevant out - redirect fout and ferr for non-verbose? - we could reformat output with prefix >>
    // fout << "UNSORTED: " << ls << ENDL;
    ls.sort();
    // fout << "SORTED: " << ls << ENDL;
    TEST_ASSERT(std::find(ls.begin(), ls.end(), "123") == ls.begin());
  }
};

TEST_REGISTER(List);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
