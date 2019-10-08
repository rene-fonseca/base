/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/xml/Stylesheet.h>

#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
//#  include <libxml2/libxml/xmlmemory.h>
//#  include <libxml2/libxml/xmlIO.h>
//#  include <libxml2/libxml/xinclude.h>
//#  include <libxml2/libxml/catalog.h>
#  include <libxml2/libxml/globals.h>
#  include <libxslt/xslt.h>
#  include <libxslt/xsltInternals.h>
//#  include <libxslt/transform.h>
//#  include <libxslt/xsltutils.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Stylesheet::StylesheetImpl::StylesheetImpl(void* context) throw() {
  this->context = context;
}

Stylesheet::StylesheetImpl::~StylesheetImpl() throw() {
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
  if (context) {
    fout << __func__ << ENDL;
    ::xsltFreeStylesheet((xsltStylesheetPtr)context);
    fout << __func__ << ENDL;
  }
#endif
}

Stylesheet::Stylesheet() throw() {
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp = ::xsltNewStylesheet();
  bassert(temp, Exception(this)); // FIXME
  try {
    stylesheet = new StylesheetImpl(temp);
  } catch (...) {
    ::xsltFreeStylesheet(temp);
    throw;
  }
#endif
}

Stylesheet::Stylesheet(const String& filename) throw() {
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
  ::xmlSubstituteEntitiesDefault(1); // FIXME
  xmlLoadExtDtdDefaultValue = 1; // FIXME
  
  xsltStylesheetPtr temp = ::xsltParseStylesheetFile(
    Cast::pointer<const xmlChar*>(filename.getElements())
  );
  bassert(temp, Exception(this)); // FIXME
  try {
    stylesheet = new StylesheetImpl(temp);
  } catch (...) {
    ::xsltFreeStylesheet(temp);
    throw;
  }
#endif
}

Stylesheet::Stylesheet(const Document& document) throw() {
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
  ::xmlSubstituteEntitiesDefault(1); // FIXME
  xmlLoadExtDtdDefaultValue = 1; // FIXME

  xmlDocPtr doc = Cast::pointer<xmlDocPtr>(document.getContext());
  xmlDocPtr newDoc = xmlCopyDoc(doc, 1);
  bassert(newDoc, Exception(this)); // FIXME
  
  xsltStylesheetPtr temp = ::xsltParseStylesheetDoc(newDoc);
  bassert(temp, Exception(this)); // FIXME
  try {
    stylesheet = new StylesheetImpl(temp);
  } catch (...) {
    ::xsltFreeStylesheet(temp);
    throw;
  }
#endif
}

String Stylesheet::getOutputMethod() const throw() {
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return NativeString(Cast::pointer<const char*>(temp->method));
#else
  return String();
#endif
}

String Stylesheet::getNamespace() const throw() {
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return NativeString(Cast::pointer<const char*>(temp->methodURI));
#else
  return String();
#endif
}

String Stylesheet::getVersion() const throw() {
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return NativeString(Cast::pointer<const char*>(temp->version));
#else
  return String();
#endif
}

String Stylesheet::getEncoding() const throw() {
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return NativeString(Cast::pointer<const char*>(temp->encoding));
#else
  return String();
#endif
}

bool Stylesheet::omitXmlDeclaration() const throw() {
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return temp->omitXmlDeclaration != 0;
#else
  return false;
#endif
}

String Stylesheet::getPublicId() const throw() {
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return NativeString(Cast::pointer<const char*>(temp->doctypePublic));
#else
  return String();
#endif
}

String Stylesheet::getSystemId() const throw() {
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return NativeString(Cast::pointer<const char*>(temp->doctypeSystem));
#else
  return String();
#endif
}

String Stylesheet::getMediaType() const throw() {
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return NativeString(Cast::pointer<const char*>(temp->mediaType));
#else
  return String();
#endif
}

bool Stylesheet::isStandalone() const throw() {
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return temp->standalone != 0;
#else
  return false;
#endif
}

// bool Stylesheet::stripAllSpaces() const throw() {
// #if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
//   xsltStylesheetPtr temp =
//     Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
//   bassert(temp, Exception(this));
//   return temp->stripAll != 0;
//#else
//   return false;
// #endif
// }

bool Stylesheet::indent() const throw() {
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return temp->indent != 0;
#else
  return false;
#endif
}

Array<String> Stylesheet::getExcludedPrefixes() const throw() {
#if defined(_DK_SDU_MIP__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  Array<String> result;
  xmlChar** current = temp->exclPrefixTab;
  while (*current) { // FIXME
    result.append(NativeString(Cast::pointer<const char*>(*current)));
    ++current;
  }
  return result;
#else
  return Array<String>();
#endif
}

#if 0
Array<String> Stylesheet::getExtensions() const throw() {
  Array<String> result;
  return result;
}
#endif

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
