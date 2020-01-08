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
#include <base/xml/Declaration.h>
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
#  include <libxml/tree.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

DocumentType Declaration::getDocumentType() noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlNode* decl = (xmlNode*)getContext();
  return decl->parent;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
