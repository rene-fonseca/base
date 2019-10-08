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
#include <base/collection/Array.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Interface Definition Language (IDL) structure.
  
  @short IDL structure.
  @ingroup rmi
  @version 1.0
*/

class IDLStructure : public IDLNamedType {
private:
  
  /** The fields of the structure. */
  Array<IDLField> fields;
public:
  
  /**
    Initializes the IDL structure.
  */
  IDLStructure(const String& name) throw();
  
  /**
    Adds the field to the structure.
  */
  void addField(const IDLField& field) throw();
  
  /**
    Returns the fields.
  */
  Array<IDLField> getFields() const throw();
  
  /**
    Destroys the IDL structure.
  */
  ~IDLStructure() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
