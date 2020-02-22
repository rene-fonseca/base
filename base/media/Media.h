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

/** Media blob. Lower level object with minimum dependencies. */
class _COM_AZURE_DEV__BASE__API MediaBlob {
public:
  
  /** Name/value pair. */
  class KeyValue {
  public:
    
    // R<ReferenceCountedAllocator<char> > key;
    // R<ReferenceCountedAllocator<char> > bytes;
    
    String key; // normally mimetype
    String bytes;
  };
private:
  
  KeyValue fields[2];
  unsigned int size = 0;
public:
  
  /** Returns 'text/plain'. */
  static const String& getPlainType();

  /** Returns 'text/html'. */
  static const String& getHTMLType();

  /** Initializes media blob. */
  MediaBlob();

  /** Initializes media blob. */
  MediaBlob(const String& key, const String& bytes, bool encode = false);
  
  /** Initializes media blob. */
  MediaBlob(String&& key, String&& bytes, bool encode = false);
  
  /** Adds key. */
  MediaBlob& add(const String& key, const String& bytes, bool encode = false);

  /** Adds key. */
  MediaBlob& add(String&& key, String&& bytes, bool encode = false);

  /** Returns true if the key exists. */
  bool hasKey(const String& key) const noexcept;

  /** Returns the size. */
  inline MemorySize getSize() const noexcept
  {
    return size;
  }
  
  /** Returns the begin iterator. */
  inline const KeyValue* begin() const noexcept
  {
    return fields;
  }

  /** Returns the end iterator. */
  inline const KeyValue* end() const noexcept
  {
    return fields + size;
  }
  
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_CLING)
  /** Returns bundle. */
  inline ClingMimeBundle getBundle() const
  {
    ClingMimeBundleHandle handle;
    for (const auto& nv : fields) {
      handle.setMimeType(nv.key, nv.bytes);
    }
    return moveObject(handle.getBundle());
  }
#endif
};

#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_CLING)
inline ClingMimeBundle _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_ID(const MediaBlob& blob) \
{ \
  return blob.getBundle(); \
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
  /** Returns media blob. */
  inline MediaBlob getMediaBlob() const
  {
    return MediaBlob(getMimeType(), getBytes(), true);
  }
};

/** Image. */
class _COM_AZURE_DEV__BASE__API Image : public Media {
public:
  
  using Media::Media;
};

/** Audio. */
class _COM_AZURE_DEV__BASE__API Audio : public Media {
public:

  using Media::Media;
  
  /** Returns HTML audio control. */
  static String getAudioControl(const String& bytes, const String& mimetype);

  /** Returns media blob. */
  inline MediaBlob getMediaBlob() const
  {
    return MediaBlob(
      MediaBlob::getHTMLType(),
      Audio::getAudioControl(getBytes(), getMimeType())
    );
  }
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
  
  /** Returns media blob. */
  inline MediaBlob getMediaBlob() const
  {
    return MediaBlob(getMimeType(), getText());
  }
};

#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_CLING)
#  define _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_MEDIA(TYPE) \
inline ClingMimeBundle _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_ID(const TYPE& media) \
{ \
  return moveObject(media.getMediaBlob().getBundle()); \
}
#else
#  define _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_MEDIA(TYPE)
#endif

_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_MEDIA(Media)
_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_MEDIA(Image)
_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_MEDIA(Audio)
_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_MEDIA(Video)
_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_MEDIA(DisplayObject)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
