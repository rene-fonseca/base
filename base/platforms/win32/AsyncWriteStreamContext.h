/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_PLATFORMS_WIN32__ASYNC_WRITE_STREAM_CONTEXT_H
#define _DK_SDU_MIP__BASE_PLATFORMS_WIN32__ASYNC_WRITE_STREAM_CONTEXT_H

#if !(_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #error inclusion of platform specific header file
#endif

#include <base/io/async/AsynchronousWriteContext.h>
#include <base/io/IOException.h>
#include <base/OperatingSystem.h>

#include <windows.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace win32 {

  class AsyncWriteStreamContext : public AsynchronousWriteContext {
    friend void CALLBACK asyncWriteStreamCallback(DWORD errorCode, DWORD bytesWritten, OVERLAPPED* overlapped);
  public:

    struct CallbackInfo {
      OVERLAPPED overlapped;
      AsyncWriteStreamContext* context;
    };
  private:
    
    AsynchronousWriteEventListener* listener;
    CallbackInfo callbackInfo;
    const void* buffer;
    unsigned int bytesToWrite;
    unsigned int bytesWritten;
    unsigned int flags;
    
    inline void notifyAsynchronousCompletionListener() throw() {
      listener->asynchronousCompletion(AsynchronousWriteCompletion(buffer, bytesToWrite, 0, bytesWritten, flags));
    }
  public:
    
    AsyncWriteStreamContext(OperatingSystem::Handle handle, const void* buffer, unsigned int bytesToWrite, AsynchronousWriteEventListener* listener) throw(IOException);
  public:

    AsynchronousWriteCompletion getCompletion() const throw() {
      return AsynchronousWriteCompletion(buffer, bytesToWrite, 0, bytesWritten, flags);
    }

    ~AsyncWriteStreamContext() throw(AsynchronousException);
  };
  
}; // win32 namespace

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
