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

const String& MediaBlob::getPlainType()
{
  static String result(MESSAGE("text/plain"));
  return result;
}

const String& MediaBlob::getHTMLType()
{
  static String result(MESSAGE("text/html"));
  return result;
}

MediaBlob::MediaBlob()
{
}

MediaBlob::MediaBlob(const String& key, const String& bytes, bool encode)
{
  add(key, bytes, encode);
}

MediaBlob::MediaBlob(String&& key, String&& bytes, bool encode)
{
  add(moveObject(key), moveObject(bytes), encode);
}

MediaBlob& MediaBlob::add(const String& _key, const String& _bytes, bool encode)
{
  if (size >= getArraySize(fields)) {
    _throw OutOfRange("Too many blobs.");
  }
  // we dont care if the same key is added more than once
  fields[size].key = _key;
  fields[size].bytes = encode ? base64encode(_bytes) : _bytes;
  ++size;
  return *this;
}

MediaBlob& MediaBlob::add(String&& _key, String&& _bytes, bool encode)
{
  if (size >= getArraySize(fields)) {
    _throw OutOfRange("Too many blobs.");
  }
  // we dont care if the same key is added more than once
  fields[size].key = moveObject(_key);
  fields[size].bytes = encode ? base64encode(_bytes) : moveObject(_bytes);
  ++size;
  return *this;
}

bool MediaBlob::hasKey(const String& key) const noexcept
{
  for (const auto& nv : *this) {
    if (nv.key == key) {
      return true;
    }
  }
  return false;
}

String Audio::getAudioControl(const String& bytes, const String& mimetype)
{
  StringOutputStream stream;
  stream << "<audio controls src=\"data:" << HTML::encode(mimetype) << ";base64," << Base64::encode(bytes) << "\"/>";
  return stream.getString();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
