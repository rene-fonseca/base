/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/xml/Transformer.h>
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
#  include <libxml/xmlmemory.h>
#  include <libxml/xmlIO.h>
#  include <libxml/DOCBparser.h>
#  include <libxml/xinclude.h>
#  include <libxml/catalog.h>
#  include <libxml/tree.h>
#  include <libxslt/xslt.h>
#  include <libxslt/xsltInternals.h>
#  include <libxslt/functions.h>
#  include <libxslt/transform.h>
#  include <libxslt/xsltutils.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Transformer::Transformer() noexcept {
}

void Transformer::clearParameters() noexcept {
  parameters = HashTable<String, String>();
}

String Transformer::getParameter(const String& name) {
  return parameters[name];
}

void Transformer::setParameter(
  const String& name, const String& value) {
  parameters.add(name, value);
}

// bool Transformer::getXInclude() const noexcept {
// #if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
//   return xsltGetXIncludeDefault() != 0;
// #endif
// }

// void Transformer::setXInclude(bool value) noexcept {
// #if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
//   return xsltSetXIncludeDefault(value ? 1 : 0);
// #endif
// }

Document Transformer::transform(
  const Document& document) {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
  Allocator<const char*> temp(parameters.getSize() * 2 + 1);
  Allocator<const char*>::Iterator i = temp.getBeginIterator();
  
  HashTable<String, String>::ReadEnumerator enu =
    parameters.getReadEnumerator();
  while (enu.hasNext()) {
    const HashTable<String, String>::HashTableAssociation* node = enu.next();
    *i++ = node->getKey()->getElements();
    *i++ = node->getValue()->getElements();
  }
  *i++ = 0; // terminate
  
  xmlDoc* doc = xsltApplyStylesheet(
    Cast::pointer<xsltStylesheet*>(stylesheet.stylesheet->getContext()),
    Cast::pointer<xmlDoc*>(document.getContext()),
    temp.getElements()
  );
  bassert(doc, TransformerException(this));
  return doc;
#else
  _throw TransformerException(this);
#endif
}

void Transformer::save(const String& filename, const Document& document) {
#if 0 && defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)document.getContext();
  int bytesWritten = xsltSaveResultToFilename(
    filename.getElements(),
    doc,
    Cast::pointer<xsltStylesheet*>(stylesheet.stylesheet->getContext()),
    0
  );
  bassert(bytesWritten >= 0, IOException(this));
#endif
}

Stylesheet Transformer::getStylesheet() noexcept {
  return stylesheet;
}

void Transformer::setStylesheet(Stylesheet stylesheet) noexcept {
  this->stylesheet = stylesheet;
}

bool Transformer::functionAvailable(
  const String& ns, const String& name) {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
//   return xsltXPathFunctionLookup(
//     0,
//     name.getElements(),
//     ns.getElements()
//   ) != 0; // FIXME
  return false;
#else
  return false;
#endif
}

Transformer::~Transformer() noexcept {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
