/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "PrimitiveInputStream.h"
#include <endian.h>
#include <float.h>

PrimitiveInputStream::PrimitiveInputStream(InputStream* in) :
  FilterInputStream(in) {
}

bool PrimitiveInputStream::readBoolean() throw(IOException) {
  bool value;
  read((char*)&value, sizeof(value));
  return value;
}

char PrimitiveInputStream::readChar() throw(IOException) {
  char value;
  read((char*)&value, sizeof(value));
  return value;
}

short PrimitiveInputStream::readShortInteger() throw(IOException) {
  short value;
#if BYTE_ORDER == BIG_ENDIAN
  read((char*)&value, sizeof(value));
#elif BYTE_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[2]; // 16 bits
  read((char*)&buffer, sizeof(buffer));
  p[1] = buffer[0];
  p[0] = buffer[1];
#else
  #err Byte order not supported
#endif
  return value;
}

unsigned short PrimitiveInputStream::readUnsignedShortInteger() throw(IOException) {
  unsigned short value;
#if BYTE_ORDER == BIG_ENDIAN
  read((char*)&value, sizeof(value));
#elif BYTE_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[2]; // 16 bits
  read((char*)&buffer, sizeof(buffer));
  p[1] = buffer[0];
  p[0] = buffer[1];
#else
  #err Byte order not supported
#endif
  return value;
}

int PrimitiveInputStream::readInteger() throw(IOException) {
  int value;
#if BYTE_ORDER == BIG_ENDIAN
  read((char*)&value, sizeof(value));
#elif BYTE_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[4]; // 32 bits
  read((char*)&buffer, sizeof(buffer));
  p[3] = buffer[0];
  p[2] = buffer[1];
  p[1] = buffer[2];
  p[0] = buffer[3];
#else
  #err Byte order not supported
#endif
  return value;
}

unsigned int PrimitiveInputStream::readUnsignedInteger() throw(IOException) {
  unsigned int value;
#if BYTE_ORDER == BIG_ENDIAN
  read((char*)&value, sizeof(value));
#elif BYTE_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[4]; // 32 bits
  read((char*)&buffer, sizeof(buffer));
  p[3] = buffer[0];
  p[2] = buffer[1];
  p[1] = buffer[2];
  p[0] = buffer[3];
#else
  #err Byte order not supported
#endif
  return value;
}

long long PrimitiveInputStream::readLongInteger() throw(IOException) {
  long long value;
#if BYTE_ORDER == BIG_ENDIAN
  read((char*)&value, sizeof(value));
#elif BYTE_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[8]; // 64 bits
  read((char*)&buffer, sizeof(buffer));
  p[7] = buffer[0];
  p[6] = buffer[1];
  p[5] = buffer[2];
  p[4] = buffer[3];
  p[3] = buffer[4];
  p[2] = buffer[5];
  p[1] = buffer[6];
  p[0] = buffer[7];
#else
  #err Byte order not supported
#endif
  return value;
}

unsigned long long PrimitiveInputStream::readUnsignedLongInteger() throw(IOException) {
  unsigned long long value;
#if BYTE_ORDER == BIG_ENDIAN
  read((char*)&value, sizeof(value));
#elif BYTE_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[8]; // 64 bits
  read((char*)&buffer, sizeof(buffer));
  p[7] = buffer[0];
  p[6] = buffer[1];
  p[5] = buffer[2];
  p[4] = buffer[3];
  p[3] = buffer[4];
  p[2] = buffer[5];
  p[1] = buffer[6];
  p[0] = buffer[7];
#else
  #err Byte order not supported
#endif
  return value;
}

float PrimitiveInputStream::readFloat() throw(IOException) {
  float value;

#if FLT_MANT_DIG != 24
  #err Type float not supported
#endif

#if __FLOAT_WORD_ORDER == BIG_ENDIAN
  read((char*)&value, sizeof(value));
#elif __FLOAT_WORD_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[4]; // 32 bits
  read((char*)&buffer, sizeof(buffer));
  p[3] = buffer[0];
  p[2] = buffer[1];
  p[1] = buffer[2];
  p[0] = buffer[3];
#else
  #err Byte order not supported
#endif
  return value;
}

double PrimitiveInputStream::readDouble() throw(IOException) {
  double value;

#if DBL_MANT_DIG != 53
  #err Type double not supported
#endif

#if __FLOAT_WORD_ORDER == BIG_ENDIAN
  read((char*)&value, sizeof(value));
#elif __FLOAT_WORD_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[8]; // 64 bits
  read((char*)&buffer, sizeof(buffer));
  p[7] = buffer[0];
  p[6] = buffer[1];
  p[5] = buffer[2];
  p[4] = buffer[3];
  p[3] = buffer[4];
  p[2] = buffer[5];
  p[1] = buffer[6];
  p[0] = buffer[7];
#else
  #err Byte order not supported
#endif
  return value;
}

long double PrimitiveInputStream::readLongDouble() throw(IOException) {
  long double value;

#if LDBL_MANT_DIG != 64
  #err Type long double not supported
#endif

#if __FLOAT_WORD_ORDER == BIG_ENDIAN
  read((char*)&value, sizeof(value));
#elif __FLOAT_WORD_ORDER == LITTLE_ENDIAN
  char* p = (char*)&value;
  char buffer[10]; // 80 bits
  read((char*)&buffer, sizeof(buffer));
  p[9] = buffer[0];
  p[8] = buffer[1];
  p[7] = buffer[2];
  p[6] = buffer[3];
  p[5] = buffer[4];
  p[4] = buffer[5];
  p[3] = buffer[6];
  p[2] = buffer[7];
  p[1] = buffer[8];
  p[0] = buffer[9];
#else
  #err Byte order not supported
#endif
  return value;
}
