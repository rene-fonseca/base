/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/io/IOException.h>
#include <base/mem/Reference.h>
#include <base/string/String.h>
#include <base/string/WideString.h>
#include <base/rmi/OrbReference.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Object Request Broker decoder.
  
  @short Object Request Broker decoder.
  @ingroup rmi
  @version 1.0
*/

class OrbDecoder : public Object, public virtual ReferenceCountedObject {
private:
  
  // OrbInputStream stream;
public:
  
  /** The default buffer size. */
  static const unsigned int BUFFER_SIZE = 4096;
  
  /**
    Decodes a reference.
  */
  virtual void read(OrbReference& value) throw(IOException) = 0;
  
  /**
    Decodes a string.
  */
  virtual void read(String& value) throw(IOException) = 0;
  
  /**
    Decodes a wide string.
  */
  virtual void read(WideString& value) throw(IOException) = 0;  
  
  /**
    Decodes an array of chars.
  */
  virtual void read(char* buffer, unsigned int size) throw(IOException) = 0;
  
  /**
    Decodes a char.
  */
  virtual void read(char& value) throw(IOException) = 0;
  
  /**
    Decodes an array of signed short integers.
  */
  virtual void read(short* buffer, unsigned int size) throw(IOException) = 0;
  
  /**
    Decodes a signed short integer.
  */
  virtual void read(short& value) throw(IOException) = 0;
  
  /**
    Decodes an array of unsigned short integers.
  */
  virtual void read(unsigned short* buffer, unsigned int size) throw(IOException) = 0;
  
  /**
    Decodes a unsigned short integer.
  */
  virtual void read(unsigned short&  value) throw(IOException) = 0;
  
  /**
    Decodes an array of signed integers.
  */
  virtual void read(int* buffer, unsigned int size) throw(IOException) = 0;

  /**
    Decodes a signed integer.
  */
  virtual void read(int& value) throw(IOException) = 0;
  
  /**
    Decodes an array of unsigned integers.
  */
  virtual void read(unsigned int* buffer, unsigned int size) throw(IOException) = 0;

  /**
    Decodes an unsigned integer.
  */
  virtual void read(unsigned int& value) throw(IOException) = 0;
  
  /**
    Decodes an array of signed long integers.
  */
  virtual void read(long* buffer, unsigned int size) throw(IOException) = 0;

  /**
    Decodes a signed long integer.
  */
  virtual void read(long& value) throw(IOException) = 0;
  
  /**
    Decodes an array of unsigned long integers.
  */
  virtual void read(unsigned long* buffer, unsigned int size) throw(IOException) = 0;

  /**
    Decodes an unsigned long integer.
  */
  virtual void read(unsigned long& value) throw(IOException) = 0;
  
  /**
    Decodes an array of signed long long integers.
  */
  virtual void read(long long* buffer, unsigned int size) throw(IOException) = 0;

  /**
    Decodes a signed long long integer.
  */
  virtual void read(long long& value) throw(IOException) = 0;
  
  /**
    Decodes an array of unsigned long long integers.
  */
  virtual void read(unsigned long long* buffer, unsigned int size) throw(IOException) = 0;
  
  /**
    Decodes an unsigned long long interger.
  */
  virtual void read(unsigned long long& value) throw(IOException) = 0;
  
  /**
    Decodes an array of float.
  */
  virtual void read(float* buffer, unsigned int size) throw(IOException) = 0;
  
  /**
    Decodes a float.
  */
  virtual void read(float& value) throw(IOException) = 0;
  
  /**
    Decodes an array of double.
  */
  virtual void read(double* buffer, unsigned int size) throw(IOException) = 0;
  
  /**
    Decodes a double.
  */
  virtual void read(double& value) throw(IOException) = 0;
  
  /**
    Decodes an array of long double.
  */
  virtual void read(long double* buffer, unsigned int size) throw(IOException) = 0;
  
  /**
    Decodes a long double.
  */
  virtual void read(long double& value) throw(IOException) = 0;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
