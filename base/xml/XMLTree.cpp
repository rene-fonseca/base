/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/xml/XMLTree.h>

#if defined(_DK_SDU_MIP__BASE__XML_GNOME)
  #include <gnome-xml/parser.h>
  #include <stdlib.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if defined(_DK_SDU_MIP__BASE__XML_GNOME)

class XMLTreeImpl {
public:

static XMLNode* buildInternalTree(xmlNodePtr node) throw() {
  String name;
  String content;

  xmlNodePtr child;
  XMLNode* temp;

  name = (node->name) ? static_cast<char*>(node->name) : String();
  temp = new XMLNode(name);

  for (xmlAttrPtr attr = node->properties; attr; attr = attr->next) {
    name = static_cast<char*>(attr->name);
    content = String();
//    if (attr->children) {
//      content = static_cast<char*>(attr->children->content);
//    }
    temp->addProperty(name, content);
  }

  temp->setContent(node->content ? static_cast<char*>(node->content) : String());

  for (child = node->childs; child; child = child->next) {
    temp->addChild(buildInternalTree(child));
  }

  return temp;
}

static void buildExternalTree(xmlDocPtr doc, XMLNode* n, xmlNodePtr p, bool root) {
  xmlNodePtr node;

  if (root) {
    node = doc->root = xmlNewDocNode(doc, 0, static_cast<xmlChar*>(n->getName()).getElements(), static_cast<xmlChar*>(n->getContent().getElements()));
  } else {
    node = xmlNewChild(p, 0, static_cast<xmlChar*>(n->getName().getElements()), static_cast<xmlChar*>(n->getContent().getElements()));
  }

  if (n->isContent()) {
    node->type = XML_TEXT_NODE;
    xmlNodeSetContentLen(node, static_cast<const xmlChar*>(n->getContent().getElements()), n->getContent().getLength());
  }

  List<XMLProperty> props = n->getProperties();
//  for(curprop = props.begin(); curprop != props.end(); ++curprop) {
//    xmlSetProp(node, static_cast<xmlChar*>((*curprop)->getName().getElements()), static_cast<xmlChar*>((*curprop)->getValue().getElements()));
//  }

  List<XMLNode> children = n->getChildren();
//  for (curchild = children.begin(); curchild != children.end(); ++curchild) {
//    writenode(doc, *curchild, node, false);
//  }
}

};

XMLTree::XMLTree() throw() : initialized(false), root(0) {
}

XMLTree::XMLTree(const XMLTree& copy) throw() : initialized(copy.initialized), root(0) {
  if (copy.root) {
    root = new XMLNode(*copy.getRoot());
  }
}

void XMLTree::setRoot(XMLNode* node) throw() {
  if (root) {
    delete root;
  }
  root = node;
}

XMLTree::~XMLTree() throw() {
  if (initialized && root) {
    delete root;
  }
}


/*
int XMLTree::set_compression(int c) {
  if(c > 9)
    c = 9;

  if(c < 0)
    c = 0;

  _compression = c;

  return _compression;
}

bool XMLTree::read(void) {
  xmlDocPtr doc;

  if(_root) {
    delete _root;
    _root = NULL;
  }

  xmlKeepBlanksDefault(0);

  doc = xmlParseFile(_filename.c_str());
  if(!doc) {
    _initialized = false;
    return false;
  }

  _root = readnode(xmlDocGetRootElement(doc));
  xmlFreeDoc(doc);
  _initialized = true;

  return true;
}
*/

void XMLTree::release() throw() {
  if (root) {
    delete root;
    root = 0;
    initialized = false;
  }
}

bool XMLTree::read(const String& buffer) throw(MemoryException) {
  release();
  xmlDocPtr doc = xmlParseMemory(static_cast<char*>(buffer.getElements()), buffer.getLength()); // TAG: wrong type of first arg - should be const

  if (!doc) { // check for error
    return false;
  }

  root = XMLTreeImpl::buildInternalTree(xmlDocGetRootElement(doc));
  xmlFreeDoc(doc);
  initialized = true;
  return true;
}

bool XMLTree::write(String& buffer) const throw(MemoryException) {
  char* buf;
  int size;
  xmlDocPtr doc;
  List<XMLNode> children;

  xmlKeepBlanksDefault(0);
  doc = xmlNewDoc(static_cast<xmlChar*>("1.0"));
  xmlSetDocCompressMode(doc, 0);
  XMLTreeImpl::buildExternalTree(doc, root, doc->root, true);
  xmlDocDumpMemory(doc, static_cast<xmlChar**>(&buf), &size);
  xmlFreeDoc(doc);

//  retval = buf;
// copy to string
  free(buf);
  return false;
};

#else // no xml support

void XMLTree::release() throw() {}
XMLTree::XMLTree() throw() {}
XMLTree::XMLTree(const XMLTree& copy) throw() {}
void XMLTree::setRoot(XMLNode* node) throw() {}
bool XMLTree::read(const String& buffer) throw(MemoryException) {}
bool XMLTree::write(String& buffer) const throw(MemoryException) {}
XMLTree::~XMLTree() throw() {}

#endif

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
