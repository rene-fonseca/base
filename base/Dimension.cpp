/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/Dimension.h>

Dimension::Dimension() throw() {
  width = 0;
  height = 0;
}

Dimension::Dimension(const Dimension& dimension) throw() {
  width = dimension.width;
  height = dimension.height;
}

Dimension::Dimension(unsigned int width, unsigned int height) throw() {
  this->width = width;
  this->height = height;
}

Dimension& Dimension::operator=(const Dimension& dimension) throw() {
  width = dimension.width;
  height = dimension.height;
  return *this;
}

void Dimension::assign(unsigned int width, unsigned int height) throw() {
  this->width = width;
  this->height = height;
}

bool Dimension::operator==(const Dimension& dimension) const throw() {
  return (width == dimension.width) && (height == dimension.height);
}

unsigned int Dimension::getWidth() const throw() {
  return width;
}

unsigned int Dimension::getHeight() const throw() {
  return height;
}

unsigned int Dimension::getSize() const throw() {
  return width * height;
}

void Dimension::setWidth(unsigned int width) throw() {
  this->width = width;
}

void Dimension::setHeight(unsigned int height) throw() {
  this->height = height;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Dimension& value) {
  return stream << '(' << value.width << ',' << value.height << ')';
}
