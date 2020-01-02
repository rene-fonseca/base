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
#include <base/xml/DocumentType.h>

#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
#  include <libxml2/libxml/tree.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

String DocumentType::getName() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlDtd* documentType = (xmlDtd*)getContext();
  return NativeString((const char*)documentType->name);
#else
  _throw DOMException(this);
#endif
}

NamedNodeMap DocumentType::getEntities() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlDtd* documentType = (xmlDtd*)getContext();
  return documentType->entities;
#else
  _throw DOMException(this);
#endif
}

NamedNodeMap DocumentType::getNotations() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlDtd* documentType = (xmlDtd*)getContext();
  return documentType->notations;
#else
  _throw DOMException(this);
#endif
}

String DocumentType::getPublicId() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlDtd* documentType = (xmlDtd*)getContext();
  return NativeString((const char*)documentType->ExternalID);
#else
  _throw DOMException(this);
#endif
}

String DocumentType::getSystemId() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlDtd* documentType = (xmlDtd*)getContext();
  return NativeString((const char*)documentType->SystemID);
#else
  _throw DOMException(this);
#endif
}

String DocumentType::getInternalSubset() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlDtd* documentType = (xmlDtd*)getContext();
  documentType = documentType->doc->intSubset;
  xmlOutputBuffer* buffer = xmlAllocOutputBuffer(0);
 	assert(buffer, DOMException(this));
 	xmlNodeDumpOutput(buffer, 0, (xmlNode*)documentType, 0, 0, 0);
 	xmlOutputBufferFlush(buffer);
 	String result((const char*)buffer->buffer->content, buffer->buffer->use);
 	xmlOutputBufferClose(buffer);
  return result;
#else
  _throw DOMException(this);
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
