/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/List.h>
#include <base/collection/Functor.h>
#include <base/Functor.h>

using namespace base;

int main() {
  fout << "Testing List implementation" << ENDL;

  fout << "Initializing list" << ENDL;
  List<int> li;

  fout << "Appending elements to list (4, 5, 6, and 7)" << ENDL;
  li.append(4);
  li.append(5);
  li.append(6);
  li.append(7);
  fout << "li: " << li << ENDL;

  fout << "Prepending elements to list (3, 2, 1, and 0)" << ENDL;
  li.prepend(3);
  li.prepend(2);
  li.prepend(1);
  li.prepend(0);
  fout << "li: " << li << ENDL;

  fout << "Adding elements to list (8, 9, 10, and 11)" << ENDL;
  li.add(8);
  li.add(9);
  li.add(10);
  li.add(11);
  fout << "li: " << li << ENDL;

  fout << "Size of list: " << li.getSize() << ENDL;

  fout << "Modifying enumerating elements of list (multiplying by 2)" << ENDL;
  transform(li, bind2First(Multiply<int>(), 2));
  fout << "li: " << li << ENDL;

  fout << "Non-modifying enumerating elements of list (calculating sum)" << ENDL;
  Sum<int> sum;
  forEach(li, sum);
  fout << "Sum: " << sum.getResult() << ENDL;

  fout << "Size: " << li.getSize() << ENDL;
  fout << "Removing all the elements" << ENDL;
  li.removeAll();
  fout << "Size: " << li.getSize() << ENDL;

  return 0;
}
