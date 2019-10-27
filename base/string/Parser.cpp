/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/Parser.h>
#include <base/string/Unicode.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

unsigned int Parser::getUCS4Bytes() const noexcept
{
  ucs4 ch = 0;
  const int status = Unicode::readUCS4(src, end, ch);
  return (status >= 0) ? status : 0;
}

ucs4 Parser::peekUCS4() const
{
  ucs4 ch = 0;
  const int status = Unicode::readUCS4(src, end, ch);
  if (status <= 0) {
    switch (status) {
    case Unicode::UTF8_ERROR_EMPTY:
      throw ParseException("Unexpected end reached.");
    case Unicode::UTF8_ERROR_INCOMPLETE:
      throw ParseException("Incomplete UTF-8 character encoding.");
    case Unicode::UTF8_ERROR_BAD_ENCODING:
    default:
      throw ParseException("Bad UTF-8 character encoding.");
    }
  }
  return ch;
}

ucs4 Parser::readUCS4() {
  ucs4 ch = 0;
  const int status = Unicode::readUCS4(src, end, ch);
  if (status <= 0) {
    switch (status) {
    case Unicode::UTF8_ERROR_EMPTY:
      throw ParseException("Unexpected end reached.");
    case Unicode::UTF8_ERROR_INCOMPLETE:
      throw ParseException("Incomplete UTF-8 character encoding.");
    case Unicode::UTF8_ERROR_BAD_ENCODING:
    default:
      throw ParseException("Bad UTF-8 character encoding.");
    }
  }
  src += status;
  return ch;
}

namespace {

  bool doesMatchPatternImpl(Parser pattern, Parser text)
  {
    // a heap stack would be a more reliable choice
    while (pattern.hasMore() || text.hasMore()) {
      if (!pattern.hasMore()) {
        return false;
      }
      const char ch = pattern.read();
      if (ch == '*') {
        if (!pattern.hasMore()) {
          return true; // if we end with * we match anything from here
        }
        if (doesMatchPatternImpl(pattern, text)) { // first try without consuming anything for *
          return true;
        }
        while (text.hasMore()) { // try until everything has been read
          text.skip(); // skip 1 char
          if (doesMatchPatternImpl(pattern, text)) {
            return true;
          }
        }
        return false;
      } else if (ch == '?') {
        if (!text.hasMore()) {
          return false;
        }
        text.skip(); // skip any
      } else { // exact match
        if (!text.hasMore()) {
          return false;
        }
        if (text.read() != ch) {
          return false;
        }
      }
    }
    return true;
  }
}

bool Parser::doesMatchPattern(const String& _pattern, const String& _text)
{
  Parser pattern(_pattern);
  Parser text(_text);
  return doesMatchPatternImpl(pattern, text);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
