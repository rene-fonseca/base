/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/ANSIEscapeSequence.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void ANSIEscapeSequence::setCursor(unsigned int line, unsigned int column) throw(IOException) {
  stream << '\033' << '[' << line << 'L' << ';' << column << 'c' << 'H'; // TAG: use buffer
}

void ANSIEscapeSequence::up(unsigned int count) throw(IOException) {
  stream << '\033' << '[' << DEC << count << 'A'; // TAG: use buffer
}

void ANSIEscapeSequence::down(unsigned int count) throw(IOException) {
  stream << '\033' << '[' << DEC << count << 'B'; // TAG: use buffer
}

void ANSIEscapeSequence::forward(unsigned int count) throw(IOException) {
  stream << '\033' << '[' << DEC << count << 'C'; // TAG: use buffer
}

void ANSIEscapeSequence::backward(unsigned int count = 1) throw(IOException) {
  stream<< '\033' << '[' << DEC << count << "D"; // TAG: use buffer
}

void ANSIEscapeSequence::setAttributes(unsigned int flags) throw(IOException) {
  char buffer[sizeof("\033[1;4;5;7;8m")];
  char* dest = buffer;
  
  *dest++ = '\033';
  *dest++ = '[';
  
  if (flags & ANSIEscapeSequence::BOLD) {
    *dest++ = '1';
    *dest++ = ';';
  }
  if (flags & ANSIEscapeSequence::UNDERSCORE) {
    *dest++ = '4';
    *dest++ = ';';
  }
  if (flags & ANSIEscapeSequence::BLINK) {
    *dest++ = '5';
    *dest++ = ';';
  }
  if (flags & ANSIEscapeSequence::REVERSE) {
    *dest++ = '7';
    *dest++ = ';';
  }
  if (flags & ANSIEscapeSequence::CONCEAL) {
    *dest++ = '8';
    *dest++ = ';';
  }
  if (dest[-1] == ';') {
    --dest; // undo last semicolon
  }
  *dest++ = 'm';
  *dest = 0;
  stream << buffer;
}

void ANSIEscapeSequence::setForeground(Color color) throw(IOException) {
  char buffer[sizeof("\033[3xm")];
  char* dest = buffer;
  *dest++ = '\033';
  *dest++ = '[';
  *dest++ = '3';
  *dest++ = color + '0';
  *dest++ = 'm';
  *dest = 0;
  stream << buffer;
}

void ANSIEscapeSequence::setBackground(Color color) throw(IOException) {
  char buffer[sizeof("\033[4xm")];
  char* dest = buffer;
  *dest++ = '\033';
  *dest++ = '[';
  *dest++ = '4';
  *dest++ = color + '0';
  *dest++ = 'm';
  *dest = 0;
  stream << buffer;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
