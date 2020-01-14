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

#if !(_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  error inclusion of platform specific header file
#endif

#include <base/io/async/AsynchronousReadContext.h>
#include <base/io/IOException.h>
#include <base/OperatingSystem.h>
#include <base/Profiler.h>

#include <windows.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

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
    Profiler::IOTask profile = "AsyncReadStreamContext::AsyncReadStreamContext()";
    
    inline void notifyAsynchronousCompletionListener() noexcept
    {
      listener->asynchronousCompletion(
        AsynchronousReadCompletion(buffer, bytesToRead, 0, bytesRead, flags)
      );
    }
  public:
    
    AsyncReadStreamContext(
      OperatingSystem::Handle handle,
      uint8* buffer,
      unsigned int bytesToRead,
      AsynchronousReadEventListener* listener);
  public:

    AsynchronousReadCompletion getCompletion() const noexcept
    {
      return AsynchronousReadCompletion(buffer, bytesToRead, 0, bytesRead, flags);
    }

    ~AsyncReadStreamContext();
  };
  
}; // win32 namespace

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
