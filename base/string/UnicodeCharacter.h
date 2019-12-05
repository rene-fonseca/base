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

#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Unicode character.
*/

class _COM_AZURE_DEV__BASE__API UnicodeCharacter {
public:

  /** The style. */
  enum Style {
    /** Java format: \\u1234. */
    STYLE_JAVA,
    /** C format: \\u1234. */
    STYLE_C = STYLE_JAVA,
    /** C++ format: \\u1234. */
    STYLE_CPP = STYLE_C,
    /** HTML format &#x1234; or &#1234; dependent on the current base integer. */
    STYLE_HTML,
    /** XML format &#x1234; or &#1234; dependent on the current base integer. */
    STYLE_XML = STYLE_HTML,
    /** Perl format: \\x{1234}. */
    STYLE_PERL
  };
private:
  
  static constexpr Style DEFAULT_STYLE = STYLE_CPP;
  ucs4 character = 0;
  Style style;
public:
  
  inline UnicodeCharacter(ucs4 _character, Style _style = DEFAULT_STYLE) noexcept
    : character(_character), style(_style)
  {
  }

  inline ucs4 getCode() const noexcept
  {
    return character;
  }
  
  inline Style getStyle() const noexcept
  {
    return style;
  }
  
  inline void setStyle(Style style) noexcept
  {
    this->style = style;
  }
};

/**
  Writes a wide character to the format output stream.
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, UnicodeCharacter character);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
