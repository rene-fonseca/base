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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

XMLReader::InputSource XMLReader::EntityResolver::resolveEntity(
  const String& publicId, const String& systemId) {
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

void XMLReader::DTDHandler::comment(const String& data) noexcept {
}

void XMLReader::DTDHandler::startDTD(
  const String& name,
  const String& publicId,
  const String& systemId) noexcept {
}

void XMLReader::DTDHandler::endDTD() noexcept {
}

void XMLReader::DTDHandler::startEntity(const String& name) noexcept {
}

void XMLReader::DTDHandler::endEntity(const String& name) noexcept {
}

void XMLReader::DTDHandler::startCDATA() noexcept {
}

void XMLReader::DTDHandler::endCDATA() noexcept {
}

void XMLReader::DTDHandler::attributeDecl(
  const String& name,
  const String& element,
  AttributeDecl::ValueType type,
  AttributeDecl::DefaultType defaultType,
  const String& defaultValue,
  const Array<String>& enumeration) noexcept {
}

void XMLReader::DTDHandler::elementDecl(
  const String& name,
  ElementDecl::ValueType type) noexcept {
}

void XMLReader::DTDHandler::internalEntityDecl(
  const String& name,
  const String& text) noexcept {
}

void XMLReader::DTDHandler::externalEntityDecl(
  const String& name,
  const String& publicId,
  const String& systemId) noexcept {
}

void XMLReader::DTDHandler::notationDecl(
  const String& name,
  const String& publicId,
  const String& systemId) noexcept {
}

void XMLReader::DTDHandler::unparsedEntityDecl(
  const String& name,
  const String& publicId,
  const String& systemId,
  const String& notationName) noexcept {
}

void XMLReader::ErrorHandler::error(
  const SAXParseException& exception) {
  throw exception;
}

void XMLReader::ErrorHandler::fatalError(
  const SAXParseException& exception) {
  throw exception;
}

void XMLReader::ErrorHandler::warning(
  const SAXParseException& exception) {
  throw exception;
}

void XMLReader::ContentHandler::startDocument() noexcept {
}

void XMLReader::ContentHandler::endDocument() noexcept {
}

void XMLReader::ContentHandler::startElement(
  const String& namespaceURI,
  const String& localName,
  const String& qName,
  const Attributes& attributes) noexcept {
}

void XMLReader::ContentHandler::endElement(
  const String& namespaceURI,
  const String& localName,
  const String& qName) noexcept {
}

void XMLReader::ContentHandler::processingInstruction(
  const String& target, const String& data) noexcept {
}

void XMLReader::ContentHandler::characters(const String& value) noexcept {
}

void XMLReader::ContentHandler::ignorableWhitespace(
  const String& value) noexcept {
}

void XMLReader::ContentHandler::comment(const String& value) noexcept {
}

void XMLReader::ContentHandler::cdataBlock(const String& value) noexcept {
}

void XMLReader::ContentHandler::skippedEntity(const String& name) noexcept {
}

void XMLReader::ContentHandler::entityReference(const String& name) noexcept {
}

void XMLReader::ContentHandler::endPrefixMapping(
  const String& prefix) noexcept {
}

void XMLReader::ContentHandler::startPrefixMapping(
  const String& prefix, const String& uri) noexcept {
}

void XMLReader::ContentHandler::setDocumentLocator(Locator* locator) noexcept {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
