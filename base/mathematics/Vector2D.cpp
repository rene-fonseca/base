/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Vector2D.h"
#include <math.h>

template Vector2D<float>;
template Vector2D<double>;
template Vector2D<long double>;

template<class TYPE>
Vector2D<TYPE>::Vector2D() throw() : x(0), y(0) {
}

template<class TYPE>
Vector2D<TYPE>::Vector2D(const TYPE& xx, const TYPE& yy) throw() : x(xx), y(yy) {
}

template<class TYPE>
Vector2D<TYPE>::Vector2D(const Vector2D& copy) throw() : x(copy.x), y(copy.y) {
}

template<class TYPE>
Vector2D<TYPE>& Vector2D<TYPE>::operator=(const Vector2D& eq) throw() {
  if (&eq != this) {
    x = eq.x;
    y = eq.y;
  }
  return *this;
}

template<class TYPE>
TYPE Vector2D<TYPE>::getAngle() const throw() {
  return atan2(y, x);
}

template<class TYPE>
TYPE Vector2D<TYPE>::getModulus() const throw() {
  return sqrt(x * x + y * y);
}

template<class TYPE>
TYPE Vector2D<TYPE>::getX() const throw() {
  return x;
}

template<class TYPE>
TYPE Vector2D<TYPE>::getY() const throw() {
  return y;
}

template<class TYPE>
void Vector2D<TYPE>::setX(const TYPE& x) throw() {
  this->x = x;
}

template<class TYPE>
void Vector2D<TYPE>::setY(const TYPE& y) throw() {
  this->y = y;
}

template<class TYPE>
Vector2D<TYPE>& Vector2D<TYPE>::zeroAdjust(const TYPE& zero) throw() {
  if (x < zero) {
    x = TYPE(0);
  }
  if (y < zero) {
    y = TYPE(0);
  }
  return *this;
}

template<class TYPE>
bool Vector2D<TYPE>::isEqual(const Vector2D& value) const throw() {
  return (x == value.x) && (y == value.y);
}

template<class TYPE>
bool Vector2D<TYPE>::isZero() const throw() {
  return (x == TYPE(0)) && (y == TYPE(0));
}

template<class TYPE>
bool Vector2D<TYPE>::isProper() const throw() {
  return (x != TYPE(0)) || (y != TYPE(0));
}

template<class TYPE>
bool Vector2D<TYPE>::isOrthogonal(const Vector2D& value) const throw() {
  return dot(value) == 0;
}

template<class TYPE>
bool Vector2D<TYPE>::isParallel(const Vector2D& value) const throw() {
  return determinant(value) == 0;
}

template<class TYPE>
Vector2D<TYPE> Vector2D<TYPE>::plus() const throw() {
  return Vector2D(*this);
}

template<class TYPE>
Vector2D<TYPE> Vector2D<TYPE>::minus() const throw() {
  return Vector2D().negate();
}

template<class TYPE>
Vector2D<TYPE>& Vector2D<TYPE>::negate() throw() {
  x = -x;
  y = -y;
  return *this;
}

template<class TYPE>
Vector2D<TYPE>& Vector2D<TYPE>::add(const Vector2D& value) throw() {
  x += value.x;
  y += value.y;
  return *this;
}

template<class TYPE>
Vector2D<TYPE>& Vector2D<TYPE>::subtract(const Vector2D& value) throw() {
  x -= value.x;
  y -= value.y;
  return *this;
}

template<class TYPE>
Vector2D<TYPE>& Vector2D<TYPE>::multiply(const TYPE& value) throw() {
  x *= value;
  y *= value;
  return *this;
}

template<class TYPE>
Vector2D<TYPE>& Vector2D<TYPE>::divide(const TYPE& value) throw() {
  x /= value;
  y /= value;
  return *this;
}

template<class TYPE>
TYPE Vector2D<TYPE>::dot(const Vector2D& value) const throw() {
  return x * value.x + y * value.y;
}

template<class TYPE>
TYPE Vector2D<TYPE>::determinant(const Vector2D& value) const throw() {
  return x * value.y - y * value.x;
}

template<class TYPE>
TYPE Vector2D<TYPE>::getAngle(const Vector2D& value) const throw() {
  TYPE temp = dot(value)/(sqrt(x * x + y * y) * sqrt(value.x * value.x + value.y * value.y));
  return atan2(sqrt(1 - temp * temp), temp);
}

template<class TYPE>
Vector2D<TYPE> Vector2D<TYPE>::getProjection(const Vector2D& value) const throw() {
  return value * dot(value)/(value.x * value.x + value.y * value.y);
}

template<class TYPE>
Vector2D<TYPE> operator*(const Vector2D<TYPE>& left, const TYPE& right) throw() {
  return Vector2D<TYPE>(left).multiply(right);
}

template<class TYPE>
Vector2D<TYPE> operator*(const TYPE& left, const Vector2D<TYPE>& right) throw() {
  return Vector2D<TYPE>(right).multiply(left);
}

template<class TYPE>
Vector2D<TYPE> operator/(const Vector2D<TYPE>& left, const TYPE& right) throw() {
  return Vector2D<TYPE>(left).divide(right);
}

template<class TYPE>
TYPE dot(const Vector2D<TYPE>& left, const Vector2D<TYPE>& right) throw() {
  return left.dot(right);
}

template<class TYPE>
TYPE determinant(const Vector2D<TYPE>& left, const Vector2D<TYPE>& right) throw() {
  return left.determinant(right);
}

template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Vector2D<TYPE>& value) {
  return stream << "(" << value.x << "," << value.y << ")";
}
