/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/string/String.h>
#include <base/collection/HashTable.h>
#include <base/mem/Reference.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Translation. */
class _COM_AZURE_DEV__BASE__API Translation : public ReferenceCountedObject {
private:

  HashTable<String, String> texts;
  // Reference<ObjectModel::Object> meta;
public:
  
  /** Returns the current translaiton. */
  static R<Translation> getCurrent();

  /** Makes the given translation current. */ 
  static void makeCurrent(R<Translation> translation);

  Translation();

  // TAG: add support for meta info - version, date, vendor
  // Reference<ObjectModel::Object> getMeta();

  /** Returns the translation for the given text. Returns nullptr if not defined. */
  const String* get(const String& text) const;

  /** Returns the translation for the given text. Returns nullptr if not defined. */
  const String* get(const String& text, const String& context) const;

  /** Adds translation. Null-terminator should not be used for translations and will be discarded. */
  void add(const String& text, const String& translated);

  /** Adds translation. Null-terminator should not be used for translations and will be discarded. */
  void add(const String& text, const String& context, const String& translated);

#if 0
    /** Loads translation from the given path. */
    static void load(const String& path);
    // TAG: need support for any file format
#endif

  /** Translates the given string. */
  String translate(const String& text) const;

  /** Translates the given string and context. */
  String translate(const String& text, const String& context) const;
};

/** Returns translation. Null-terminator should not be used for translations. */
_COM_AZURE_DEV__BASE__API String translate(const Literal& text);
/** Returns translation. Null-terminator should not be used for translations. */
_COM_AZURE_DEV__BASE__API String translate(const WideLiteral& text);
/** Returns translation. Null-terminator should not be used for translations. */
_COM_AZURE_DEV__BASE__API String translate(const String& text);
/** Returns translation. Null-terminator should not be used for translations. */
_COM_AZURE_DEV__BASE__API String translate(const String& text, const String& context);
/** Returns translation. Null-terminator should not be used for translations. */
_COM_AZURE_DEV__BASE__API String operator "" _translate(const char* text, size_t length);
/** Returns translation. Null-terminator should not be used for translations. */
_COM_AZURE_DEV__BASE__API String operator "" _translate(const wchar* text, size_t length);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
