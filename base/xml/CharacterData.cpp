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
#include <base/xml/CharacterData.h>

#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
#  include <libxml2/libxml/tree.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

String CharacterData::getData() const throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  return NativeString((const char*)node->content);
#else
  throw DOMException(this);
#endif
}

void CharacterData::setData(const String& data) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
	xmlNodeSetContent(node, (const xmlChar*)data.getElements());
#else
  throw DOMException(this);
#endif
}

unsigned int CharacterData::getLength() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  const char* content = (const char*)node->content;
  const char* terminator = find<char>(
    content,
    String::MAXIMUM_LENGTH,
    0
  );
  return terminator ? (terminator - content) : String::MAXIMUM_LENGTH;
#else
  throw DOMException(this);
#endif
}

String CharacterData::substringData(
  unsigned int offset, unsigned int count) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
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
  throw DOMException(this);
#endif
}

void CharacterData::appendData(const String& value) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  xmlNodeAddContent(node, (const xmlChar*)value.getElements());
  // TAG: how to detect failure
#else
  throw DOMException(this);
#endif
}

void CharacterData::insertData(
  unsigned int offset, const String& value) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  String temp((const char*)node->content);
  temp.insert(offset, value);
	xmlNodeSetContent(node, (const xmlChar*)temp.getElements());
#else
  throw DOMException(this);
#endif
}

void CharacterData::deleteData(
  unsigned int offset, unsigned int count) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  String temp((const char*)node->content);
  temp.remove(offset, offset + count); // TAG: overflow problem
	xmlNodeSetContent(node, (const xmlChar*)temp.getElements());
#else
  throw DOMException(this);
#endif
}

void CharacterData::replaceData(
  unsigned int offset,
  unsigned int count,
  const String& value) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  String temp((const char*)node->content);
  temp.replace(offset, offset + count, value); // TAG: overflow problem
	xmlNodeSetContent(node, (const xmlChar*)temp.getElements());
#else
  throw DOMException(this);
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
