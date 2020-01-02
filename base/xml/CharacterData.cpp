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
#include <base/xml/CharacterData.h>

#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
#  include <libxml2/libxml/tree.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

String CharacterData::getData() const {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  return NativeString((const char*)node->content);
#else
  _throw DOMException(this);
#endif
}

void CharacterData::setData(const String& data) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
	xmlNodeSetContent(node, (const xmlChar*)data.getElements());
#else
  _throw DOMException(this);
#endif
}

unsigned int CharacterData::getLength() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  const char* content = (const char*)node->content;
  const char* terminator = find<char>(
    content,
    String::MAXIMUM_LENGTH,
    0
  );
  return terminator ? (terminator - content) : String::MAXIMUM_LENGTH;
#else
  _throw DOMException(this);
#endif
}

String CharacterData::substringData(
  unsigned int offset, unsigned int count) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  const char* content = (const char*)node->content;
  const char* terminator = find<char>(
    content,
    minimum(offset, String::MAXIMUM_LENGTH),
    0
  );
  if (terminator == 0) {
    return String();
  }
  return String(content + offset, count);
#else
  _throw DOMException(this);
#endif
}

void CharacterData::appendData(const String& value) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  xmlNodeAddContent(node, (const xmlChar*)value.getElements());
  // TAG: how to detect failure
#else
  _throw DOMException(this);
#endif
}

void CharacterData::insertData(
  unsigned int offset, const String& value) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  String temp((const char*)node->content);
  temp.insert(offset, value);
	xmlNodeSetContent(node, (const xmlChar*)temp.getElements());
#else
  _throw DOMException(this);
#endif
}

void CharacterData::deleteData(
  unsigned int offset, unsigned int count) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  String temp((const char*)node->content);
  temp.remove(offset, offset + count); // TAG: overflow problem
	xmlNodeSetContent(node, (const xmlChar*)temp.getElements());
#else
  _throw DOMException(this);
#endif
}

void CharacterData::replaceData(
  unsigned int offset,
  unsigned int count,
  const String& value) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  String temp((const char*)node->content);
  temp.replace(offset, offset + count, value); // TAG: overflow problem
	xmlNodeSetContent(node, (const xmlChar*)temp.getElements());
#else
  _throw DOMException(this);
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
