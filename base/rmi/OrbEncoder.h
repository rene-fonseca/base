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

#include <base/Object.h>
#include <base/io/IOException.h>
#include <base/mem/Reference.h>
#include <base/string/String.h>
#include <base/string/WideString.h>
#include <base/rmi/OrbReference.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Object Request Broker encoder.
  
  @short Object Request Broker encoder.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OrbEncoder : public Object, public virtual ReferenceCountedObject {
private:
  
  // OrbOutputStream stream;
public:
  
  /** The default buffer size. */
  static const unsigned int BUFFER_SIZE = 4096;
  
  /**
    Encodes the specified reference.
  */
  virtual void write(const OrbReference& value) = 0;
  
  /**
    Encodes the specified string.
  */
  virtual void write(const String& value) = 0;
  
  /**
    Encodes the specified wide string.
  */
  virtual void write(const WideString& value) = 0;
  
  /**
    Encodes the elements of the specified array.
  */
  virtual void write(const char* buffer, unsigned int size) = 0;

  /**
    Encodes the specified value.
  */
  virtual void write(char value) = 0;
  
  /**
    Encodes the elements of the specified array.
  */
  virtual void write(const short* buffer, unsigned int size) = 0;

  /**
    Encodes the specified value.
  */
  virtual void write(short value) = 0;
  
  /**
    Encodes the elements of the specified array.
  */
  virtual void write(const unsigned short* buffer, unsigned int size) = 0;

  /**
    Encodes the specified value.
  */
  virtual void write(unsigned short value) = 0;
  
  /**
    Encodes the elements of the specified array.
  */
  virtual void write(const int* buffer, unsigned int size) = 0;

  /**
    Encodes the specified value.
  */
  virtual void write(int value) = 0;
  
  /**
    Encodes the elements of the specified array.
  */
  virtual void write(const unsigned int* buffer, unsigned int size) = 0;

  /**
    Encodes the specified value.
  */
  virtual void write(unsigned int value) = 0;
  
  /**
    Encodes the elements of the specified array.
  */
  virtual void write(const long* buffer, unsigned int size) = 0;

  /**
    Encodes the specified value.
  */
  virtual void write(long value) = 0;
  
  /**
    Encodes the elements of the specified array.
  */
  virtual void write(const unsigned long* buffer, unsigned int size) = 0;

  /**
    Encodes the specified value.
  */
  virtual void write(unsigned long value) = 0;
  
  /**
    Encodes the elements of the specified array.
  */
  virtual void write(const long long* buffer, unsigned int size) = 0;

  /**
    Encodes the specified value.
  */
  virtual void write(long long value) = 0;
  
  /**
    Encodes the elements of the specified array.
  */
  virtual void write(const unsigned long long* buffer, unsigned int size) = 0;

  /**
    Encodes the specified value.
  */
  virtual void write(unsigned long long value) = 0;
  
  /**
    Encodes the elements of the specified array.
  */
  virtual void write(const float* buffer, unsigned int size) = 0;

  /**
    Encodes the specified value.
  */
  virtual void write(float value) = 0;
  
  /**
    Encodes the elements of the specified array.
  */
  virtual void write(const double* buffer, unsigned int size) = 0;

  /**
    Encodes the specified value.
  */
  virtual void write(double value) = 0;
  
  /**
    Encodes the elements of the specified array.
  */
  virtual void write(const long double* buffer, unsigned int size) = 0;

  /**
    Encodes the specified value.
  */
  virtual void write(long double value) = 0;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
