/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/data/DataTable.h>
#include <base/data/CSVFormat.h>
#include <base/io/FileInputStream.h>
#include <base/string/Posix.h>
#include <base/Integer.h>
#include <base/LongInteger.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

DataTable DataTable::load(const String& path, const Array<Column>& columns)
{
  FileInputStream fis(path);
  return load(&fis, columns);
}

DataTable DataTable::load(InputStream* is, const Array<Column>& columns)
{
  // TAG: add info about error position - line/column
  DataTable table;
  CSVFormat csv;
  Array<Array<String> > lines = csv.load(is);
  table.rows.setSize(lines.getSize());
  // TAG: lookup strings and reuse
  // TAG: add support for multiple threads
  Posix posix;
  AnyValue v;
  for (MemorySize index = 0; index < lines.getSize(); ++index) {
    const auto& line = lines[index];
    if (line.getSize() < columns.getSize()) {
      _throw OutOfRange("Too few columns."); // TAG: fix exception
    }
    Array<AnyValue> row;
    row.ensureCapacity(columns.getSize());
    for (MemorySize c = 0; c < columns.getSize(); ++c) {
      const String& s = line[c];
      switch (columns[c].type) {
      case DataTable::TYPE_BOOL:
        v = !(!s || (s == "false") || (s == "0")); // TAG: need definition of true and false
        break;
      case DataTable::TYPE_INT32:
        v = Integer::parse(s);
        break;
      case DataTable::TYPE_INT64:
        v = LongInteger::parse(s);
        break;
      case DataTable::TYPE_FLOAT32:
        {
          float f = 0;
          if (!posix.getSeries(s, f)) {
            _throw InvalidException("Not a float.");
          }
          v = f;
        }
        break;
      case DataTable::TYPE_FLOAT64:
        {
          float d = 0;
          if (!posix.getSeries(s, d)) {
            _throw InvalidException("Not a double.");
          }
          v = d;
        }
        break;
      case DataTable::TYPE_STRING:
      default:
        v = s;
      }
      row.append(v);
    }
    table.rows[index] = row;
  }
  return table;
}

DataTable DataTable::load(R<ObjectModel::Array> data, const Array<Column>& columns)
{
  DataTable table;
  if (data) {
    for (auto v : data->values) {
#if 0
      for (auto w : v.checkedCast<ObjectModel::Array>()) {
      }
#endif
    }
  }
  return table;
}

DataTable::DataTable()
{
}

String getContainerAsHTML(const DataTable& table)
{
  StringOutputStream stream;
  stream << "<table>";
  stream << "<tr>" << "<th style=\"text-align: right\">" << "Key" << "</th>"
    << "<th style=\"text-align: left\">" << "Value" << "</th>" << "</tr>";
  
  stream << "<tr>";
  for (MemorySize c = 0; c < table.getNumberOfColumns(); ++c) {
    const auto& column = table.getColumnInfo(c);
    stream << ((column.type == DataTable::TYPE_STRING) ? "<td style=\"text-align: left\">" : "<td style=\"text-align: right\">")
      << HTMLEncode<String>::map(column.name) << "</td>";
  }
  stream << "</tr>";

  MemorySize index = 0;
  for (const auto& row : table.getRows()) {
    stream << "<tr>";
    stream << "<td style=\"text-align: right\">" << index++ << "</td>";
    MemorySize c = 0;
    for (const auto& value : row) {
      stream << ((table.getColumnInfo(c++).type == DataTable::TYPE_STRING) ? "<td style=\"text-align: left\">" : "<td style=\"text-align: right\">")
        << HTMLEncode<String>::map(value.getString()) << "</td>";
    }
    stream << "</tr>";
  }
  stream << "<table>";
  return stream.getString();
}

String getContainerAsHTML(const DataTable::DataTableSnippet& snippet)
{
  StringOutputStream stream;
  stream << "<table>";
  stream << "<tr>" << "<th style=\"text-align: right\">" << "Key" << "</th>"
    << "<th style=\"text-align: left\">" << "Value" << "</th>" << "</tr>";
  
  DataTable table = snippet.table;
  stream << "<tr>";
  for (MemorySize c = 0; c < table.getNumberOfColumns(); ++c) {
    const auto& column = table.getColumnInfo(c);
    stream << ((column.type == DataTable::TYPE_STRING) ? "<td style=\"text-align: left\">" : "<td style=\"text-align: right\">")
      << HTMLEncode<String>::map(column.name) << "</td>";
  }
  stream << "</tr>";

  const auto& rows = table.getRows();
  for (MemorySize index = snippet.begin; index < snippet.end; ++index) {
    stream << "<tr>";
    stream << "<td style=\"text-align: right\">" << index << "</td>";
    MemorySize c = 0;
    for (const auto& value : rows[index]) {
      stream << ((table.getColumnInfo(c++).type == DataTable::TYPE_STRING) ? "<td style=\"text-align: left\">" : "<td style=\"text-align: right\">")
        << HTMLEncode<String>::map(value.getString()) << "</td>";
    }
    stream << "</tr>";
  }
  stream << "<table>";
  return stream.getString();
}

// allow multiple threads support
// need datatypes? e.g. ISO to date, microseconds, ...
// add info about bad data
// TAG: add column mapping 0->0,1->2 when loading
// TAG: separator, comments, and other info when loading

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
