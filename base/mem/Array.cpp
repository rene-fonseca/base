/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mem/Array.h>

/*
template<class TYPE>
FormatOutputStream& toFormatStream(FormatOutputStream& stream, const TYPE* value, unsigned int size) {
  stream << '[';

  if (size > 0) {
    unsigned int count = size - 1;
    while (count) {
      stream << *value << ',';
      ++value;
      --count;
    }
    stream << *value;
  }

  stream << ']';
  return stream;
}
*/
