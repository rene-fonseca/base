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
#include <base/SystemInformation.h>
#include <base/UnitTest.h>

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
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
  int options = REG_EXTENDED;
  if (!caseSensitive) {
    options |= REG_ICASE;
  }
  regex_t* preq = new regex_t;
  int result = regcomp(preq, pattern.getElements(), options);
  if (result == 0) { // succesful
    compiled = preq;
  } else {
    delete preq;
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
    regex_t* _compiled = reinterpret_cast<regex_t*>(compiled);
    regfree(_compiled);
    delete _compiled;
#elif defined(_COM_AZURE_DEV__BASE__REGEXP_PCRE)
    pcre_free(compiled);
#endif
    compiled = nullptr;
  }
}

RegExp::RegExp() noexcept
{
}

RegExp::RegExp(const String& _pattern, bool _caseSensitive)
  : pattern(_pattern), caseSensitive(_caseSensitive)
{
  compile();
}

void RegExp::setPattern(const String& pattern)
{
  release();
  this->pattern = pattern;
  compile();
}

bool RegExp::isRegularExpression(const String& pattern, bool caseSensitive) noexcept
{
  #if defined(_COM_AZURE_DEV__BASE__REGEXP_POSIX)
    int options = REG_EXTENDED;
    if (!caseSensitive) {
      options |= REG_ICASE;
    }
    regex_t preq;
    int result = regcomp(&preq, pattern.getElements(), options);
    if (result == 0) { // succesful
      regfree(&preq);
      return true;
    }
    return false;
  #elif defined(_COM_AZURE_DEV__BASE__REGEXP_PCRE)
    int errorOffset = 0;
    int options = 0;
    if (!caseSensitive) {
      options |= PCRE_CASELESS;
    }
    void* compiled = pcre_compile(pattern.getElements(), options, 0, &errorOffset, 0);
    if (compiled) {
      pcre_free(compiled);
      return true;
    }
  #else // no regexp support
  #endif
  return false;
}

bool RegExp::doesMatch(const String& value, MemorySize start) const
{
  if (!compiled) {
    _throw RegExpException("Regular expression is invalid.", this);
  }
  if (start > value.getLength()) {
    _throw OutOfRange(this);
  }
  #if defined(_COM_AZURE_DEV__BASE__REGEXP_POSIX)
    regmatch_t pmatch[1];
    int code = regexec(
      static_cast<regex_t*>(compiled),
      value.getElements() + start,
      1,
      pmatch,
      0
    );
    if (code == 0) { // successful
      return (pmatch[0].rm_so == 0) && (pmatch[0].rm_eo == (value.getLength() - start));
    } else if (code == REG_NOMATCH) {
      return false;
    } else {
      _throw RegExpException(this); // should never happen
    }
  #elif defined(_COM_AZURE_DEV__BASE__REGEXP_PCRE)
    int offsets[3]; // yes 3 is correct - pcre_exec uses offsets[2] for temp. storage
    int code = pcre_exec((const pcre*)compiled, 0, value.getElements(), value.getLength(), start, 0, offsets, 3);
    if (code < 0) { // error
      if (code == PCRE_ERROR_NOMATCH) {
        return false;
      } else if (code == PCRE_ERROR_NOMEMORY) {
        _throw MemoryException(this);
      } else {
        _throw RegExpException(this); // should never happen
      }
    }
    return true;
  #else // no regexp support
    return false; // never here - RegExpException is raised first
  #endif
}

RegExp::Substring RegExp::match(
  const String& value,
  MemorySize start) const
{
  if (!compiled) {
    _throw RegExpException("Regular expression is invalid.", this);
  }
  if (start > value.getLength()) {
    _throw OutOfRange(this);
  }
  #if defined(_COM_AZURE_DEV__BASE__REGEXP_POSIX)
    regmatch_t pmatch[1];
    int code = regexec(
      static_cast<regex_t*>(compiled),
      value.getElements() + start,
      1,
      pmatch,
      0
    );
    if (code == 0) { // successful
      return Substring(pmatch[0].rm_so + start, pmatch[0].rm_eo + start);
    } else if (code == REG_NOMATCH) {
      return Substring();
    } else {
      _throw RegExpException(this); // should never happen
    }
  #elif defined(_COM_AZURE_DEV__BASE__REGEXP_PCRE)
    int offsets[3]; // yes 3 is correct - pcre_exec uses offsets[2] for temp. storage
    int code = pcre_exec((const pcre*)compiled, 0, value.getElements(), value.getLength(), start, 0, offsets, 3);
    if (code < 0) { // error
      if (code == PCRE_ERROR_NOMATCH) {
        return Substring();
      } else if (code == PCRE_ERROR_NOMEMORY) {
        _throw MemoryException(this);
      } else {
        _throw RegExpException(this); // should never happen
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
  MemorySize start) const
{
  if (!compiled) {
    _throw RegExpException("Regular expression is invalid.", this);
  }
  if (start > value.getLength()) {
    _throw OutOfRange(this);
  }
  #if defined(_COM_AZURE_DEV__BASE__REGEXP_POSIX)
    regmatch_t pmatch[result.getSize()];
    int code = regexec(
      static_cast<regex_t*>(compiled),
      value.getElements() + start,
      result.getSize(),
      pmatch,
      0
    );
    if (code == 0) { // successful
      for (int i = 1; i < result.getSize(); ++i) {
        result[i] = Substring(pmatch[i].rm_so + start, pmatch[i].rm_eo + start);
      }
      return Substring(pmatch[0].rm_so, pmatch[0].rm_eo);
    } else if (code == REG_NOMATCH) {
      return Substring();
    } else {
      _throw RegExpException(this); // should never happen
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
        _throw MemoryException(this);
      } else {
        _throw RegExpException(this); // should never happen
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

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(RegExp) : public UnitTest {
public:

  TEST_PRIORITY(80);
  TEST_PROJECT("base/string");
  TEST_IMPACT(CRITICAL);
  TEST_TIMEOUT_MS(30 * 1000);
  
  void run() override
  {
    if (auto name = SystemInformation::getOS()) {
      if ((name != "GNU/Linux") && (name != "macOS")) {
        return;
      }
    }
    
    TEST_ASSERT(RegExp::isRegularExpression("([0-9]+):?([A-Z]+):{0,3}:.*"));
    TEST_ASSERT(!RegExp::isRegularExpression("?"));
    TEST_ASSERT(!RegExp::isRegularExpression("[A--]"));

    RegExp r1("[0-9]+");
    TEST_ASSERT(!r1.doesMatch(""));
    TEST_ASSERT(!r1.doesMatch("abc"));
    TEST_ASSERT(r1.doesMatch("1"));
    TEST_ASSERT(r1.doesMatch("123"));

    RegExp r2("[0-9]+\\.[0-9]+");
    TEST_ASSERT(r2.doesMatch("123.456"));
    TEST_ASSERT(!r2.doesMatch("123.456e123"));

    RegExp r3("[a-zA-Z_][a-zA-Z0-9_]*");
    TEST_ASSERT(!r3.doesMatch("123"));
    TEST_ASSERT(r3.doesMatch("myId"));
    TEST_ASSERT(r3.doesMatch("MY_ID_123"));
  }
};

TEST_REGISTER(RegExp);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
