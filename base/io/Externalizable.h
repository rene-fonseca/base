/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__EXTERNALIZABLE_H
#define _DK_SDU_MIP__BASE_IO__EXTERNALIZABLE_H

#include <base/io/InputStream.h>
#include <base/io/OutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Interface implemented by classes that can be externalized (i.e. instances can
  be read from and written to a stream).

  @author René Møller Fonseca
  @version 1.0
*/

class Externalizable {
public:

//  static TYPE readType(InputStream* in) {
//    PrimitiveInputStream stream(in);
//    unsigned int length = stream->readUnsignedInteger();
//    char id[length];
//    stream->read(id, length);
//    return lookupType(id, length);
//  }
//
//  template<class TYPE>
//  static void writeType(OutputStream* out, TYPE object) {
//    PrimitiveOutputStream stream(out);
//    const char* id = typeid(object).name();
//    unsigned int length = strlen(id);
//    out->writeUnsignedInteger(length);
//    out->write(id, length);
//  }

  /**
    Reads object from input stream.

    @param in The stream to get the object from.
  */
  virtual void readExternal(InputStream* in) = 0;

  /**
    Writes object to output stream.

    @param out The stream to receive the object.
  */
  virtual void writeExternal(OutputStream* out) = 0;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
