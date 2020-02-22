/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Jupyter.h>
#include <base/string/HTML.h>
#include <base/string/Format.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

MediaBlob Jupyter::blob(const String& text, const String& mimetype)
{
  return MediaBlob(mimetype, text);
}

MediaBlob Jupyter::html(const String& text)
{
  return MediaBlob(MediaBlob::getHTMLType(), text);
}

MediaBlob Jupyter::script(const String& text)
{
  return MediaBlob(MediaBlob::getHTMLType(),
    "<script type=\"application/javascript\">" + /*HTML::encode*/(text) + "</script>");
}

MediaBlob Jupyter::iframe(const String& url, unsigned width, unsigned int height)
{
  return MediaBlob(MediaBlob::getHTMLType(),
    "<iframe src=\"%1\" width=\"%2\" height=\"%3\"/>" % Subst(HTML::encode(url), width, height));
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
