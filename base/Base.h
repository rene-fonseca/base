/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__BASE_H
#define _DK_SDU_MIP__BASE__BASE_H

/** This cast function is used to down cast pointers. */
template<class TYPE>
inline TYPE down_cast(TYPE argument) throw() {return argument;}

/**
  Returns the minimum value.
*/
/*template<class TYPE>
inline TYPE min(TYPE a, TYPE b) {return (a <= b) ? a : b;}
*/
/**
  Returns the maximum value.
*/
/*
template<class TYPE>
inline TYPE max(TYPE a, TYPE b) {return (a >= b) ? a : b;}
*/
/**
  Swaps the values.
*/
/*template<class TYPE>
inline void swap(TYPE& a, TYPE& b) {
  TYPE temp = a;
  a = b;
  b = temp;
};*/

/**
  Returns the absolute value of the specified argument.
*/
template<class TYPE> inline TYPE absolute(const TYPE& value) {return (value >= TYPE(0)) ? value : -value;}
//template<class TYPE> inline TYPE absolute<double>(const TYPE& value) {return fabs(value);}

#endif
