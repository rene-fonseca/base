/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__TRANSFORMER_H
#define _DK_SDU_MIP__BASE_XML__TRANSFORMER_H

#include <base/Object.h>
#include <base/collection/HashTable.h>
#include <base/xml/Stylesheet.h>
#include <base/xml/TransformerException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// ErrorListener
// Method Summary
//  void error(TransformerException exception)
//           Receive notification of a recoverable error.
//  void fatalError(TransformerException exception)
//           Receive notification of a non-recoverable error.
//  void warning(TransformerException exception)
//           Receive notification of a warning.

/**
  XSLT transformer.
  
  @short XSLT transformer.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Transformer : public Object {
private:

  /** Parameters. */
  HashTable<String, String> parameters;
  /** The stylesheet. */
  Stylesheet stylesheet;
public:
  
  /**
    Initializes XSLT processor.
  */
  Transformer() throw();
#if 0
  /**
    Initializes document from other document.
  */
  inline Transformer(const Transformer& copy) throw()
    : transformer(copy.transformer) {
  }
  
  /**
    Assignment of transformer by transformer.
  */
  inline Transformer& operator=(const Transformer& eq) throw() {
    transformer = eq.transformer;
    return *this;
  }
#endif
  
  /**
    Clears all parameters.
  */
  void clearParameters() throw();

  /**
    Returns the value of the parameter.
  */
  String getParameter(const String& name) throw(InvalidKey);

  /**
    Sets the parameter.
  */
  void setParameter(
    const String& name, const String& value) throw(MemoryException);
  
  /**
    Transforms the source into the result tree.

    @param document The XML document to be transformed.
  */
  Document transform(const Document& document) throw(TransformerException);

  /**
    Save the document to the specified file.

    @param filename The name of the file.
    @param document The XML document.
  */
  void save(
    const String& filename,
    const Document& document) throw(DOMException, IOException);

  /**
    Returns the stylesheet.
  */
  Stylesheet getStylesheet() throw();

  /**
    Sets the stylesheet.
  */
  void setStylesheet(Stylesheet stylesheet) throw();

  /**
    Returns true is the function is available.
  */
  bool functionAvailable(
    const String& ns, const String& name) throw(TransformerException);
  
  // URIResolver
  
  /**
    Destroys the transformer.
  */
  ~Transformer() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif