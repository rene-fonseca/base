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
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
#  include <libxml/tree.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

ElementDecl::ValueType ElementDecl::getValueType() const
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
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
    _throw DOMException(this);
  }
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

bool ElementDecl::hasAttributes() const noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlElement* element = (xmlElement*)getContext();
  return element->attributes;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

AttributeDecl ElementDecl::getFirstAttribute() noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlElement* element = (xmlElement*)getContext();
  return element->attributes;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
