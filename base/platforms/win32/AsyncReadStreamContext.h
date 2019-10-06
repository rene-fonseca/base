/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#if !(_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  error inclusion of platform specific header file
#endif

#include <base/io/async/AsynchronousReadContext.h>
#include <base/io/IOException.h>
#include <base/OperatingSystem.h>

#include <windows.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace win32 {

  class AsyncReadStreamContext : public AsynchronousReadContext {
    friend void CALLBACK asyncReadStreamCallback(DWORD errorCode, DWORD bytesRead, OVERLAPPED* overlapped);
  public:

    struct CallbackInfo {
      OVERLAPPED overlapped;
      AsyncReadStreamContext* context;
    };
  private:
    
    AsynchronousReadEventListener* listener = nullptr;
    CallbackInfo callbackInfo;
    uint8* buffer = nullptr;
    unsigned int bytesToRead = 0;
    unsigned int bytesRead = 0;
    unsigned int flags = 0;
    
    inline void notifyAsynchronousCompletionListener() throw() {
      listener->asynchronousCompletion(
        AsynchronousReadCompletion(buffer, bytesToRead, 0, bytesRead, flags)
      );
    }
  public:
    
    AsyncReadStreamContext(
      OperatingSystem::Handle handle,
      uint8* buffer,
      unsigned int bytesToRead,
      AsynchronousReadEventListener* listener) throw(IOException);
  public:

    AsynchronousReadCompletion getCompletion() const throw() {
      return AsynchronousReadCompletion(buffer, bytesToRead, 0, bytesRead, flags);
    }

    ~AsyncReadStreamContext();
  };
  
}; // win32 namespace

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
