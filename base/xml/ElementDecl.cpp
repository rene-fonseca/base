/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/xml/ElementDecl.h>

#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
#  include <libxml2/libxml/tree.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

ElementDecl::ValueType ElementDecl::getValueType() const throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlElement* element = (xmlElement*)getContext();
  switch (element->etype) {
  case XML_ELEMENT_TYPE_EMPTY:
    return ElementDecl::EMPTY;
  case XML_ELEMENT_TYPE_ANY:
    return ElementDecl::ANY;
  case XML_ELEMENT_TYPE_MIXED:
    return ElementDecl::MIXED;
  case XML_ELEMENT_TYPE_ELEMENT:
    return ElementDecl::ELEMENT;
  default:
    throw DOMException(this);
  }
#else
  throw DOMException(this);
#endif
}

bool ElementDecl::hasAttributes() const throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlElement* element = (xmlElement*)getContext();
  return element->attributes;
#else
  throw DOMException(this);
#endif
}

AttributeDecl ElementDecl::getFirstAttribute() throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlElement* element = (xmlElement*)getContext();
  return element->attributes;
#else
  throw DOMException(this);
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
