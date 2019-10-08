/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/collection/List.h>
#include <base/collection/Functor.h>
#include <base/Functor.h>

using namespace dk::sdu::mip::base;

class ListApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  ListApplication(int argc, const char* argv[], const char* env[]) throw()
    : Application("List", argc, argv, env) {
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "https://www.fonseca.dk/base" << EOL
         << "Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>" << EOL
         << ENDL;
    
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
  }
};

STUB(ListApplication);
