/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_PLATFORMS_WIN32__ASYNC_READ_FILE_CONTEXT_H
#define _DK_SDU_MIP__BASE_PLATFORMS_WIN32__ASYNC_READ_FILE_CONTEXT_H

#if !(_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  error inclusion of platform specific header file
#endif

#include <base/io/async/AsynchronousReadContext.h>
#include <base/io/IOException.h>
#include <base/OperatingSystem.h>
#include <base/Primitives.h>

#include <windows.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace win32 {

  class AsyncReadFileContext : public AsynchronousReadContext {
    friend void CALLBACK asyncReadFileCallback(DWORD errorCode, DWORD bytesRead, OVERLAPPED* overlapped);
  public:

    struct CallbackInfo {
      OVERLAPPED overlapped;
      AsyncReadFileContext* context;
    };
  private:
    
    AsynchronousReadEventListener* listener;
    CallbackInfo callbackInfo;
    uint8* buffer;
    unsigned int bytesToRead;
    unsigned long long offset;
    unsigned int bytesRead;
    unsigned int flags;
    
    inline void notifyAsynchronousCompletionListener() throw() {
      listener->asynchronousCompletion(
        AsynchronousReadCompletion(
          buffer,
          bytesToRead,
          offset,
          bytesRead,
          flags
        )
      );
    }
  public:
    
    AsyncReadFileContext(
      OperatingSystem::Handle handle,
      uint8* buffer,
      unsigned int bytesToRead,
      unsigned long long offset,
      AsynchronousReadEventListener* listener) throw(IOException);
  public:

    AsynchronousReadCompletion getCompletion() const throw() {
      return AsynchronousReadCompletion(
        buffer,
        bytesToRead,
        offset,
        bytesRead,
        flags
      );
    }

    ~AsyncReadFileContext();
  };

}; // win32 namespace

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
