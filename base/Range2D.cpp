/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Range2D.h"

Range2D::Range2D() throw() {
  assign(0, 0);
}

Range2D::Range2D(const Range2D& range) throw() {
  assign(range.minimum, range.maximum);
}

Range2D::Range2D(float minimum, float maximum) throw() {
  assign(minimum, maximum);
}

void Range2D::assign(const Range2D& range) throw() {
  assign(range.minimum, range.maximum);
}

void Range2D::assign(float minimum, float maximum) throw() {
  this->minimum = minimum;
  this->maximum = maximum;
}

bool Range2D::isAbove(float value) const throw() {
  return value > maximum;
}

bool Range2D::isBelow(float value) const throw() {
  return value < minimum;
}

bool Range2D::isOutside(float value) const throw() {
  return (value < minimum) || (value > maximum);
}

bool Range2D::isWithin(float value) const throw() {
  return (value >= minimum) && (value <= maximum);
}

float Range2D::getMinimum() const throw() {
  return minimum;
}

float Range2D::getMaximum() const throw() {
  return maximum;
}

void Range2D::setMinimum(float minimum) throw() {
  this->minimum = minimum;
}

void Range2D::setMaximum(float maximum) throw() {
  this->maximum = maximum;
}

ostream& Range2D::toString(ostream& stream) const {
  return stream << '(' << minimum << ',' << maximum << ')';
}
