/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__XML_NODE_H
#define _DK_SDU_MIP__BASE_XML__XML_NODE_H

#include <base/Object.h>
#include <base/string/String.h>
#include <base/collection/List.h>
#include <base/xml/XMLProperty.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Node in a XML tree.

  @short XML node.
  @see XMLTree
  @author René Møller Fonseca
*/

class XMLNode : public Object {
private:

  /** Specifies whether this node has been initialized. */
  bool initialized;
  /** Specifies whether this node is a content node. */
  bool content;
  /** The name of the node. */
  String name;
  /** The content id. */
  String contentId;
  /** The child nodes of this node. */
  List<XMLNode> children;
  /** The attributes of this node. */
  List<XMLProperty> properties;
//  XMLPropertyHash prophash;
public:

  /**
    Initializes XML node with the speicified name.

    @param name The desired name of the node.
  */
  XMLNode(const String& name) throw();

  /**
    Initializes XML node with the speicified name and content id.

    @param name The desired name of the node.
    @param content The desired content id of the node.
  */
  XMLNode(const String& name, const String& content) throw();

  /**
    Initializes XML node from other XML node.
  */
  XMLNode(const XMLNode& copy) throw();

  /**
    Returns true if the node is a content node.
  */
  inline bool isContent() const throw() {return content;}

  /**
    Returns true if the node has been initialized.
  */
  inline bool isInitialized() const throw() {return initialized;}

  /**
    Returns the content id of the node.
  */
  inline const String& getContent() const throw() {return contentId;}

  /**
    Returns the name of the node.
  */
  inline const String& getName() const throw() {return name;}

  /**
    Sets the content of the node.
  */
  void setContent(const String& content) throw();

  /**
    Returns all the children nodes.
  */
  inline const List<XMLNode>& XMLNode::getChildren() const throw() {return children;}

  /**
    Adds a child the this node.
  */
  XMLNode* addChild(const String& node) throw();

  /**
    Adds a child the this node.
  */
  XMLNode* addChild(XMLNode* node) throw();

  /**
    Adds a content node to this node.
  */
  XMLNode* addContent(const String& name) throw();

  /**
    Removes a child node from this node.
  */
  void removeChild(XMLNode* node) throw();

  /**
    Returns the properties of the node.
  */
  inline const List<XMLProperty>& getProperties() const throw() {return properties;}

  /**
    Returns the specified attribute.

    @return 0 if the attribute doesn't exist.
  */
  XMLProperty* getProperty(const String& name) throw();

  /**
    Returns the specifies attribute.
  */
  inline const XMLProperty* getProperty(const String& name) const throw() {
    return const_cast<XMLNode*>(this)->getProperty(name); // TAG: fix this
  }

  /**
    Adds the specified attrbute to this node. Throws XMLException if the attribute already exists.

    @param name The name of the property.
    @param value The value of the property.
  */
  void addProperty(const String& name, const String& value) throw();

  /**
    Removes the specified property from the node.

    @param name The name of the property.
  */
  void removeProperty(const String& name) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
