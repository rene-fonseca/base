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
#include <base/xml/Entity.h>

#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
#  include <libxml2/libxml/tree.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

String Entity::getPublicId() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  xmlEntity* entity = (xmlEntity*)node->name;
  if (entity->etype != XML_EXTERNAL_GENERAL_UNPARSED_ENTITY) {
    return String();
  } else {
    return (const char*)entity->ExternalID;
  }
#else
  return String();
#endif
}
  
String Entity::getSystemId() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  xmlEntity* entity = (xmlEntity*)node->name;
  if (entity->etype != XML_EXTERNAL_GENERAL_UNPARSED_ENTITY) {
    return String();
  } else {
    return (const char*)entity->SystemID;
  }
#else
  return String();
#endif
}
  
String Entity::getNotationName() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  xmlEntity* entity = (xmlEntity*)node->name;
  if (entity->etype != XML_EXTERNAL_GENERAL_UNPARSED_ENTITY) { // TAG: check
    return String();
  } else {
    return (const char*)entity->content;
  }
#else
  return String();
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
