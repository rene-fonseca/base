/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Vector2D.h"
#include <math.h>

template Vector2D<float>;
template Vector2D<double>;

const Float2D zeroFloat2D(0, 0);
const Double2D zeroDouble2D(0, 0);

template<class TYPE> Vector2D<TYPE>::Vector2D() throw() {
  assign(0, 0);
}

template<class TYPE> Vector2D<TYPE>::Vector2D(const Vector2D& vector) throw() {
  assign(vector);
}

template<class TYPE> Vector2D<TYPE>::Vector2D(const TYPE& x, const TYPE& y) throw() {
  assign(x, y);
}

template<class TYPE> void Vector2D<TYPE>::assign(const Vector2D& vector) throw() {
  x = vector.x;
  y = vector.y;
}

template<class TYPE> void Vector2D<TYPE>::assign(const TYPE& x, const TYPE& y) throw() {
  this->x = x;
  this->y = y;
}

template<class TYPE> TYPE Vector2D<TYPE>::getAngle() const throw() {
  return atan2(y, x);
}

template<class TYPE> TYPE Vector2D<TYPE>::getModulus() const throw() {
  return sqrt(x * x + y * y);
}

template<class TYPE> TYPE Vector2D<TYPE>::getX() const throw() {
  return x;
}

template<class TYPE> TYPE Vector2D<TYPE>::getY() const throw() {
  return y;
}

template<class TYPE> void Vector2D<TYPE>::setX(const TYPE& x) throw() {
  this->x = x;
}

template<class TYPE> void Vector2D<TYPE>::setY(const TYPE& y) throw() {
  this->y = y;
}

template<class TYPE> ostream& Vector2D<TYPE>::toString(ostream& stream) const {
  return stream << "(" << x << "," << y << ")";
}
