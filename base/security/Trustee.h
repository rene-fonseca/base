/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_SECURITY__TRUSTEE_H
#define _DK_SDU_MIP__BASE_SECURITY__TRUSTEE_H

#include <base/string/String.h>
#include <base/mem/ReferenceCountedAllocator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A trustee (e.g. user or group).
  
  @short Trustee
  @ingroup security
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Trustee : public Object {
public:

  /** The type of trustee. */
  enum TrusteeType {
    UNSPECIFIED, /**< Trustee is unspecified. */
    USER, /**< Trustee is a user. */
    GROUP, /**< Trustee is a group. */
    CLASS, /**< Trustee is a class. */
    EVERYONE /**< Everyone. */
  };
private:

  /** Cached type of trustee. */
  mutable TrusteeType type;
  /** Identifier of the trustee represented as an integral. */
  unsigned long integralId;
  /** Opaque identifier of the trustee. */
  ReferenceCountedObjectPointer<ReferenceCountedAllocator<char> > id;
public:
  
  /**
    This exception is raised by the Trustee class to indicate a non-existent trustee.

    @short Trustee exception
    @ingroup exceptions security
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  
  class TrusteeException : public Exception {
  public:
    
    /**
      Initializes the exception object with no message.
    */
    inline TrusteeException() throw() {}

    /**
      Initializes the exception object.

      @param message The message.
    */
    inline TrusteeException(const char* message) throw() : Exception(message) {}

    /**
      Initializes the exception object without an associated message.

      @param type The identity of the type.
    */
    inline TrusteeException(Type type) throw() : Exception(type) {}

    /**
      Initializes the exception object.

      @param message An NULL-terminated string (ASCII).
      @param type The identity of the type.
    */
    inline TrusteeException(const char* message, Type type) throw() : Exception(message, type) {}
  };
  
  /**
    Initializes trustee as invalid.
  */
  Trustee() throw();
  
  /**
    Initializes trustee by id. Raises OutOfDomain if the specified id is not
    supported by the platform.
  */
  Trustee(TrusteeType type, const void* id) throw(OutOfDomain);
  
  /**
    Initializes trustee from other trustee.
  */
  Trustee(const Trustee& copy) throw();
  
  /**
    Assignment of trustee by trustee.
  */
  Trustee& operator=(const Trustee& eq) throw();
  
  /**
    Initializes trustee by name.
  */
  Trustee(const String& name) throw(TrusteeException);
  
  /**
    Returns true if the trustees are identical. The method returns true if both
    trustees are invalid.
  */
  bool operator==(const Trustee& eq) throw();
  
  /**
    Returns false if the trustees are not identical. The method returns false if
    both trustees are invalid.
  */
  inline bool operator!=(const Trustee& eq) throw() {
    return !(*this == eq);
  }
  
  /**
    Returns true if the trustee is initialized. This does not mean that the
    trustee exists.
  */
  bool isInitialized() const throw();
  
  /**
    Returns type of the trustee.
  */
  TrusteeType getType() const throw(TrusteeException);
  
  /**
    Returns true if the specified trustee is a member of this trustee (if a
    group) and otherwise returns false (also if trustee is currently INVALID).
    Raises TrusteeException if group could not be enumerated.
  */
  bool isMemberOf(const Trustee& trustee) const throw(TrusteeException);
  
  /**
    Returns the short name of the trustee.
  */
  String getName() const throw(TrusteeException);
  
  /**
    Returns the full name of the trustee.
  */
  String getFullName() const throw(TrusteeException); 
  
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const Trustee& value) throw(IOException);
};

/**
  Writes the trustee id to the format output stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Trustee& value) throw(IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
