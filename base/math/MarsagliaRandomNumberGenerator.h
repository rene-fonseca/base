/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Primitives.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Implementation of the pseudorandom number generator by George Marsaglia: The
  mother of all random number generators. The RNG has a period of about 2^250.
  
  @short Marsaglia random number generator.
  @ingroup math
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API MarsagliaRandomNumberGenerator {
private:

  /** State size. */
  static const unsigned int STATE_SIZE = 8;
  /** Internal state. */
  uint16 x1[STATE_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};
  /** Carry. */
  uint16 carry1 = 0;
  /** Internal state. */
  uint16 x2[STATE_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};
  /** Carry. */
  uint16 carry2 = 0;
public:

  /**
    Initializes the internal state of the generator from the current time.
  */
  MarsagliaRandomNumberGenerator() noexcept;

  /**
    Initializes the internal state of the generator.

    @param seed The desired seed.
  */
  MarsagliaRandomNumberGenerator(uint32 seed) noexcept;
  
  /**
    Initializes the internal state of the generator from the current time.
  */
  void randomize() noexcept;

  /**
    Initializes the internal state of the generator.

    @param seed The desired seed.
  */
  void randomize(uint32 seed) noexcept;

  /**
    Returns a random integer (unsigned) utilizing the full range of the type.
  */
  uint32 getInteger() noexcept;

  /**
    Returns a random real number in the range [0;1].
  */
  inline double getDouble() noexcept
  {
    return (1.0/0xffffffff) * getInteger();
  }
  
  /**
    Returns a random real number in the range [0;1].
  */
  inline long double getLongDouble() noexcept
  {
    return (1.0L/0xffffffff) * getInteger();
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
