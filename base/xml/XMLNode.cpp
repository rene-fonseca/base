/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/xml/XMLNode.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

XMLNode::XMLNode(const String& n) throw()
  : initialized(!n.isEmpty()), content(false), name(n) {
}

XMLNode::XMLNode(const String& n, const String& c) throw()
  : initialized(!n.isEmpty()), content(!c.isEmpty()), name(n), contentId(c) {
}

XMLNode::XMLNode(const XMLNode& copy) throw()
  : initialized(copy.initialized),
    content(copy.content),
    name(copy.name),
    contentId(copy.contentId),
    children(copy.children),
    properties(copy.properties) {
}

void XMLNode::setContent(const String& content) throw() {
  contentId = content;
  this->content = !content.isEmpty();
}

XMLNode* XMLNode::addChild(const String& name) throw() {
  XMLNode* result = new XMLNode(name);
  return addChild(result);
}

XMLNode* XMLNode::addChild(XMLNode* node) throw() {
//  assert(node, InvalidNode());

//  _children.insert(_children.end(), n);
  return node;
}

XMLNode* XMLNode::addContent(const String& content) throw() {
  XMLNode* node = new XMLNode(String(), content);
  return addChild(node);
}

void XMLNode::removeChild(XMLNode* node) throw() {
  if (node) {
//    _children.remove(n);
  }
}

XMLProperty* XMLNode::getProperty(const String& name) throw() {
  // TAG: need to use hash value
  List<XMLProperty>::Enumerator enu = properties.getEnumerator();
  while (enu.hasNext()) {
    XMLProperty* attribute = enu.next();
    if (attribute->getName() == name) {
      return attribute;
    }
  }
  return 0; // not found
}

void XMLNode::addProperty(const String& name, const String& value) throw() {
  List<XMLProperty>::ReadEnumerator enu = properties.getReadEnumerator();
  while (enu.hasNext()) {
    assert(enu.next()->getName() != name, Exception("Attribute already exists"));
  }
  properties.append(XMLProperty(name, value));
}

void XMLNode::removeProperty(const String& name) throw() {
  List<XMLProperty>::Enumerator enu = properties.getEnumerator();
  while (enu.hasNext()) {
    if (enu.next()->getName() == name) {
      // remove node
    }
  }
  throw Exception("Attribute doesn't exist");
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
