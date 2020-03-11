/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/string/String.h>
#include <base/collection/Array.h>
#include <base/io/InputStream.h>
#include <base/AnyValue.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** CSV format. See https://tools.ietf.org/html/rfc4180. */
class _COM_AZURE_DEV__BASE__API CSVFormat {
private:

  ucs4 separator = ';';
public:

  /** Quote string. */
  static String quote(const String& text);

  /** Initializes CSV format. */
  CSVFormat(ucs4 separator = ';') noexcept;

  /** Joing items into row. */
  String join(const Array<String>& items);

  /** Joing items into row. */
  String join(const Array<AnyValue>& items);

  /** Joing items into row. */
  String join(const std::initializer_list<String>& items);

  /** Parses line. */
  void parse(const String& line, Array<String>& result);

  /** Loads CSV data. */
  Array<Array<String> > load(InputStream* is);

  /** Loads CSV data. */
  Array<Array<String> > load(const String& path);

  /** Line consumer. */
  class LineConsumer {
  public:
    
    virtual void operator()(const Array<String>& line) = 0;
  };
  
  /** Loads CSV data. */
  void load(InputStream* is, LineConsumer* consumer);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
