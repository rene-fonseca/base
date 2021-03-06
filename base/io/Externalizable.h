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

class _COM_AZURE_DEV__BASE__API Externalizable {
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
//    const char* id = Type::getType(object).getLocalName();
//    unsigned int length = getNullTerminatedLength(id);
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
