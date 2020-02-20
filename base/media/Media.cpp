/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/media/Media.h>
#include <base/string/HTML.h>
#include <base/string/StringOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

String Audio::getAudioControl(const String& bytes, const String& mimetype)
{
  StringOutputStream stream;
  stream << "<audio controls src=\"data:" << HTML::encode(mimetype) << ";base64," << Base64::encode(bytes) << "\"/>";
  return stream.getString();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
