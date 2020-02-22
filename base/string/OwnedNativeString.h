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

#include <base/features.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class ReferenceCountedObject;
class String;

/** Owned native string. */
class _COM_AZURE_DEV__BASE__API OwnedNativeString {
  friend class String;
private:
  
  char* text = nullptr;
  ReferenceCountedObject* buffer = nullptr; // ATTENTION: we must have operator= implemented since primitive!
public:
  
  /** Initializes empty string. */
  OwnedNativeString() noexcept;

  /** Initializes string. */
  OwnedNativeString(const char* text);

  /** Initializes string with literal. Do NOT initialize with temporary buffer! */
  template<decltype(sizeof(void*)) SIZE>
  inline OwnedNativeString(const char (&value)[SIZE]) noexcept
    : text(value)
  {
  }

  /** Initializes string. */
  OwnedNativeString(const OwnedNativeString& copy) noexcept;

  /** Initializes string. */
  OwnedNativeString(OwnedNativeString&& move) noexcept;

  /** Assigns string. */
  OwnedNativeString& operator=(const OwnedNativeString& assign) noexcept;

  /** Assigns string. */
  OwnedNativeString& operator=(OwnedNativeString&& assign) noexcept;

  /** Assigns string. */
  OwnedNativeString& operator=(const char* assign) noexcept;

  /** Do NOT keep string pointer after life time of OwnedNativeString. */
  inline operator const char*() const noexcept
  {
    return text;
  }

  /** Returns the length of the string. */
  // MemorySize getLength() const noexcept;
  
  /** Destruct string. */
  ~OwnedNativeString();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
