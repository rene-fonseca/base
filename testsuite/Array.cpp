/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/collection/Array.h>
#include <base/collection/Functor.h>
#include <base/Functor.h>
#include <base/Type.h>

using namespace base;

int main() {
  fout << "Testing implementation of the Array class" << ENDL;

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

  fout << "Invalid insert (expecting OutOfRange)" << ENDL;
  try {
    ai.insert(11, 123);
  } catch (Exception& e) {
    fout << "  Catched exception: " << getTypename(e) << ENDL;
  }

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

  return 0;
}
