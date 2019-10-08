/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/BidirectionalIterator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short Random iterator interface.
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
