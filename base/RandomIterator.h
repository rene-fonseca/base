/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__RANDOMITERATOR_H
#define _DK_SDU_MIP__BASE__RANDOMITERATOR_H

#include <base/BidirectionalIterator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Random iterator interface.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TRAITS = IteratorTraits<TYPE> >
class RandomIterator : public Iterator<TYPE, TRAITS> {
public:
};

template<class TYPE>
class ReadRandomIterator : public RandomIterator<TYPE, ReadIteratorTraits<TYPE> > {
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
