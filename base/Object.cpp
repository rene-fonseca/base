/***************************************************************************
    begin       : Fri May 12 2000
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Object.h"
#include "Exception.h"
#include <iostream>

using namespace ::std;

bool Object::operator==(const Object& object) {
  return (this == &object);
}

void Object::debug() const {
  cout << "CLASS/Object\n";
}



void* operator new(size_t size) {
  void* p = malloc(size);
  if (!p) {
    throw MemoryException();
  }
  return p;
}

void operator delete(void* p) {
  free(p);
}


/*
ostream& operator<<(ostream& stream, Object& object) {
  object.toStream(stream);
  return stream;
}
*/