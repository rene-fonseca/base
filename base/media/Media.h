/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/security/Base64.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if 1
/** Jupyter media blob. */
class _COM_AZURE_DEV__BASE__API JupyterMediaBlob {
public:
  
  class Blob {
  public:
    
    String mimetype;
    String bytes;
  };
  
  // Blob fields[2];
  // TAG: we could add support for up to 2 types

  String mimetype;
  String bytes;
  
  inline JupyterMediaBlob()
  {
  }

  inline JupyterMediaBlob(const String& _mimetype, const String& _bytes, bool encode = false)
    : mimetype(_mimetype), bytes(encode ? base64encode(_bytes) : _bytes)
  {
  }
};
#endif

#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_CLING)

inline ClingMimeBundle _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_ID(const JupyterMediaBlob& blob) \
{ \
  return ClingMimeBundleHandle().setMimeType(blob.mimetype, blob.bytes); \
}

#endif



/** Media object. */
class _COM_AZURE_DEV__BASE__API Media {
private:

  String mimetype;
  String bytes;
public:

  inline Media(const String& _mimetype, const String& _bytes)
    : mimetype(_mimetype), bytes(_bytes)
  {
  }
  
  /** Returns the MIME type. */
  inline const String& getMimeType() const noexcept
  {
    return mimetype;
  }

  /** Returns the bytes. */
  inline const String& getBytes() const noexcept
  {
    return bytes;
  }

  // cling: would like cling to pick this up for display
  inline operator JupyterMediaBlob() const
  {
    return JupyterMediaBlob(getMimeType(), getBytes(), true);
  }
};

/** Image. */
class _COM_AZURE_DEV__BASE__API Image : public Media {
public:
  
  using Media::Media;
  
  inline operator JupyterMediaBlob() const
  {
    return JupyterMediaBlob(getMimeType(), getBytes(), true);
  }
};

/** Audio. */
class _COM_AZURE_DEV__BASE__API Audio : public Media {
public:

  using Media::Media;
  
  /** Returns HTML audio control. */
  static String getAudioControl(const String& bytes, const String& mimetype);
};

/** Video. */
class _COM_AZURE_DEV__BASE__API Video : public Media {
public:

  using Media::Media;
};

/** Display object. */
class _COM_AZURE_DEV__BASE__API DisplayObject {
private:

  String mimetype;
  String text;
public:

  inline DisplayObject(const String& _mimetype, const String& _text)
    : mimetype(_mimetype), text(_text)
  {
  }
  
  /** Returns the MIME type. */
  inline const String& getMimeType() const noexcept
  {
    return mimetype;
  }

  /** Returns the text. */
  inline const String& getText() const noexcept
  {
    return text;
  }
};

/** Display data in Jupyter. */
inline DisplayObject display(const String& text, const String& mimetype = "text/html")
{
  return DisplayObject(mimetype, text);
}

#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_CLING)

#  define _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_MEDIA(TYPE) \
inline ClingMimeBundle _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_ID(const TYPE& media) \
{ \
  return cling_getAnyMimeBundle(media.getMimeType(), Base64::encode(media.getBytes())); \
}

#  define _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_DISPLAY(TYPE) \
inline ClingMimeBundle _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_ID(const TYPE& object) \
{ \
  return cling_getAnyMimeBundle(object.getMimeType(), object.getText()); \
}

#else
#  define _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_MEDIA(TYPE)
#  define _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_DISPLAY(TYPE)
#endif

#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_CLING)

inline ClingMimeBundle _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_ID(const Audio& audio)
{
  return ClingMimeBundleHandle().setHTMLText(Audio::getAudioControl(audio.getBytes(), audio.getMimeType()));
}
#endif

_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_MEDIA(Media)
_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_MEDIA(Image)
_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_MEDIA(Video)
_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_DISPLAY(DisplayObject)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
