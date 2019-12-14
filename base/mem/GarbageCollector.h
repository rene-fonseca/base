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

#include <base/mem/Reference.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Garbage collector.
 
  @ingroup memory
*/

class _COM_AZURE_DEV__BASE__API GarbageCollector {
public:

  /** Returns true if the garbage collector is running. */
  static bool isRunning();

  /** Stops the garbage collector. */
  static void stop();

  /** Starts the garbage collector. */
  static bool start();

  /**
    Tells garbage collector to release the given object. The object cannot be destructed until all references are
    removed.
  */
  static void release(const Reference<ReferenceCountedObject>& reference);

  /** Tells garbage collector tor release the given object. The reference will be set to nullptr. */
  static void release(Reference<ReferenceCountedObject>& reference);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
