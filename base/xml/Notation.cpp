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
#include <base/xml/Notation.h>

#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
#  include <libxml/tree.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

String Notation::getPublicId() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  xmlNotationPtr notation = (xmlNotationPtr)node->name;
  return (const char*)notation->PublicID;
#else
  return String();
#endif
}
  
String Notation::getPrivateId() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNodePtr node = (xmlNodePtr)getContext();
  xmlNotationPtr notation = (xmlNotationPtr)node->name;
  return (const char*)notation->SystemID;
#else
  return String();
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE