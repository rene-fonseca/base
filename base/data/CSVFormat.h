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

/** CSV format. */
class _COM_AZURE_DEV__BASE__API CSVFormat {
public:

  /** Quote string. */
  static String quote(const String& text);

  /** Joing items into row. */
  static String join(const Array<String>& items);

  /** Joing items into row. */
  static String join(const Array<AnyValue>& items);

  /** Joing items into row. */
  static String join(const std::initializer_list<String>& items);

  /** Parses line. */
  static Array<String> parse(const String& line);

  /** Loads CSV data. */
  static Array<Array<AnyValue> > load(InputStream* is);

  /** Loads CSV data. */
  static Array<Array<AnyValue> > load(const String& path);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
