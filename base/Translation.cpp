/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Translation.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace {
  
  R<Translation> current;
}

Translation::Translation()
{
}

R<Translation> Translation::getCurrent()
{
  return current;
}

void Translation::makeCurrent(R<Translation> translation)
{
  current = translation;
}

const String* Translation::get(const String& text) const
{
  // TAG: add fast implementation later
  auto i = texts.find(text);
  return i;
#if 0
  if (i) {
    return *i;
  }
  return nullptr;
#endif
}

const String* Translation::get(const String& text, const String& context) const
{
  // TAG: add support for context with fallback to global context/namespace
  // TAG: add fast implementation later
  auto i = texts.find(text);
  return i;
#if 0
  if (i) {
    return *i;
  }
  return nullptr;
#endif
}

void Translation::add(const String& text, const String& translated)
{
  texts.add(text, translated);
}

void Translation::add(const String& text, const String& context, const String& translated)
{
  // TAG: add support for context
  texts.add(text, translated);
}

String Translation::translate(const String& text) const
{
  const String* translated = get(text);
  return translated ? *translated : text;
}

String Translation::translate(const String& text, const String& context) const
{
  // TAG: add support for context
  const String* translated = get(text);
  return translated ? *translated : text;
}

String translate(const char* text)
{
  R<Translation> t = current;
  if (current) {
    const String* translated = t->get(text);
    return translated ? *translated : text;
  }
  return text;
}

String translate(const Literal& text)
{
  R<Translation> t = current;
  if (current) {
    const String* translated = t->get(text);
    return translated ? *translated : text;
  }
  return text;
}

#if 0
String translate(const WideLiteral& text)
{
  R<Translation> t = current;
  if (current) {
    const String* translated = t->get(text);
    return translated ? *translated : text;
  }
  return text;
}
#endif

String translate(const String& text)
{
  R<Translation> t = current;
  if (current) {
    const String* translated = t->get(text);
    return translated ? *translated : text;
  }
  return text;
}

String operator "" _translate(const char* _text, size_t _length)
{
  String text(_text, _length); // TAG: what about null terminator - bad anyway to include for translated texts
  R<Translation> t = current;
  if (current) {
    const String* translated = t->get(text);
    return translated ? *translated : text;
  }
  return text;
}

String operator "" _translate(const wchar* _text, size_t _length)
{
  String text(_text, _length); // TAG: what about null terminator - bad anyway to include for translated texts
  R<Translation> t = current;
  if (current) {
    const String* translated = t->get(text);
    return translated ? *translated : text;
  }
  return text;
}

#if defined(_COM_AZURE_DEV__BASE__CPP_CHAR8_T)
String operator "" _translate(const char8_t* _text, size_t _length)
{
  String text(reinterpret_cast<const char*>(_text), _length); // TAG: what about null terminator - bad anyway to include for translated texts
  R<Translation> t = current;
  if (current) {
    const String* translated = t->get(text);
    return translated ? *translated : text;
  }
  return text;
}
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
