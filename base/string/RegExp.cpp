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
#include <base/string/RegExp.h>

#if defined(_DK_SDU_MIP__BASE__REGEXP_POSIX)
  #include <stdlib.h> // provides malloc...
  #include <regex.h>
#elif defined(_DK_SDU_MIP__BASE__REGEXP_PCRE)
  #include <pcre/pcre.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void RegExp::compile() throw(MemoryException) {
  #if defined(_DK_SDU_MIP__BASE__REGEXP_POSIX)
    int options = 0;
    if (!caseSensitive) {
      options |= REG_ICASE;
    }
    regex_t preq;
    int result = regcomp(&preq, pattern.getElements(), options);
    if (result) { // succesful
      compiled = malloc(sizeof(regex_t));
      assert(compiled, MemoryException());
      copy<char>((char*)compiled, (const char*)&preq, sizeof(regex_t));
    }
  #elif defined(_DK_SDU_MIP__BASE__REGEXP_PCRE)
    int errorOffset;
    int options = 0;
    if (!caseSensitive) {
      options |= PCRE_CASELESS;
    }
    compiled = pcre_compile(pattern.getElements(), options, 0, &errorOffset, 0);
  #else // no regexp support
  #endif
}

void RegExp::release() throw() {
  if (compiled) {
  #if defined(_DK_SDU_MIP__BASE__REGEXP_POSIX)
    regfree(static_cast<regex_t*>(compiled));
    free(compiled);
  #elif defined(_DK_SDU_MIP__BASE__REGEXP_PCRE)
    pcre_free(compiled);
  #endif
    compiled = 0;
  }
}

RegExp::RegExp() throw() : compiled(0), caseSensitive(true) {
}

RegExp::RegExp(const String& p, bool c) throw(MemoryException) : pattern(p), compiled(0), caseSensitive(c) {
  compile();
}

void RegExp::setPattern(const String& pattern) throw(MemoryException) {
  release();
  this->pattern = pattern;
  compile();
}

RegExp::Substring RegExp::match(const String& value, unsigned int start) throw(RegExpException, OutOfRange) {
  assert(compiled, RegExpException("Regular expression is invalid"));
  assert(start < value.getLength(), OutOfRange());
  #if defined(_DK_SDU_MIP__BASE__REGEXP_POSIX)
    regmatch_t pmatch[1];
    int code = regexec(static_cast<regex_t*>(compiled), value.getElements(), 1, pmatch, 0);
    if (!code) { // successful
      return Substring(pmatch[0].rm_so, pmatch[0].rm_eo);
    } else if (code == REG_NOMATCH) {
      return Substring();
    } else {
      throw RegExpException(); // should never happen
    }
  #elif defined(_DK_SDU_MIP__BASE__REGEXP_PCRE)
    int offsets[3]; // yes 3 is correct - pcre_exec uses offsets[2] for temp. storage
    int code = pcre_exec(compiled, 0, value.getElements(), value.getLength(), start, 0, offsets, 3);
    if (code < 0) { // error
      if (code == PCRE_ERROR_NOMATCH) {
        return Substring();
      } else if (code == PCRE_ERROR_NOMEMORY) {
        throw MemoryException();
      } else {
        throw RegExpException(); // should never happen
      }
    }
    return Substring(offsets[0], offsets[1]);
  #else // no regexp support
    return Substring(); // never here - RegExpException is thrown first
  #endif
}

RegExp::Substring RegExp::match(const String& value, Array<Substring>& result, unsigned int start) throw(RegExpException, OutOfRange) {
  assert(compiled, RegExpException("Regular expression is invalid"));
  assert(start < value.getLength(), OutOfRange());
  #if defined(_DK_SDU_MIP__BASE__REGEXP_POSIX)
    regmatch_t pmatch[result.getSize()];
    int code = regexec(static_cast<regex_t*>(compiled), value.getElements(), result.getSize(), pmatch, 0);
    if (!code) { // successful
      for (int i = 1; i < result.getSize(); ++i) {
        result[i] = Substring(pmatch[i].rm_so, pmatch[i].rm_eo);
      }
      return Substring(pmatch[0].rm_so, pmatch[0].rm_eo);
    } else if (code == REG_NOMATCH) {
      return Substring();
    } else {
      throw RegExpException(); // should never happen
    }
  #elif defined(_DK_SDU_MIP__BASE__REGEXP_PCRE)
    unsigned int size = result.getSize() * 3;
    int offsets[size];
    int code = pcre_exec(compiled, 0, value.getElements(), value.getLength(), start, 0, offsets, size);
    if (code < 0) { // handle any error
      if (code == PCRE_ERROR_NOMATCH) {
        return Substring();
      } else if (code == PCRE_ERROR_NOMEMORY) {
        throw MemoryException();
      } else {
        throw RegExpException(); // should never happen
      }
    }
    for (int i = 1; i < code; ++i) {
      result[i] = Substring(offsets[i * 2], offsets[i * 2 + 1]);
    }
    return Substring(offsets[0], offsets[1]);
  #else // no regexp support
    return Substring(); // never here - RegExpException is thrown first 'cause compiled is 0
  #endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
