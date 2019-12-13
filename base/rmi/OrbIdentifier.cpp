/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/OrbIdentifier.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

OrbIdentifier::OrbIdentifier(const String& identifier) {
  String::ReadIterator i = identifier.getBeginReadIterator();
  String::ReadIterator end = identifier.getEndReadIterator();
  
  // TAG: check characters see URI RFC
  int schemeSeparator = endPoint.indexOf("://");
  if (schemeSeparator >= 0) {
    scheme = identifier.substring(0, schemeSeparator);
    int pathSeparator = identifier.indexOf('/');
    if (pathSeparator >= 0) {
      endPoint = identifier.substring(
        schemeSeparator + Literal("://").getLength(),
        pathSeparator
      );
      path = identifier.substring(pathSeparator + Literal("/").getLength());
    } else {
      endPoint = identifier.substring(
        schemeSeparator + Literal("://").getLength(),
        pathSeparator
      );
    }
  } else {
    int pathSeparator = identifier.indexOf('/');
    if (pathSeparator >= 0) {
      endPoint = identifier.substring(0, pathSeparator);
    } else {
      endPoint = identifier;
    }
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
