/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "PrimitiveOutputStream.h"
#include <endian.h>

PrimitiveOutputStream::PrimitiveOutputStream(OutputStream* out) :
  FilterOutputStream(out) {
}

void PrimitiveOutputStream::writeBoolean(bool value) {
  write((char*)&value, sizeof(char));
}

void PrimitiveOutputStream::writeChar(char value) {
  write((char*)&value, sizeof(char));
}

void PrimitiveOutputStream::writeShortInteger(short value) {
#if BYTE_ORDER == BIG_ENDIAN
  write((char*)&value, sizeof(value));
#elif BYTE_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[2]; // 16 bits
  buffer[1] = p[0]; // most significant
  buffer[0] = p[1]; // least significant
  write((char*)&buffer, sizeof(buffer));
#else
  #err Byte order not supported
#endif
}

void PrimitiveOutputStream::writeUnsignedShortInteger(unsigned short value) {
#if BYTE_ORDER == BIG_ENDIAN
  write((char*)&value, sizeof(value));
#elif BYTE_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[2]; // 16 bits
  buffer[1] = p[0]; // most significant
  buffer[0] = p[1]; // least significant
  write((char*)&buffer, sizeof(buffer));
#else
  #err Byte order not supported
#endif
}

void PrimitiveOutputStream::writeInteger(int value) {
#if BYTE_ORDER == BIG_ENDIAN
  write((char*)&value, sizeof(value));
#elif BYTE_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[4]; // 32 bits
  buffer[3] = p[0]; // most significant
  buffer[2] = p[1]; // second most significant
  buffer[1] = p[2]; // second least significant
  buffer[0] = p[3]; // least significant
  write((char*)&buffer, sizeof(buffer));
#else
  #err Byte order not supported
#endif
}

void PrimitiveOutputStream::writeUnsignedInteger(unsigned int value) {
#if BYTE_ORDER == BIG_ENDIAN
  write((char*)&value, sizeof(value));
#elif BYTE_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[4]; // 32 bits
  buffer[3] = p[0]; // most significant
  buffer[2] = p[1]; // second most significant
  buffer[1] = p[2]; // second least significant
  buffer[0] = p[3]; // least significant
  write((char*)&buffer, sizeof(buffer));
#else
  #err Byte order not supported
#endif
}

void PrimitiveOutputStream::writeLongInteger(long long value) {
#if BYTE_ORDER == BIG_ENDIAN
  write((char*)&value, sizeof(value));
#elif BYTE_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[8]; // 64 bits
  buffer[7] = p[0];
  buffer[6] = p[1];
  buffer[5] = p[2];
  buffer[4] = p[3];
  buffer[3] = p[4];
  buffer[2] = p[5];
  buffer[1] = p[6];
  buffer[0] = p[7];
  write((char*)&buffer, sizeof(buffer));
#else
  #err Byte order not supported
#endif
}

void PrimitiveOutputStream::writeUnsignedLongInteger(unsigned long long value) {
#if BYTE_ORDER == BIG_ENDIAN
  write((char*)&value, sizeof(value));
#elif BYTE_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[8]; // 64 bits
  buffer[7] = p[0];
  buffer[6] = p[1];
  buffer[5] = p[2];
  buffer[4] = p[3];
  buffer[3] = p[4];
  buffer[2] = p[5];
  buffer[1] = p[6];
  buffer[0] = p[7];
  write((char*)&buffer, sizeof(buffer));
#else
  #err Byte order not supported
#endif
}

void PrimitiveOutputStream::writeFloat(float value) {
#if __FLOAT_WORD_ORDER == BIG_ENDIAN
  write((char*)&value, sizeof(value));
#elif __FLOAT_WORD_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[4]; // 32 bits
  buffer[3] = p[0];
  buffer[2] = p[1];
  buffer[1] = p[2];
  buffer[0] = p[3];
  write((char*)&buffer, sizeof(buffer));
#else
  #err Byte order not supported
#endif
}

void PrimitiveOutputStream::writeDouble(double value) {
#if __FLOAT_WORD_ORDER == BIG_ENDIAN
  write((char*)&value, sizeof(value));
#elif __FLOAT_WORD_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[8]; // 64 bits
  buffer[7] = p[0];
  buffer[6] = p[1];
  buffer[5] = p[2];
  buffer[4] = p[3];
  buffer[3] = p[4];
  buffer[2] = p[5];
  buffer[1] = p[6];
  buffer[0] = p[7];
  write((char*)&buffer, sizeof(buffer));
#else
  #err Byte order not supported
#endif
}

void PrimitiveOutputStream::writeLongDouble(long double value) {
#if __FLOAT_WORD_ORDER == BIG_ENDIAN
  write((char*)&value, sizeof(value));
#elif __FLOAT_WORD_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[10]; // 80 bits
  buffer[9] = p[0];
  buffer[8] = p[1];
  buffer[7] = p[2];
  buffer[6] = p[3];
  buffer[5] = p[4];
  buffer[4] = p[5];
  buffer[3] = p[6];
  buffer[2] = p[7];
  buffer[1] = p[8];
  buffer[0] = p[9];
  write((char*)&buffer, sizeof(buffer));
#else
  #err Byte order not supported
#endif
}
