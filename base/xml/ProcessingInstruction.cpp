/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/xml/ProcessingInstruction.h>

#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
#  include <libxml/tree.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

String ProcessingInstruction::getTarget() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  return (const char*)node->name; // TAG: check
#else
  throw DOMException(this);
#endif
}

String ProcessingInstruction::getData() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  return (const char*)node->content; // TAG: check
#else
  throw DOMException(this);
#endif
}

void ProcessingInstruction::setData(const String& value) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  xmlNodeSetContent(node, (const xmlChar*)value.getElements());
  // TAG: how to detect failure
#else
  throw DOMException(this);
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
