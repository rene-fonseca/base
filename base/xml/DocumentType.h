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

#include <base/xml/Node.h>
#include <base/xml/NamedNodeMap.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Declaration;
class Document;
class DOMImplementation;

/**
  Each Document has a doctype attribute whose value is either null or a
  DocumentType node. The DocumentType clas in the DOM Core provides an
  interface to the list of entities that are defined for the document, and
  little else because the effect of namespaces and the various XML schema
  efforts on DTD representation are not clearly understood as of this writing.
  
  @short DOM document type node.
  @ingroup xml
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API DocumentType : public Node {
  friend class Declaration;
  friend class Document;
  friend class DOMImplementation;
protected:

  /**
    Initializes the document type node.
  */
  inline DocumentType(void* context) throw() : Node(context) {
  }
public:

  /**
    Initializes the document type node from a node.
  */
  inline DocumentType(Node node) throw(CastException) : Node(node) {
    bassert(
      node.isInvalid() || (node.getType() == DOCUMENT_TYPE_NODE),
      CastException(this)
    );
  }
  
  /**
    Returns the name.
  */
  String getName() const throw();
  
  /**
    Returns the entities.
  */
  NamedNodeMap getEntities() const throw();
  
  /**
    Returns the notations.
  */
  NamedNodeMap getNotations() const throw();
  
  /**
    Returns the public identifier.
  */
  String getPublicId() const throw();
  
  /**
    Returns the system identifier.
  */
  String getSystemId() const throw();
  
  /**
    Returns the internal subset.
  */
  String getInternalSubset() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
