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
#include <base/xml/Notation.h>
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
#  include <libxml/tree.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

String Notation::getPublicId() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  const xmlNode* node = (const xmlNode*)getContext();
  const xmlNotation* notation = (const xmlNotation*)node->name;
  return NativeString((const char*)notation->PublicID);
#else
  return String();
#endif
}
  
String Notation::getSystemId() const noexcept {
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  const xmlNode* node = (const xmlNode*)getContext();
  const xmlNotation* notation = (const xmlNotation*)node->name;
  return NativeString((const char*)notation->SystemID);
#else
  return String();
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
