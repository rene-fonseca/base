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
#include <base/xml/Attribute.h>
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
#  include <libxml/tree.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

String Attribute::getName() const noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlAttr* attribute = (xmlAttr*)getContext();
  if (attribute->ns && attribute->ns->prefix) {
    return String((const char*)attribute->ns->prefix) +
      MESSAGE(":") + ((const char*)attribute->name);
  } else {
    return NativeString((const char*)attribute->name);
  }
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

String Attribute::getValue() const noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlAttr* attribute = (xmlAttr*)getContext();
  xmlChar* content = xmlNodeGetContent((xmlNode*)attribute);
  String result((const char*)content);
  xmlFree(content);
  return result;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

void Attribute::setValue(const String& value)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlAttr* attribute = (xmlAttr*)getContext();
  if (attribute->children) {
    xmlFreeNodeList(attribute->children);
  }
  attribute->children = 0;
  attribute->last = 0;
  
  xmlChar* encoded = xmlEncodeEntitiesReentrant(
    attribute->doc,
    (const xmlChar*)value.getElements()
  );
  attribute->children = xmlStringGetNodeList(attribute->doc, encoded);
  xmlNode* node = attribute->children;
  while (node) {
    node->parent = (xmlNode*)attribute;
    node->doc = attribute->doc;
    if (!node->next) {
      attribute->last = node;
    }
    node = node->next;
  }
  xmlFree(encoded);
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

bool Attribute::isSpecified() const noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlAttr* attribute = (xmlAttr*)getContext();
  if (attribute) {
  }
  return true; // TAG: fixme
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

Node::ShadowElement Attribute::getOwnerElement() noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlAttr* attribute = (xmlAttr*)getContext();
  return Node::ShadowElement(attribute->parent);
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
