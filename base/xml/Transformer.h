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

#include <base/Object.h>
#include <base/collection/HashTable.h>
#include <base/xml/Stylesheet.h>
#include <base/xml/TransformerException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

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
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Transformer : public Object {
private:

  /** Parameters. */
  HashTable<String, String> parameters;
  /** The stylesheet. */
  Stylesheet stylesheet;
public:
  
  /**
    Initializes XSLT processor.
  */
  Transformer() noexcept;
#if 0
  /**
    Initializes document from other document.
  */
  inline Transformer(const Transformer& copy) noexcept
    : transformer(copy.transformer) {
  }
  
  /**
    Assignment of transformer by transformer.
  */
  inline Transformer& operator=(const Transformer& assign) noexcept {
    transformer = assign.transformer;
    return *this;
  }
#endif
  
  /**
    Clears all parameters.
  */
  void clearParameters() noexcept;

  /**
    Returns the value of the parameter.
  */
  String getParameter(const String& name);

  /**
    Sets the parameter.
  */
  void setParameter(
    const String& name, const String& value);
  
  /**
    Transforms the source into the result tree.

    @param document The XML document to be transformed.
  */
  Document transform(const Document& document);

  /**
    Save the document to the specified file.

    @param filename The name of the file.
    @param document The XML document.
  */
  void save(
    const String& filename,
    const Document& document);

  /**
    Returns the stylesheet.
  */
  Stylesheet getStylesheet() noexcept;

  /**
    Sets the stylesheet.
  */
  void setStylesheet(Stylesheet stylesheet) noexcept;

  /**
    Returns true is the function is available.
  */
  bool functionAvailable(
    const String& ns, const String& name);
  
  // URIResolver
  
  /**
    Destroys the transformer.
  */
  ~Transformer() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
