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

DisplayObject Jupyter::displayMime(const String& text, const String& mimetype)
{
  return DisplayObject(mimetype, text);
}

DisplayObject Jupyter::html(const String& text)
{
  return DisplayObject("text/html", text);
}

DisplayObject Jupyter::iframe(const String& url, unsigned width, unsigned int height)
{
  return DisplayObject("text/html",
    "<iframe src=\"%1\" width=\"%2\" height=\"%3\"/>" % Subst(HTML::encode(url), width, height));
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
