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
  This class provides support for writing ANSI escape sequences to a format
  output stream.
  
  @short ANSI escape sequence.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API ANSIEscapeSequence {
public:

  /** Text attributes. */
  enum Attributes {
    BOLD = 1, /**< Selects bold. */
    UNDERSCORE = 2, /**< Selects underscore. */
    BLINK = 4, /**< Selects blink. */
    REVERSE = 8, /**< Selects reversed. */
    CONCEAL = 16 /**< Selects concealed. */
  };

  /** Color. */
  enum Color {
    BLACK, /**< Black. */
    RED, /**< Red. */
    GREEN, /**< Green. */
    YELLOW, /**< Yellow. */
    BLUE, /**< Blue. */
    MAGENTA, /**< Magenta. */
    CYAN, /**< Cyan. */
    WHITE /**< White. */
  };
private:

  /** The output stream. */
  FormatOutputStream& stream;
public:

  /* Simple escape sequence. */
  class Escape {
  private:
    
    Literal literal;
  public:
    
    inline Escape(Literal _literal) noexcept
      : literal(_literal)
    {
    }
    
    inline FormatOutputStream& operator()(FormatOutputStream& stream)
    {
      return stream << literal;
    }
  };

  /* Set attributes support class. */
  class SetAttributes {
  private:

    const unsigned int attributes = 0;
  public:

    inline SetAttributes(unsigned int _attributes) noexcept
      : attributes(_attributes) {
    }
    
    inline FormatOutputStream& operator()(FormatOutputStream& stream)
    {
      ANSIEscapeSequence(stream).setAttributes(attributes);
      return stream;
    }
  };

  /* Set color support class. */
  class SetColor {
  private:

    typedef void (ANSIEscapeSequence::*Method)(Color color);
    Method method;
    const Color color;
  public:

    inline SetColor(Method _method, Color _color) noexcept
      : method(_method), color(_color)
    {
    }

    inline FormatOutputStream& operator()(FormatOutputStream& stream)
    {
      (ANSIEscapeSequence(stream).*method)(color);
      return stream;
    }
  };

  /* Set cursor position support class. */
  class SetCursor {
  private:

    const unsigned int line = 0;
    const unsigned int column = 0;
  public:

    inline SetCursor(unsigned int _line, unsigned int _column) noexcept
      : line(_line), column(_column)
    {
    }
    
    inline FormatOutputStream& operator()(FormatOutputStream& stream)
    {
      ANSIEscapeSequence(stream).setCursor(line, column);
      return stream;
    }    
  };

  /* Cursor movement support class. */
  class MoveCursor {
  private:

    typedef void (ANSIEscapeSequence::*Method)(unsigned int count);
    Method method;
    const unsigned int count = 0;
  public:

    inline MoveCursor(Method _method, unsigned int _count) noexcept
      : method(_method), count(_count)
    {
    }

    inline FormatOutputStream& operator()(FormatOutputStream& stream)
    {
      (ANSIEscapeSequence(stream).*method)(count);
      return stream;
    }
  };

  /**
    Initializes the ANSI escape sequence object.
  */
  inline ANSIEscapeSequence(FormatOutputStream& _stream) noexcept
    : stream(_stream)
  {
  }

  /**
    Sets the cursor position.
  */
  void setCursor(unsigned int line, unsigned int column);

  /**
    Moves the cursor up by the specified number of lines.
  */
  void up(unsigned int count);

  /**
    Moves the cursor down by the specified number of lines.
  */
  void down(unsigned int count);

  /**
    Moves the cursor forward by the specified number of columns.
  */
  void forward(unsigned int count);

  /**
    Moves the cursor backward by the specified number of columns.
  */
  void backward(unsigned int count);
  
  /**
    Sets the text attributes.
  */
  void setAttributes(unsigned int flags);

  /**
    Sets the foreground color.
  */
  void setForeground(Color color);

  /**
    Sets the background color.
  */
  void setBackground(Color color);

  /** Activates color by index. Not using standard. */
  static String color(uint8 index);

  /** Activates RGB color. Not using standard. */
  static String color(uint8 red, uint8 green, uint8 blue);

  /** Activates background color by index. Not using standard. */
  static String backgroundColor(uint8 index);

  /** Activates background RGB color. Not using standard. */
  static String backgroundColor(uint8 red, uint8 green, uint8 blue);
};

/** Moves the cursor to the home position. */
inline ANSIEscapeSequence::Escape home() noexcept {
  return ANSIEscapeSequence::Escape(Literal("ESC[H"));
}

/** Set the cursor position. */
inline ANSIEscapeSequence::SetCursor setCursor(
  unsigned int line, unsigned int column) noexcept {
  return ANSIEscapeSequence::SetCursor(line, column);
}

/** Moves the cursor up by the specified number of lines. The default is 1. */
inline ANSIEscapeSequence::MoveCursor up(unsigned int count = 1) noexcept {
  return ANSIEscapeSequence::MoveCursor(&ANSIEscapeSequence::up, count);
}

/** Moves the cursor down by the specified number of lines. The default is 1. */
inline ANSIEscapeSequence::MoveCursor down(unsigned int count = 1) noexcept {
  return ANSIEscapeSequence::MoveCursor(&ANSIEscapeSequence::down, count);
}

/**
  Moves the cursor forward by the specified number of columns. The default is 1.
*/
inline ANSIEscapeSequence::MoveCursor forward(unsigned int count = 1) noexcept {
  return ANSIEscapeSequence::MoveCursor(&ANSIEscapeSequence::forward, count);
}

/**
  Moves the cursor backward by the specified number of columns. The default is 1.
*/
inline ANSIEscapeSequence::MoveCursor backward(unsigned int count = 1) noexcept {
  return ANSIEscapeSequence::MoveCursor(&ANSIEscapeSequence::backward, count);
}

/** Saves the position. */
inline ANSIEscapeSequence::Escape savePosition() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[s"));
}

/** Restores the position. */
inline ANSIEscapeSequence::Escape restorePosition() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[u"));
}

/** Erases the display. */
inline ANSIEscapeSequence::Escape clearDisplay() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[2J"));
}

/** Erases the current line. */
inline ANSIEscapeSequence::Escape clearLine() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[K"));
}

/** Enables wrapping. */
inline ANSIEscapeSequence::Escape wrap() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[=7h"));
}

/** Disables wrapping. */
inline ANSIEscapeSequence::Escape nowrap() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[=7l"));
}

/** Deactivates all the text attributes. */
inline ANSIEscapeSequence::Escape normal() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[0m"));
}

/** Activates the bold attribute. */
inline ANSIEscapeSequence::Escape bold() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[1m"));
}

/** Deactivates the bold attribute. */
inline ANSIEscapeSequence::Escape nobold() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[22m"));
}

/** Activates the dim attribute. ATTENTION: This code is NOT generally supported. */
inline ANSIEscapeSequence::Escape dim() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[2m"));
}

/** Deactivates the dim attribute. ATTENTION: This code is NOT generally supported. */
inline ANSIEscapeSequence::Escape nodim() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[22m"));
}

/** Activates the italic attribute. ATTENTION: This code is NOT generally supported. */
inline ANSIEscapeSequence::Escape italic() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[3m"));
}

/** Deactivates the italic attribute. ATTENTION: This code is NOT generally supported. */
inline ANSIEscapeSequence::Escape noitalic() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[23m"));
}

/** Activates the strike-through attribute. ATTENTION: This code is NOT generally supported. */
inline ANSIEscapeSequence::Escape strikethrough() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[9m"));
}

/** Deactivates the strike-through attribute. ATTENTION: This code is NOT generally supported. */
inline ANSIEscapeSequence::Escape nostrikethrough() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[29m"));
}

/** Activates the underscore attribute. */
inline ANSIEscapeSequence::Escape underscore() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[4m"));
}

/** Deactivates the underscore attribute. */
inline ANSIEscapeSequence::Escape nounderscore() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[24m"));
}

/** Activates the overline attribute. ATTENTION: This code is NOT generally supported. */
inline ANSIEscapeSequence::Escape overline() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[53m"));
}

/** Deactivates the overline attribute. ATTENTION: This code is NOT generally supported. */
inline ANSIEscapeSequence::Escape nooverline() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[55m"));
}

/** Activates the blink attribute. */
inline ANSIEscapeSequence::Escape blink() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[5m"));
}

/** Deactivates the blink attribute. */
inline ANSIEscapeSequence::Escape noblink() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[25m"));
}

/** Activates the reverse attribute. */
inline ANSIEscapeSequence::Escape reverse() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[7m"));
}

/** Deactivates the reverse attribute. */
inline ANSIEscapeSequence::Escape noreverse() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[27m"));
}

/** Activates the conceal attribute. */
inline ANSIEscapeSequence::Escape conceal() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[8m"));
}

/** Dectivates the conceal attribute. */
inline ANSIEscapeSequence::Escape noconceal() noexcept {
  return ANSIEscapeSequence::Escape(Literal("\033[28m"));
}

/** Sets the text attributes. */
inline ANSIEscapeSequence::SetAttributes setAttributes(unsigned int attributes)
{
  return ANSIEscapeSequence::SetAttributes(attributes);
}

/** Sets the foreground color. */
inline ANSIEscapeSequence::SetColor setForeground(ANSIEscapeSequence::Color color)
{
  return ANSIEscapeSequence::SetColor(&ANSIEscapeSequence::setForeground, color);
}

/** Sets the background color. */
inline ANSIEscapeSequence::SetColor setBackground(ANSIEscapeSequence::Color color)
{
  return ANSIEscapeSequence::SetColor(&ANSIEscapeSequence::setBackground, color);
}

/** Reset foreground RGB color. */
inline ANSIEscapeSequence::Escape nocolor()
{
  return ANSIEscapeSequence::Escape(Literal("\033[39m"));
}

/** Reset background RGB color. */
inline ANSIEscapeSequence::Escape nobackgroundcolor()
{
  return ANSIEscapeSequence::Escape(Literal("\033[49m"));
}

/** Writes the escape sequence to the format output stream. */
inline FormatOutputStream& operator<<(FormatOutputStream& stream, ANSIEscapeSequence::Escape escape)
{
  return escape(stream);
}

/** Writes the cursor movement escape sequence to the format output stream. */
inline FormatOutputStream& operator<<(FormatOutputStream& stream, ANSIEscapeSequence::MoveCursor moveCursor)
{
  return moveCursor(stream);
}

/** Writes the attribute escape sequence to the format output stream. */
inline FormatOutputStream& operator<<(FormatOutputStream& stream, ANSIEscapeSequence::SetAttributes setAttributes)
{
  return setAttributes(stream);
}

/** Writes the color escape sequence to the format output stream. */
inline FormatOutputStream& operator<<(FormatOutputStream& stream, ANSIEscapeSequence::SetColor setColor)
{
  return setColor(stream);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
