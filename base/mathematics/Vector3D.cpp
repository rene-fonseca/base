/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mathematics/Vector3D.h>
#include <math.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

template Vector3D<float>;
template Vector3D<double>;
template Vector3D<long double>;

template<class TYPE>
Vector3D<TYPE>::Vector3D() throw() : x(0), y(0), z(0) {
}

template<class TYPE>
Vector3D<TYPE>::Vector3D(const TYPE& xx, const TYPE& yy, const TYPE& zz) throw() :
  x(xx), y(yy), z(zz) {
}

template<class TYPE>
Vector3D<TYPE>::Vector3D(const Vector3D& copy) throw() :
  x(copy.x), y(copy.y), z(copy.z) {
}

template<class TYPE>
Vector3D<TYPE>& Vector3D<TYPE>::operator=(const Vector3D& eq) throw() {
  if (&eq != this) {
    x = eq.x;
    y = eq.y;
    z = eq.z;
  }
  return *this;
}

template<class TYPE>
TYPE Vector3D<TYPE>::getModulus() const throw() {
  return sqrt(x * x + y * y + z * z);
}

template<class TYPE>
TYPE Vector3D<TYPE>::getXYAngle() const throw() {
  return atan2(y, x);
}

template<class TYPE>
TYPE Vector3D<TYPE>::getZAngle() const throw() {
  return atan2(z, sqrt(x * x + y * y));
}

template<class TYPE>
TYPE Vector3D<TYPE>::getX() const throw() {
  return x;
}

template<class TYPE>
TYPE Vector3D<TYPE>::getY() const throw() {
  return y;
}

template<class TYPE>
TYPE Vector3D<TYPE>::getZ() const throw() {
  return z;
}

template<class TYPE>
void Vector3D<TYPE>::setX(const TYPE& x) throw() {
  this->x = x;
}

template<class TYPE>
void Vector3D<TYPE>::setY(const TYPE& y) throw() {
  this->y = y;
}

template<class TYPE>
void Vector3D<TYPE>::setZ(const TYPE& z) throw() {
  this->z = z;
}

template<class TYPE>
Vector3D<TYPE>& Vector3D<TYPE>::zeroAdjust(const TYPE& zero) throw() {
  if (x < zero) {
    x = TYPE(0);
  }
  if (y < zero) {
    y = TYPE(0);
  }
  if (z < zero) {
    z = TYPE(0);
  }
  return *this;
}

template<class TYPE>
bool Vector3D<TYPE>::isEqual(const Vector3D& value) const throw() {
  return (x == value.x) && (y == value.y) && (z == value.z);
}

template<class TYPE>
bool Vector3D<TYPE>::isZero() const throw() {
  return (x == TYPE(0)) && (y == TYPE(0) && (z == TYPE(0)));
}

template<class TYPE>
bool Vector3D<TYPE>::isProper() const throw() {
  return (x != TYPE(0)) || (y != TYPE(0) || (z != TYPE(0)));
}

template<class TYPE>
bool Vector3D<TYPE>::isOrthogonal(const Vector3D& value) const throw() {
  return dot(value) == 0;
}

template<class TYPE>
bool Vector3D<TYPE>::isParallel(const Vector3D& value) const throw() {
  return cross(value).isZero();
}

template<class TYPE>
Vector3D<TYPE> Vector3D<TYPE>::plus() const throw() {
  return Vector3D();
}

template<class TYPE>
Vector3D<TYPE> Vector3D<TYPE>::minus() const throw() {
  return Vector3D().negate();
}

template<class TYPE>
Vector3D<TYPE>& Vector3D<TYPE>::negate() throw() {
  x = -x;
  y = -y;
  z = -z;
  return *this;
}

template<class TYPE>
Vector3D<TYPE>& Vector3D<TYPE>::add(const Vector3D& value) throw() {
  x += value.x;
  y += value.y;
  z += value.z;
  return *this;
}

template<class TYPE>
Vector3D<TYPE>& Vector3D<TYPE>::subtract(const Vector3D& value) throw() {
  x -= value.x;
  y -= value.y;
  z -= value.z;
  return *this;
}

template<class TYPE>
Vector3D<TYPE>& Vector3D<TYPE>::multiply(const TYPE& value) throw() {
  x *= value;
  y *= value;
  z *= value;
  return *this;
}

template<class TYPE>
Vector3D<TYPE>& Vector3D<TYPE>::divide(const TYPE& value) throw() {
  x /= value;
  y /= value;
  z /= value;
  return *this;
}

template<class TYPE>
TYPE Vector3D<TYPE>::dot(const Vector3D& value) const throw() {
  return x * value.x + y * value.y + z * value.z;
}

template<class TYPE>
Vector3D<TYPE> Vector3D<TYPE>::cross(const Vector3D& value) const throw() {
  return Vector3D(y * value.z - z * value.y, z * value.x - x * value.z, x * value.y - y * value.x);
}

template<class TYPE>
TYPE Vector3D<TYPE>::getAngle(const Vector3D& value) const throw() {
  TYPE temp = dot(value)/(sqrt(x * x + y * y) * sqrt(value.x * value.x + value.y * value.y));
  return atan2(sqrt(1 - temp * temp), temp);
}

template<class TYPE>
Vector3D<TYPE> Vector3D<TYPE>::getProjection(const Vector3D& value) const throw() {
  return value * dot(value)/(value.x * value.x + value.y * value.y);
}

template<class TYPE>
Vector3D<TYPE> operator*(const Vector3D<TYPE>& left, const TYPE& right) throw() {
  return Vector3D<TYPE>(left).multiply(right);
}

template<class TYPE>
Vector3D<TYPE> operator*(const TYPE& left, const Vector3D<TYPE>& right) throw() {
  return Vector3D<TYPE>(right).multiply(left);
}

template<class TYPE>
Vector3D<TYPE> operator/(const Vector3D<TYPE>& left, const TYPE& right) throw() {
  return Vector3D<TYPE>(left).divide(right);
}

template<class TYPE>
TYPE dot(const Vector3D<TYPE>& left, const Vector3D<TYPE>& right) throw() {
  return left.dot(right);
}

template<class TYPE>
Vector3D<TYPE> cross(const Vector3D<TYPE>& left, const Vector3D<TYPE>& right) throw() {
  return left.cross(right);
}

template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Vector3D<TYPE>& value) {
  return stream << "(" << value.x << "," << value.y << "," << value.z << ")";
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
