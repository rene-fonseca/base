/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/List.h>

template List<void*>;

template<class TYPE>
List<TYPE>::List() throw() : first(0), last(0) {
}

template<class TYPE>
List<TYPE>::List(const List& copy) throw(MemoryException) : first(0), last(0) {
//  append(copy.getEnumeration());
}

template<class TYPE>
int List<TYPE>::indexOf(const ListNode& node) const throw() {
  ListNode* current = first;
  unsigned int index = 0;

  while (current) {
    if (current == &node) {
      return index;
    }
    current->getNext();
    ++index;
  }
  return -1; // not found
}

template<class TYPE>
List<TYPE>::ListEnumeration List<TYPE>::getEnumeration() const throw() {
  return ListEnumeration(first);
}

template<class TYPE>
void List<TYPE>::append(const TYPE& value) throw() {
  ListNode* node = new ListNode(0, last, value);
  last->setNext(node);
  last = node;
  ++size;
}

template<class TYPE>
void List<TYPE>::append(Enumeration<TYPE>& enu) throw() {
  while (enu.hasNext()) {
    append(*enu.next());
  }
}

template<class TYPE>
void List<TYPE>::prepend(const TYPE& value) throw() {
  ListNode* node = new ListNode(first, 0, value);
  first->setPrevious(node);
  first = node;
  ++size;
}

template<class TYPE>
void List<TYPE>::prepend(Enumeration<TYPE>& enu) throw() {
  ListNode* position = first;
  while (enu.hasNext()) {
    insert(*position, *enu.next());
  }
}

template<class TYPE>
void List<TYPE>::add(const TYPE& value) throw() {
  ListNode* node = new ListNode(0, last, value);
  last->setNext(node);
  last = node;
  ++size;
}

template<class TYPE>
void List<TYPE>::add(Enumeration<TYPE>& enu) throw() {
  while (enu.hasNext()) {
    add(*enu.next());
  }
}

template<class TYPE>
void List<TYPE>::insert(ListNode& pos, const TYPE& value) throw() {
  // is pos in this list
  // cannot be the last
  ListNode* newNode = new ListNode(&pos, pos.getPrevious(), value);
  pos.setPrevious(newNode);
  pos.getPrevious()->setNext(newNode);
  ++size;
}

template<class TYPE>
void List<TYPE>::remove(ListNode* node) throw() {
  // is node in this list.
  // if null
  // if first
  // if last
  // else
  node->getPrevious()->setNext(node->getNext());
  delete node;
}

template<class TYPE>
void List<TYPE>::removeAll() throw() {
}

template<class TYPE>
List<TYPE>::~List() {
  removeAll();
}
