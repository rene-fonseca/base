/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

/*
  This program compiles if the compiler is Intel C++ Compiler compatible.
*/

#if (!(defined(__INTEL_COMPILER) || defined(__ICC) || defined(__ECC)) || !defined(__cplusplus))
die // force compiler to return with error
#  error Not Intel C++ Compiler
#endif
