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
#include <base/xml/Stylesheet.h>

#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Stylesheet::StylesheetImpl::StylesheetImpl(void* context) noexcept {
  this->context = context;
}

Stylesheet::StylesheetImpl::~StylesheetImpl() noexcept {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
  if (context) {
    fout << __func__ << ENDL;
    ::xsltFreeStylesheet((xsltStylesheetPtr)context);
    fout << __func__ << ENDL;
  }
#endif
}

Stylesheet::Stylesheet() noexcept {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp = ::xsltNewStylesheet();
  bassert(temp, Exception(this)); // FIXME
  try {
    stylesheet = new StylesheetImpl(temp);
  } catch (...) {
    ::xsltFreeStylesheet(temp);
    _rethrow;
  }
#endif
}

Stylesheet::Stylesheet(const String& filename) noexcept {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
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
    _rethrow;
  }
#endif
}

Stylesheet::Stylesheet(const Document& document) noexcept {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
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
    _rethrow;
  }
#endif
}

String Stylesheet::getOutputMethod() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return NativeString(Cast::pointer<const char*>(temp->method));
#else
  return String();
#endif
}

String Stylesheet::getNamespace() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return NativeString(Cast::pointer<const char*>(temp->methodURI));
#else
  return String();
#endif
}

String Stylesheet::getVersion() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return NativeString(Cast::pointer<const char*>(temp->version));
#else
  return String();
#endif
}

String Stylesheet::getEncoding() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return NativeString(Cast::pointer<const char*>(temp->encoding));
#else
  return String();
#endif
}

bool Stylesheet::omitXmlDeclaration() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return temp->omitXmlDeclaration != 0;
#else
  return false;
#endif
}

String Stylesheet::getPublicId() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return NativeString(Cast::pointer<const char*>(temp->doctypePublic));
#else
  return String();
#endif
}

String Stylesheet::getSystemId() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return NativeString(Cast::pointer<const char*>(temp->doctypeSystem));
#else
  return String();
#endif
}

String Stylesheet::getMediaType() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return NativeString(Cast::pointer<const char*>(temp->mediaType));
#else
  return String();
#endif
}

bool Stylesheet::isStandalone() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return temp->standalone != 0;
#else
  return false;
#endif
}

// bool Stylesheet::stripAllSpaces() const noexcept {
// #if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
//   xsltStylesheetPtr temp =
//     Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
//   bassert(temp, Exception(this));
//   return temp->stripAll != 0;
//#else
//   return false;
// #endif
// }

bool Stylesheet::indent() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
  xsltStylesheetPtr temp =
    Cast::pointer<xsltStylesheetPtr>(stylesheet->getContext());
  bassert(temp, Exception(this));
  return temp->indent != 0;
#else
  return false;
#endif
}

Array<String> Stylesheet::getExcludedPrefixes() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XSLT_XMLSOFT_ORG)
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
Array<String> Stylesheet::getExtensions() const noexcept {
  Array<String> result;
  return result;
}
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
