/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mathematics/Random.h>
#include <base/concurrency/SpinLock.h>
#include <base/Timer.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace RandomImpl {

  template<>
  class Traits<Random::MT11213A> {
  public:

    static const unsigned int w = 32;
    static const unsigned int n = 351;
    static const unsigned int m = 175;
    static const unsigned int r = 19;
    static const unsigned int a = 0xe4bd75f5;
    static const unsigned int u = 11;
    static const unsigned int s = 7;
    static const unsigned int b = 0x655e5280;
    static const unsigned int t = 15;
    static const unsigned int c = 0xffd58000;
    static const unsigned int l = 17;
  };

  template<>
  class Traits<Random::MT11213B> {
  public:

    static const unsigned int w = 32;
    static const unsigned int n = 351;
    static const unsigned int m = 175;
    static const unsigned int r = 19;
    static const unsigned int a = 0xccab8ee7;
    static const unsigned int u = 11;
    static const unsigned int s = 7;
    static const unsigned int b = 0x31b6ab00;
    static const unsigned int t = 15;
    static const unsigned int c = 0xffe50000;
    static const unsigned int l = 17;
  };

  template<>
  class Traits<Random::MT19937> {
  public:

    static const unsigned int w = 32;
    static const unsigned int n = 624;
    static const unsigned int m = 397;
    static const unsigned int r = 31;
    static const unsigned int a = 0x9908b0df;
    static const unsigned int u = 11;
    static const unsigned int s = 7;
    static const unsigned int b = 0x9d2c5680;
    static const unsigned int t = 15;
    static const unsigned int c = 0xefc60000;
    static const unsigned int l = 18;
  };

  template<>
  class Traits<Random::TT800> {
  public:

    static const unsigned int w = 32;
    static const unsigned int n = 25;
    static const unsigned int m = 7;
    static const unsigned int r = 0;
    static const unsigned int a = 0x8ebfd028;
    static const unsigned int u = 0; // zero => disabled
    static const unsigned int s = 7;
    static const unsigned int b = 0x2b5b2500;
    static const unsigned int t = 15;
    static const unsigned int c = 0xdb8b0000;
    static const unsigned int l = 16;
  };

  class Randomizer {
  public:
    inline Randomizer() throw() {Random::randomize();}
  };

  // TAG: fixme
  SpinLock lock; // before randomization!
  Randomizer randomizer; // randomize during application initialization

}; // end of namespace RandomImpl

unsigned int Random::state[Random::Traits::n];
unsigned int Random::nextWord; // initialized by randomize()

void Random::randomize() throw() {
  Timer timer;
  long long seed = timer.getStartTime();
  seed += 20010908014640LL; // magic date/time in UTC - 09/08/2001 01:46:40
  seed &= 0xffffffff;
  if (seed == 0) {
    seed = 20010908014640LL;
  }
  randomize(seed);
}

void Random::randomize(unsigned int seed) throw() {
  ASSERT((Traits::w == 32) && (sizeof(unsigned int) == 4));
  RandomImpl::lock.exclusiveLock();
    for (unsigned int i = 0; i < Traits::n; ++i) {
      // assert: (seed != 0) && (seed < (1 << 32))
      state[i] = seed;
      seed *= 69069;
    }
    nextWord = 0;
  RandomImpl::lock.releaseLock();
}

unsigned int Random::getInteger() throw() {
  ASSERT((Traits::w == 32) && (sizeof(unsigned int) == 4));
  unsigned int bits;
  RandomImpl::lock.exclusiveLock();
    unsigned int i = (nextWord + Traits::m < Traits::n) ? (nextWord + Traits::m) : (nextWord + Traits::m - Traits::n);
    unsigned int y = state[i] & ~((1 << Traits::r) - 1) | state[i + 1] & ((1 << Traits::r) - 1);
    bits = state[nextWord] = state[i] ^ (y >> 1) ^ ((y & 0x1) ? Traits::a : 0);
    nextWord = ++nextWord % Traits::n;
  RandomImpl::lock.releaseLock();

  if (Traits::u != 0) {
    bits ^= (bits >> Traits::u);
  }
  bits ^= (bits << Traits::s) & Traits::b;
  bits ^= (bits << Traits::t) & Traits::c;
  bits ^= (bits >> Traits::l);
  return bits;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
