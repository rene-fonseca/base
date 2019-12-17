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

    Array<String> a1{};
    Array<String> a2{"1"};
    Array<String> a3{"1", "2"};

    Array<String> as;
    as.append("abc");
    as.append("123");
    as.append("up");
    as.append("down");
    as.append("snake");
    as.append("giraf");
    as.append("penguin");
    as.append("elefant");
    as.append("bat");
    as.append("dog");
    as.append("lion");

    // TAG: need UTF-8 comparison for String

    TEST_ASSERT(std::find(as.begin(), as.end(), "up") != as.end());
    TEST_ASSERT((std::find(as.begin(), as.end(), "up") - as.begin()) == 2);
    TEST_ASSERT(std::find(as.begin(), as.end(), "left") == as.end());

    as.shuffle();
    as.sort();
    // fout << "as: " << as << ENDL;

    auto testMergeSort = as;
    testMergeSort.shuffle();
    mergeSort(testMergeSort.begin(), testMergeSort.end());
    // testMergeSort.sort();
    TEST_ASSERT(testMergeSort == as);
    // fout << "testMergeSort: " << testMergeSort << ENDL;

    mergeSort(testMergeSort.begin(), testMergeSort.end(), [](const String& a, const String& b) {return b <= a;});
    // testMergeSort.sort([](const String& a, const String& b) {return b <= a; }); // reversed
    // fout << "reversed testMergeSort: " << testMergeSort << ENDL;

    auto testStdSort = as;
    testStdSort.shuffle();
    std::sort(testStdSort.begin(), testStdSort.end());
    TEST_ASSERT(testStdSort == as);
    // fout << "testStdSort: " << testStdSort << ENDL;

    TEST_ASSERT((std::find(as.begin(), as.end(), "123") - as.begin()) == 0);
    TEST_ASSERT((std::find(as.begin(), as.end(), "up") - as.begin()) == (as.getSize() - 1));
    TEST_ASSERT(as.indexOf("123") == 0);
    TEST_ASSERT(as.lastIndexOf("123") == 0);
    TEST_ASSERT(as.indexOf("left") == -1);
    TEST_ASSERT(as.lastIndexOf("left") == -1);

    for (auto i = as.begin(); i != as.end(); ++i) {
      String& s = *i;
    }
    for (auto i = as.cbegin(); i != as.cend(); ++i) {
      const String& s = *i;
    }

    TEST_ASSERT(as);

    as.getAt(0);
    as.getAt(as.getSize() - 1);
    as.setAt(as.getSize() - 1, "SET");
    // as.getAt(as.getSize()); // you can enable this to see how a failing test is handled
    TEST_EXCEPTION(as.getAt(as.getSize()), OutOfRange);
    TEST_EXCEPTION(as.setAt(as.getSize(), "out of range"), OutOfRange);
    
    as.prepend("prepend");
    as.insert(as.begin(), "prepend2");
    as.remove(as.begin());
    as.insert(as.end(), "append2");
    as.remove(as.getSize() - 1);
    as.append("append");
    as.setSize(100, "fill by value");
    as.setSize(10, "reduce");
    as.shuffle();
    std::reverse(as.begin(), as.end());
    // std::move(as.begin(), as.end(), as.begin());
    as.remove(2, 4);
    as.removeAll();
    TEST_ASSERT(!as);
    
    auto as2 = as;
    as2 = Array<String>();
        
    TEST_ASSERT(std::is_move_constructible<Array<String> >());
    TEST_ASSERT(std::is_move_assignable<Array<String> >());
    
    Array<unsigned int> al(100, 99);
    Array<unsigned int> al2;
    swapper(al, al2);

    al2 = {1,2,3,4};
    TEST_ASSERT(al2.getLast() == 4);
    al2.append({10,20,30,40});
    al2.prepend({100,200,300,400});
    TEST_ASSERT(al2.getFirst() == 100);
    TEST_ASSERT(al2.getLast() == 40);

    auto al3 = al2.slice(3, 7);
    al3 = al2.slice(al2.begin() + 3, al2.begin() + 7);

    Array<int> ai2;
    TEST_ASSERT(binarySearch(ai2.begin(), ai2.end(), 123) == ai2.end());
    for (auto i : range(-10, 20)) {
      ai2.append(i);
    }
    TEST_ASSERT(binarySearch(ai2.begin(), ai2.end(), -10) == ai2.begin());
    TEST_ASSERT(binarySearch(ai2.begin(), ai2.end(), 19) != ai2.end());
    TEST_ASSERT(binarySearch(ai2.begin(), ai2.end(), 20) == ai2.end());
  }
};

TEST_REGISTER(Array);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
