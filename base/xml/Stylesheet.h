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
#include <base/collection/Array.h>
#include <base/xml/Document.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Stylesheet.
  
  @short Stylesheet.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Stylesheet : public Object {
  friend class Transformer;
private:

  class _COM_AZURE_DEV__BASE__API StylesheetImpl : public ReferenceCountedObject {
  private:

    /** The context. */
    void* context = nullptr;
  public:
    
    /**
      Initializes invalid stylesheet.
    */
    StylesheetImpl(void* context) noexcept;

    /**
      Returns the context.
    */
    inline void* getContext() const noexcept
    {
      return context;
    }

    /**
      Destroys the stylesheet.
    */
    ~StylesheetImpl() noexcept;
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
  Stylesheet();
  
  /**
    Initializes the stylesheet.

    @param filename The name of the file.
  */
  Stylesheet(const String& filename);
  
  /**
    Initializes the stylesheet.

    @param document The XML document.
  */
  Stylesheet(const Document& document);

  /**
    Initializes stylesheet from other stylesheet.
  */
  inline Stylesheet(const Stylesheet& copy) noexcept
    : stylesheet(copy.stylesheet)
  {
  }
  
  /**
    Assignment of stylesheet by stylesheet.
  */
  inline Stylesheet& operator=(const Stylesheet& assign) noexcept
  {
    stylesheet = assign.stylesheet;
    return *this;
  }

  //OutputType getOutputType() const;
  
  String getOutputMethod() const;
  String getNamespace() const;
  String getVersion() const;
  String getEncoding() const;
  bool omitXmlDeclaration() const;
  
  String getPublicId() const;
  String getSystemId() const;
  String getMediaType() const;
  bool isStandalone() const;
  bool indent() const;

  Array<String> getExcludedPrefixes() const;
  
  //bool isOutputMethodSet() const;
  //Array<String> getExtensions() const;
  
  // Array<?> getStylesheetPIs(const Document& document);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
