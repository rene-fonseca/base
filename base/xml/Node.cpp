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
#include <base/UnexpectedFailure.h>
#include <base/string/FormatOutputStream.h>

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
  xmlNode* node = (xmlNode*)context;
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
  xmlNode* node = (xmlNode*)context;
  // TAG: DOMException
  return (const char*)node->content;
#else
  return String();
#endif
}

void Node::setValue(const String& value) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  // TAG: DOMException
  //return node->content;
#else
  throw DOMException(this); // TAG: cause
#endif
}

Node::NodeType Node::getType() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  switch (node->type) {
  case XML_ELEMENT_NODE:
    return Node::ELEMENT_NODE;
  case XML_ATTRIBUTE_NODE:
    return Node::ATTRIBUTE_NODE;
  case XML_TEXT_NODE:
    return Node::TEXT_NODE;
  case XML_CDATA_SECTION_NODE:
    return Node::CDATA_SECTION_NODE;
  case XML_ENTITY_REF_NODE:
    return Node::ENTITY_REFERENCE_NODE;
  case XML_ENTITY_NODE:
    return Node::ENTITY_NODE;
  case XML_PI_NODE:
    return Node::PROCESSING_INSTRUCTION_NODE;
  case XML_COMMENT_NODE:
    return Node::COMMENT_NODE;
  case XML_DOCUMENT_NODE:
    return Node::DOCUMENT_NODE;
  case XML_DTD_NODE: // not XML_DOCUMENT_TYPE_NODE
    return Node::DOCUMENT_TYPE_NODE;
  case XML_DOCUMENT_FRAG_NODE:
    return Node::DOCUMENT_FRAGMENT_NODE;
  case XML_NOTATION_NODE:
    return Node::NOTATION_NODE;
  default:
    throw UnexpectedFailure(this);
  }
#else
  throw DOMException(this);
#endif
}

Node Node::getParent() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  return node->parent;
#else
  return 0;
#endif
}

Node Node::getPreviousSibling() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  return node->prev;
#else
  return 0;
#endif
}

Node Node::getNextSibling() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  return node->next;
#else
  return 0;
#endif
}

Node Node::getFirstChild() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  return node->children;
#else
  return 0;
#endif
}

Node Node::getLastChild() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  return node->last;
#else
  return 0;
#endif
}

Node::ShadowDocument Node::getOwnerDocument() throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  return node->doc;
#else
  return 0;
#endif
}

class NodeImpl {
public:

  // see section 1.1.1. of "The DOM Structure Model"
  static inline bool isAppendAble(
    const Node& _node, const Node& _child) throw() {
    xmlNode* node = (xmlNode*)_node.context;
    xmlNode* child = (xmlNode*)_child.context;
    
    switch (node->type) {
    case XML_DTD_NODE:
    case XML_PI_NODE:
    case XML_COMMENT_NODE:
    case XML_TEXT_NODE:
    case XML_CDATA_SECTION_NODE:
    case XML_NOTATION_NODE:
      return false;
    case XML_DOCUMENT_NODE:
      switch (child->type) {
      case XML_ELEMENT_NODE: // maximum of one
        if (xmlDocGetRootElement((xmlDoc*)node)) {
          return false; // document already has root element
        }
      case XML_PI_NODE:
        break;
      case XML_COMMENT_NODE:
        break;
      case XML_DTD_NODE: // maximum of one
        if (((xmlDoc*)node)->intSubset != 0) {
          WRITE_SOURCE_LOCATION();
          return false; // document already has document type node
        }
      default:
        return false;
      }
      break;
    case XML_DOCUMENT_FRAG_NODE:
    case XML_ENTITY_REF_NODE:
    case XML_ELEMENT_NODE:
    case XML_ENTITY_NODE:
      switch (child->type) {
      case XML_ELEMENT_NODE: // maximum of one
      case XML_PI_NODE:
      case XML_COMMENT_NODE:
      case XML_DTD_NODE: // maximum of one
      case XML_TEXT_NODE:
      case XML_CDATA_SECTION_NODE:
      case XML_ENTITY_REF_NODE:
        break;
      default:
        return false;
      }
      break;
    case XML_ATTRIBUTE_NODE:
      if ((child->type != XML_TEXT_NODE) &&
          (child->type != XML_ENTITY_REF_NODE)) {
        return false;
      }
      break;
    default:
      throw UnexpectedFailure(Type::getType<NodeImpl>());
    }
    
		while (node) {
			if (node == child) {
				return false;
			}
			node = node->parent;
		}
    return true;
  }

  // TAG: use this while debugging
  static inline void checkNode(const Node& _node) throw(DOMException) {
    const xmlNode* node = (const xmlNode*)_node.getContext();
    const xmlNode* parent = node->parent;

    // check links from parent to node
    if (parent) {
      assert(
        parent->children,
        DOMException("Link to first child of parent not set")
      );
      assert(
        parent->last,
        DOMException("Link to last child of parent not set")
      );
      const xmlNode* temp = 0;
      for (xmlNode* child = parent->children; child; child = child->next) {
        if (child == node) {
          temp = child;
        }
      }
      assert(
        temp,
        DOMException("Link to node missing from children of parent")
      );
    }
    
    // check links from previous sibling to node
    if (node->prev) {
      assert(
        node->prev->next == node,
        DOMException("Link from previous sibling to node is invalid")
      );
    } else {
      assert(
        !parent || (parent->children == node),
        DOMException("Link from first child of parent to node is invalid")
      );
    }
    
    // check links from next sibling to node
    if (node->next) {
      assert(
        node->next->prev == node,
        DOMException("Link from next sibling to node is invalid")
      );
    } else {
      assert(
        !parent || (parent->last == node),
        DOMException("Link from last child of parent to node is invalid")
      );
    }
    
    // check links from children to node
    if (node->children) {
      for (const xmlNode* child = node->children; child; child = child->next) {
        assert(
          child->parent == node,
          DOMException("Link from child to node is invalid")
        );
      }
      assert(
        node->children->prev == 0,
        DOMException("First child has invalid link to previous sibling")
      );
      assert(
        node->last->next == 0,
        DOMException("Last child has invalid link to next sibling")
      );
    } else { // no children
      assert(
        node->last == 0,
        DOMException("Last child is not 0")
      );
    }
  }
  
  static inline bool isUnlinked(const Node& _node) throw() {
    const xmlNode* node = (const xmlNode*)_node.getContext();
    return (node->parent == 0); // && (node->prev == 0) && (node->next == 0);
  }
  
  static inline void unlink(Node _node) throw() {
    xmlNode* node = (xmlNode*)_node.getContext();
    if (node->parent) {
      xmlNode* parent = node->parent;
      if (node->prev) {
        node->prev->next = node->next;
      } else {
        parent->children = node->next;
      }
      if (node->next) {
        node->next->prev = node->prev;
      } else {
        parent->last = node->prev;
      }
      node->prev = 0;
      node->next = 0;
      node->parent = 0;
    }
    
    NodeImpl::checkNode(_node); // TAG: fixme
    ASSERT(isUnlinked(_node));
  }
};

Node& Node::appendChild(Node _child) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  xmlNode* child = (xmlNode*)_child.context;
  
  assert(node && child, NullPointer(Type::getType<NodeImpl>()));
  
  assert(
    node->doc == child->doc,
    bindCause(DOMException(this), DOMException::WRONG_DOCUMENT)
  );
  
  assert(
    NodeImpl::isAppendAble(*this, _child),
    bindCause(DOMException(this), DOMException::HIERARCHY_REQUEST)
  );
  
  NodeImpl::unlink(child);

  if (child->type == XML_DOCUMENT_FRAG_NODE) {
    if (child->children && child->last) {
      for (xmlNode* i = child->children; i; i = i->next) {
        i->parent = node;
      }
      if (node->last) {
        node->last->next = child->children;
        child->children->prev = node->last;
      } else {
        node->children = child->children;
        child->children->prev = 0;
      }
      // child->last->next = 0;
      node->last = child->last;
    }
  } else {
    if (node->last) {
      node->last->next = child;
      child->prev = node->last;
    } else {
      node->children = child;
      // child->prev = 0;
    }
    child->parent = node;
    // child->next = 0;
    node->last = child;
  }
  
  NodeImpl::checkNode(*this); // TAG: fixme
  
  return *this;
#else
  return 0;
#endif
}

bool Node::hasChildNodes() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  return node->children;
#else
  return false;
#endif
}

String Node::getNamespaceURI() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  return (const char*)node->ns->href;
#else
  return String();
#endif
}

String Node::getPrefix() const throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  return (const char*)node->ns->prefix;
#else
  return String();
#endif
}

String Node::getLocalName() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  return String(); // TAG: fixme
#else
  return String();
#endif
}

bool Node::hasAttributes() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  return node->properties != 0; // TAG: check
#else
  return false;
#endif
}

// remove()???
// unlink()???

// Node::~Node() throw() {
//   xmlFreeNode(node);
// }

// void Node::setPrefix(const String& prefix) const throw(DOMException) {
// #if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
//   xmlNode* node = (xmlNode*)context;
//   return node->ns;
// #else
//   return String();
// #endif
// }

bool Node::isUnlinked() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  return NodeImpl::isUnlinked(*this);
#else
  return false;
#endif
}

bool Node::hasSameOwner(const Node& _node) const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  const xmlNode* a = (const xmlNode*)getContext();
  const xmlNode* b = (const xmlNode*)_node.getContext();
  return a->doc == b->doc;
#else
  return false;
#endif
}

// bool Node::isAncestor(const Node& _node) const throw() {
// #if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
//   const xmlNode* node = (const xmlNode*)getContext();
//   const xmlNode* b = (const xmlNode*)_node.getContext();
  
//   while (node) {
//     node = node->parent;
//   }
// #else
//   return false;
// #endif
// }

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
