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

#include <base/concurrency/SpinLock.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace RandomImpl {

  template<unsigned int ID>
  class Traits {
  };

};

/**
  Implementation of a pseudorandom number generator. The generator in described
  in the paper "Mersenne Twister: A 623-dimensionally equidistributed uniform
  pseudorandom number generator" by Makoto Matsumoto and Takuji Nishimura. The
  implementation is MT-safe.

  @short Random number generator
  @ingroup mathematics
  @version 1.0
*/

// TAG: rename
class _COM_AZURE_DEV__BASE__API RandomLegacy {
  friend class Initialization;
public:

  /** The supported generators. */
  enum Generator {MT11213A = 0, MT11213B = 1, MT19937 = 2, TT800 = 3};
  /** Specifies the generator to be used. */
  typedef RandomImpl::Traits<TT800> Traits;
private:

  /** The internal state of the random number generator. */
  static unsigned int state[];
  /** The internal state of the random number generator. */
  static unsigned int nextWord;
  /** Synchronization object. */
  static SpinLock spinLock;
public:

  /**
    Initializes the internal state of the generator from the current time.
  */
  static void randomize() throw();

  /**
    Initializes the internal state of the generator.

    @param seed The desired seed (generator does not work with zero).
  */
  static void randomize(unsigned int seed) throw();

  /**
    Returns a random integer (unsigned) utilizing the full range of the type.
  */
  static unsigned int getInteger() throw();

  /**
    Returns a random real number in the range [0;1].
  */
  static inline float getFloat() throw() {
    return (1.0f/0xffffffff) * getInteger();
  }

  /**
    Returns a random real number in the range [0;1].
  */
  static inline double getDouble() throw() {
    return (1.0/0xffffffff) * getInteger();
  }

  /**
    Returns a random real number in the range [0;1].
  */
  static inline long double getLongDouble() throw() {
    return (1.0L/0xffffffff) * getInteger();
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
