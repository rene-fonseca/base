/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Range2D.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Range2D::Range2D() throw() {
  assign(0, 0);
}

Range2D::Range2D(const Range2D& range) throw() {
  assign(range.minimum, range.maximum);
}

Range2D::Range2D(double minimum, double maximum) throw() {
  assign(minimum, maximum);
}

void Range2D::assign(const Range2D& range) throw() {
  assign(range.minimum, range.maximum);
}

void Range2D::assign(double minimum, double maximum) throw() {
  this->minimum = minimum;
  this->maximum = maximum;
}

bool Range2D::isAbove(double value) const throw() {
  return value > maximum;
}

bool Range2D::isBelow(double value) const throw() {
  return value < minimum;
}

bool Range2D::isOutside(double value) const throw() {
  return (value < minimum) || (value > maximum);
}

bool Range2D::isWithin(double value) const throw() {
  return (value >= minimum) && (value <= maximum);
}

double Range2D::getMinimum() const throw() {
  return minimum;
}

double Range2D::getMaximum() const throw() {
  return maximum;
}

void Range2D::setMinimum(double minimum) throw() {
  this->minimum = minimum;
}

void Range2D::setMaximum(double maximum) throw() {
  this->maximum = maximum;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Range2D& value) throw(IOException) {
  FormatOutputStream::PushContext push(stream);
  return stream << '(' << value.getMinimum() << ',' << value.getMaximum() << ')';
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
