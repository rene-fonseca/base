/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/win32/AsyncWriteStreamContext.h>
#include <base/Trace.h>

#if !(_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #error Inclusion of platform specific source file
#endif

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#endif // flavour

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace win32 {
  
  void CALLBACK asyncWriteStreamCallback(DWORD errorCode, DWORD bytesWritten, OVERLAPPED* overlapped) {
    AsyncWriteStreamContext* context = pointer_cast<AsyncWriteStreamContext::CallbackInfo*>(overlapped)->context;
    unsigned int flags = context->flags | AsynchronousWriteCompletion::COMPLETED;
    if (errorCode == 0) {
      flags |= AsynchronousWriteCompletion::SUCCESSFUL;
    } else if (errorCode == ERROR_OPERATION_ABORTED) { // FIXME: is this ok to do
      flags |= AsynchronousWriteCompletion::ABORTED;
    }
    context->flags = flags;
    context->bytesWritten = bytesWritten;
    context->notifyAsynchronousCompletionListener();
    context->selfReference = 0; // release destruction lock (do NOT access state hereafter)
  }

  AsyncWriteStreamContext::AsyncWriteStreamContext(OperatingSystem::Handle handle, const char* _buffer, unsigned int _bytesToWrite, AsynchronousWriteEventListener* _listener) throw(IOException)
    : listener(_listener),
      buffer(_buffer),
      bytesToWrite(_bytesToWrite),
      bytesWritten(0),
      flags(0) {
    clear(callbackInfo.overlapped);
    callbackInfo.context = this;
    
    BOOL result = ::WriteFileEx(handle, buffer, bytesToWrite, &callbackInfo.overlapped, pointer_cast<LPOVERLAPPED_COMPLETION_ROUTINE>(&asyncWriteStreamCallback));
    if (!result) {
      //DWORD lastError = ::GetLastError();
      selfReference = 0; // release destruction lock (do NOT access state hereafter)
      throw IOException();
    }
  }

  AsyncWriteStreamContext::~AsyncWriteStreamContext() throw(AsynchronousException) {
    assert((flags & AsynchronousWriteCompletion::COMPLETED) != 0, AsynchronousException());
    Trace::message(__PRETTY_FUNCTION__);
  }
  
}; // win32 namespace

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
