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
#include <base/xml/Node.h>
#include <base/NotSupported.h>
#include <base/UnexpectedFailure.h>
#include <base/string/FormatOutputStream.h>

#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
#  include <libxml2/libxml/tree.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

bool Node::isSupported(
    const String& feature,
    const String& version) const throw() {
  // TAG: fixme
  return false;
}

String Node::getName() const throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  switch (node->type) {
  case XML_ATTRIBUTE_NODE: // name of attribute
    if (node->ns && node->ns->prefix) {
      return String((const char*)node->ns->prefix) + MESSAGE(":") +
        NativeString((const char*)node->name);
    } else {
      return NativeString((const char*)node->name);
    }
  case XML_ELEMENT_NODE: // tag name
    if (node->ns && node->ns->prefix) {
      return String((const char*)node->ns->prefix) + MESSAGE(":") +
        NativeString((const char*)node->name);
    } else {
      return NativeString((const char*)node->name);
    }
  case XML_CDATA_SECTION_NODE:
    return Literal("#cdata-section");
  case XML_COMMENT_NODE:
    return Literal("#comment");
  case XML_DOCUMENT_NODE:
    return Literal("#document");
  case XML_DOCUMENT_FRAG_NODE:
    return Literal("#document-fragment");
  case XML_TEXT_NODE:
    return Literal("#text");
  case XML_ATTRIBUTE_DECL:
  case XML_DTD_NODE: // not XML_DOCUMENT_TYPE_NODE
  case XML_ELEMENT_DECL:
  case XML_ENTITY_DECL:
  case XML_ENTITY_REF_NODE:
  case XML_NOTATION_NODE:
  case XML_PI_NODE: // target
    return NativeString((const char*)node->name);
  case XML_ENTITY_NODE:
    // TAG: is this ok
    return NativeString((const char*)((const xmlEntity*)(node->name))->name);
  default:
    throw UnexpectedFailure(this);
  }
#else
  return String();
#endif
}

String Node::getValue() const throw(DOMException) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  switch (node->type) {
  case XML_ATTRIBUTE_NODE:
  case XML_CDATA_SECTION_NODE:
  case XML_COMMENT_NODE:
  case XML_PI_NODE:
  case XML_TEXT_NODE:
    return NativeString((const char*)node->content);
  case XML_DOCUMENT_NODE:
  case XML_DOCUMENT_FRAG_NODE:
  case XML_ATTRIBUTE_DECL:
  case XML_DTD_NODE: // not XML_DOCUMENT_TYPE_NODE
  case XML_ELEMENT_DECL:
  case XML_ELEMENT_NODE:
  case XML_ENTITY_DECL:
  case XML_ENTITY_REF_NODE:
  case XML_NOTATION_NODE:
  case XML_ENTITY_NODE:
    return String();
  default:
    throw UnexpectedFailure(this);
  }
#else
  return String();
#endif
}

void Node::setValue(const String& value) throw(DOMException) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  switch (node->type) {
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttr* attribute = (xmlAttr*)node;
      if (attribute->children) {
        xmlFreeNodeList(attribute->children);
      }
      attribute->children = 0;
      attribute->last = 0;
      
      xmlChar* encoded = xmlEncodeEntitiesReentrant(
        attribute->doc,
        (const xmlChar*)value.getElements()
      );
      attribute->children = xmlStringGetNodeList(attribute->doc, encoded);
      xmlNode* node = attribute->children;
      while (node) {
        node->parent = (xmlNode*)attribute;
        node->doc = attribute->doc;
        if (!node->next) {
          attribute->last = node;
        }
        node = node->next;
      }
      xmlFree(encoded);
    }
    break;
  case XML_CDATA_SECTION_NODE:
  case XML_COMMENT_NODE:
  case XML_PI_NODE:
  case XML_TEXT_NODE:
    xmlNodeSetContent(node, (const xmlChar*)value.getElements());    
    break;
  case XML_DOCUMENT_NODE:
  case XML_DOCUMENT_FRAG_NODE:
  case XML_ATTRIBUTE_DECL:
  case XML_DTD_NODE: // not XML_DOCUMENT_TYPE_NODE
  case XML_ELEMENT_DECL:
  case XML_ELEMENT_NODE:
  case XML_ENTITY_DECL:
  case XML_ENTITY_REF_NODE:
  case XML_NOTATION_NODE:
  case XML_ENTITY_NODE:
    break; // ignore according to recommendation
  default:
    throw UnexpectedFailure(this);
  }
#else
  throw DOMException(this); // TAG: cause
#endif
}

Node::NodeType Node::getType() const throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
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
  case XML_ENTITY_NODE: // TAG: XML_ENTITY_DECL?
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
  case XML_ATTRIBUTE_DECL:
    return Node::ATTRIBUTE_DECL_NODE;
  case XML_ELEMENT_DECL:
    return Node::ELEMENT_DECL_NODE;
  case XML_ENTITY_DECL:
    return Node::ENTITY_DECL_NODE;
  default:
    throw UnexpectedFailure(this);
  }
#else
  throw DOMException(this);
#endif
}

Node Node::getParent() const throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  
  switch (node->type) {
  case XML_ELEMENT_NODE:
  case XML_PI_NODE:
  case XML_COMMENT_NODE:
  case XML_DTD_NODE:
  case XML_TEXT_NODE:
  case XML_CDATA_SECTION_NODE:
  case XML_ENTITY_REF_NODE:
    return node->parent;
  default:
    return 0;
  }
#else
  throw DOMException(this);
#endif
}

Node Node::getPreviousSibling() const throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  switch (node->type) {
  case XML_ATTRIBUTE_NODE:
    // return 0; // TAG: need AttributeList class
  default:
    return node->prev;
  }
#else
  throw DOMException(this);
#endif
}

Node Node::getNextSibling() const throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  switch (node->type) {
  case XML_ATTRIBUTE_NODE:
    // return 0; // TAG: need AttributeList class
  default:
    return node->next;
  }
#else
  throw DOMException(this);
#endif
}

Node Node::getFirstChild() const throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  
  switch (node->type) {
  case XML_DOCUMENT_NODE:
  case XML_DOCUMENT_FRAG_NODE:
  case XML_ENTITY_REF_NODE:
  case XML_ELEMENT_NODE:
  case XML_ATTRIBUTE_NODE:
  case XML_ENTITY_DECL: // not XML_ENTITY_NODE
    return node->children;
  case XML_DTD_NODE: // not XML_DOCUMENT_TYPE_NODE
  case XML_PI_NODE:
  case XML_COMMENT_NODE:
  case XML_TEXT_NODE:
  case XML_CDATA_SECTION_NODE:
  case XML_NOTATION_NODE:
    return 0;
  default:
    throw UnexpectedFailure(this);
  }
#else
  throw DOMException(this);
#endif
}

Node Node::getLastChild() const throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  
  switch (node->type) {
  case XML_DOCUMENT_NODE:
  case XML_DOCUMENT_FRAG_NODE:
  case XML_ENTITY_REF_NODE:
  case XML_ELEMENT_NODE:
  case XML_ATTRIBUTE_NODE:
  case XML_ENTITY_DECL: // not XML_ENTITY_NODE
    return node->last;
  case XML_DTD_NODE: // not XML_DOCUMENT_TYPE_NODE
  case XML_PI_NODE:
  case XML_COMMENT_NODE:
  case XML_TEXT_NODE:
  case XML_CDATA_SECTION_NODE:
  case XML_NOTATION_NODE:
    return 0;
  default:
    throw UnexpectedFailure(this);
  }
#else
  throw DOMException(this);
#endif
}

Node::ShadowDocument Node::getOwnerDocument() throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  if (node->type == XML_DOCUMENT_NODE) {
    return 0;
  } else {
    return node->doc;
  }
#else
  throw DOMException(this);
#endif
}

#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
class NodeImpl {
public:

  // see section 1.1.1. of "The DOM Structure Model"
  static inline bool isAppendAble(
    const Node& _node, const Node& _child) throw() {
    const xmlNode* node = (const xmlNode*)_node.context;
    const xmlNode* child = (const xmlNode*)_child.context;
    
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
      bassert(
        parent->children,
        DOMException("Link to first child of parent not set")
      );
      bassert(
        parent->last,
        DOMException("Link to last child of parent not set")
      );
      const xmlNode* temp = nullptr;
      for (xmlNode* child = parent->children; child; child = child->next) {
        if (child == node) {
          temp = child;
        }
      }
      bassert(
        temp,
        DOMException("Link to node missing from children of parent")
      );
    }
    
    // check links from previous sibling to node
    if (node->prev) {
      bassert(
        node->prev->next == node,
        DOMException("Link from previous sibling to node is invalid")
      );
    } else {
      bassert(
        !parent || (parent->children == node),
        DOMException("Link from first child of parent to node is invalid")
      );
    }
    
    // check links from next sibling to node
    if (node->next) {
      bassert(
        node->next->prev == node,
        DOMException("Link from next sibling to node is invalid")
      );
    } else {
      bassert(
        !parent || (parent->last == node),
        DOMException("Link from last child of parent to node is invalid")
      );
    }
    
    // check links from children to node
    if (node->children) {
      for (const xmlNode* child = node->children; child; child = child->next) {
        bassert(
          child->parent == node,
          DOMException("Link from child to node is invalid")
        );
      }
      bassert(
        node->children->prev == 0,
        DOMException("First child has invalid link to previous sibling")
      );
      bassert(
        node->last->next == 0,
        DOMException("Last child has invalid link to next sibling")
      );
    } else { // no children
      bassert(
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
    BASSERT(isUnlinked(_node));
  }
};
#endif

Node Node::appendChild(Node _child) throw(DOMException) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  xmlNode* child = (xmlNode*)_child.context;
  
  bassert(node && child, NullPointer(Type::getType<NodeImpl>()));
  
  bassert(
    child->doc == node->doc,
    bindCause(DOMException(this), DOMException::WRONG_DOCUMENT)
  );
  
  bassert(
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
  
  return child;
#else
  throw DOMException(this);
#endif
}

Node Node::insertBefore(Node _newChild, Node _refChild) throw(DOMException) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
	xmlNode* node = (xmlNode*)context;
	xmlNode* newChild = (xmlNode*)_newChild.context;
	xmlNode* refChild = (xmlNode*)_refChild.context;

	if (!refChild) {
    return appendChild(newChild);
  }
  
  bassert(node && newChild, NullPointer(Type::getType<NodeImpl>()));
  
  bassert(refChild->parent == node, DOMException(this));
  
  bassert(
    newChild->doc == node->doc,
    bindCause(DOMException(this), DOMException::WRONG_DOCUMENT)
  );
  
  bassert(
    NodeImpl::isAppendAble(*this, newChild),
    bindCause(DOMException(this), DOMException::HIERARCHY_REQUEST)
  );

  NodeImpl::unlink(newChild);
  
  if (newChild->type == XML_DOCUMENT_FRAG_NODE) {
    if (newChild->children && newChild->last) {
      for (xmlNode* i = newChild->children; i; i = i->next) {
        i->parent = node;
      }
      if (refChild->prev) {
        refChild->prev->next = newChild->children;
        newChild->children->prev = refChild->prev;
      } else {
        node->children = newChild->children;
        newChild->children->prev = 0;
      }
      refChild->prev = newChild->last;
      newChild->last->next = refChild;
    }
    
    newChild->children = 0;
    newChild->last = 0;
  } else {
    if (refChild->prev) {
      refChild->prev->next = newChild;
      newChild->prev = refChild->prev;
    } else {
      node->children = newChild;
      newChild->prev = 0;
    }
    refChild->prev = newChild;
    newChild->next = refChild;
    newChild->parent = node;
  }
  
  return newChild;
#else
  throw DOMException(this);
#endif
}

Node Node::removeChild(Node _oldChild) throw(DOMException) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  xmlNode* oldChild = (xmlNode*)_oldChild.context;
  bassert(oldChild->parent == node, DOMException(this));
  NodeImpl::unlink(oldChild);
  return oldChild;
#else
  throw DOMException(this);
#endif
}

Node Node::replaceChild(Node newChild, Node oldChild) throw(DOMException) {
  insertBefore(newChild, oldChild);
	removeChild(oldChild);
  return oldChild;
}

bool Node::hasChildNodes() const throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  const xmlNode* node = (const xmlNode*)context;
  
  switch (node->type) {
  case XML_DOCUMENT_NODE:
  case XML_DOCUMENT_FRAG_NODE:
  case XML_ENTITY_REF_NODE:
  case XML_ELEMENT_NODE:
  case XML_ATTRIBUTE_NODE:
  case XML_ENTITY_DECL: // not XML_ENTITY_NODE
    return node->children;
  case XML_DTD_NODE: // not XML_DOCUMENT_TYPE_NODE
  case XML_PI_NODE:
  case XML_COMMENT_NODE:
  case XML_TEXT_NODE:
  case XML_CDATA_SECTION_NODE:
  case XML_NOTATION_NODE:
    return false;
  default:
    throw UnexpectedFailure(this);
  }
#else
  throw DOMException(this);
#endif
}

String Node::getNamespaceURI() const throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  if (node->ns &&
      ((node->type == XML_ELEMENT_NODE) ||
       (node->type == XML_ATTRIBUTE_NODE))) {
    return NativeString((const char*)node->ns->href);
  } else {
    return String();
  }
#else
  throw DOMException(this);
#endif
}

String Node::getPrefix() const throw(DOMException) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  return NativeString((const char*)node->ns->prefix);
  if (node->ns &&
      ((node->type == XML_ELEMENT_NODE) ||
       (node->type == XML_ATTRIBUTE_NODE))) {
    return NativeString((const char*)node->ns->prefix);
  } else {
    return String();
  }
#else
  throw DOMException(this);
#endif
}

void Node::setPrefix(const String& prefix) throw(DOMException) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;

  if (node->ns &&
      ((node->type == XML_ELEMENT_NODE) ||
       (node->type == XML_ATTRIBUTE_NODE))) {
    const char* prefix = (const char*)node->ns->prefix;
    const char* uri = (const char*)node->ns->href;
    // TAG: what is uri ends with '/'
		if (!uri ||
				((compare(prefix, "xml", sizeof("xml")) == 0) &&
				 (compare(uri, "http://www.w3.org/XML/1998/namespace", sizeof("http://www.w3.org/XML/1998/namespace")) != 0)) ||
				((node->type == XML_ATTRIBUTE_NODE) &&
				 (compare(prefix, "xmlns", sizeof("xmlns") == 0)) &&
				 (compare(uri, "http://www.w3.org/2000/xmlns", sizeof("http://www.w3.org/2000/xmlns")) != 0)) ||
				((node->type == XML_ATTRIBUTE_NODE) &&
				 (compare((const char*)node->name, "xmlns", sizeof("xmlns")) == 0))) {
			throw bindCause(DOMException(this), DOMException::NAMESPACE_ERROR);
		}
		xmlNs* ns = xmlNewNs(0, (const xmlChar*)uri, (const xmlChar*)prefix);
    node->ns = ns;
	}
#else
  throw DOMException(this);
#endif
}

String Node::getLocalName() const throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  if (/*node->ns &&*/ // TAG: should ns be ignored
      ((node->type == XML_ELEMENT_NODE) ||
       (node->type == XML_ATTRIBUTE_NODE))) {
		return NativeString((const char*)node->name);
  } else {
    return String();
  }
#else
  throw DOMException(this);
#endif
}

bool Node::hasAttributes() const throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)context;
  return (node->type == XML_ELEMENT_NODE) && (node->properties != 0);
#else
  throw DOMException(this);
#endif
}

// remove()???
// unlink()???

bool Node::isUnlinked() const throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  return NodeImpl::isUnlinked(*this);
#else
  throw DOMException(this);
#endif
}

bool Node::hasSameOwner(const Node& _node) const throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  const xmlNode* a = (const xmlNode*)getContext();
  const xmlNode* b = (const xmlNode*)_node.getContext();
  return a->doc == b->doc;
#else
  throw DOMException(this);
#endif
}

bool Node::isAncestor(const Node& _child) const throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  const xmlNode* node = (const xmlNode*)getContext();
  const xmlNode* child  = (const xmlNode*)_child.getContext();
  
  while (node) {
    if (node == child) {
      return true;
    }
    child = child->parent;
  }
  return false;
#else
  return false;
#endif
}

// TAG: should cloneNode() be const?
Node Node::cloneNode(bool deep) throw(DOMException) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)getContext();
  
  switch (node->type) {
  case XML_DOCUMENT_NODE:
    {
      xmlDoc* result = xmlCopyDoc((xmlDoc*)node, deep ? 1 : 0);
      bassert(result, DOMException(this));
      return result;
    }
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttr* attribute = xmlCopyProp(0, (xmlAttr*)node);
      attribute->doc = node->doc;
      return attribute;
    }
  case XML_ENTITY_DECL: // not XML_ENTITY_NODE
  case XML_NOTATION_NODE:
  case XML_DTD_NODE: // not XML_DOCUMENT_TYPE_NODE
    throw bindCause(DOMException(this), DOMException::NOT_SUPPORTED);
	default:
    xmlNode* result = xmlDocCopyNode(node, node->doc, deep ? 1 : 0);
    bassert(result, DOMException(this));
    return result;
  }
#else
  throw DOMException(this);
#endif
}

Node::~Node() throw() {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  // TAG: need reference counting!
#if 0
  xmlNode* node = (xmlNode*)getContext();
  if (false && !node->t) {
    switch (node->type) {
    case XML_ELEMENT_NODE:
      xmlFreeNode(node);
      break;
    case XML_ATTRIBUTE_NODE:
      xmlFreeProp((xmlAttr*)node);
      break;
    case XML_TEXT_NODE:
      xmlFreeNode(node);
      break;
    case XML_CDATA_SECTION_NODE:
      xmlFreeNode(node);
      break;
    case XML_ENTITY_REF_NODE:
      xmlFreeNode(node);
      break;
    case XML_ENTITY_NODE:
      xmlFreeNode(node);
      break;
    case XML_PI_NODE:
      xmlFreeNode(node);
      break;
    case XML_COMMENT_NODE:
      xmlFreeNode(node);
      break;
    case XML_DOCUMENT_NODE:
      // xmlFreeDoc((xmlDoc*)node);
      break;
    case XML_DTD_NODE: // not XML_DOCUMENT_TYPE_NODE
      // xmlFreeDtd((xmlDtd*)node);
      break;
    case XML_DOCUMENT_FRAG_NODE:
      // xmlFreeNode(node);
      break;
    case XML_NOTATION_NODE:
      xmlFreeNode(node);
      break;
    case XML_ATTRIBUTE_DECL:
      xmlFreeNode(node);
      break;
    case XML_ELEMENT_DECL:
      xmlFreeNode(node);
      break;
    case XML_ENTITY_DECL:
      xmlFreeNode(node);
      break;
    default:
      throw UnexpectedFailure(this);
    }
  }
#endif
#else
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
