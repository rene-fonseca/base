/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_MATRIX_EXCEPTION_H
#define _BASE_MATRIX_EXCEPTION_H

#include "base/Exception.h"

/**
  Matrix exception.

  @author René Møller Fonseca
  @version 1.0
*/

class MatrixException : public Exception {
public: 

	MatrixException();

	~MatrixException();
};

#endif
