/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATHEMATICS__MARSAGLIA_RANDOM_NUMBER_GENERATOR_H
#define _DK_SDU_MIP__BASE_MATHEMATICS__MARSAGLIA_RANDOM_NUMBER_GENERATOR_H

#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Implementation of the pseudorandom number generator by George Marsaglia: The
  mother of all random number generators. The RNG has a period of about 2^250.
  
  @short Marsaglia random number generator.
  @ingroup mathematics
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class MarsagliaRandomNumberGenerator {
private:

  /** State size. */
  static const unsigned int STATE_SIZE = 8;
  /** Internal state. */
  uint16 x1[STATE_SIZE];
  /** Carry. */
  uint16 carry1;
  /** Internal state. */
  uint16 x2[STATE_SIZE];
  /** Carry. */
  uint16 carry2;
public:

  /**
    Initializes the internal state of the generator from the current time.
  */
  MarsagliaRandomNumberGenerator() throw();

  /**
    Initializes the internal state of the generator.

    @param seed The desired seed.
  */
  MarsagliaRandomNumberGenerator(uint32 seed) throw();
  
  /**
    Initializes the internal state of the generator from the current time.
  */
  void randomize() throw();

  /**
    Initializes the internal state of the generator.

    @param seed The desired seed.
  */
  void randomize(uint32 seed) throw();

  /**
    Returns a random integer (unsigned) utilizing the full range of the type.
  */
  uint32 getInteger() throw();

  /**
    Returns a random real number in the range [0;1].
  */
  inline double getDouble() throw() {
    return (1.0/0xffffffff) * getInteger();
  }
  
  /**
    Returns a random real number in the range [0;1].
  */
  inline long double getLongDouble() throw() {
    return (1.0L/0xffffffff) * getInteger();
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif