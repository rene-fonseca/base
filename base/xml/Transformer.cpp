/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/xml/Transformer.h>

#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
#  include <libxml/xmlmemory.h>
#  include <libxml/debugXML.h>
#  include <libxml/HTMLtree.h>
#  include <libxml/xmlIO.h>
#  include <libxml/DOCBparser.h>
#  include <libxml/xinclude.h>
#  include <libxml/catalog.h>
#  include <libxslt/xslt.h>
#  include <libxslt/xsltInternals.h>
#  include <libxslt/functions.h>
#  include <libxslt/transform.h>
#  include <libxslt/xsltutils.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

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
// #if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
//   return ::xsltGetXIncludeDefault() != 0;
// #endif
// }

// void Transformer::setXInclude(bool value) throw() {
// #if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
//   return ::xsltSetXIncludeDefault(value ? 1 : 0);
// #endif
// }

Document Transformer::transform(
  const Document& document) throw(TransformerException) {
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
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
  
	xmlDocPtr nativeResult = ::xsltApplyStylesheet(
    Cast::pointer<xsltStylesheetPtr>(stylesheet.stylesheet->getContext()),
    Cast::pointer<xmlDocPtr>(document.document->getContext()),
    temp.getElements()
  );
  assert(nativeResult, TransformerException(this));
  Document result;
  result.document = new Document::DocumentImpl(nativeResult);
  return result;
#else
  throw TransformerException(this);
#endif
}

void Transformer::save(const String& filename, const Document& document) throw(DOMException, IOException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  //assert(temp, DOMException(this));
  // int fd = file.getHandle();
  // ::xsltSaveResultToFd(fd, ?, ?);
  int bytesWritten = ::xsltSaveResultToFilename(
    filename.getElements(),
    Cast::pointer<xmlDocPtr>(document.document->getContext()),
    Cast::pointer<xsltStylesheetPtr>(stylesheet.stylesheet->getContext()),
    0
  );
  assert(bytesWritten >= 0, IOException(this));
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
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
//   return ::xsltXPathFunctionLookup(
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
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
