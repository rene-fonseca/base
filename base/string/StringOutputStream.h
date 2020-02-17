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

#include <base/Object.h>
#include <base/string/String.h>
#include <base/io/OutputStream.h>
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Internal string output stream. */
class _COM_AZURE_DEV__BASE__API StringOutputStreamWrapper : public virtual Object,
                                                            public virtual OutputStream {
private:

  /** The string holding the stream data. */
  String string;
  /** Specifies whether the stream has been closed. */
  bool closed = false;
public:

  /** Initializes buffer. */
  explicit StringOutputStreamWrapper() noexcept;

  /**
    Reserves capacity in the internal buffer.
  */
  void ensureCapacity(MemorySize capacity);

  /** Close stream. */
  void close();

  /** Flush stream. */
  void flush();

  /** Restarts stream. */
  void restart();

  /** Writes to string. */
  unsigned int write(
    const uint8* buffer,
    unsigned int size,
    bool nonblocking = false);

  /** Returns string. */
  inline const String& getString() const noexcept
  {
    return string;
  }
  
  /** Destructs buffer. */
  ~StringOutputStreamWrapper();
};



/** Helper class used by StringOutputStream. */
class _COM_AZURE_DEV__BASE__API StringOutputStreamImpl {
protected:

  StringOutputStreamWrapper stream;
};



/**
  A string output stream is a format output stream that directs the stream data
  into a string.

  @short String output stream.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API StringOutputStream : protected StringOutputStreamImpl, public FormatOutputStream {
public:

  /** Specifies the default capacity. */
  static const MemorySize DEFAULT_CAPACITY = 1024;
public:

  /**
    Initializes string output stream.
  */
  StringOutputStream();

  /**
    Initializes string output stream.

    @param capacity Specifies the initial capacity.
  */
  explicit StringOutputStream(MemorySize capacity);

  /**
    Reserves capacity in the internal buffer.
  */
  void ensureCapacity(MemorySize capacity);

  /**
    Returns the string associated with the stream. Same as toString() but does NOT restart stream.
  */
  const String& getString() const;

  /**
    Returns the string associated with the stream and restarts the stream.
  */
  String toString();
  
  /**
    Returns the string associated with the stream. Same as toString() but does NOT restart stream.
  */
  inline operator const String&() const
  {
    return getString();
  }

  /**
    Destroy stream object.
  */
  ~StringOutputStream();
};

template<class TYPE>
inline UTF8Stringify::UTF8Stringify(const TYPE& src)
{
  StringOutputStream sos;
  sos << src;
  setString(sos);
}

/** HTML. */
class _COM_AZURE_DEV__BASE__API HTML {
public:

  /** Encode string for HTML. */
  static String encode(const String& text);
};

template<class TYPE>
class HTMLEncode {
public:
  
  static inline const TYPE& map(const TYPE& value) noexcept
  {
    return value;
  }
};

template<>
class HTMLEncode<String> {
public:
  
  static inline String map(const String& value) noexcept
  {
    return HTML::encode(value);
  }
};

template<>
class HTMLEncode<WideString> {
public:
  
  static inline String map(const WideString& value) noexcept
  {
    return HTML::encode(value);
  }
};

/** Returns container as HTML. */
template<class TYPE>
String getContainerAsHTML(const TYPE& value)
{
  StringOutputStream stream;
  typename TYPE::ReadEnumerator enu = value.getReadEnumerator();
  stream << "<table>";
  stream << "<tr>" << "<th colspan=\"2\" align=\"center\">" << Type::getType<TYPE>().getLocalName()
    << " SIZE=" << value.getSize() << "<th>" << "</tr>";
  stream << "<tr>" << "<th>" << "Index" << "</th>" << "<th align=\"left\">" << "Value" << "</th>" << "</tr>";
  MemorySize index = 0;
  while (enu.hasNext()) { // TAG: what happens if nested
    const auto& value = enu.next();
    stream << "<tr>" << "<td>" << index++ << "</td>" << "<td align=\"left\">"
      << HTMLEncode<decltype(value)>::map(value) << "</td>" << "</tr>";
  }
  stream << "<table>";
  return stream.getString();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
