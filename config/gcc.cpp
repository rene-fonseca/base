/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

/*
  This program compiles if the compiler is GNU C++ Compiler compatible.
*/

#if (!defined(__GNUC__) || !defined(__GNUC_MINOR__) || !defined(__cplusplus))
die // force compiler to return with error
#  error Not GNU C++ Compiler
#endif
