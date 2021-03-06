/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications
    
    See COPYRIGHT.txt for details.
    
    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    
    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/TypeInfo.h>
#include <base/concurrency/Thread.h>
#include <base/string/ASCIITraits.h>
#include <base/collection/Array.h>
#include <base/string/StringOutputStream.h>
#include <base/build.h>
#include <stdlib.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#elif (_COM_AZURE_DEV__BASE__DEMANGLE == _COM_AZURE_DEV__BASE__DEMANGLE_GCCV3)
#  include <cxxabi.h>
#elif (_COM_AZURE_DEV__BASE__DEMANGLE == _COM_AZURE_DEV__BASE__DEMANGLE_GCCV23)
  extern "C" char* cplus_demangle_new_abi(const char* mangled);
#elif (_COM_AZURE_DEV__BASE__DEMANGLE == _COM_AZURE_DEV__BASE__DEMANGLE_GCCV2)
  extern "C" char* cplus_demangle(const char* mangled, int options);
#elif (_COM_AZURE_DEV__BASE__DEMANGLE == _COM_AZURE_DEV__BASE__DEMANGLE_SUNWSPRO)
  #include <demangle.h>
#elif (_COM_AZURE_DEV__BASE__DEMANGLE == _COM_AZURE_DEV__BASE__DEMANGLE_MIPSPRO)
  #include <dem.h>
#elif (_COM_AZURE_DEV__BASE__DEMANGLE == _COM_AZURE_DEV__BASE__DEMANGLE_LLVM)
#  include <cxxabi.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

_COM_AZURE_DEV__BASE__GLOBAL_PRINT();

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)

String TypeInfo::demangleName(const char* mangled)
{
  return mangled; // TAG: DynamicLinker::demangle(mangled);
}

#elif (_COM_AZURE_DEV__BASE__DEMANGLE == _COM_AZURE_DEV__BASE__DEMANGLE_V3MV)

class V3MultiVendorABIDemangler {
private:

  struct CandidateRange {
    unsigned int begin; // The index of the first char
    unsigned int end; // The index of the end char
  };
  
  /* The current position within the mangled string. */
  const char* p = nullptr;
  /* The end of the mangled string. */
  const char* end = nullptr;
  /* The current class name. */
  CandidateRange className;
  /* Candidates for normal substitution. */
  Array<CandidateRange> candidates;
  /* Candidates for template substitution. */
  Array<CandidateRange> templateCandidates;
  /* The demangled string. */
  String demangled;
public:

  inline V3MultiVendorABIDemangler(const char* mangled)
    : p(mangled)
  {
    end = find<char>(mangled, 1024, 0); // find terminator
    bassert(end, InvalidFormat(this));
    encoding();
  }

  inline void addCandidate(unsigned int begin)
  {
    CandidateRange range;
    range.begin = begin;
    range.end = demangled.getLength();
    unsigned int length = range.end - range.begin;
    for (int s = candidates.getSize() - 1; s >= 0; --s) { // for all candidates do
      if ((static_cast<CandidateRange>(candidates[s]).end - static_cast<CandidateRange>(candidates[s]).begin) == length) {
        if (compare<char>(demangled.getElements() + static_cast<CandidateRange>(candidates[s]).begin, demangled.getElements() + range.begin, length) == 0) {
          return; // candidate already in dictionary
        }
      }
    }
    candidates.append(range); // add new candidate
  }

  inline void addTemplateCandidate(unsigned int begin)
  {
    CandidateRange range;
    range.begin = begin;
    range.end = demangled.getLength();
    unsigned int length = range.end - range.begin;
    Array<CandidateRange>::ReadEnumerator enu = templateCandidates.getReadEnumerator();
    while (enu.hasNext()) {
      const CandidateRange& candidate = enu.next();
      if ((candidate.end - candidate.begin) == length) {
        if (compare<char>(demangled.getElements() + candidate.begin, demangled.getElements() + begin, length) == 0) {
          return; // candidate already in dictionary
        }
      }
    }
    templateCandidates.append(range); // add new candidate
  }


  inline void dump(const Literal& literal)
  {
    demangled.append(literal);
  }

  inline void encoding()
  {
    if (name()) {
      if (p < end) {
        // remove candidate for function (if present)
        if (candidates.getSize() > 0) {
          if (static_cast<CandidateRange>(candidates[candidates.getSize() - 1]).end == demangled.getLength()) {
            candidates.remove(candidates.getSize() - 1);
          }
        }
        if (demangled[demangled.getLength() - 1] == '>') { // handle return type // TAG: are there other cases
          unsigned int beginReturnType = demangled.getLength();
          bassert(type(), InvalidFormat(this));
          dump(MESSAGE(" "));
          String returnTypeString(demangled.substring(beginReturnType));
          demangled.removeFrom(beginReturnType);
          demangled.prepend(returnTypeString);

          Array<CandidateRange>::Enumerator enu =
            candidates.getEnumerator(); // remap candidates
          while (enu.hasNext()) {
            CandidateRange& range(enu.next());
            range.begin += returnTypeString.getLength();
            range.end += returnTypeString.getLength();
          }
          Array<CandidateRange>::Enumerator enuTemplateCandidate =
            templateCandidates.getEnumerator(); // remap candidates
          while (enuTemplateCandidate.hasNext()) {
            CandidateRange& range(enuTemplateCandidate.next());
            range.begin += returnTypeString.getLength();
            range.end += returnTypeString.getLength();
          }
        }
        dump(MESSAGE("("));
        if (*p == 'v') { // special case
          ++p;
          bassert(p == end, InvalidFormat(this));
        } else {
          bassert(type(), InvalidFormat(this));
          while (p < end) {
            dump(MESSAGE(", "));
            bassert(type(), InvalidFormat(this));
          }
        }
        dump(MESSAGE(")"));
      }
    } else if (specialName()) {
    } else if (type()) { // TAG: not validated
    } else {
      _throw InvalidFormat(this);
    }
  }

  inline void CVQualifier() noexcept
  {
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

  inline bool templateArgument()
  {
    switch (*p) {
    case 'L': // literal
      ++p; // skip L
      bassert(type(), InvalidFormat(this));
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
      unsigned int beginTemplateArgument = demangled.getLength();
      bassert(type(), InvalidFormat(this));
      addTemplateCandidate(beginTemplateArgument);
      return true;
    }
    return false;
  }

  inline bool templateArguments()
  {
    if (*p == 'I') {
      ++p; // skip I
      demangled += '<';
      bassert(templateArgument(), InvalidFormat(this));
      while (*p != 'E') {
        dump(MESSAGE(", "));
        bassert(templateArgument(), InvalidFormat(this));
      }
      if (demangled.endsWith(">")) {
        dump(MESSAGE(" >"));
      } else {
        demangled += '>';
      }
      ++p; // skip E
      return true;
    }
    return false;
  }

  inline bool name()
  {
    unsigned int begin = demangled.getLength();
    if (nested()) {
      // FIXME: insert candidate?
      return true;
    } else if (unscopedName()) {
      className.begin = begin;
      className.end = demangled.getLength();
      templateArguments(); // not required
      addCandidate(begin);
      return true;
    } else if (substitution()) {
      className.begin = begin;
      className.end = demangled.getLength();
      bassert(templateArguments(), InvalidFormat(this));
      addCandidate(begin);
      return true;
    } else {
      return false;
    }
  }

  inline bool unscopedName()
  {
    if ((*p == 'S') && (p[1] == 't')) {
      p += 2; // skip St
      dump(MESSAGE("std::"));
      bassert(unqualifiedName(), InvalidFormat(this));
      // FIXME: insert candidate?
      return true;
    } else if (unqualifiedName()) {
      return true;
    } else {
      return false;
    }
  }

  inline bool nested()
  {
    if (*p == 'N') {
      ++p; // skip N
      CVQualifier(); // not required

      unsigned int begin = demangled.getLength(); // beginning of substitution range

      bool delimiterPending = false;
      if (substitution()) { // not required
        delimiterPending = true;
      }

      while (true) { // prefix+
        unsigned int beginClassName = demangled.getLength();
        if (templateArguments()) { // do not add delimiter
          addCandidate(begin);
        } else {
          if (delimiterPending) {
            demangled.append(MESSAGE("::")); // delimiter
          }
          if (sourceName()) {
            className.begin = beginClassName;
            className.end = demangled.getLength();
            addCandidate(begin);
          } else if (operatorName()) {
          } else if (constructorAndDestructorName()) {
          } else {
            _throw InvalidFormat(this);
          }
        }
        delimiterPending = true;
        if (*p == 'E') {
          break;
        }
      }
      ++p; // skip E
      return true;
    }
    return false;
  }

  inline bool constructorAndDestructorName()
  {
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
    return true;
  }

  inline bool specialName()
  {
    if (*p == 'T') {
      ++p; // skip T
      switch (*p) {
      case 'V':
        ++p; // skip V
        dump(MESSAGE("virtual table for "));
        bassert(type(), InvalidFormat(this));
        return true;
      case 'T':
        ++p; // skip T
        dump(MESSAGE("VTT for "));
        bassert(type(), InvalidFormat(this));
        return true;
      case 'I':
        ++p; // skip I
        dump(MESSAGE("typeinfo for "));
        bassert(type(), InvalidFormat(this));
        return true;
      case 'S':
        ++p; // skip S
        dump(MESSAGE("typeinfo name for "));
        bassert(type(), InvalidFormat(this));
        return true;
      }
    }
    return false;
  }

  inline bool unqualifiedName()
  {
    unsigned int begin = demangled.getLength();
    if (operatorName()) {
    } else if (sourceName()) {
      addCandidate(begin);
    } else if (constructorAndDestructorName()) {
    } else {
      return false;
    }
    return true;
  }

  inline bool type()
  {
    unsigned int begin = demangled.getLength();
    switch (*p) {
    case 'r':
      ++p;
      bassert(type(), InvalidFormat(this));
      dump(MESSAGE(" restrict"));
      break;
    case 'V':
      ++p;
      bassert(type(), InvalidFormat(this));
      dump(MESSAGE(" volatile"));
      break;
    case 'K':
      ++p;
      bassert(type(), InvalidFormat(this));
      dump(MESSAGE(" const"));
      break;
    case 'P':
      ++p;
      bassert(type(), InvalidFormat(this));
      dump(MESSAGE("*"));
      break;
    case 'R':
      ++p;
      bassert(type(), InvalidFormat(this));
      dump(MESSAGE("&"));
      break;
    case 'C':
      ++p;
      bassert(type(), InvalidFormat(this));
      dump(MESSAGE("complex "));
      break;
    case 'G':
      ++p;
      bassert(type(), InvalidFormat(this));
      dump(MESSAGE("imaginary "));
      break;
    case 'U':
      ++p;
      bassert(type(), InvalidFormat(this));
      return sourceName();
      break;
    case 'A': // array type
      ++p; // skip A
      if (ASCIITraits::isDigit(*p)) {
        unsigned int dimension = 0;
        while (ASCIITraits::isDigit(*p)) {
          dimension = dimension * 10 + ASCIITraits::digitToValue(*p++);
        }
        bassert(*p++ == '_', InvalidFormat(this));
        bassert(type(), InvalidFormat(this));
        dump(MESSAGE("["));
        StringOutputStream stream; // insert dimension
        stream << dimension << FLUSH;
        demangled.append(stream.getString());
        dump(MESSAGE("]"));
      } else { // variable length array
        bassert(*p++ == '_', InvalidFormat(this));
        bassert(type(), InvalidFormat(this));
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
      bassert(type(), InvalidFormat(this)); // return type
      dump(MESSAGE(" (*)("));
      bassert(type(), InvalidFormat(this));
      while (*p != 'E') {
        dump(MESSAGE(", "));
        bassert(type(), InvalidFormat(this));
      }
      dump(MESSAGE(")"));
      ++p; // skip E
      break;
    case 'M': // pointer to member type
      {
        ++p; // skip M

        unsigned int beginName = demangled.getLength();
        bassert(name(), InvalidFormat(this));
        String nameString = demangled.substring(beginName);

        if (*p == 'F') { // special case
          ++p; // skip F
          if (*p == 'Y') {
            ++p; // skip Y
            dump(MESSAGE("extern \"C\" "));
          }

          unsigned int beginType = demangled.getLength();
          bassert(type(), InvalidFormat(this)); // return type

          unsigned int lengthOfName = beginType - beginName;
          unsigned int lengthOfType = demangled.getLength() - beginType;

          Array<CandidateRange>::Enumerator enu = candidates.getEnumerator();
          while (enu.hasNext()) {
            CandidateRange& range(enu.next());
            if (range.begin >= beginType) { // is range within type
              range.begin -= lengthOfName;
              range.end -= lengthOfName;
            } else if ((range.begin >= beginName) &&
                       (range.end < beginType)) { // is range within name
              range.begin += lengthOfType;
              range.end += lengthOfType;
            }
          }
          Array<CandidateRange>::Enumerator enuTemplateCandidate =
            templateCandidates.getEnumerator(); // remap candidates
          while (enuTemplateCandidate.hasNext()) {
            CandidateRange& range(enuTemplateCandidate.next());
            if (range.begin >= beginType) { // is range within type
              range.begin -= lengthOfName;
              range.end -= lengthOfName;
            } else if ((range.begin >= beginName) &&
                       (range.end < beginType)) { // is range within name
              range.begin += lengthOfType;
              range.end += lengthOfType;
            }
          }

          demangled.remove(beginName, beginType); // remove old name

          dump(MESSAGE(" ("));
          demangled.append(nameString);
          dump(MESSAGE("::*)("));

          bassert(type(), InvalidFormat(this));
          while (*p != 'E') {
            dump(MESSAGE(", "));
            bassert(type(), InvalidFormat(this));
          }
          dump(MESSAGE(")"));
          ++p; // skip E
        } else {
          unsigned int beginType = demangled.getLength();
          bassert(type(), InvalidFormat(this));

          unsigned int lengthOfName = beginType - beginName;
          unsigned int lengthOfType = demangled.getLength() - beginType;

          Array<CandidateRange>::Enumerator enu = candidates.getEnumerator();
          while (enu.hasNext()) {
            CandidateRange& range(enu.next());
            if (range.begin >= beginType) { // is range within type
              range.begin -= lengthOfName;
              range.end -= lengthOfName;
            } else if ((range.begin >= beginName) &&
                       (range.end < beginType)) { // is range within name
              range.begin += lengthOfType;
              range.end += lengthOfType;
            }
          }

          demangled.remove(beginName, beginType); // remove old name
          dump(MESSAGE(" "));
          demangled.append(nameString);
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
      dump(MESSAGE("ellipsis")); // TAG: ???
      return true; // no substitution for builtin type
    case 'u': // vendor specific
      ++p;
      bassert(sourceName(), InvalidFormat(this));
      break; // substitute
    default:
      if (nested()) {
      } else if (unscopedName()) {
        templateArguments(); // not required
      } else if (substitution()) {
        templateArguments(); // name rule - not required
      } else if (templateParameter()) {
        templateArguments(); // not required
      } else {
        return false;
      }
    }
    addCandidate(begin);
    return true;
  }

  inline bool operatorName()
  {
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
//    } else if ((*p == 's') && (p[1] == 'r')) {
// FIXME: ::= sr # scope resolution (::), see below
//      dump(MESSAGE("???"));
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

  inline bool sourceName() noexcept
  {
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

  inline void appendTemplateCandidate(unsigned int candidate)
  {
    bassert(candidate < templateCandidates.getSize(), InvalidFormat(this));
    demangled.append(
      demangled.substring(
        static_cast<CandidateRange>(templateCandidates[candidate]).begin,
        static_cast<CandidateRange>(templateCandidates[candidate]).end
      )
    );
  }

  inline bool templateParameter()
  {
    if (*p == 'T') {
      ++p; // skip T
      switch (*p) {
      case '_':
        ++p; // skip underscore
        appendTemplateCandidate(0);
        return true;
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
          bassert(*p++ == '_', InvalidFormat(this)); // skip underscore
          appendTemplateCandidate(id + 1);
        }
        return true;
      }
    }
    return false;
  }

  inline void appendCandidate(unsigned int candidate)
  {
    bassert(candidate < candidates.getSize(), InvalidFormat(this));
    demangled.append(
      demangled.substring(
        static_cast<CandidateRange>(candidates[candidate]).begin,
        static_cast<CandidateRange>(candidates[candidate]).end
      )
    );
  }

  inline bool substitution()
  {
    if (*p == 'S') {
      ++p; // skip S
      switch (*p) {
      case '_':
        ++p; // skip underscore
        appendCandidate(0);
        break;
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
      case 'A': case 'B': case 'C': case 'D': case 'E':
      case 'F': case 'G': case 'H': case 'I': case 'J':
      case 'K': case 'L': case 'M': case 'N': case 'O':
      case 'P': case 'Q': case 'R': case 'S': case 'T':
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
          bassert(*p++ == '_', InvalidFormat(this)); // skip underscore
          appendCandidate(id + 1);
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
        dump(MESSAGE("std::string"));
        //dump(MESSAGE("std::basic_string<char, std::char_traits<char>, std::allocator<char> >"));
        break;
      case 'i':
        ++p; // skip i
        dump(MESSAGE("std::istream"));
        //dump(MESSAGE("std::basic_istream<char, std::char_traits<char> >"));
        break;
      case 'o':
        ++p; // skip o
        dump(MESSAGE("std::ostream"));
        //dump(MESSAGE("std::basic_ostream<char, std::char_traits<char> >"));
        break;
      case 'd':
        ++p; // skip d
        dump(MESSAGE("std::iostream"));
        //dump(MESSAGE("std::basic_iostream<char, std::char_traits<char> >"));
        break;
      default:
        return false;
      }
      return true;
    }
    return false;
  }

  inline const String& getDemangled() const noexcept {
    return demangled;
  }
};



String TypeInfo::demangleName(const char* mangled)
{
  return V3MultiVendorABIDemangler(mangled).getDemangled();
}

#elif ((_COM_AZURE_DEV__BASE__DEMANGLE == _COM_AZURE_DEV__BASE__DEMANGLE_LLVM) || \
       (_COM_AZURE_DEV__BASE__DEMANGLE == _COM_AZURE_DEV__BASE__DEMANGLE_GCCV3))

String TypeInfo::demangleName(const char* mangled)
{
  if (!mangled) {
    return String();
  }
  int status = 0;
  char* demangled = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);
  if (!demangled) { // failed?
    return String(mangled); // return mangled type name
  }
  String result(demangled);
  free(demangled);
  return result;
}

#elif (_COM_AZURE_DEV__BASE__DEMANGLE == _COM_AZURE_DEV__BASE__DEMANGLE_GCCV3)

String TypeInfo::demangleName(const char* mangled)
{
  static const String prefix(MESSAGE("_Z"));
  String temp = prefix;
  temp.append(NativeString(mangled));
  char* demangled = cplus_demangle_v3(temp.getElements());
  if (!demangled) { // failed?
    return String(mangled); // return mangled type name
  }
  String result(demangled);
  free(demangled);
  return result;
}

#elif (_COM_AZURE_DEV__BASE__DEMANGLE == _COM_AZURE_DEV__BASE__DEMANGLE_GCCV23)

String TypeInfo::demangleName(const char* mangled)
{
  static const String prefix(MESSAGE("_Z"));
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

#elif (_COM_AZURE_DEV__BASE__DEMANGLE == _COM_AZURE_DEV__BASE__DEMANGLE_GCCV2)

String TypeInfo::demangleName(const char* mangled)
{
  static const String prefix(MESSAGE("a__"));
  static const String suffix(MESSAGE("::a"));
  // cplus_demangle only demangles function names - need alternative demangler
  String temp = prefix; // make function name
  temp.append(mangled);
  // include const and volatile
  char* demangled = cplus_demangle(temp.getElements(), 1 << 1);
  if (!demangled) { // failed?
    return String(mangled); // return mangled type name
  }
  String result(demangled);
  free(demangled);
  BASSERT(result.endsWith(suffix));
  result.setAt(result.getLength() - 3, 0); // remove function name
  return result;
}

#elif (_COM_AZURE_DEV__BASE__DEMANGLE == _COM_AZURE_DEV__BASE__DEMANGLE_SUNWSPRO)

String TypeInfo::demangleName(const char* mangled)
{
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  int result = cplus_demangle(mangled, buffer.getElements(), buffer.getSize());
  BASSERT(!result);
  return String(buffer->getElements());
}

#elif (_COM_AZURE_DEV__BASE__DEMANGLE == _COM_AZURE_DEV__BASE__DEMANGLE_MIPSPRO)

String TypeInfo::demangleName(const char* mangled)
{
  char buffer[MAXDBUF];
  int result = demangle(mangled, buffer);
  BASSERT(!result);
  return String(buffer);
}

#else // no demangling support

String TypeInfo::demangleName(const char* mangled)
{
  return mangled;
}

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
