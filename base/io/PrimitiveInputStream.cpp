/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/PrimitiveInputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

PrimitiveInputStream::PrimitiveInputStream(InputStream& in) throw(BindException) :
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
#if defined(HAVE_BIG_ENDIAN)
  read((char*)&value, sizeof(value));
#else
  char* p = (char*)&value;
  char buffer[2]; // 16 bits
  read((char*)&buffer, sizeof(buffer));
  p[1] = buffer[0];
  p[0] = buffer[1];
#endif
  return value;
}

unsigned short PrimitiveInputStream::readUnsignedShortInteger() throw(IOException) {
  unsigned short value;
#if defined(HAVE_BIG_ENDIAN)
  read((char*)&value, sizeof(value));
#else
  char* p = (char*)&value;
  char buffer[2]; // 16 bits
  read((char*)&buffer, sizeof(buffer));
  p[1] = buffer[0];
  p[0] = buffer[1];
#endif
  return value;
}

int PrimitiveInputStream::readInteger() throw(IOException) {
  int value;
#if defined(HAVE_BIG_ENDIAN)
  read((char*)&value, sizeof(value));
#else
  char* p = (char*)&value;
  char buffer[4]; // 32 bits
  read((char*)&buffer, sizeof(buffer));
  p[3] = buffer[0];
  p[2] = buffer[1];
  p[1] = buffer[2];
  p[0] = buffer[3];
#endif
  return value;
}

unsigned int PrimitiveInputStream::readUnsignedInteger() throw(IOException) {
  unsigned int value;
#if defined(HAVE_BIG_ENDIAN)
  read((char*)&value, sizeof(value));
#else
  char* p = (char*)&value;
  char buffer[4]; // 32 bits
  read((char*)&buffer, sizeof(buffer));
  p[3] = buffer[0];
  p[2] = buffer[1];
  p[1] = buffer[2];
  p[0] = buffer[3];
#endif
  return value;
}

long long PrimitiveInputStream::readLongInteger() throw(IOException) {
  long long value;
#if defined(HAVE_BIG_ENDIAN)
  read((char*)&value, sizeof(value));
#else
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
#endif
  return value;
}

unsigned long long PrimitiveInputStream::readUnsignedLongInteger() throw(IOException) {
  unsigned long long value;
#if defined(HAVE_BIG_ENDIAN)
  read((char*)&value, sizeof(value));
#else
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
#endif
  return value;
}

float PrimitiveInputStream::readFloat() throw(IOException) {
  float value;
// float may have different order
#if defined(HAVE_BIG_ENDIAN)
  read((char*)&value, sizeof(value));
#else
  char* p = (char*)&value;
  char buffer[4]; // 32 bits
  read((char*)&buffer, sizeof(buffer));
  p[3] = buffer[0];
  p[2] = buffer[1];
  p[1] = buffer[2];
  p[0] = buffer[3];
#endif
  return value;
}

double PrimitiveInputStream::readDouble() throw(IOException) {
  double value;

#if defined(HAVE_BIG_ENDIAN)
  read((char*)&value, sizeof(value));
#else
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
#endif
  return value;
}

long double PrimitiveInputStream::readLongDouble() throw(IOException) {
  long double value;

#if defined(HAVE_BIG_ENDIAN)
  read((char*)&value, sizeof(value));
#else
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
#endif
  return value;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
