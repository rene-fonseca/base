/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/collection/List.h>
#include <base/collection/Functor.h>
#include <base/Functor.h>

using namespace base;

class ListApplication : public Application {
public:

  ListApplication(int argc, const char* argv[], const char* env[]) : Application(MESSAGE("List"), argc, argv, env) {
  }
  
  void main() throw() {
    fout << MESSAGE("Testing List implementation") << ENDL;

    fout << MESSAGE("Initializing list") << ENDL;
    List<int> li;

    fout << MESSAGE("Appending elements to list (4, 5, 6, and 7)") << ENDL;
    li.append(4);
    li.append(5);
    li.append(6);
    li.append(7);
    fout << MESSAGE("li: ") << li << ENDL;

    fout << MESSAGE("Prepending elements to list (3, 2, 1, and 0)") << ENDL;
    li.prepend(3);
    li.prepend(2);
    li.prepend(1);
    li.prepend(0);
    fout << MESSAGE("li: ") << li << ENDL;

    fout << MESSAGE("Adding elements to list (8, 9, 10, and 11)") << ENDL;
    li.add(8);
    li.add(9);
    li.add(10);
    li.add(11);
    fout << MESSAGE("li: ") << li << ENDL;

    fout << MESSAGE("Size of list: ") << li.getSize() << ENDL;

    fout << MESSAGE("Modifying enumerating elements of list (multiplying by 2)") << ENDL;
    transform(li, bind2First(Multiply<int>(), 2));
    fout << MESSAGE("li: ") << li << ENDL;

    fout << MESSAGE("Non-modifying enumerating elements of list (calculating sum)") << ENDL;
    Sum<int> sum;
    forEach(li, sum);
    fout << MESSAGE("Sum: ") << sum.getResult() << ENDL;

    fout << MESSAGE("Size: ") << li.getSize() << ENDL;
    fout << MESSAGE("Removing all the elements") << ENDL;
    li.removeAll();
    fout << MESSAGE("Size: ") << li.getSize() << ENDL;
  }
};

int main(int argc, const char* argv[], const char* env[]) {
  ListApplication application(argc, argv, env);
  try {
    application.main();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::getApplication()->getExitCode();
}
