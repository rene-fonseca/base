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
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
#  include <libxml/tree.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

String DocumentType::getName() const noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDtd* documentType = (xmlDtd*)getContext();
  return NativeString((const char*)documentType->name);
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

NamedNodeMap DocumentType::getEntities() const noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDtd* documentType = (xmlDtd*)getContext();
  return documentType->entities;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

NamedNodeMap DocumentType::getNotations() const noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDtd* documentType = (xmlDtd*)getContext();
  return documentType->notations;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

String DocumentType::getPublicId() const noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDtd* documentType = (xmlDtd*)getContext();
  return NativeString((const char*)documentType->ExternalID);
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

String DocumentType::getSystemId() const noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDtd* documentType = (xmlDtd*)getContext();
  return NativeString((const char*)documentType->SystemID);
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

String DocumentType::getInternalSubset() const
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDtd* documentType = (xmlDtd*)getContext();
  documentType = documentType->doc->intSubset;
  xmlOutputBuffer* buffer = xmlAllocOutputBuffer(0);
 	bassert(buffer, DOMException(this));
 	xmlNodeDumpOutput(buffer, 0, (xmlNode*)documentType, 0, 0, 0);
 	xmlOutputBufferFlush(buffer);
 	// TAG: String result((const char*)buffer->buffer->content, buffer->buffer->use);
  String result;
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
 	xmlOutputBufferClose(buffer);
  return result;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
