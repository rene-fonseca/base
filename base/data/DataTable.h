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

#include <base/collection/Array.h>
#include <base/collection/HashSet.h>
#include <base/io/InputStream.h>
#include <base/AnyValue.h>
#include <base/objectmodel/ObjectModel.h>
#include <base/InvalidException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Data table. */
class _COM_AZURE_DEV__BASE__API DataTable {
public:

  /** Column types. */
  enum Type {
    TYPE_BOOL,
    TYPE_INT32,
    TYPE_INT64,
    TYPE_FLOAT32,
    TYPE_FLOAT64,
    TYPE_STRING/*,
    TYPE_ENUM*/
  };

  /** Custom converter. */
  class Custom {
  public:
    
    virtual AnyValue operator()(const String& text) = 0;
  };
  
  /** Column info. */
  class Column {
  public:
    
    /** The name. */
    String name;
    /** The type. */
    Type type = TYPE_INT32;
    // TAG: convert int to string Array<String> presentation;
    
    inline Column()
    {
    }

    inline Column(const String& _name, Type _type) noexcept
      : name(_name), type(_type)
    {
    }

    Column(const String& _name, const String& _type);
  };

  /** A row. */
  typedef Array<AnyValue> Row;
private:

  /** Columns. */
  Array<Column> columns;
  /** The data. */
  Array<Row> rows;
public:
  
  /** Load config. */
  class Config {
  public:
    
    enum Header {
      HEADER_NONE,
      HEADER_SKIP,
      HEADER_USE
    };
    
    /** Skip first line. */
    Header header = HEADER_NONE;
    /** Separator. */
    ucs4 separator = ';';
    /** Trim spaces. */
    bool trimSpaces = true;
    Array<Custom*> customColumns;
    /** Map columns. */
    Array<unsigned int> mapColumns;
    
    /** Initializes config. */
    Config();

    /** Initializes config. */
    Config(Header header, char separator = ';');
  };
  
  /** Loads table from file. */
  static DataTable load(const String& path, const Array<Column>& columns, const Config& config = Config());

  /** Loads data table from input stream. */
  static DataTable load(InputStream* is, const Array<Column>& columns, const Config& config = Config());

  /** Loads data table from string. */
  static DataTable loadFromString(const String& data, const Array<Column>& columns, const Config& config = Config());

  /** Loads data table from ObjectModel. */
  static DataTable load(R<ObjectModel::Array> data, const Array<Column>& columns, const Config& config = Config());
  
  /** Initializes empty table. */
  DataTable();

  /** Returns the number of columns. */
  inline unsigned int getNumberOfColumns() const noexcept
  {
    return columns.getSize();
  }
  
  /** Returns the number of rows. */
  inline MemorySize getNumberOfRows() const noexcept
  {
    return rows.getSize();
  }

  /** Returns the rows. */
  inline const Array<Row>& getRows() const noexcept
  {
    return rows;
  }

  /** Returns the rows. */
  inline Array<Row>& getRows() noexcept
  {
    return rows;
  }

  /** Returns the given row. */
  inline const Row& getRow(MemorySize index) const
  {
    return rows[index];
  }

  /** Returns the given row. */
  inline Row& getRow(MemorySize index)
  {
    return rows[index];
  }

  /** Returns the column. */
  inline const Column& getColumnInfo(unsigned int column) const
  {
    return columns[column];
  }

  /** Returns the type of the column. */
  inline Type getColumnType(unsigned int column) const
  {
    return getColumnInfo(column).type;
  }

  /** Returns the name of the column. */
  inline const String& getColumnName(unsigned int column) const
  {
    return getColumnInfo(column).name;
  }

  /** Sets the name of the column. */
  inline void setColumnName(unsigned int column, const String& name)
  {
    columns[column].name = name;
  }

  /** Returns the value as a string. */
  String getValueAsString(MemorySize index, unsigned int column) const;

  /** Returns the value. */
  inline const AnyValue& getValue(MemorySize index, unsigned int column) const
  {
    return getRow(index)[column];
  }

  /** Returns the value. */
  inline bool getBool(MemorySize index, unsigned int column) const
  {
    return getValue(index, column).getBoolean();
  }

  /** Returns the value. */
  inline int getInt(MemorySize index, unsigned int column) const
  {
    if (getColumnInfo(column).type != TYPE_INT32) {
      _throw InvalidException("Column is not <int> type.");
    }
    return getValue(index, column).getInteger();
  }

  /** Returns the value. */
  inline int64 getInt64(MemorySize index, unsigned int column) const
  {
    if (getColumnInfo(column).type != TYPE_INT64) {
      _throw InvalidException("Column is not <int64> type.");
    }
    return getValue(index, column).getLongLongInteger();
  }

  /** Returns the value. */
  inline float getFloat(MemorySize index, unsigned int column) const
  {
    if (getColumnInfo(column).type != TYPE_FLOAT32) {
      _throw InvalidException("Column is not <float> type.");
    }
    return getValue(index, column).getFloat();
  }

  /** Returns the value. */
  inline double getDouble(MemorySize index, unsigned int column) const
  {
    if (getColumnInfo(column).type != TYPE_FLOAT64) {
      _throw InvalidException("Column is not <double> type.");
    }
    return getValue(index, column).getDouble();
  }

  /** Returns the value. */
  inline String getString(MemorySize index, unsigned int column) const
  {
    return getValue(index, column).getString();
  }
  
  /** Convert value to argument. */
  template<typename TYPE>
  class ToArgument {
  private:
    
    const AnyValue& v;
  public:
    
    inline ToArgument(const AnyValue& _v) noexcept
      : v(_v)
    {
    }
    
    inline operator TYPE() const
    {
      return v.getValue<TYPE>();
    }
  };
  
  /** Forward rows to function. */
  template<typename... ARGS>
  void forward(void (*function)(ARGS...))
  {
    if (sizeof...(ARGS) != columns.getSize())
    {
      _throw OutOfRange("Arguments do not match number of columns.");
    }
    for (const auto& row : rows) {
      MemorySize column = 0;
      function(ToArgument<ARGS>(columns[column++])...);
    }
  }

  /** Shuffles table. */
  inline void shuffle()
  {
    rows.shuffle();
  }

  /** Sorts table. */
  inline void sort()
  {
    rows.sort();
  }
  
  template<typename TYPE>
  class IntStats {
  private:
    
    TYPE min = PrimitiveTraits<TYPE>::MAXIMUM;
    TYPE max = PrimitiveTraits<TYPE>::MINIMUM;
    // TYPE sum = 0;
    int64 count = 0;
  public:
    
    /** Update stats. */
    inline void operator()(TYPE v) noexcept
    {
      if (v < min) {
        min = v;
      }
      if (v > max) {
        max = v;
      }
      // sum += v;
      ++count;
    }

    /** Returns the minimum value. */
    inline TYPE getMinimum() const noexcept
    {
      return min;
    }

    /** Returns the maximum value. */
    inline TYPE getMaximum() const noexcept
    {
      return max;
    }
  };
  
  /** Stats for floating point type. */
  template<typename TYPE>
  class FloatStats {
  private:
    
    TYPE min = Math::getInfinity<TYPE>();
    TYPE max = -Math::getInfinity<TYPE>();
    TYPE sum = 0;
    int64 count = 0;
  public:
    
    /** Update stats. */
    inline void operator()(TYPE v) noexcept
    {
      if (v < min) {
        min = v;
      }
      if (v > max) {
        max = v;
      }
      sum += v;
      ++count;
    }

    /** Returns the minimum value. */
    inline TYPE getMinimum() const noexcept
    {
      return min;
    }

    /** Returns the maximum value. */
    inline TYPE getMaximum() const noexcept
    {
      return max;
    }

    /** Returns the average value. */
    inline TYPE getAverage() const noexcept
    {
      return (count > 0) ? sum/count : 0;
    }
    
    // TAG: normal distribution support
  };

  /** Gets the unique strings. */
  class Unique {
  private:
    
    // ReferenceCountedObject* previous = nullptr;
  public:
    
    HashSet<String> values;
    
    inline void operator()(const String& s)
    {
      /*
      auto p = s.getContainer().getValue();
      if (p == previous) {
        return;
      }
      preivous = p;
      */
      values.add(s);
    }
  };

  template<typename TYPE>
  class Clamp {
  private:
  
    TYPE min = 0;
    TYPE max = 0;
  public:
    
    inline Clamp(TYPE _min, TYPE _max) noexcept
      : min(_min), max(_max)
    {
    }
    
    inline TYPE operator()(const TYPE& v) const noexcept
    {
      if (v < min) {
        return min;
      } else if (v > max) {
        return max;
      } else {
        return v;
      }
    }
  };
  
  /** Fixes enum. */
  class FixEnum {
  public:
    
    HashSet<String> allowed;
    String defaultValue;
    
    String operator()(const String& v)
    {
      if (allowed.hasKey(v)) { // TAG: use string from set
        return v;
      } else {
        return defaultValue;
      }
    }
  };

  // TAG: could be multi-threaded - if operator has merge()
  /** Calculates value for column. */
  template<typename OP>
  void calculate(unsigned int column, OP& op) const
  {
    switch (columns[column].type) {
    case TYPE_INT32:
      for (const auto& row : rows) {
        op(row[column].getValue<int>());
      }
      break;
    case TYPE_INT64:
      for (const auto& row : rows) {
        op(row[column].getValue<int64>());
      }
      break;
    case TYPE_FLOAT32:
      for (const auto& row : rows) {
        op(row[column].getValue<float>());
      }
      break;
    case TYPE_FLOAT64:
      for (const auto& row : rows) {
        op(row[column].getValue<double>());
      }
      break;
    default:
      for (const auto& row : rows) {
        op(row[column].getValue<String>());
      }
    }
  }

  // TAG: could be multi-threaded - if operator has merge()
  /** Calculates value for column. */
  template<typename TYPE>
  void calculate(void (*op)(TYPE), unsigned int column) const
  {
    for (const auto& row : rows) {
      op(ToArgument<TYPE>(row[column]));
    }
  }

  // TAG: could be multi-threaded
  /** Transforms a column. */
  template<typename TYPE>
  void transform(TYPE (*op)(TYPE), unsigned int column)
  {
    for (auto& row : rows) {
      AnyValue& v = row[column];
      v = op(ToArgument<TYPE>(v));
    }
  }

  // TAG: could be multi-threaded
  /** Validates a column. */
  template<typename TYPE>
  bool validate(bool (*op)(TYPE), unsigned int column) const
  {
    for (auto& row : rows) {
      if (!op(ToArgument<TYPE>(row[column]))) {
        return false;
      }
    }
    return true;
  }

  /** Prints stats for the columns. */
  FormatOutputStream& dumpStats(FormatOutputStream& stream) const;

  /** Returns HTML for the given table snippet. */
  String getHTML(MemorySize from, MemorySize to, std::initializer_list<unsigned int> columns);

  /** A snippet of a DataTable. */
  class DataTableSnippet {
  public:

    const DataTable& table;
    MemorySize begin = 0;
    MemorySize end = 0;

    inline DataTableSnippet(const DataTable& _table, MemorySize _begin, MemorySize _end) noexcept
      : table(_table), begin(_begin), end(_end)
    {
    }
  };

  /** Returns the first items of the data table. */
  DataTableSnippet head(MemorySize n = 5) const noexcept;
  
  /** Returns the last items of the data table. */
  DataTableSnippet tail(MemorySize n = 5) const noexcept;

  /** Returns the indices of the rows matching the given filter. */
  template<typename... ARGS>
  Array<MemorySize> filter(bool (*predicate)(ARGS...)) const
  {
    Array<MemorySize> result;
    result.ensureCapacity(rows.getSize());
    if (sizeof...(ARGS) != columns.getSize()) {
      _throw OutOfRange("Arguments do not match number of columns.");
    }
    for (MemorySize i = 0; i < rows.getSize(); ++i) {
      const auto& row = rows[i];
      MemorySize column = 0;
      if (predicate(ToArgument<ARGS>(row[column++])...)) {
        result.append(i);
      }
    }
    return result;
  }

  /** Returns the indices of the rows matching the given filter. */
  template<typename... ARGS>
  Array<unsigned int> filter32(bool (*predicate)(ARGS...)) const
  {
    Array<unsigned int> result;
    if (rows.getSize() > PrimitiveTraits<unsigned int>::MAXIMUM) {
      _throw OutOfRange("Table to big.");
    }
    result.ensureCapacity(rows.getSize());
    if (sizeof...(ARGS) != columns.getSize()) {
      _throw OutOfRange("Arguments do not match number of columns.");
    }
    for (MemorySize i = 0; i < rows.getSize(); ++i) {
      const auto& row = rows[i];
      MemorySize column = 0;
      if (predicate(ToArgument<ARGS>(row[column++])...)) {
        result.append(i);
      }
    }
    return result;
  }
};

/**
  Writes any-value object to format output stream.
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const DataTable& value);

/**
  Writes a string representation of DataTable to a format stream.
*/
inline FormatOutputStream& operator<<(FormatOutputStream& stream, const DataTable& table)
{
  return stream << table.getRows();
}

/** Returns container as HTML. */
_COM_AZURE_DEV__BASE__API String getContainerAsHTML(const DataTable& table);

/** Returns container as HTML. */
_COM_AZURE_DEV__BASE__API String getContainerAsHTML(const DataTable::DataTableSnippet& table);

#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_CLING)
inline ClingMimeBundle _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_ID(const DataTable& v)
{
  return cling_getHTMLMimeBundle(getContainerAsHTML(v));
}

inline ClingMimeBundle _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_ID(const DataTable::DataTableSnippet& v)
{
  return cling_getHTMLMimeBundle(getContainerAsHTML(v));
}
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
