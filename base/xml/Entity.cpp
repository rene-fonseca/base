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
#include <base/xml/Entity.h>
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
#  include <libxml/tree.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

String Entity::getPublicId() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlNodePtr node = (xmlNodePtr)getContext();
  xmlEntity* entity = (xmlEntity*)node->name;
  if (entity->etype != XML_EXTERNAL_GENERAL_UNPARSED_ENTITY) {
    return String();
  } else {
    return NativeString((const char*)entity->ExternalID);
  }
#else
  return String();
#endif
}
  
String Entity::getSystemId() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlNodePtr node = (xmlNodePtr)getContext();
  xmlEntity* entity = (xmlEntity*)node->name;
  if (entity->etype != XML_EXTERNAL_GENERAL_UNPARSED_ENTITY) {
    return String();
  } else {
    return NativeString((const char*)entity->SystemID);
  }
#else
  return String();
#endif
}
  
String Entity::getNotationName() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlNodePtr node = (xmlNodePtr)getContext();
  xmlEntity* entity = (xmlEntity*)node->name;
  if (entity->etype != XML_EXTERNAL_GENERAL_UNPARSED_ENTITY) { // TAG: check
    return String();
  } else {
    return NativeString((const char*)entity->content);
  }
#else
  return String();
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
