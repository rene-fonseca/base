/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__STYLESHEET_H
#define _DK_SDU_MIP__BASE_XML__STYLESHEET_H

#include <base/Object.h>
#include <base/collection/Array.h>
#include <base/xml/Document.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Stylesheet.
  
  @short Stylesheet.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Stylesheet : public Object {
  friend class Transformer;
private:

  class StylesheetImpl : public ReferenceCountedObject {
  private:

    /** The context. */
    void* context;
  public:
    
    /**
      Initializes invalid stylesheet.
    */
    StylesheetImpl(void* context) throw();

    /**
      Returns the context.
    */
    inline void* getContext() const throw() {
      return context;
    }

    /**
      Destroys the stylesheet.
    */
    ~StylesheetImpl() throw();
  };
  
  /**
    The stylesheet.
  */
  Reference<StylesheetImpl> stylesheet;
public:

  /** The output type. */
  enum OutputType {
    OUTPUT_XML, /**< XML. */
    OUTPUT_HTML, /**< HTML. */
    OUTPUT_TEXT /**< Text. */
  };
  
  /**
    Initializes stylesheet.
  */
  Stylesheet() throw();
  
  /**
    Initializes the stylesheet.

    @param filename The name of the file.
  */
  Stylesheet(const String& filename) throw();
  
  /**
    Initializes the stylesheet.

    @param document The XML document.
  */
  Stylesheet(const Document& document) throw();

  /**
    Initializes stylesheet from other stylesheet.
  */
  inline Stylesheet(const Stylesheet& copy) throw()
    : stylesheet(copy.stylesheet) {
  }
  
  /**
    Assignment of stylesheet by stylesheet.
  */
  inline Stylesheet& operator=(const Stylesheet& eq) throw() {
    stylesheet = eq.stylesheet;
    return *this;
  }

  //OutputType getOutputType() const throw();
  
  String getOutputMethod() const throw();
  String getNamespace() const throw();
  String getVersion() const throw();
  String getEncoding() const throw();
  bool omitXmlDeclaration() const throw();
  
  String getPublicId() const throw();
  String getSystemId() const throw();
  String getMediaType() const throw();
  bool isStandalone() const throw();
  bool indent() const throw();

  Array<String> getExcludedPrefixes() const throw();
  
  //bool isOutputMethodSet() const throw();
  //Array<String> getExtensions() const throw();
  
  // Array<?> getStylesheetPIs(const Document& document) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif