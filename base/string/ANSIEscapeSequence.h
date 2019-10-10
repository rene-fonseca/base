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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class provides support for writing ANSI escape sequences to a format
  output stream.
  
  @short ANSI escape sequence.
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API ANSIEscapeSequence {
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
    
    inline Escape(Literal _literal) throw() : literal(_literal) {
    }
    
    inline FormatOutputStream& operator()(
      FormatOutputStream& stream) throw(IOException) {
      return stream << literal;
    }
  };

  /* Set attributes support class. */
  class SetAttributes {
  private:

    const unsigned int attributes = 0;
  public:

    inline SetAttributes(unsigned int _attributes) throw()
      : attributes(_attributes) {
    }
    
    inline FormatOutputStream& operator()(
      FormatOutputStream& stream) throw(IOException) {
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

    inline SetColor(Method _method, Color _color) throw()
      : method(_method), color(_color) {
    }

    inline FormatOutputStream& operator()(
      FormatOutputStream& stream) throw(IOException) {
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

    inline SetCursor(unsigned int _line, unsigned int _column) throw()
      : line(_line), column(_column) {
    }
    
    inline FormatOutputStream& operator()(
      FormatOutputStream& stream) throw(IOException) {
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

    inline MoveCursor(Method _method, unsigned int _count) throw()
      : method(_method), count(_count) {
    }

    inline FormatOutputStream& operator()(
      FormatOutputStream& stream) throw(IOException) {
      (ANSIEscapeSequence(stream).*method)(count);
      return stream;
    }
  };

  /**
    Initializes the ANSI escape sequence object.
  */
  inline ANSIEscapeSequence(FormatOutputStream& _stream) throw()
    : stream(_stream) {
  }

  /**
    Sets the cursor position.
  */
  void setCursor(unsigned int line, unsigned int column) throw(IOException);

  /**
    Moves the cursor up by the specified number of lines.
  */
  void up(unsigned int count) throw(IOException);

  /**
    Moves the cursor down by the specified number of lines.
  */
  void down(unsigned int count) throw(IOException);

  /**
    Moves the cursor forward by the specified number of columns.
  */
  void forward(unsigned int count) throw(IOException);

  /**
    Moves the cursor backward by the specified number of columns.
  */
  void backward(unsigned int count) throw(IOException);
  
  /**
    Sets the text attributes.
  */
  void setAttributes(unsigned int flags) throw(IOException);

  /**
    Sets the foreground color.
  */
  void setForeground(Color color) throw(IOException);

  /**
    Sets the background color.
  */
  void setBackground(Color color) throw(IOException);
};

/** Moves the cursor to the home position. */
inline ANSIEscapeSequence::Escape home() throw() {
  return ANSIEscapeSequence::Escape(Literal("ESC[H"));
}

/** Set the cursor position. */
inline ANSIEscapeSequence::SetCursor setCursor(
  unsigned int line, unsigned int column) throw() {
  return ANSIEscapeSequence::SetCursor(line, column);
}

/** Moves the cursor up by the specified number of lines. The default is 1. */
inline ANSIEscapeSequence::MoveCursor up(unsigned int count = 1) throw() {
  return ANSIEscapeSequence::MoveCursor(&ANSIEscapeSequence::up, count);
}

/** Moves the cursor down by the specified number of lines. The default is 1. */
inline ANSIEscapeSequence::MoveCursor down(unsigned int count = 1) throw() {
  return ANSIEscapeSequence::MoveCursor(&ANSIEscapeSequence::down, count);
}

/**
  Moves the cursor forward by the specified number of columns. The default is 1.
*/
inline ANSIEscapeSequence::MoveCursor forward(unsigned int count = 1) throw() {
  return ANSIEscapeSequence::MoveCursor(&ANSIEscapeSequence::forward, count);
}

/**
  Moves the cursor backward by the specified number of columns. The default is 1.
*/
inline ANSIEscapeSequence::MoveCursor backward(unsigned int count = 1) throw() {
  return ANSIEscapeSequence::MoveCursor(&ANSIEscapeSequence::backward, count);
}

/** Saves the position. */
inline ANSIEscapeSequence::Escape savePosition() throw() {
  return ANSIEscapeSequence::Escape(Literal("\033[s"));
}

/** Restores the position. */
inline ANSIEscapeSequence::Escape restorePosition() throw() {
  return ANSIEscapeSequence::Escape(Literal("\033[u"));
}

/** Erases the display. */
inline ANSIEscapeSequence::Escape clearDisplay() throw() {
  return ANSIEscapeSequence::Escape(Literal("\033[2J"));
}

/** Erases the current line. */
inline ANSIEscapeSequence::Escape clearLine() throw() {
  return ANSIEscapeSequence::Escape(Literal("\033[K"));
}

/** Enables wrapping. */
inline ANSIEscapeSequence::Escape wrap() throw() {
  return ANSIEscapeSequence::Escape(Literal("\033[=7h"));
}

/** Disables wrapping. */
inline ANSIEscapeSequence::Escape nowrap() throw() {
  return ANSIEscapeSequence::Escape(Literal("\033[=7l"));
}

/** Deactivates all the text attributes. */
inline ANSIEscapeSequence::Escape normal() throw() {
  return ANSIEscapeSequence::Escape(Literal("\033[0m"));
}

/** Activates the bold attribute. */
inline ANSIEscapeSequence::Escape bold() throw() {
  return ANSIEscapeSequence::Escape(Literal("\033[1m"));
}

/** Activates the underscore attribute. */
inline ANSIEscapeSequence::Escape underscore() throw() {
  return ANSIEscapeSequence::Escape(Literal("\033[4m"));
}

/** Activates the blink attribute. */
inline ANSIEscapeSequence::Escape blink() throw() {
  return ANSIEscapeSequence::Escape(Literal("\033[5m"));
}

/** Activates the reverse attribute. */
inline ANSIEscapeSequence::Escape reverse() throw() {
  return ANSIEscapeSequence::Escape(Literal("\033[7m"));
}

/** Activates the conceal attribute. */
inline ANSIEscapeSequence::Escape conceal() throw() {
  return ANSIEscapeSequence::Escape(Literal("\033[8m"));
}

/** Sets the text attributes. */
inline ANSIEscapeSequence::SetAttributes setAttributes(
  unsigned int attributes) throw() {
  return ANSIEscapeSequence::SetAttributes(attributes);
}

/** Sets the foreground color. */
inline ANSIEscapeSequence::SetColor setForeground(
  ANSIEscapeSequence::Color color) throw() {
  return ANSIEscapeSequence::SetColor(&ANSIEscapeSequence::setForeground, color);
}

/** Sets the background color. */
inline ANSIEscapeSequence::SetColor setBackground(
  ANSIEscapeSequence::Color color) throw() {
  return ANSIEscapeSequence::SetColor(&ANSIEscapeSequence::setBackground, color);
}

/** Writes the escape sequence to the format output stream. */
inline FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  ANSIEscapeSequence::Escape escape) throw(IOException) {
  return escape(stream);
}

/** Writes the cursor movement escape sequence to the format output stream. */
inline FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  ANSIEscapeSequence::MoveCursor moveCursor) throw(IOException) {
  return moveCursor(stream);
}

/** Writes the attribute escape sequence to the format output stream. */
inline FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  ANSIEscapeSequence::SetAttributes setAttributes) throw(IOException) {
  return setAttributes(stream);
}

/** Writes the color escape sequence to the format output stream. */
inline FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  ANSIEscapeSequence::SetColor setColor) throw(IOException) {
  return setColor(stream);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
