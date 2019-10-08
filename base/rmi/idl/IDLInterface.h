/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/rmi/idl/IDLNamedType.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Interface Definition Language (IDL) interface.
  
  @short IDL interface.
  @ingroup rmi
  @version 1.0
*/

class IDLInterface : public IDLNamedType {
private:

  /** The methods of the interface. */
  HashSet<IDLMethod> methods;
  /** The version of the interface. */
  unsigned int version;
public:
  
  /**
    Initializes the IDL interface.
  */
  IDLInterface(const String& name, unsigned int version) throw();
  
  /**
    Adds the specified method.
  */
  void addMethod(const IDLMethod& method) throw();
  
  /**
    Returns true if the method is available.
  */
  bool hasMethod(const String& name) const throw();

  /**
    Returns the specified method.
  */
  IDLMethod getMethod(const String& name) const throw(InvalidKey);

  /**
    Returns the methods.
  */
  HashSet<IDLMethod> getMethods() const throw();

  /**
    Returns true if the interfaces are compatible.
  */
  bool isCompatible(const IDLInterface& interface) const throw();
  
  /**
    Destroys the IDL interface.
  */
  ~IDLInterface() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
