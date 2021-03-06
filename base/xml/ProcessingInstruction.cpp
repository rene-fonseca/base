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
#include <base/xml/ProcessingInstruction.h>
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
#  include <libxml/tree.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

String ProcessingInstruction::getTarget() const noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  const xmlNode* node = (const xmlNode*)getContext();
  return NativeString((const char*)node->name);
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

String ProcessingInstruction::getData() const noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  const xmlNode* node = (const xmlNode*)getContext();
  return NativeString((const char*)node->content);
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

void ProcessingInstruction::setData(const String& value)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlNode* node = (xmlNode*)getContext();
  xmlNodeSetContent(node, (const xmlChar*)value.getElements());
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
