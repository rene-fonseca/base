/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "PrimitiveOutputStream.h"

PrimitiveOutputStream::PrimitiveOutputStream(OutputStream* out) : FilterOutputStream(out) {
}

void PrimitiveOutputStream::writeBoolean(bool value) {
  write((char*)&value, sizeof(value));
}

void PrimitiveOutputStream::writeChar(char value) {
  write((char*)&value, sizeof(value));
}

void PrimitiveOutputStream::writeShortInteger(short value) {
  write((char*)&value, sizeof(value));
}

void PrimitiveOutputStream::writeUnsignedShortInteger(unsigned short value) {
  write((char*)&value, sizeof(value));
}

void PrimitiveOutputStream::writeInteger(int value) {
  write((char*)&value, sizeof(value));
}

void PrimitiveOutputStream::writeUnsignedInteger(unsigned int value) {
  write((char*)&value, sizeof(value));
}

void PrimitiveOutputStream::writeLongInteger(long value) {
  write((char*)&value, sizeof(value));
}

void PrimitiveOutputStream::writeUnsignedLongInteger(unsigned long value) {
  write((char*)&value, sizeof(value));
}

void PrimitiveOutputStream::writeLongLongInteger(long long value) {
  write((char*)&value, sizeof(value));
}

void PrimitiveOutputStream::writeUnsignedLongLongInteger(unsigned long long value) {
  write((char*)&value, sizeof(value));
}

void PrimitiveOutputStream::writeFloat(float value) {
  write((char*)&value, sizeof(value));
}

void PrimitiveOutputStream::writeDouble(double value) {
  write((char*)&value, sizeof(value));
}
