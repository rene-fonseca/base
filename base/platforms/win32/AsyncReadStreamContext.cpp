/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/win32/AsyncReadStreamContext.h>
#include <base/Trace.h>

#if !(_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #error Inclusion of platform specific source file
#endif

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#endif // flavour

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace win32 {
  
  void CALLBACK asyncReadStreamCallback(DWORD errorCode, DWORD bytesRead, OVERLAPPED* overlapped) {
    AsyncReadStreamContext* context = pointer_cast<AsyncReadStreamContext::CallbackInfo*>(overlapped)->context;
    unsigned int flags = context->flags | AsynchronousReadCompletion::COMPLETED;
    if (errorCode == 0) {
      flags |= AsynchronousReadCompletion::SUCCESSFUL;
    } else if (errorCode == ERROR_HANDLE_EOF) {
      flags |= AsynchronousReadCompletion::END_OF_FILE;
    } else if (errorCode == ERROR_OPERATION_ABORTED) { // FIXME: is this ok to do
      flags |= AsynchronousReadCompletion::ABORTED;
    }
    context->flags = flags;
    context->bytesRead = bytesRead;
    context->notifyAsynchronousCompletionListener();
    context->selfReference = 0; // release destruction lock (do NOT access state hereafter)
  }

  AsyncReadStreamContext::AsyncReadStreamContext(OperatingSystem::Handle handle, char* _buffer, unsigned int _bytesToRead, AsynchronousReadEventListener* _listener) throw(IOException)
    : listener(_listener),
      buffer(_buffer),
      bytesToRead(_bytesToRead),
      bytesRead(0),
      flags(0) {
    clear(callbackInfo.overlapped);
    callbackInfo.context = this;
    
    BOOL result = ::ReadFileEx(handle, buffer, bytesToRead, &callbackInfo.overlapped, pointer_cast<LPOVERLAPPED_COMPLETION_ROUTINE>(&asyncReadStreamCallback));
    if (!result) {
      DWORD lastError = ::GetLastError();
      if (lastError == ERROR_HANDLE_EOF) {
        flags |= AsynchronousReadCompletion::COMPLETED | AsynchronousReadCompletion::END_OF_FILE;
        notifyAsynchronousCompletionListener(); // call manually on error
        selfReference = 0; // release destruction lock (do NOT access state hereafter)
      } else {
        selfReference = 0; // release destruction lock (do NOT access state hereafter)
        throw IOException();
      }
    }
  }

  AsyncReadStreamContext::~AsyncReadStreamContext() throw(AsynchronousException) {
    assert((flags & AsynchronousReadCompletion::COMPLETED) != 0, AsynchronousException());
    Trace::message(__PRETTY_FUNCTION__);
  }
  
}; // win32 namespace

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
