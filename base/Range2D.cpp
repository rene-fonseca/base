/***************************************************************************
    begin       : Fri May 12 2000
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Range2D.h"

Range2D::Range2D() {
  assign(0, 0);
}

Range2D::Range2D(const Range2D &range) {
  assign(range.minimum, range.maximum);
}

Range2D::Range2D(float minimum, float maximum) {
  assign(minimum, maximum);
}

void Range2D::assign(const Range2D &range) {
  assign(range.minimum, range.maximum);
}

void Range2D::assign(float minimum, float maximum) {
  this->minimum = minimum;
  this->maximum = maximum;
}

bool Range2D::isAbove(float value) const {
  return value > maximum;
}

bool Range2D::isBelow(float value) const {
  return value < minimum;
}

bool Range2D::isOutside(float value) const {
  return (value < minimum) || (value > maximum);
}

bool Range2D::isWithin(float value) const {
  return (value >= minimum) && (value <= maximum);
}

float Range2D::getMinimum() const {
  return minimum;
}

float Range2D::getMaximum() const {
  return maximum;
}

void Range2D::setMinimum(float minimum) {
  this->minimum = minimum;
}

void Range2D::setMaximum(float maximum) {
  this->maximum = maximum;
}
/*
void Range2D::toStream(ostream& stream) const {
  stream << "Range2D{" << minimum << "," << maximum << "}";
}
*/
