/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__NODE_H
#define _DK_SDU_MIP__BASE_XML__NODE_H

#include <base/AutomationObject.h>
#include <base/string/String.h>
#include <base/xml/DOMException.h>
#include <base/xml/NamedNodeMap.h>
#include <base/CastException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @defgroup xml XML
*/

class Document;

/**
  DOM node.
  
  @short DOM node.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Node : public AutomationObject {
  friend class Document;
public:
  
  /** Leaf node types. */
  enum NodeType {
    ELEMENT_NODE = 1,
    ATTRIBUTE_NODE,
    TEXT_NODE,
    CDATA_SECTION_NODE,
    ENTITY_REFERENCE_NODE,
    ENTITY_NODE,
    PROCESSING_INSTRUCTION_NODE,
    COMMENT_NODE,
    DOCUMENT_NODE,
    DOCUMENT_TYPE_NODE,
    DOCUMENT_FRAGMENT_NODE,
    NOTATION_NODE
  };
  
  class ShadowDocument : public AutomationObject {
  private:
    
    void* context;
  public:
    
    inline ShadowDocument(void* _context) throw() : context(_context) {
    }
    
    inline ShadowDocument(const ShadowDocument& copy) throw()
      : context(copy.context) {
    }
    
    inline ShadowDocument& operator=(const ShadowDocument& eq) throw() {
      context = eq.context;
      return *this;
    }
    
    inline Node getNode() const throw() {
      return context;
    }
  };
  
  class ShadowElement : public AutomationObject {
  private:
    
    void* context;
  public:
    
    inline ShadowElement(void* _context) throw() : context(_context) {
    }
    
    inline ShadowElement(const ShadowElement& copy) throw()
      : context(copy.context) {
    }
    
    inline ShadowElement& operator=(const ShadowElement& eq) throw() {
      context = eq.context;
      return *this;
    }
    
    inline Node getNode() const throw() {
      return context;
    }
  };
private:

  /** Context. */
  void* context;
protected:
  
  /**
    Initializes node.
  */
  inline Node(void* _context) throw() : context(_context) {
  }

  /**
    Returns the context.
  */
  inline void* getContext() const throw() {
    return context;
  }
public:
  
  /**
    Initializes XML node from other XML node.
  */
  inline Node(const Node& copy) throw() : context(copy.context) {
  }
  
  /**
    Assignment of node by node.
  */
  inline Node& operator=(const Node& eq) throw() {
    context = eq.context;
    return *this;
  }

  /**
    Adds the node to the end of the list of children of this node.
  */
  Node appendChild(Node child) throw(DOMException);

  /**
    A NamedNodeMap containing the attributes of this node (if it is an Element)
    or an invalid NamedNodeMap otherwise.
  */
  //NamedNodeMap getAttributes() throw();

  /**
    Returns a NodeList that contains all children of this node.
  */
  // NodeList getChildNodes() throw();
  
  /**
    Returns a duplicate of this node.
  */
  Node cloneNode(bool deep) throw(DOMException);
  
  /**
    Returns the first child node.
  */
  Node getFirstChild() throw();
  
  /**
    Returns the last child node.
  */
  Node getLastChild() throw();
  
  /**
    Returns the local part of the qualified name of this node.
  */
  String getLocalName() const throw();
  
  /**
    The namespace URI of this node, or an improper string if it is unspecified.
  */
  String getNamespaceURI() const throw();
  
  /**
    Returns the node immediately succeeding this node.
  */
  Node getNextSibling() throw();
  
  /**
    Returns the name of the node.
  */
  String getName() const throw();
  
  /**
    Returns the type of the node.
  */
  NodeType getType() const throw();
  
  /**
    Returns the value of the node.
  */
  String getValue() const throw(DOMException);

  /**
    Returns the Document object associated with this node.
  */
  ShadowDocument getOwnerDocument() throw();

  /**
    Returns the parent node.
  */
  Node getParent() throw();

  /**
    The namespace prefix of this node, or an improper string if it is
    unspecified.
  */
  String getPrefix() const throw(DOMException);
  
  /**
    Returns the node immediately preceding this node.
  */
  Node getPreviousSibling() throw();
  
  /**
    Returns whether this node (if it is an element) has any attributes.
  */
  bool hasAttributes() const throw();
  
  /**
    Returns whether this node has any children.
  */
  bool hasChildNodes() const throw();

  /**
    Inserts the node newChild before the existing child reference node.
  */
  Node insertBefore(Node child, Node reference) throw(DOMException);

  /**
    Tests whether the DOM implementation implements a specific feature and that
    feature is supported by this node.
  */
  bool isSupported(const String& feature, const String& version) const throw();

  /**
    Puts all Text nodes in the full depth of the sub-tree underneath this Node,
    including attribute nodes, into a "normal" form where only structure (e.g.
    elements, comments, processing instructions, CDATA sections, and entity
    references) separates Text nodes, i.e., there are neither adjacent Text
    nodes nor empty Text nodes.
  */
  void normalize() throw(DOMException);

  /**
    Removes the child node indicated by child from the list of children, and
    returns it.
  */
  Node removeChild(Node child) throw(DOMException);

  /**
    Replaces the child node oldChild with newChild in the list of children, and
    returns the oldChild node.
  */
  Node replaceChild(Node newChild, Node oldChild) throw(DOMException);
  
  /**
    Sets the value of the node.
  */
  void setValue(const String& value) throw(DOMException);

  /**
    Sets the prefix.
  */
  void setPrefix(const String& prefix) throw(DOMException);
  
  /**
    Returns true if the node has been initialized.
  */
  inline bool isValid() const throw() {
    return context;
  }
  
  /**
    Returns true if the node has been initialized.
  */
  inline bool isInvalid() const throw() {
    return !context;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
