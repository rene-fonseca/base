/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/data/CSVFormat.h>
#include <base/string/StringOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

String CSVFormat::quote(const String& text)
{
  String result(text.getLength() + 2);
  result += '\"';
  for (auto ch : text) {
    if (ch == '"') {
      result += '\\';
    }
    result += ch;
  }
  result += '\"';
  return result;
}

  // TAG: AnyValue support - never quote arith
String CSVFormat::join(const Array<String>& items)
{
  MemorySize capacity = 0;
  for (const auto& item : items) {
    capacity += item.getLength();
    capacity += 1 + 2;
  }
  String result(capacity);
  bool first = true;
  for (const auto& item : items) {
    // TAG: only quote if required
    if (!first) {
      result += ';';
    }
    first = false;
    result += quote(item);
  }
  return result;
}

String CSVFormat::join(const Array<AnyValue>& items)
{
  StringOutputStream stream;
  bool first = true;
  for (const auto& item : items) {
    // TAG: only quote if required
    if (!first) {
      stream << ';';
    }
    first = false;
    switch (item.getRepresentation()) {
    case AnyValue::CHARACTER:
    case AnyValue::WIDE_CHARACTER:
    case AnyValue::STRING:
    case AnyValue::WIDE_STRING:
      stream << quote(item.getString());
      break;
    default:
      stream << item.getString();
    }
  }
  return stream;
}

String CSVFormat::join(const std::initializer_list<String>& items)
{
  MemorySize capacity = 0;
  for (const auto& item : items) {
    capacity += item.getLength();
    capacity += 1 + 2;
  }
  String result(capacity);
  bool first = true;
  for (const auto& item : items) {
    // TAG: only quote if required
    if (!first) {
      result += ';';
    }
    first = false;
    result += quote(item);
  }
  return result;
}

Array<String> CSVFormat::parse(const String& line)
{
  Array<String> result;
  // TAG: read fields + dequote
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
  return result;
}

Array<Array<AnyValue> > load(InputStream* is)
{
  Array<Array<AnyValue> > result;
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
  return result;
}

Array<Array<AnyValue> > load(const String& path)
{
  Array<Array<AnyValue> > result;
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
  return result;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
