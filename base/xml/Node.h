/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/AutomationObject.h>
#include <base/string/String.h>
#include <base/xml/DOMException.h>
#include <base/xml/NamedNodeMap.h>
#include <base/CastException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  @defgroup xml XML
*/

class Document;
class NodeImpl;

/**
  DOM node.
  
  @short DOM node.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Node : public AutomationObject {
  friend class Document;
  friend class NodeImpl;
public:
  
  /** Leaf node types. */
  enum NodeType {
    ELEMENT_NODE,
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
    NOTATION_NODE,
    ATTRIBUTE_DECL_NODE,
    ELEMENT_DECL_NODE,
    ENTITY_DECL_NODE
  };

  /*
    Unspecified internal context.
  */
  struct Context {
  };
  
  class _COM_AZURE_DEV__BASE__API ShadowDocument : public AutomationObject {
  private:
    
    void* context = nullptr;
  public:
    
    inline ShadowDocument(void* _context) noexcept : context(_context) {
    }
    
    inline ShadowDocument(const ShadowDocument& copy) noexcept
      : context(copy.context) {
    }
    
    inline ShadowDocument& operator=(const ShadowDocument& assign) noexcept {
      context = assign.context;
      return *this;
    }
    
    inline Node getNode() const noexcept {
      return context;
    }
  };
  
  class _COM_AZURE_DEV__BASE__API ShadowElement : public AutomationObject {
  private:
    
    void* context = nullptr;
  public:
    
    inline ShadowElement(void* _context) noexcept : context(_context) {
    }
    
    inline ShadowElement(const ShadowElement& copy) noexcept
      : context(copy.context) {
    }
    
    inline ShadowElement& operator=(const ShadowElement& assign) noexcept {
      context = assign.context;
      return *this;
    }
    
    inline Node getNode() const noexcept {
      return context;
    }
  };
private:

  /** Context. */
  void* context = nullptr;
  /** Specifies if access is confined to read-only access. */
  bool readonly = false; // TAG: fixme
protected:
  
  /**
    Initializes node.
  */
  inline Node(void* _context) noexcept : context(_context) {
  }

  /**
    Returns the context for modifying access.
  */
  inline void* getContext() noexcept {
    return context;
  }
  
  /**
    Returns the context for non-modifiable access.
  */
  inline const void* getContext() const noexcept {
    return context;
  }
public:
  
  /**
    Initializes XML node as invalid.
  */
  inline Node() noexcept {
  }
  
  /**
    Initializes XML node from other XML node.
  */
  inline Node(const Node& copy) noexcept : context(copy.context) {
  }
  
  /**
    Assignment of node by node.
  */
  inline Node& operator=(const Node& assign) noexcept {
    if (&assign != this) {
      context = assign.context;
      readonly = assign.readonly;
    }
    return *this;
  }

  /**
    Adds the node to the end of the list of children of this node.

    @return The node being appended.
  */
  Node appendChild(Node child);

  /**
    Inserts the node newChild before the existing child node refChild. If
    refChild is invalid, insert newChild at the end of the list of children. If
    newChild is a DocumentFragment object, all of its children are inserted, in
    the same order, before refChild. If the newChild is already in the tree, it
    is first removed.

    @return The node being inserted.
  */
  Node insertBefore(Node newChild, Node refChild);

  /**
    Removes the child node indicated by child from the list of children, and
    returns it.
  */
  Node removeChild(Node child);
  
  /**
    Replaces the child node oldChild with newChild  in the list of children,
    and returns the oldChild node.
  */
  Node replaceChild(Node newChild, Node oldChild);
  
  /**
    Returns a duplicate of this node.
  */
  Node cloneNode(bool deep);
  
  /**
    Returns the local part of the qualified name of this node.
  */
  String getLocalName() const noexcept;
  
  /**
    The namespace URI of this node, or an improper string if it is unspecified.
  */
  String getNamespaceURI() const noexcept;
  
  /**
    Returns the name of the node.
  */
  String getName() const noexcept;
  
  /**
    Returns the type of the node.
  */
  NodeType getType() const noexcept;
  
  /**
    Returns the value of the node.
  */
  String getValue() const;

  /**
    Returns the Document object associated with this node.
  */
  ShadowDocument getOwnerDocument() noexcept;

  /**
    The namespace prefix of this node, or an improper string if it is
    unspecified.
  */
  String getPrefix() const;

  /**
    Sets the prefix.
  */
  void setPrefix(const String& prefix);
  
  /**
    Returns the parent node.
  */
  Node getParent() const noexcept;
  
  /**
    Returns the node immediately preceding this node.
  */
  Node getPreviousSibling() const noexcept;

  /**
    Returns the node immediately succeeding this node.
  */
  Node getNextSibling() const noexcept;

  /**
    Returns the first child node.
  */
  Node getFirstChild() const noexcept;
  
  /**
    Returns the last child node.
  */
  Node getLastChild() const noexcept;
  
  /**
    Returns whether this node (if it is an element) has any attributes.
  */
  bool hasAttributes() const noexcept;
  
  /**
    Returns whether this node has any children.
  */
  bool hasChildNodes() const noexcept;
  
  /**
    Tests whether the DOM implementation implements a specific feature and that
    feature is supported by this node.
  */
  bool isSupported(const String& feature, const String& version) const noexcept;

  /**
    Puts all Text nodes in the full depth of the sub-tree underneath this Node,
    including attribute nodes, into a "normal" form where only structure (e.g.
    elements, comments, processing instructions, CDATA sections, and entity
    references) separates Text nodes, i.e., there are neither adjacent Text
    nodes nor empty Text nodes.
  */
  void normalize();
  
  /**
    Sets the value of the node.
  */
  void setValue(const String& value);
  
  /**
    Returns true if the node has been initialized.
  */
  inline bool isValid() const noexcept {
    return context;
  }
  
  /**
    Returns true if the node has been initialized.
  */
  inline bool isInvalid() const noexcept {
    return !context;
  }

  /**
    Returns true if the node is unlinked.
  */
  bool isUnlinked() const noexcept;
  
  /**
    Returns true if the nodes refer to the the same node.
  */
  inline bool isSame(const Node& node) const noexcept {
    return context == node.context;
  }
  
  /**
    Returns true if the nodes refer to the the same node.
  */
  inline bool operator==(const Node& node) const noexcept {
    return context == node.context;
  }
  
  /**
    Returns true if the specified node has the same owner as this node.
  */
  bool hasSameOwner(const Node& node) const noexcept;

  /**
    Returns true if this node is an ancestor of the specified node.
  */
  bool isAncestor(const Node& node) const noexcept;
  
  /**
    Returns true if the node is valid.
  */
  inline operator bool() const noexcept {
    return context;
  }

  /**
    Destroys the handle. The node is destroyed when detached.
  */
  ~Node() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
