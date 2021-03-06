/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/math/Random.h>
#include <base/Timer.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace RandomImpl {

  template<>
  class Traits<RandomLegacy::MT11213A> {
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
  class Traits<RandomLegacy::MT11213B> {
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
  class Traits<RandomLegacy::MT19937> {
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
  class Traits<RandomLegacy::TT800> {
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

//  class Randomizer {
//  public:
//    inline Randomizer() noexcept {RandomLegacy::randomize();}
//  };

}; // end of namespace RandomImpl

unsigned int RandomLegacy::state[RandomLegacy::Traits::n];
unsigned int RandomLegacy::nextWord; // initialized by randomize()

void RandomLegacy::randomize() noexcept {
  long long seed = Timer().getStartTime();
  seed += 20010908014640LL; // magic date/time in UTC - 09/08/2001 01:46:40
  seed &= 0xffffffff;
  if (seed == 0) {
    seed = 20010908014640LL;
  }
  randomize(static_cast<unsigned int>(seed));
}

void RandomLegacy::randomize(unsigned int seed) noexcept
{
  BASSERT((Traits::w == 32) && (sizeof(unsigned int) == 4));
  RandomLegacy::spinLock.exclusiveLock();
    for (unsigned int i = 0; i < Traits::n; ++i) {
      // assert: (seed != 0) && (seed < (1 << 32))
      state[i] = seed;
      seed *= 69069;
    }
    nextWord = 0;
  RandomLegacy::spinLock.releaseLock();
}

unsigned int RandomLegacy::getInteger() noexcept
{
  BASSERT((Traits::w == 32) && (sizeof(unsigned int) == 4));
  unsigned int bits = 0;
  RandomLegacy::spinLock.exclusiveLock();
    unsigned int i = (nextWord + Traits::m < Traits::n) ? (nextWord + Traits::m) : (nextWord + Traits::m - Traits::n);
    unsigned int y = (state[i] & ~((1 << Traits::r) - 1)) | (state[i + 1] & ((1 << Traits::r) - 1));
    bits = state[nextWord] = state[i] ^ (y >> 1) ^ ((y & 0x1) ? Traits::a : 0);
    nextWord = ++nextWord % Traits::n;
  RandomLegacy::spinLock.releaseLock();

  if (Traits::u != 0) {
    bits ^= (bits >> Traits::u);
  }
  bits ^= (bits << Traits::s) & Traits::b;
  bits ^= (bits << Traits::t) & Traits::c;
  bits ^= (bits >> Traits::l);
  return bits;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
