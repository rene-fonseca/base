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

#include <base/rmi/idl/IDLObject.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Interface Definition Language (IDL) method.
  
  @short IDL method.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API IDLMethod : public IDLObject {
private:
  
  /** The result type of the method. */
  IDLType result;
  /** The arguments of the method. */
  Array<IDLType> arguments;
  /** The exceptions raised by the method. */
  Set<IDLException> raises;
public:
  
  /**
    Initializes the IDL method with a void return type.
    
    @param name The name of the method.
  */  
  IDLMethod(const String& name) noexcept;
  
  /**
    Initializes the IDL method.
    
    @param name The name of the method.
    @param type The return type of the method.
  */
  IDLMethod(const String& name, IDLType type) noexcept;
  
  /**
    Adds an argument to the argument list.
  */
  void addArgument(IDLType type);
  
  /**
    Adds an exception to the exception specification.
  */
  void addException(IDLException exception);
  
  /**
    Returns true if the method is static.
  */
  bool isStatic() const noexcept;
  
  /**
    Returns true if the method is non-modifying.
  */
  bool isNonModifying() const noexcept;
  
  /**
    Returns true if the method has the specified exception in its exception
    specification.
  */
  bool raises(IDLException exception) const noexcept;
  
  /**
    Returns true if this method is compatible with the specified method.
  */
  bool isCompatible(const IDLMethod& method) const noexcept;
  
  /**
    Destroys the IDL method.
  */
  ~IDLMethod() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
