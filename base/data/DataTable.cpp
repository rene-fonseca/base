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
#include <base/io/MemoryInputStream.h>
#include <base/string/Posix.h>
#include <base/Integer.h>
#include <base/LongInteger.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

DataTable::Config::Config()
{
}

DataTable::Config::Config(Header _header, char _separator)
  : header(_header), separator(_separator)
{
}

DataTable::Column::Column(const String& _name, const String& _type)
  : name(_name)
{
  if (_type == "string") {
    type = TYPE_STRING;
  } else if (_type == "int32") {
    type = TYPE_INT32;
  } else if (_type == "int64") {
    type = TYPE_INT64;
  } else if (_type == "float32") {
    type = TYPE_FLOAT32;
  } else if (_type == "float64") {
    type = TYPE_FLOAT64;
  } else if (_type == "bool") {
    type = TYPE_BOOL;
  } else {
    _throw InvalidKey("Invalid type.");
  }
}

DataTable DataTable::load(const String& path, const Array<Column>& columns, const Config& config)
{
  FileInputStream fis(path);
  return load(&fis, columns, config);
}

DataTable DataTable::loadFromString(const String& data, const Array<Column>& columns, const Config& config)
{
  MemoryInputStream mis(data);
  return load(&mis, columns, config);
}

namespace {

  class BuildDataTable : public CSVFormat::LineConsumer {
  public:

    const Array<DataTable::Column>& columns;
    const DataTable::Config& config;
    Array<DataTable::Custom*> customColumns;
    bool mapColumns = false;
    DataTable table;
    mutable Posix posix;
    mutable AnyValue v;
    bool firstLine = true;
    
    BuildDataTable(const Array<DataTable::Column>& _columns, const DataTable::Config& _config)
      : columns(_columns), config(_config)
    {
      customColumns = config.customColumns;
      customColumns.setSize(columns.getSize(), nullptr);
      mapColumns = config.mapColumns;
    }

    void operator()(const Array<String>& line) override
    {
      // TAG: add info about error position - line/column
      // TAG: add support for pipelining with threads
      // TAG: convert to values during load support - e.g. date
      // TAG: lookup strings and reuse
      if (!line) {
        return;
      }
      
      if (!config.assumeBlank) {
        if (line.getSize() < columns.getSize()) {
          if (config.printInvalid) {
            ferr << "Error: " << "Too few columns." << EOL
                 << "> " << line << ENDL;
          }
          _throw OutOfRange("Too few columns.");
        }
      }

      if (firstLine) {
        firstLine = false;
        if (config.header != DataTable::HEADER_NONE) {
          if (config.header == DataTable::HEADER_USE) {
            for (MemorySize c = 0; c < columns.getSize(); ++c) {
              table.setColumnName(c, line[c]);
            }
          }
          return;
        }
      }
      
      Array<AnyValue> row;
      row.ensureCapacity(columns.getSize());
      for (MemorySize c = 0; c < columns.getSize(); ++c) {
        const String& s = !mapColumns ? line[c] : line[config.mapColumns[c]];
        const DataTable::Column& column = columns[c];
        if (DataTable::Custom* custom = customColumns[c]) {
          v = (*custom)(s);
          // TAG: should we check type here
          row.append(v);
          continue;
        }
        
        switch (column.type) {
        case DataTable::TYPE_BOOL:
          v = !(!s || (s == "false") || (s == "0")); // TAG: need definition of true and false
          break;
        case DataTable::TYPE_INT32:
          v = Integer::parse(s);
          break;
        case DataTable::TYPE_INT64:
          // TAG: print on error - use nothrow
          v = LongInteger::parse(s);
          break;
        case DataTable::TYPE_FLOAT32:
          {
            float f = 0;
            if (!posix.getSeries(s, f)) {
              if (config.printInvalid) {
                ferr << "Error: " << "Not a float." << EOL
                     << "> " << line << ENDL;
              }
              _throw InvalidException("Not a float.");
            }
            v = f;
          }
          break;
        case DataTable::TYPE_FLOAT64:
          {
            float d = 0;
            if (!posix.getSeries(s, d)) {
              if (config.printInvalid) {
                ferr << "Error: " << "Not a double." << EOL
                     << "> " << line << ENDL;
              }
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
      
      Array<DataTable::Row>& rows = table.getRows();
      if (rows.getSize() >= rows.getCapacity()) {
        rows.ensureCapacity(maximum<MemorySize>(rows.getCapacity() * 2, 1024));
      }
      rows.append(row);
    }
  };
}

DataTable DataTable::load(InputStream* is, const Array<Column>& columns, const Config& config)
{
  BuildDataTable build(columns, config);
  build.table.columns = columns;
  CSVFormat csv(config.separator, config.trimSpaces);
  csv.load(is, &build);
  return build.table;
}

DataTable DataTable::load(R<ObjectModel::Array> data, const Array<Column>& columns, const Config& config)
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

String DataTable::getValueAsString(MemorySize index, unsigned int column) const
{
  return rows[index][column].getString();
}

DataTable::DataTableSnippet DataTable::head(MemorySize n) const noexcept
{
  return DataTableSnippet(*this, 0, minimum(n, getNumberOfRows()));
}

DataTable::DataTableSnippet DataTable::tail(MemorySize n) const noexcept
{
  MemorySize count = getNumberOfRows();
  return DataTableSnippet(*this, count - minimum(count, n), count);
}

FormatOutputStream& DataTable::dumpStats(FormatOutputStream& stream) const
{
  for (MemorySize c = 0; c < getNumberOfColumns(); ++c) {
    switch (getColumnInfo(c).type) {
    case TYPE_BOOL:
      stream << "Column %1: Type:%2" % Subst(c, "bool") << EOL;
      break;
    case TYPE_INT32:
      {
        IntStats<int> stats;
        calculate(c, stats);
        stream << "Column %1: Type:%2 Min:%3 Max:%4" %
          Subst(c, "int32", stats.getMinimum(), stats.getMaximum()) << EOL;
      }
      break;
    case TYPE_INT64:
      {
        IntStats<int64> stats;
        calculate(c, stats);
        stream << "Column %1: Type:%2 Min:%3 Max:%4" %
          Subst(c, "int64", stats.getMinimum(), stats.getMaximum()) << EOL;
      }
      break;
    case TYPE_FLOAT32:
      {
        FloatStats<float> stats;
        calculate(c, stats);
        stream << "Column %1: Type:%2 Min:%3 Max:%4" %
          Subst(c, "float32", stats.getMinimum(), stats.getMaximum(), stats.getAverage()) << EOL;
      }
      break;
    case TYPE_FLOAT64:
      {
        FloatStats<double> stats;
        calculate(c, stats);
        stream << "Column %1: Type:%2 Min:%3 Max:%4" %
          Subst(c, "float64", stats.getMinimum(), stats.getMaximum(), stats.getAverage()) << EOL;
      }
      break;
    case TYPE_STRING:
    default:
      stream << "Column %1: Type:%2" % Subst(c, "string") << EOL;
      break;
    }
  }
  stream << FLUSH;
  return stream;
}

String getContainerAsHTML(const DataTable& table)
{
  StringOutputStream stream;
  stream << "<table>";
  
  stream << "<tr>";
  stream << "<th style=\"text-align: right\">" << "Index" << "</th>";
  for (MemorySize c = 0; c < table.getNumberOfColumns(); ++c) {
    const auto& column = table.getColumnInfo(c);
    stream << ((column.type == DataTable::TYPE_STRING) ?
      "<th style=\"text-align: left\">" : "<td style=\"text-align: right\">")
      << HTMLEncode<String>::map(column.name) << "</th>";
  }
  stream << "</tr>";

  const auto& rows = table.getRows();
  if (!rows) {
    stream << "<tr>"
           << "<td  colspan=\"%1\" style=\"text-align: center\">" % Subst(table.getNumberOfColumns() + 1)
           << HTMLEncode<String>::map("None") << "</td>"
           << "</tr>";
  }
  MemorySize index = 0;
  for (const auto& row : rows) {
    stream << "<tr>";
    stream << "<td style=\"text-align: right\">" << index++ << "</td>";
    MemorySize c = 0;
    for (const auto& value : row) {
      stream << ((table.getColumnInfo(c++).type == DataTable::TYPE_STRING) ?
        "<td style=\"text-align: left\">" : "<td style=\"text-align: right\">")
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
  
  DataTable table = snippet.table;
  stream << "<tr>";
  stream << "<th style=\"text-align: right\">" << "Index" << "</th>";
  for (MemorySize c = 0; c < table.getNumberOfColumns(); ++c) {
    const auto& column = table.getColumnInfo(c);
    stream << ((column.type == DataTable::TYPE_STRING) ?
      "<th style=\"text-align: left\">" : "<td style=\"text-align: right\">")
      << HTMLEncode<String>::map(column.name) << "</th>";
  }
  stream << "</tr>";

  const auto& rows = table.getRows();
  if (!rows) {
    stream << "<tr>"
           << "<td  colspan=\"%1\" style=\"text-align: center\">" % Subst(table.getNumberOfColumns() + 1)
           << HTMLEncode<String>::map("None") << "</td>"
           << "</tr>";
  }
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

// need datatypes? e.g. ISO to date, microseconds, ...
// add info about bad data
// TAG: add column mapping 0->0,1->2 when loading, {{0,0},{1,1},{2,3}}
// TAG: add support for splitting string into one or more columns
// add geo locaton https://en.wikipedia.org/wiki/ISO_6709

#if 0 && defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(DataTable) : public UnitTest {
public:

  TEST_PRIORITY(200);
  TEST_PROJECT("base");
  TEST_IMPACT(NORMAL);
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    DataTable table = DataTable::load("path", {
      DataTable::Column{"First", DataTable::TYPE_STRING},
      DataTable::Column{"Last", DataTable::TYPE_STRING}
    });
  }
};

TEST_REGISTER(DataTable);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
