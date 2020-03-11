/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/LineReader.h>
#include <base/io/EndOfFile.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

void LineReader::readLine(String& result)
{
  if (!bis.available()) {
    _throw EndOfFile("No more lines.");
  }
  result.forceToLength(0);
  while (bis.available()) {

    if (!bis.getNumberOfBytes()) {
      bis.fill();
      if (!bis.getNumberOfBytes()) {
        _throw EndOfFile("No more lines.");
      }
    }

    const uint8* src = bis.getElements();
    const uint8* end = src + bis.getNumberOfBytes();

    // handle partial eol
    if (skipCode) {
      if (*src == skipCode) {
        skipCode = 0;
        ++src;
      }
    }

    // look for eol
    const uint8* begin = src;
    while (src != end) {
      if ((*src == '\n') || (*src == '\r')) {
        const uint8 otherCode = (*src == '\n') ? '\r' : '\n';
        ++src;
        if (src != end) {
          if (*src == otherCode) {
            ++src;
          }
        } else {
          skipCode = otherCode;
        }

        bis.skip(src - begin);
        result.append(ConstSpan<char>(reinterpret_cast<const char*>(begin), src - begin));
        return;
      }
      ++src;
    }

    result.append(ConstSpan<char>(reinterpret_cast<const char*>(begin), src - begin));
    bis.skip(src - begin);
  }
}

String LineReader::readLine()
{
  if (!bis.available()) {
    _throw EndOfFile("No more lines.");
  }
  String result;
  while (bis.available()) {

    if (!bis.getNumberOfBytes()) {
      bis.fill();
      if (!bis.getNumberOfBytes()) {
        _throw EndOfFile("No more lines.");
      }
    }

    const uint8* src = bis.getElements();
    const uint8* end = src + bis.getNumberOfBytes();

    // handle partial eol
    if (skipCode) {
      if (*src == skipCode) {
        skipCode = 0;
        ++src;
      }
    }

    // look for eol
    const uint8* begin = src;
    while (src != end) {
      if ((*src == '\n') || (*src == '\r')) {
        const uint8 otherCode = (*src == '\n') ? '\r' : '\n';
        ++src;
        if (src != end) {
          if (*src == otherCode) {
            ++src;
          }
        } else {
          skipCode = otherCode;
        }

        bis.skip(src - begin);
        if (!result) {
          return String(reinterpret_cast<const char*>(begin), src - begin); // common case
        } else {
          result.append(ConstSpan<char>(reinterpret_cast<const char*>(begin), src - begin));
          return result;
        }
      }
      ++src;
    }

    result.append(ConstSpan<char>(reinterpret_cast<const char*>(begin), src - begin));
    bis.skip(src - begin);
  }
  return result;
}

Array<String> LineReader::readLines()
{
  Array<String> lines;
  while (!hasMore()) {
    lines.append(readLine());
  }
  return lines;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
