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
#include <base/xml/DOMImplementation.h>

// TAG: fixme

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

bool DOMImplementation::hasFeature(
  const String& name, const String& version) throw() {
  return false; // TAG: fixme
}

DocumentType DOMImplementation::createDocumentType(
  const String& qualifiedName,
  const String& publicId,
  const String& systemId) throw(DOMException) {
  return DocumentType(0); // TAG: fixme
}

Document DOMImplementation::createDocument(
  const String& namespaceURI,
  const String& qualifiedName,
  const DocumentType& doctype) throw(DOMException) {
  return Document(); // TAG: fixme
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
