/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Vector3D.h"
#include <math.h>

using namespace ::std;

const Float3D zeroFloat3D(0, 0, 0);
const Double3D zeroDouble3D(0, 0, 0);

template<class TYPE> Vector3D<TYPE>::Vector3D() throw() {
  assign(0, 0, 0);
}

template<class TYPE> Vector3D<TYPE>::Vector3D(const Vector3D& vector) throw() {
  assign(vector);
}

template<class TYPE> Vector3D<TYPE>::Vector3D(const TYPE& x, const TYPE& y, const TYPE& z) throw() {
  assign(x, y, z);
}

template<class TYPE> void Vector3D<TYPE>::assign(const Vector3D& vector) throw() {
  x = vector.getX();
  y = vector.getY();
  z = vector.getZ();
}

template<class TYPE> void Vector3D<TYPE>::assign(const TYPE& x, const TYPE& y, const TYPE& z) throw() {
  this->x = x;
  this->y = y;
  this->z = z;
}

template<class TYPE> TYPE Vector3D<TYPE>::getModulus() const throw() {
  return sqrt(x * x + y * y + z * z);
}

template<class TYPE> TYPE Vector3D<TYPE>::getX() const throw() {
  return x;
}

template<class TYPE> TYPE Vector3D<TYPE>::getY() const throw() {
  return y;
}

template<class TYPE> TYPE Vector3D<TYPE>::getZ() const throw() {
  return z;
}

template<class TYPE> void Vector3D<TYPE>::setX(const TYPE& x) throw() {
  this->x = x;
}

template<class TYPE> void Vector3D<TYPE>::setY(const TYPE& y) throw() {
  this->y = y;
}

template<class TYPE> void Vector3D<TYPE>::setZ(const TYPE& z) throw() {
  this->z = z;
}

template<class TYPE> ostream& Vector3D<TYPE>::toString(ostream& stream) const {
  stream << "(" << x << "," << y << "," << z << ")";
}
