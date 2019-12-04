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
#include <base/string/RegExp.h>

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
#  define _COM_AZURE_DEV__BASE__REGEXP_POSIX
#endif

#if defined(_COM_AZURE_DEV__BASE__REGEXP_POSIX)
#  include <regex.h>
#elif defined(_COM_AZURE_DEV__BASE__REGEXP_PCRE)
#  if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
#    include <pcre.h>
#  else
#    include <pcre/pcre.h>
#  endif
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

void RegExp::compile()
{
  #if defined(_COM_AZURE_DEV__BASE__REGEXP_POSIX)
    int options = 0;
    if (!caseSensitive) {
      options |= REG_ICASE;
    }
    regex_t preq;
    int result = regcomp(&preq, pattern.getElements(), options);
    if (result) { // succesful
      compiled = new char[sizeof(regex_t)];
      copy<char>(
        Cast::pointer<char*>(compiled),
        Cast::pointer<const char*>(&preq),
        sizeof(regex_t)
      );
    }
  #elif defined(_COM_AZURE_DEV__BASE__REGEXP_PCRE)
    int errorOffset = 0;
    int options = 0;
    if (!caseSensitive) {
      options |= PCRE_CASELESS;
    }
    compiled = pcre_compile(pattern.getElements(), options, 0, &errorOffset, 0);
  #else // no regexp support
  #endif
}

void RegExp::release() noexcept
{
  if (compiled) {
  #if defined(_COM_AZURE_DEV__BASE__REGEXP_POSIX)
    regfree(static_cast<regex_t*>(compiled));
    delete[] Cast::pointer<char*>(compiled);
  #elif defined(_COM_AZURE_DEV__BASE__REGEXP_PCRE)
    pcre_free(compiled);
  #endif
    compiled = nullptr;
  }
}

RegExp::RegExp() noexcept
  : compiled(0), caseSensitive(true)
{
}

RegExp::RegExp(const String& _pattern, bool _caseSensitive)
  : pattern(_pattern), compiled(0), caseSensitive(_caseSensitive)
{
  compile();
}

void RegExp::setPattern(const String& pattern)
{
  release();
  this->pattern = pattern;
  compile();
}

RegExp::Substring RegExp::match(
  const String& value,
  unsigned int start) const
{
  if (!compiled) {
    throw RegExpException("Regular expression is invalid.", this);
  }
  if (start >= value.getLength()) {
    throw OutOfRange(this);
  }
  #if defined(_COM_AZURE_DEV__BASE__REGEXP_POSIX)
    regmatch_t pmatch[1];
    int code = regexec(
      static_cast<regex_t*>(compiled),
      value.getElements(),
      1,
      pmatch,
      0
    );
    if (!code) { // successful
      return Substring(pmatch[0].rm_so, pmatch[0].rm_eo);
    } else if (code == REG_NOMATCH) {
      return Substring();
    } else {
      throw RegExpException(this); // should never happen
    }
  #elif defined(_COM_AZURE_DEV__BASE__REGEXP_PCRE)
    int offsets[3]; // yes 3 is correct - pcre_exec uses offsets[2] for temp. storage
    int code = pcre_exec((const pcre*)compiled, 0, value.getElements(), value.getLength(), start, 0, offsets, 3);
    if (code < 0) { // error
      if (code == PCRE_ERROR_NOMATCH) {
        return Substring();
      } else if (code == PCRE_ERROR_NOMEMORY) {
        throw MemoryException(this);
      } else {
        throw RegExpException(this); // should never happen
      }
    }
    return Substring(offsets[0], offsets[1]);
  #else // no regexp support
    return Substring(); // never here - RegExpException is raised first
  #endif
}

RegExp::Substring RegExp::match(
  const String& value,
  Array<Substring>& result,
  unsigned int start) const
{
  if (!compiled) {
    throw RegExpException("Regular expression is invalid.", this);
  }
  if (start >= value.getLength()) {
    throw OutOfRange(this);
  }
  #if defined(_COM_AZURE_DEV__BASE__REGEXP_POSIX)
    regmatch_t pmatch[result.getSize()];
    int code = regexec(
      static_cast<regex_t*>(compiled),
      value.getElements(),
      result.getSize(),
      pmatch,
      0
    );
    if (!code) { // successful
      for (int i = 1; i < result.getSize(); ++i) {
        result[i] = Substring(pmatch[i].rm_so, pmatch[i].rm_eo);
      }
      return Substring(pmatch[0].rm_so, pmatch[0].rm_eo);
    } else if (code == REG_NOMATCH) {
      return Substring();
    } else {
      throw RegExpException(this); // should never happen
    }
  #elif defined(_COM_AZURE_DEV__BASE__REGEXP_PCRE)
    unsigned int size = result.getSize() * 3;
    int offsets[size];
    int code = pcre_exec(
      (const pcre*)compiled,
      0,
      value.getElements(),
      value.getLength(),
      start,
      0,
      offsets,
      size
    );
    if (code < 0) { // handle any error
      if (code == PCRE_ERROR_NOMATCH) {
        return Substring();
      } else if (code == PCRE_ERROR_NOMEMORY) {
        throw MemoryException(this);
      } else {
        throw RegExpException(this); // should never happen
      }
    }
    for (int i = 1; i < code; ++i) {
      result[i] = Substring(offsets[i * 2], offsets[i * 2 + 1]);
    }
    return Substring(offsets[0], offsets[1]);
  #else // no regexp support
    return Substring(); // never here - RegExpException is raised first 'cause compiled is 0
  #endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
