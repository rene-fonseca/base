/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_VECTOR3D_H
#define _BASE_VECTOR3D_H

#include "base/Object.h"

/**
  3D vector.

  @author Morten Baun Møller and René Møller Fonseca
  @version 1.01
*/

template<class TYPE>
class Vector3D : public Object {
protected:

  /** X coordinate. */
  TYPE x;
  /** Y coordinate. */
  TYPE y;
  /** Z coordinate. */
  TYPE z;
public:

  /**
    Initializes vector as origin (0, 0, 0).
  */
  Vector3D() throw();

  /**
    Initializes vector. The vector coordinates are copied from the specified vector.

    @param vector The desired vector.
  */
  Vector3D(const Vector3D& vector) throw();

  /**
    Initializes vector.

    @param x The desired X coordinate.
    @param y The desired Y coordinate.
    @param z The desired Z coordinate.
  */
  Vector3D(const TYPE& x, const TYPE& y, const TYPE& z) throw();

  /**
    Assignment of vector to vector.

    @param vector The desired vector.
  */
  void assign(const Vector3D& vector) throw();

  /**
    Sets the X, Y and Z coordinates of the vector.

    @param x The desired X coordinate.
    @param y The desired Y coordinate.
    @param z The desired Z coordinate.
  */
  void assign(const TYPE& x, const TYPE& y, const TYPE& z) throw();

  /**
    Returns the modulus of the vector.
  */
  TYPE getModulus() const throw();

  /**
    Returns the X coordinate of the vector.
  */
  TYPE getX() const throw();

  /**
    Returns the Y coordinate of the vector.
  */
  TYPE getY() const throw();

  /**
    Returns the Z coordinate of the vector.
  */
  TYPE getZ() const throw();

  /**
    Sets the X coordinate of the vector.

    @param x The desired X coordinate.
  */
  void setX(const TYPE& x) throw();

  /**
    Sets the Y coordinate of the vector.

    @param y The desired Y coordinate.
  */
  void setY(const TYPE& y) throw();

  /**
    Sets the Z coordinate of the vector.

    @param z The desired Z coordinate.
  */
  void setZ(const TYPE& z) throw();

  /**
    Writes a string representation of the object to a stream.
  */
  ostream& toString(ostream& stream) const;
};

template<class TYPE>
ostream& operator<<(ostream& stream, const Vector3D<TYPE>& object) {return object.toString(stream);}

/** 3D vector of float. */
typedef Vector3D<float> Float3D;

/** 3D vector of double. */
typedef Vector3D<double> Double3D;

/** 3D vector of float holding the origin. */
extern const Float3D zeroFloat3D;

/** 3D vector of double holding the origin. */
extern const Double3D zeroDouble3D;

#endif
