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
#include <base/xml/ProcessingInstruction.h>

#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
#  include <libxml2/libxml/tree.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

String ProcessingInstruction::getTarget() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  const xmlNode* node = (const xmlNode*)getContext();
  return NativeString((const char*)node->name);
#else
  throw DOMException(this);
#endif
}

String ProcessingInstruction::getData() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  const xmlNode* node = (const xmlNode*)getContext();
  return NativeString((const char*)node->content);
#else
  throw DOMException(this);
#endif
}

void ProcessingInstruction::setData(const String& value) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)getContext();
  xmlNodeSetContent(node, (const xmlChar*)value.getElements());
#else
  throw DOMException(this);
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
