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
#include <base/xml/Node.h>
#include <base/NotSupported.h>

#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
#  include <libxml/tree.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

bool Node::isSupported(
    const String& feature,
    const String& version) const throw() {
  // TAG: fixme
  return false;
}

String Node::getName() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)context;
  if (node->type == XML_ENTITY_NODE) {
    return (const char*)((const xmlEntity*)(node->name))->name;
  } else {
    return (const char*)node->name;
  }
#else
  return String();
#endif
}

String Node::getValue() const throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)context;
  // TAG: DOMException
  return (const char*)node->content;
#else
  return String();
#endif
}

void Node::setValue(const String& value) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)context;
  // TAG: DOMException
  //return node->content;
#else
  throw DOMException(this); // TAG: cause
#endif
}

Node::NodeType Node::getType() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)context;
  xmlElementType type = node->type;
  assert(
    (type >= static_cast<xmlElementType>(ELEMENT_NODE)) &&
    (type <= static_cast<xmlElementType>(NOTATION_NODE)),
    NotSupported(this) // TAG: UnexpectedException?
  );
  return static_cast<NodeType>(type);
#else
  return Node::ELEMENT_NODE; // TAG: use unspecified
#endif
}

Node Node::getParent() throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)context;
  return node->parent;
#else
  return Node(0);
#endif
}

// Node Node::getChildNode() throw() {
// #if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
//   xmlNodePtr node = (xmlNodePtr)context;
//   return node->children;
// #else
//   return Node(0);
// #endif
// }

Node Node::getFirstChild() throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)context;
  return node->children;
#else
  return Node(0);
#endif
}

Node Node::getLastChild() throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)context;
  return node->last;
#else
  return Node(0);
#endif
}

Node Node::getPreviousSibling() throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)context;
  return node->prev;
#else
  return Node(0);
#endif
}

Node Node::getNextSibling() throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)context;
  return node->next;
#else
  return Node(0);
#endif
}

#if 0
XXX Node::getAttributes() throw() {
}
#endif

// Array<String> Node::getProperties() const throw() {
//   xmlNodePtr node = (xmlNodePtr)context;
//   return node->content;
// }

Node::ShadowDocument Node::getOwnerDocument() throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)context;
  return node->doc;
#else
  return 0;
#endif
}

Node Node::appendChild(Node child) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)context;
  return 0; // TAG: fixme
#else
  return 0;
#endif
}

bool Node::hasChildNodes() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)context;
  return false; // TAG: fixme
#else
  return false;
#endif
}

String Node::getNamespaceURI() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)context;
  return (const char*)node->ns->href;
#else
  return String();
#endif
}

String Node::getPrefix() const throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)context;
  return (const char*)node->ns->prefix;
#else
  return String();
#endif
}

String Node::getLocalName() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)context;
  return String(); // TAG: fixme
#else
  return String();
#endif
}

bool Node::hasAttributes() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)context;
  return node->properties != 0; // TAG: check
#else
  return false;
#endif
}

// remove()???
// unlink()???

// Node::~Node() throw() {
//   xmlUnlinkNode(node);
//   xmlFreeNode(node);
// }

// void Node::setPrefix(const String& prefix) const throw(DOMException) {
// #if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
//   xmlNodePtr node = (xmlNodePtr)context;
//   return node->ns;
// #else
//   return String();
// #endif
// }

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
