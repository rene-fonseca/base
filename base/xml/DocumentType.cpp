/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/xml/DocumentType.h>

#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
#  include <libxml2/libxml/tree.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

String DocumentType::getName() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDtd* documentType = (xmlDtd*)getContext();
  return NativeString((const char*)documentType->name);
#else
  throw DOMException(this);
#endif
}

NamedNodeMap DocumentType::getEntities() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDtd* documentType = (xmlDtd*)getContext();
  return documentType->entities;
#else
  throw DOMException(this);
#endif
}

NamedNodeMap DocumentType::getNotations() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDtd* documentType = (xmlDtd*)getContext();
  return documentType->notations;
#else
  throw DOMException(this);
#endif
}

String DocumentType::getPublicId() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDtd* documentType = (xmlDtd*)getContext();
  return NativeString((const char*)documentType->ExternalID);
#else
  throw DOMException(this);
#endif
}

String DocumentType::getSystemId() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDtd* documentType = (xmlDtd*)getContext();
  return NativeString((const char*)documentType->SystemID);
#else
  throw DOMException(this);
#endif
}

String DocumentType::getInternalSubset() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
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
  throw DOMException(this);
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
