/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__DOM_IMPLEMENTATION_H
#define _DK_SDU_MIP__BASE_XML__DOM_IMPLEMENTATION_H

#include <base/Object.h>
#include <base/xml/Document.h>
#include <base/xml/DocumentType.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Document Object Model (DOM) implementation.
  
  @short DOM implementation.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class DOMImplementation : public Object {
public:

  /**
    Initializes DOM implementation.
  */
  inline DOMImplementation() throw() {
  }
  
  inline DOMImplementation(const DOMImplementation& copy) throw() {
  }

  inline DOMImplementation& operator=(const DOMImplementation& eq) throw() {
    return *this;
  }
  
  /**
    Returns true if the feature is available.
  */
  bool hasFeature(const String& name, const String& version) throw();

  /**
    Creates a document type.
  */
  DocumentType createDocumentType(
    const String& qualifiedName,
    const String& publicId,
    const String& systemId) throw(DOMException);
  
  /**
    Creates a document.
  */
  Document createDocument(
    const String& namespaceURI,
    const String& qualifiedName,
    const DocumentType& doctype) throw(DOMException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
