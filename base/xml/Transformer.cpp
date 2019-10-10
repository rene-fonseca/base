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

#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
#  include <libxml2/libxml/xmlmemory.h>
#  include <libxml2/libxml/xmlIO.h>
#  include <libxml2/libxml/DOCBparser.h>
#  include <libxml2/libxml/xinclude.h>
#  include <libxml2/libxml/catalog.h>
#  include <libxml2/libxml/tree.h>
#  include <libxslt/xslt.h>
#  include <libxslt/xsltInternals.h>
#  include <libxslt/functions.h>
#  include <libxslt/transform.h>
#  include <libxslt/xsltutils.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Transformer::Transformer() throw() {
}

void Transformer::clearParameters() throw() {
  parameters = HashTable<String, String>();
}

String Transformer::getParameter(const String& name) throw(InvalidKey) {
  return parameters[name];
}

void Transformer::setParameter(
  const String& name, const String& value) throw(MemoryException) {
  parameters.add(name, value);
}

// bool Transformer::getXInclude() const throw() {
// #if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
//   return xsltGetXIncludeDefault() != 0;
// #endif
// }

// void Transformer::setXInclude(bool value) throw() {
// #if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
//   return xsltSetXIncludeDefault(value ? 1 : 0);
// #endif
// }

Document Transformer::transform(
  const Document& document) throw(TransformerException) {
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
  throw TransformerException(this);
#endif
}

void Transformer::save(const String& filename, const Document& document) throw(DOMException, IOException) {
#if 0 && defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
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

Stylesheet Transformer::getStylesheet() throw() {
  return stylesheet;
}

void Transformer::setStylesheet(Stylesheet stylesheet) throw() {
  this->stylesheet = stylesheet;
}

bool Transformer::functionAvailable(
  const String& ns, const String& name) throw(TransformerException) {
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

Transformer::~Transformer() throw() {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
