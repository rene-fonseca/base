/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

/*
  This source compiles if static const with initializer is supported by 
  the compiler.
*/

class MyClass {
public:

  static const char myConstChar = 'A';
  static const int myConstInt = 123;
  static const long myConstLong = 123;
  static const long long myConstLongLong = 123;
  static const float myConstFloat = 123.456;
  static const double myConstDouble = 123.456;
  static const long double myConstLongDouble = 123.456;
};
