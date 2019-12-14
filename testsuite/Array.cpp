/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/collection/Array.h>
#include <base/collection/Functor.h>
#include <base/Functor.h>
#include <base/TypeInfo.h>

using namespace com::azure::dev::base;

class ArrayApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  ArrayApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[])
    : Application("Array", numberOfArguments, arguments, environment)
  {
  }
  
  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;
    
    fout << "Initializing array" << ENDL;
    
    Array<int> ai;
    fout << "Appending values (1, 2, 3, 4, and 5)" << ENDL;
    ai.append(1);
    ai.append(2);
    ai.append(3);
    ai.append(4);
    ai.append(5);
    fout << "ai: " << ai << ENDL;

    fout << "Prepending (10, and 11)" << ENDL;
    ai.prepend(10);
    ai.prepend(11);
    fout << "ai: " << ai << ENDL;

    fout << "Inserting 21 at index 0" << ENDL;
    ai.insert(0, 21);
    fout << "Inserting 22 at index 4" << ENDL;
    ai.insert(4, 22);
    fout << "Inserting 23 at index 9" << ENDL;
    ai.insert(9, 23);
    fout << "ai: " << ai << ENDL;

    fout << "Invalid insert (expecting OutOfRange) for size " << ai.getSize() << ENDL;
    try {
      ai.insert(11, 123);
    } catch (Exception& e) {
      fout << indent(2) << "Catched exception: " << TypeInfo::getTypename(e) << ENDL;
    }
    ai.insert(10, 1234); // at end

    fout << "Removing element at index 2" << ENDL;
    ai.remove(2);
    fout << "ai: " << ai << ENDL;

    fout << "Modifying enumeration of elements (multiply values by 2)" << ENDL;
    transform(ai, bind2First(Multiply<int>(), 2));
    fout << "ai: " << ai << ENDL;

    fout << "Non-modifying enumerating elements of list (calculating sum)" << ENDL;
    Sum<int> sum;
    forEach(ai, sum);
    fout << "Sum: " << sum.getResult() << ENDL;

    fout << "Size: " << ai.getSize() << ENDL;
    fout << "Removing all the elements" << ENDL;
    ai.removeAll();
    fout << "Size: " << ai.getSize() << ENDL;

    Array<float> af = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f};
    fout << "af: " << af << ENDL;

    Array<double> ad = {1., 2., 3., 4., 5., 6., 7., 8., 9.};
    transform(ad, bind2First(Multiply<double>(), -0.42));
    fout << "ad: " << ad << ENDL;
  }
};

APPLICATION_STUB(ArrayApplication);



#if 0 // simple test for WASI
#include <stdio.h>
#include <base/math/Vector3D.h>
#include <base/string/StringOutputStream.h>

// this causes base library to be included in wasm
#if 0
void doit()
{
  Array<int64> a;
  a.append(4);
  a.append(2);
  a.append(6);
  a.append(8);
}
#endif

int main(int argc, const char* argv[])
{
  printf("BEGIN!\n");
  dvector3 result(0,0,1);
  result += dvector3(2,3,4);
  double m = result.getModulus();
  printf("MATH %f\n", m);
  // String hello = "Hello123";
  // printf("TEST %s!\n", hello.native());
  printf("DONE!\n");
  return 1;
}
#endif
