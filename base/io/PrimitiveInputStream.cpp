/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "PrimitiveInputStream.h"

PrimitiveInputStream::PrimitiveInputStream(InputStream* in) : FilterInputStream(in) {
}

bool PrimitiveInputStream::readBoolean() {
  bool value;
  read((char*)&value, sizeof(value));
  return value;
}

char PrimitiveInputStream::readChar() {
  char value;
  read((char*)&value, sizeof(value));
  return value;
}

short PrimitiveInputStream::readShortInteger() {
  short value;
  read((char*)&value, sizeof(value));
  return value;
}

unsigned short PrimitiveInputStream::readUnsignedShortInteger() {
  unsigned short value;
  read((char*)&value, sizeof(value));
  return value;
}

int PrimitiveInputStream::readInteger() {
  int value;
  read((char*)&value, sizeof(value));
  return value;
}

unsigned int PrimitiveInputStream::readUnsignedInteger() {
  unsigned int value;
  read((char*)&value, sizeof(value));
  return value;
}

long PrimitiveInputStream::readLongInteger() {
  long value;
  read((char*)&value, sizeof(value));
  return value;
}

unsigned long PrimitiveInputStream::readUnsignedLongInteger() {
  unsigned long value;
  read((char*)&value, sizeof(value));
  return value;
}

long long PrimitiveInputStream::readLongLongInteger() {
  long long value;
  read((char*)&value, sizeof(value));
  return value;
}

unsigned long long PrimitiveInputStream::readUnsignedLongLongInteger() {
  unsigned long long value;
  read((char*)&value, sizeof(value));
  return value;
}

float PrimitiveInputStream::readFloat() {
  float value;
  read((char*)&value, sizeof(value));
  return value;
}

double PrimitiveInputStream::readDouble() {
  double value;
  read((char*)&value, sizeof(value));
  return value;
}
