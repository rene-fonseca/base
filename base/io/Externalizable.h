/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/io/InputStream.h>
#include <base/io/OutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Interface implemented by classes that can be externalized (i.e. instances can
  be read from and written to a stream).
  
  @short Externalizable interface.
  @ingroup io
  @version 1.0
*/

class Externalizable {
public:

//  static TYPE readType(InputStream* in) throw(IOException) {
//    PrimitiveInputStream stream(in);
//    unsigned int length = stream->readUnsignedInteger();
//    char id[length];
//    stream->read(id, length);
//    return lookupType(id, length);
//  }
//
//  template<class TYPE>
//  static void writeType(OutputStream* out, TYPE object) throw(IOException) {
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
  virtual void readExternal(InputStream* in) throw(IOException) = 0;

  /**
    Writes object to output stream.

    @param out The stream to receive the object.
  */
  virtual void writeExternal(OutputStream* out) throw(IOException) = 0;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
