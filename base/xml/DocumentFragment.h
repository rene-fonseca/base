/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__DOCUMENT_FRAGMENT_H
#define _DK_SDU_MIP__BASE_XML__DOCUMENT_FRAGMENT_H

#include <base/xml/Node.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Document;

/**
  DocumentFragment is a "lightweight" or "minimal" Document object. It is very
  common to want to be able to extract a portion of a document's tree or to
  create a new fragment of a document. Imagine implementing a user command like
  cut or rearranging a document by moving fragments around. It is desirable to
  have an object which can hold such fragments and it is quite natural to use a
  Node for this purpose. While it is true that a Document object could fulfill
  this role, a Document object can potentially be a heavyweight object,
  depending on the underlying implementation. What is really needed for this is
  a very lightweight object. DocumentFragment is such an object.
  
  @short DOM document fragment node.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class DocumentFragment : public Node {
  friend class Document;
protected:

  inline DocumentFragment(void* context) throw() : Node(context) {
  }
public:

  inline DocumentFragment(Node node) throw(CastException) : Node(node) {
    assert(
      node.isInvalid() || (node.getType() == DOCUMENT_FRAGMENT_NODE),
      CastException(this)
    );
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif