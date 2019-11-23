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

#include <base/string/String.h>
#include <base/collection/Set.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Shared strins.
*/

class _COM_AZURE_DEV__BASE__API SharedStrings {
public:

  class Bucket {
  public:

    Set<String> strings; // TAG: hash should be ok
  };

  static constexpr MemorySize MAXIMUM_BUCKETS = 1024;
  static constexpr MemorySize MAXIMUM_BUCKET_SIZE = 1024;
  Bucket buckets[MAXIMUM_BUCKETS]; // lengths are likely all quite small
  // CachePerformance performance;
public:

  /** Returns the number of strings. Not cached size. */
  MemorySize getSize() const noexcept;

  /** Clears the buffers. */
  void clear();

  /** Returns a shared string to conserve memory (might return the same string). */
  String getString(const String& temp);

  /** Returns a shared string to conserve memory. */
  inline String getString(const char* native)
  {
    return native ? getString(native) : String();
  }

  /** Garbage collect strings which are not used. */
  void garbageCollect();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
