/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_MATRIX_EXCEPTION_H
#define _BASE_MATRIX_EXCEPTION_H

#include "base/Exception.h"

/**
  Matrix exception.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class MatrixException : public Exception {
public: 

	MatrixException();

	~MatrixException();
};

#endif
