/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/Type.h>
#include <base/string/String.h>
#include <base/concurrency/Thread.h>
#include <base/string/ASCIITraits.h>
#include <base/collection/Array.h>
#include <base/string/StringOutputStream.h>
#include <stdlib.h>

#if defined(_DK_SDU_MIP__BASE__DEMANGLE_GCCV3)
  extern "C" char* cplus_demangle_v3(const char* mangled);
#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_GCCV23)
  extern "C" char* cplus_demangle_new_abi(const char* mangled);
#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_GCCV2)
  extern "C" char* cplus_demangle(const char* mangled, int options);
#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_SUNWSPRO)
  #include <demangle.h>
#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_MIPSPRO)
  #include <dem.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class ManglingException : public Exception {
public:
};

#if defined(_DK_SDU_MIP__BASE__DEMANGLE_V3MV)

class V3MultiVendorABIDemangler {
private:

  struct Substitution {
    /** The index of the first char */
    unsigned int begin;
    /** The index of the end char */
    unsigned int end;
  };

  const char* p;
  const char* end;
  bool templateArgumentsAvailable;
  Substitution className;
  Array<Substitution> substitutions;
  String demangled;
public:

  inline V3MultiVendorABIDemangler(const char* mangled) throw() : p(mangled) {
    templateArgumentsAvailable = false;
    end = find<char>(mangled, 1024, 0); // find terminator
    assert(end, ManglingException());
    encoding();
  }

  inline void addSubstitutionCandidate(unsigned int begin) throw() {
    Substitution substitution;
    substitution.begin = begin;
    substitution.end = demangled.getLength();
    unsigned int length = substitution.end - substitution.begin;
    for (int s = substitutions.getSize() - 1; s >= 0; --s) { // for all candidates do
      if ((static_cast<Substitution>(substitutions[s]).end - static_cast<Substitution>(substitutions[s]).begin) == length) {
        if (compare<char>(demangled.getElements() + static_cast<Substitution>(substitutions[s]).begin, demangled.getElements() + substitution.begin, length) == 0) {
          return; // candidate already in dictionary
        }
      }
    }
    substitutions.append(substitution); // add new candidate
  }

  inline void dump(unsigned int substitution) throw(ManglingException) {
    assert(substitution < substitutions.getSize(), ManglingException());
    demangled.append(demangled.substring(static_cast<Substitution>(substitutions[substitution]).begin, static_cast<Substitution>(substitutions[substitution]).end));
  }

  inline void dump(const StringLiteral& literal) throw() {
    demangled.append(literal);
  }

  inline void encoding() throw() {
    assert(name(), ManglingException());
    if (p < end) {
      // remove substitution for function (if present)
      if (substitutions.getSize() > 0) {
      if (static_cast<Substitution>(substitutions[substitutions.getSize() - 1]).end = demangled.getLength()) {
        substitutions.remove(substitutions.getSize() - 1);
      }}
      if (templateArgumentsAvailable) { // handle return type // TAG: are there other cases
        String temp = demangled;
        demangled = String();
        assert(type(), ManglingException()); // FIXME: does not work with substitutions
        dump(MESSAGE(" "));
        for (unsigned int s = 0; s < substitutions.getSize(); ++s) { // remap substitutions
          Substitution substitution(substitutions[s]);
          substitution.begin += demangled.getLength();
          substitution.end += demangled.getLength();
          substitutions[s] = substitution;
        }
        demangled.append(temp);
      }
      dump(MESSAGE("("));
      if (*p == 'v') { // special case
        ++p;
        assert(p == end, ManglingException());
      } else {
        assert(type(), ManglingException());
        while (p < end) {
          dump(MESSAGE(", "));
          assert(type(), ManglingException());
        }
      }
      dump(MESSAGE(")"));

/*    for (unsigned int i = 0; i < substitutions.getSize(); ++i) {
        dump(MESSAGE("#S"));
        dump(i);
      }*/
    }
  }

  inline void CVQualifier() throw() {
    switch (*p) { // CV-qualifiers
    case 'r':
      ++p;
      dump(MESSAGE("restrict "));
      break;
    case 'V':
      ++p;
      dump(MESSAGE("volatile "));
      break;
    case 'K':
      ++p;
      dump(MESSAGE("const "));
      break;
    }
  }

  inline bool templateArgument() throw() {
    switch (*p) {
    case 'L': // literal
      ++p; // skip L
      assert(type(), ManglingException());
      if (*p == 'n') {
        demangled += '-'; // negative number
      }
      // encoding depends on type
      ++p; // skip E
      break;
    case 'X':
      ++p; // skip X
      // expression();
      ++p; // skip E
      break;
    default:
      assert(type(), ManglingException());
      return true;
    }
    return false;
  }

  inline bool templateArguments() throw() {
    if (*p == 'I') {
      ++p; // skip I
      demangled += '<';
      assert(templateArgument(), ManglingException());
      while (*p != 'E') {
        dump(MESSAGE(", "));
        assert(templateArgument(), ManglingException());
      }
      if (demangled.endsWith(MESSAGE(">"))) {
        dump(MESSAGE(" >"));
      } else {
        demangled += '>';
      }
      ++p; // skip E
      templateArgumentsAvailable = true; // last thing to do
      return true;
    }
    return false;
  }

  inline bool name() throw() {
    templateArgumentsAvailable = false;
    unsigned int begin = demangled.getLength();
    if (nested()) {
      // FIXME: insert candidate?
      return true;
    } else if (unscopedName()) {
      className.begin = begin;
      className.end = demangled.getLength();
      templateArguments(); // not required
addSubstitutionCandidate(begin);
      // FIXME: insert candidate?
      return true;
    } else if (substitution()) {
      className.begin = begin;
      className.end = demangled.getLength();
      assert(templateArguments(), ManglingException());
addSubstitutionCandidate(begin);
      // FIXME: insert candidate?
      return true;
    } else {
      return false;
    }
  }

  inline bool unscopedName() throw() {
    if ((*p == 'S') && (p[1] == 't')) {
      p += 2; // skip St
      dump(MESSAGE("std::"));
      assert(unqualifiedName(), ManglingException());
      // FIXME: insert candidate?
      return true;
    } else if (unqualifiedName()) {
      return true;
    } else {
      return false;
    }
  }

  inline bool nested() throw() {
    if (*p == 'N') {
      ++p; // skip N
      CVQualifier(); // not required

      unsigned int begin = demangled.getLength(); // substitution

      if (substitution()) { // not required
        dump(MESSAGE("::"));
      }

      while (true) { // prefix+
        unsigned int beginClassName = demangled.getLength();
        if (sourceName()) {
          className.begin = beginClassName;
          className.end = demangled.getLength();
          addSubstitutionCandidate(begin);
        } else if (operatorName()) {
        } else if (constructorAndDestructorName()) {
        } else {
          throw ManglingException();
        }
        if (templateArguments()) {
          addSubstitutionCandidate(begin);
        }
        if (*p == 'E') {
          break;
        }
        demangled.append(MESSAGE("::")); // delimiter
      }
      ++p; // skip E
      return true;
    }
    return false;
  }

  inline bool constructorAndDestructorName() throw() {
    if ((*p == 'C') && (p[1] == '1')) {
      p += 2; // skip C1
      demangled.append(demangled.substring(className.begin, className.end));
      // TAG: does this work
    } else if ((*p == 'C') && (p[1] == '2')) {
      p += 2; // skip C2
      demangled.append(demangled.substring(className.begin, className.end));
      // TAG: does this work
    } else if ((*p == 'C') && (p[1] == '3')) {
      p += 2; // skip C3
      demangled.append(demangled.substring(className.begin, className.end));
      // TAG: does this work
    } else if ((*p == 'D') && (p[1] == '0')) {
      p += 2; // skip D0
      dump(MESSAGE("~"));
      demangled.append(demangled.substring(className.begin, className.end));
      // TAG: does this work
    } else if ((*p == 'D') && (p[1] == '1')) {
      p += 2; // skip D1
      dump(MESSAGE("~"));
      demangled.append(demangled.substring(className.begin, className.end));
      // TAG: does this work
    } else if ((*p == 'D') && (p[1] == '2')) {
      p += 2; // skip D2
      dump(MESSAGE("~"));
      demangled.append(demangled.substring(className.begin, className.end));
      // TAG: does this work
    } else {
      return false;
    }
    templateArgumentsAvailable = false;
    return true;
  }

  inline bool unqualifiedName() throw() {
    unsigned int begin = demangled.getLength();
    if (operatorName()) {
    } else if (sourceName()) {
      addSubstitutionCandidate(begin);
    } else if (constructorAndDestructorName()) {
    } else {
      return false;
    }
    templateArgumentsAvailable = false;
    return true;
  }

  inline bool type() throw() {
    unsigned int begin = demangled.getLength();
    switch (*p) {
    case 'r':
      ++p;
      assert(type(), ManglingException());
      dump(MESSAGE(" restrict"));
      break;
    case 'V':
      ++p;
      assert(type(), ManglingException());
      dump(MESSAGE(" volatile"));
      break;
    case 'K':
      ++p;
      assert(type(), ManglingException());
      dump(MESSAGE(" const"));
      break;
    case 'P':
      ++p;
      assert(type(), ManglingException());
      dump(MESSAGE("*"));
      break;
    case 'R':
      ++p;
      assert(type(), ManglingException());
      dump(MESSAGE("&"));
      break;
    case 'C':
      ++p;
      assert(type(), ManglingException());
      dump(MESSAGE("complex "));
      break;
    case 'G':
      ++p;
      assert(type(), ManglingException());
      dump(MESSAGE("imaginary "));
      break;
    case 'U':
      ++p;
      assert(type(), ManglingException());
      return sourceName();
      break;
    case 'A': // array type
      ++p; // skip A
      if (ASCIITraits::isDigit(*p)) {
        unsigned int dimension = 0;
        while (ASCIITraits::isDigit(*p)) {
          dimension = dimension * 10 + ASCIITraits::digitToValue(*p++);
        }
        assert(*p++ == '_', ManglingException());
        assert(type(), ManglingException());
        dump(MESSAGE("["));
        StringOutputStream stream; // insert dimension
        stream << dimension << FLUSH;
        demangled.append(stream.getString());
        dump(MESSAGE("]"));
      } else { // variable length array
        assert(*p++ == '_', ManglingException());
        assert(type(), ManglingException());
        dump(MESSAGE("[]"));
      }
// FIXME: need support for - A [<dimension expression>] _ <element type>
      break;
    case 'F': // function type
      ++p; // skip F
      if (*p == 'Y') {
        ++p; // skip Y
        dump(MESSAGE("extern \"C\" "));
      }
      assert(type(), ManglingException()); // return type
      dump(MESSAGE(" (*)("));
      assert(type(), ManglingException());
      while (*p != 'E') {
        dump(MESSAGE(", "));
        assert(type(), ManglingException());
      }
      dump(MESSAGE(")"));
      ++p; // skip E
      break;
    case 'M': // pointer to member type
      {
        ++p; // skip M
        String temp(demangled);
        demangled = String();
        assert(name(), ManglingException()); // FIXME: does not work with substitution
        String ownerName(demangled);
        demangled = temp;
        if (*p == 'F') { // special case
          ++p; // skip F
          if (*p == 'Y') {
            ++p; // skip Y
            dump(MESSAGE("extern \"C\" "));
          }
          assert(type(), ManglingException()); // return type
          dump(MESSAGE(" ("));
          demangled.append(ownerName);
          dump(MESSAGE("::*)("));
          assert(type(), ManglingException());
          while (*p != 'E') {
            dump(MESSAGE(", "));
            assert(type(), ManglingException());
          }
          dump(MESSAGE(")"));
          ++p; // skip E
        } else {
          assert(type(), ManglingException());
          dump(MESSAGE(" "));
          demangled.append(ownerName);
          dump(MESSAGE("::*"));
        }
      }
      break;

    // builtin types
    case 'v':
      ++p;
      dump(MESSAGE("void"));
      return true; // no substitution for builtin type
    case 'w':
      ++p;
      dump(MESSAGE("wchar_t"));
      return true; // no substitution for builtin type
    case 'b':
      ++p;
      dump(MESSAGE("bool"));
      return true; // no substitution for builtin type
    case 'c':
      ++p;
      dump(MESSAGE("char"));
      return true; // no substitution for builtin type
    case 'a':
      ++p;
      dump(MESSAGE("signed char"));
      return true; // no substitution for builtin type
    case 'h':
      ++p;
      dump(MESSAGE("unsigned char"));
      return true; // no substitution for builtin type
    case 's':
      ++p;
      dump(MESSAGE("short"));
      return true; // no substitution for builtin type
    case 't':
      ++p;
      dump(MESSAGE("unsigned short"));
      return true; // no substitution for builtin type
    case 'i':
      ++p;
      dump(MESSAGE("int"));
      return true; // no substitution for builtin type
    case 'j':
      ++p;
      dump(MESSAGE("unsigned int"));
      return true; // no substitution for builtin type
    case 'l':
      ++p;
      dump(MESSAGE("long"));
      return true; // no substitution for builtin type
    case 'm':
      ++p;
      dump(MESSAGE("unsigned long"));
      return true; // no substitution for builtin type
    case 'x':
      ++p;
      dump(MESSAGE("long long"));
      return true; // no substitution for builtin type
    case 'y':
      ++p;
      dump(MESSAGE("unsigned long long"));
      return true; // no substitution for builtin type
    case 'n':
      ++p;
      dump(MESSAGE("__int128"));
      return true; // no substitution for builtin type
    case 'o':
      ++p;
      dump(MESSAGE("unsigned __int128"));
      return true; // no substitution for builtin type
    case 'f':
      ++p;
      dump(MESSAGE("float"));
      return true; // no substitution for builtin type
    case 'd':
      ++p;
      dump(MESSAGE("double"));
      return true; // no substitution for builtin type
    case 'e':
      ++p;
      dump(MESSAGE("long double"));
      return true; // no substitution for builtin type
    case 'g':
      ++p;
      dump(MESSAGE("__float128"));
      return true; // no substitution for builtin type
    case 'z':
      ++p;
      dump(MESSAGE("ellipsis")); // ???
      return true; // no substitution for builtin type
    case 'u': // vendor specific
      ++p;
      assert(sourceName(), ManglingException());
      break; // substitute
    default:
      if (nested()) {
      } else if (unscopedName()) {
        templateArguments(); // not required
      } else if (substitution()) {
        templateArguments(); // name rule - not required
      } else {
        return false;
      }
    }
    addSubstitutionCandidate(begin);
    return true;
  }

  inline bool operatorName() throw() {
    if ((*p == 'n') && (p[1] == 'w')) {
      p += 2;
      dump(MESSAGE("new"));
    } else if ((*p == 'n') && (p[1] == 'a')) {
      p += 2;
      dump(MESSAGE("new[]"));
    } else if ((*p == 'd') && (p[1] == 'l')) {
      p += 2;
      dump(MESSAGE("delete"));
    } else if ((*p == 'd') && (p[1] == 'a')) {
      p += 2;
      dump(MESSAGE("delate[]"));
    } else if ((*p == 'n') && (p[1] == 'g')) {
      p += 2;
      dump(MESSAGE("operator-")); // unary
    } else if ((*p == 'a') && (p[1] == 'd')) {
      p += 2;
      dump(MESSAGE("operator&")); // unary
    } else if ((*p == 'd') && (p[1] == 'e')) {
      p += 2;
      dump(MESSAGE("operator*")); // unary
    } else if ((*p == 'c') && (p[1] == 'o')) {
      p += 2;
      dump(MESSAGE("operator~"));
    } else if ((*p == 'p') && (p[1] == 'l')) {
      p += 2;
      dump(MESSAGE("operator+"));
    } else if ((*p == 'm') && (p[1] == 'i')) {
      p += 2;
      dump(MESSAGE("operator-"));
    } else if ((*p == 'm') && (p[1] == 'l')) {
      p += 2;
      dump(MESSAGE("operator*"));
    } else if ((*p == 'd') && (p[1] == 'v')) {
      p += 2;
      dump(MESSAGE("operator/"));
    } else if ((*p == 'r') && (p[1] == 'm')) {
      p += 2;
      dump(MESSAGE("operator%"));
    } else if ((*p == 'a') && (p[1] == 'n')) {
      p += 2;
      dump(MESSAGE("operator&"));
    } else if ((*p == 'o') && (p[1] == 'r')) {
      p += 2;
      dump(MESSAGE("operator|"));
    } else if ((*p == 'e') && (p[1] == 'o')) {
      p += 2;
      dump(MESSAGE("operator^"));
    } else if ((*p == 'a') && (p[1] == 'S')) {
      p += 2;
      dump(MESSAGE("operator="));
    } else if ((*p == 'p') && (p[1] == 'L')) {
      p += 2;
      dump(MESSAGE("operator+="));
    } else if ((*p == 'm') && (p[1] == 'I')) {
      p += 2;
      dump(MESSAGE("operator-="));
    } else if ((*p == 'm') && (p[1] == 'L')) {
      p += 2;
      dump(MESSAGE("operator*="));
    } else if ((*p == 'd') && (p[1] == 'V')) {
      p += 2;
      dump(MESSAGE("operator/="));
    } else if ((*p == 'r') && (p[1] == 'M')) {
      p += 2;
      dump(MESSAGE("operator%="));
    } else if ((*p == 'a') && (p[1] == 'N')) {
      p += 2;
      dump(MESSAGE("operator&="));
    } else if ((*p == 'o') && (p[1] == 'R')) {
      p += 2;
      dump(MESSAGE("operator|="));
    } else if ((*p == 'e') && (p[1] == 'O')) {
      p += 2;
      dump(MESSAGE("operator^="));
    } else if ((*p == 'l') && (p[1] == 's')) {
      p += 2;
      dump(MESSAGE("operator<<"));
    } else if ((*p == 'r') && (p[1] == 's')) {
      p += 2;
      dump(MESSAGE("operator>>"));
    } else if ((*p == 'l') && (p[1] == 'S')) {
      p += 2;
      dump(MESSAGE("operator<<="));
    } else if ((*p == 'r') && (p[1] == 'S')) {
      p += 2;
      dump(MESSAGE("operator>>="));
    } else if ((*p == 'e') && (p[1] == 'q')) {
      p += 2;
      dump(MESSAGE("operator=="));
    } else if ((*p == 'n') && (p[1] == 'e')) {
      p += 2;
      dump(MESSAGE("operator!="));
    } else if ((*p == 'l') && (p[1] == 't')) {
      p += 2;
      dump(MESSAGE("operator<"));
    } else if ((*p == 'g') && (p[1] == 't')) {
      p += 2;
      dump(MESSAGE("operator>"));
    } else if ((*p == 'l') && (p[1] == 'e')) {
      p += 2;
      dump(MESSAGE("operator<="));
    } else if ((*p == 'g') && (p[1] == 'e')) {
      p += 2;
      dump(MESSAGE("operator>="));
    } else if ((*p == 'n') && (p[1] == 't')) {
      p += 2;
      dump(MESSAGE("operator!"));
    } else if ((*p == 'a') && (p[1] == 'a')) {
      p += 2;
      dump(MESSAGE("operator&&"));
    } else if ((*p == 'o') && (p[1] == 'o')) {
      p += 2;
      dump(MESSAGE("operator||"));
    } else if ((*p == 'p') && (p[1] == 'p')) {
      p += 2;
      dump(MESSAGE("operator++"));
    } else if ((*p == 'm') && (p[1] == 'm')) {
      p += 2;
      dump(MESSAGE("operator--"));
    } else if ((*p == 'c') && (p[1] == 'm')) {
      p += 2;
      dump(MESSAGE("operator,"));
    } else if ((*p == 'p') && (p[1] == 'm')) {
      p += 2;
      dump(MESSAGE("operator->*"));
    } else if ((*p == 'p') && (p[1] == 't')) {
      p += 2;
      dump(MESSAGE("operator->"));
    } else if ((*p == 'c') && (p[1] == 'l')) {
      p += 2;
      dump(MESSAGE("operator()"));
    } else if ((*p == 'i') && (p[1] == 'x')) {
      p += 2;
      dump(MESSAGE("operator[]"));
    } else if ((*p == 'q') && (p[1] == 'u')) {
      p += 2;
      dump(MESSAGE("operator?"));
    } else if ((*p == 's') && (p[1] == 'z')) {
      p += 2;
      dump(MESSAGE("sizeof"));
    } else if ((*p == 's') && (p[1] == 'r')) {
/*
  ::= sr	# scope resolution (::), see below
*/
      dump(MESSAGE("")); // ???
    } else if ((*p == 'c') && (p[1] == 'v')) { // cast
      p += 2; // skip cv
      dump(MESSAGE("operator "));
      type();
      // output '()'
    } else if (*p == 'v') { // vendor extended operator
      ++p; // skip v
      ++p; // skip operand count (single digit)
      sourceName();
    } else {
      return false;
    }
    return true;
  }

  inline bool sourceName() throw() {
    if (!ASCIITraits::isDigit(*p)) {
      return false;
    }
    unsigned int length = 0; // unqualified name
    while (ASCIITraits::isDigit(*p)) {
      length = length * 10 + ASCIITraits::digitToValue(*p++);
    }
    demangled.append(p, length);
    p += length;
    return true;
  }

  inline bool substitution() throw() {
    if (*p == 'S') {
      ++p; // skip S
      switch (*p) {
      case '_':
        ++p; // skip underscore
        dump(0);
        break;
      case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
      case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
      case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
      case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
        // base 36 encoding
        {
          unsigned int id = 0;
          while ((*p >= '0') && (*p <= '9') || (*p >= 'A') && (*p <= 'Z')) {
            if ((*p >= '0') && (*p <= '9')) {
              id += id * 36 + (*p - '0');
            } else {
              id += id * 36 + (*p - 'A');
            }
            ++p;
          }
          assert(*p++ == '_', ManglingException()); // skip underscore
          dump(id + 1);
        }
        break;
      case 't':
        ++p; // skip t
        dump(MESSAGE("std")); // need not be nested
        break;
      case 'a':
        ++p; // skip a
        dump(MESSAGE("std::allocator"));
        break;
      case 'b':
        ++p; // skip b
        dump(MESSAGE("std::basic_string"));
        break;
      case 's':
        ++p; // skip s
        dump(MESSAGE("std::basic_string<char, std::char_traits<char>, std::allocator<char> >"));
        break;
      case 'i':
        ++p; // skip i
        dump(MESSAGE("std::basic_istream<char, std::char_traits<char> >"));
        break;
      case 'o':
        ++p; // skip o
        dump(MESSAGE("std::basic_ostream<char, std::char_traits<char> >"));
        break;
      case 'd':
        ++p; // skip d
        dump(MESSAGE("std::basic_iostream<char, std::char_traits<char> >"));
        break;
      default:
        return false;
      }
      return true;
    }
    return false;
  }

  inline String getDemangled() const throw() {
    return demangled;
  }
};



String demangleTypename(const char* mangled) throw() {
  return V3MultiVendorABIDemangler(mangled).getDemangled();
}

#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_GCCV3)

String demangleTypename(const char* mangled) throw() {
  static const String prefix("_Z");
  String temp = prefix;
  temp.append(mangled);
  char* demangled = cplus_demangle_v3(temp.getElements());
  if (!demangled) { // failed?
    return String(mangled); // return mangled type name
  }
  String result(demangled);
  free(demangled);
  return result;
}

#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_GCCV23)

String demangleTypename(const char* mangled) throw() {
  static const String prefix("_Z");
  String temp = prefix;
  temp.append(mangled);
  char* demangled = cplus_demangle_new_abi(temp.getElements());
  if (!demangled) { // failed?
    return String(mangled); // return mangled type name
  }
  String result(demangled);
  free(demangled);
  return result;
}

#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_GCCV2)

String demangleTypename(const char* mangled) throw() {
  static const String prefix("a__");
  static const String suffix("::a");
  // cplus_demangle only demangles function names - need alternative demangler
  String temp = prefix; // make function name
  temp.append(mangled);
  char* demangled = cplus_demangle(temp.getElements(), 1<<1); // include const and volatile
  if (!demangled) { // failed?
    return String(mangled); // return mangled type name
  }
  String result(demangled);
  free(demangled);
  ASSERT(result.endsWith(suffix));
  result.setAt(result.getLength() - 3, 0); // remove function name
  return result;
}

#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_SUNWSPRO)

String demangleTypename(const char* mangled) throw() {
  Allocator<char>* buffer = Thread::getLocalStorage();
  int result = cplus_demangle(mangled, buffer->getElements(), buffer->getSize());
  ASSERT(!result);
  return String(buffer->getElements());
}

#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_MIPSPRO)

String demangleTypename(const char* mangled) throw() {
  char buffer[MAXDBUF];
  int result = demangle(mangled, buffer);
  ASSERT(!result);
  return String(buffer);
}

#else // no demangling support

String demangleTypename(const char* mangled) throw() {
  return mangled;
}

#endif

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
