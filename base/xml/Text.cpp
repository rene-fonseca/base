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
#include <base/xml/Text.h>

#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
#  include <libxml2/libxml/tree.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Text Text::splitText(unsigned int index) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)getContext();
  
  String content((const char*)node->content);
  String newContent(content.substring(index));
  content.removeFrom(index);
  
  xmlNode* newNode = xmlNewText(
    Cast::pointer<const xmlChar*>(newContent.getElements())
  );
  bassert(newNode, DOMException(this));
  
	xmlNodeSetContent(node, (const xmlChar*)content.getElements());
  
  xmlNode* parent = node->parent;
  if (parent) {
    newNode->parent = node->parent;
    if (parent->last == node) {
      parent->last = newNode;
    }
  }
  
  // insert after original node
  newNode->next = node->next;
  newNode->prev = node;
  node->next->prev = newNode;
  node->next = newNode;
  
  return newNode;
#else
  return 0;
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
