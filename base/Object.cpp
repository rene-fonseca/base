/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/Object.h>

Object::Object() throw() {
}

Object::~Object() throw() {
}

/*
void* operator new(size_t size) throw(MemoryException) {
  void* p = malloc(size);
  if (!p) {
    throw MemoryException();
  }
  return p;
}

void operator delete(void* p) {
  free(p);
}
*/
