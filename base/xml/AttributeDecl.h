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

#include <base/xml/Declaration.h>
#include <base/collection/Array.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class Document;
class ElementDecl;

/**
  @short XML attribute declaration node.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API AttributeDecl : public Declaration {
  friend class Document;
  friend class ElementDecl;
protected:
  
  /**
    Initializes attribute declaration.
  */
  inline AttributeDecl(void* context) noexcept : Declaration(context) {
  }
public:
  
  enum ValueType {
    CDATA,
    ID,
    IDREF,
    IDREFS,
    ENTITY,
    ENTITIES,
    NMTOKEN,
    NMTOKENS,
    ENUMERATION,
    NOTATION
  };
  
  enum DefaultType {
    NONE,
    REQUIRED,
    IMPLIED,
    FIXED
  };
  
  inline AttributeDecl(Node node) throw(CastException)
    : Declaration(node) {
    bassert(
      node.isInvalid() || (node.getType() == ATTRIBUTE_DECL_NODE),
      CastException(this)
    );
  }
  
  inline AttributeDecl(Declaration node) throw(CastException)
    : Declaration(node) {
    bassert(
      node.isInvalid() || (node.getType() == ATTRIBUTE_DECL_NODE),
      CastException(this)
    );
  }  
  
  /**
    Returns the type of the attribute.
  */
  ValueType getValueType() const throw();
  
  /**
    Returns the default value constraint.
  */
  DefaultType getDefaultValueType() const throw();
  
  /**
    Returns the default value.
  */
  String getDefaultValue() const throw();
  
  /**
    Returns the enumeration value. Only valid if type is ATTRIBUTE_ENUMERATION.
  */
  Array<String> getEnumerationValues() const throw(DOMException);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
