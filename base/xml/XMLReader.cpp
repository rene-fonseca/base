/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/xml/XMLReader.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

XMLReader::InputSource XMLReader::EntityResolver::resolveEntity(
  const String& publicId, const String& systemId) throw(SAXException) {
// TAG: fixme
//   use default resolver input from systemId
//     return defaultLoader(
//       systemId.getElements(),
//       publicId.getElements(),
//       context
//     );
//   }
  return 0;
}

void XMLReader::DTDHandler::comment(const String& data) throw() {
}

void XMLReader::DTDHandler::startDTD(
  const String& name,
  const String& publicId,
  const String& systemId) throw() {
}

void XMLReader::DTDHandler::endDTD() throw() {
}

void XMLReader::DTDHandler::startEntity(const String& name) throw() {
}

void XMLReader::DTDHandler::endEntity(const String& name) throw() {
}

void XMLReader::DTDHandler::startCDATA() throw() {
}

void XMLReader::DTDHandler::endCDATA() throw() {
}

void XMLReader::DTDHandler::attributeDecl(
  const String& name,
  const String& element,
  AttributeDecl::ValueType type,
  AttributeDecl::DefaultType defaultType,
  const String& defaultValue,
  const Array<String>& enumeration) throw() {
}

void XMLReader::DTDHandler::elementDecl(
  const String& name,
  ElementDecl::ValueType type) throw() {
}

void XMLReader::DTDHandler::internalEntityDecl(
  const String& name,
  const String& text) throw() {
}

void XMLReader::DTDHandler::externalEntityDecl(
  const String& name,
  const String& publicId,
  const String& systemId) throw() {
}

void XMLReader::DTDHandler::notationDecl(
  const String& name,
  const String& publicId,
  const String& systemId) throw() {
}

void XMLReader::DTDHandler::unparsedEntityDecl(
  const String& name,
  const String& publicId,
  const String& systemId,
  const String& notationName) throw() {
}

void XMLReader::ErrorHandler::error(
  const SAXParseException& exception) throw(SAXException) {
  throw exception;
}

void XMLReader::ErrorHandler::fatalError(
  const SAXParseException& exception) throw(SAXException) {
  throw exception;
}

void XMLReader::ErrorHandler::warning(
  const SAXParseException& exception) throw(SAXException) {
  throw exception;
}

void XMLReader::ContentHandler::startDocument() throw() {
}

void XMLReader::ContentHandler::endDocument() throw() {
}

void XMLReader::ContentHandler::startElement(
  const String& namespaceURI,
  const String& localName,
  const String& qName,
  const Attributes& attributes) throw() {
}

void XMLReader::ContentHandler::endElement(
  const String& namespaceURI,
  const String& localName,
  const String& qName) throw() {
}

void XMLReader::ContentHandler::processingInstruction(
  const String& target, const String& data) throw() {
}

void XMLReader::ContentHandler::characters(const String& value) throw() {
}

void XMLReader::ContentHandler::ignorableWhitespace(
  const String& value) throw() {
}

void XMLReader::ContentHandler::comment(const String& value) throw() {
}

void XMLReader::ContentHandler::cdataBlock(const String& value) throw() {
}

void XMLReader::ContentHandler::skippedEntity(const String& name) throw() {
}

void XMLReader::ContentHandler::entityReference(const String& name) throw() {
}

void XMLReader::ContentHandler::endPrefixMapping(
  const String& prefix) throw() {
}

void XMLReader::ContentHandler::startPrefixMapping(
  const String& prefix, const String& uri) throw() {
}

void XMLReader::ContentHandler::setDocumentLocator(Locator* locator) throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
